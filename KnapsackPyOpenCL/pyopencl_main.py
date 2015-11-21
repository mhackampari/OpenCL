import numpy
import pyopencl as cl
#import myconstants as myconst
import DataGen as datagen
import time
import printresult

srcKernel = '''
void kernel knapsack(global uint *input_f, global uint *output_f, global uint *m_d, uint cmax, uint weightk, uint pk, uint maxelem, uint i){
                if(get_global_id(0)<maxelem){
                    int c = get_global_id(0)+cmax;
                    if(input_f[c] < input_f[c - weightk] + pk){
                        output_f[c] = input_f[c - weightk] + pk;
                        m_d[c] += 1 << i;
                    }
                    else{
                        output_f[c] = input_f[c];
                    }
                }
            }
'''


class KnapsackOcl:

    verbose = None
    devices = None
    work_group_size = None
    context = None
    program = None
    kernel = None
    queue = None
    f0_mem = None
    f1_mem = None
    m_d_mem = None
    M = None
    executiontime = None
    printonce = 0

    def __init__(self, numelem, printonce=False, verbose=False):
        print()
        self.printonce = printonce
        self.verbose = verbose

        self.datagen = datagen.Data(numelem, verbose=self.verbose)

    def generate_platforms(self):

        platforms = cl.get_platforms()
        if self.verbose or self.printonce:
            print("PLATFORM_NAME: {0}".format(platforms[0].name))
        if self.verbose and self.printonce:
            print("PLATFORM_VERSION: {0}".format(platforms[0].version))
            print("PLATFORM_EXTENSIONS: {0}".format(platforms[0].extensions))

        return platforms

    def generate_context_build_program(self, device):
        """ Create Context; Creating Program; Build Kernel

        1. prints device info
        2. create context associated to device
        3. create program associated to source kernel code
        4. build program
        """

        print("DEVICE_NAME: {0}".format(device.name))
        """print device info"""
        if self.verbose:
            print("DEVICE_GLOBAL_MEM_SIZE: {0}".format(device.global_mem_size//1024//1024), 'MB')
            print("MAX_WORK_GROUP_SIZE: ", device.max_work_group_size)
            print("MAX_WORK_ITEM_SIZES: ", device.max_work_item_sizes)

        self.work_group_size = device.max_work_group_size
        if self.printonce:
            print("Work Group Size: ", self.work_group_size)

        self.context = cl.Context([device])
        """create context"""

        if self.verbose or self.printonce:
            print("Building kernel from source code: ")
            print("***********************************")
            print(srcKernel)

        self.program = cl.Program(self.context, srcKernel)
        """associates kernel source code to program"""

        try:
            self.program.build(["-cl-no-signed-zeros"])
            """build program with the specific flag """
        except:
            print("Build log:")
            print(self.program.get_build_info(device, cl.program_build_info.LOG))
            """"in case of failure prints error to stdout"""
            raise
        else:
            if self.verbose:
                print("Program has been built!")

        self.queue = cl.CommandQueue(self.context)
        """associates context to queue"""
        self.kernel = cl.Kernel(self.program, "knapsack")
        """creates kernel from built program"""

    def generate_memory_buffers_transfer_to_device(self):

        # create opencl buffer
        self.f0_mem = cl.Buffer(self.context,
                                cl.mem_flags.READ_WRITE | cl.mem_flags.COPY_HOST_PTR,
                                size=self.datagen.f0.nbytes,
                                hostbuf=self.datagen.f0)
        self.f1_mem = cl.Buffer(self.context,
                                cl.mem_flags.READ_WRITE | cl.mem_flags.COPY_HOST_PTR,
                                size=self.datagen.f1.nbytes,
                                hostbuf=self.datagen.f1)
        self.m_d_mem = cl.Buffer(self.context,
                                 cl.mem_flags.WRITE_ONLY | cl.mem_flags.COPY_HOST_PTR,
                                 self.datagen.m_d.nbytes,
                                 hostbuf=self.datagen.m_d)

        # write/transfer buffer synchronously (is_blocking) to device memory
        cl.enqueue_copy(self.queue, self.f0_mem, self.datagen.f0, is_blocking=True)
        cl.enqueue_copy(self.queue, self.f1_mem, self.datagen.f1, is_blocking=True)
        cl.enqueue_copy(self.queue, self.m_d_mem, self.datagen.m_d, is_blocking=True)

    def execute_on_device(self):

        # for each device we have to reinitialize variables
        row = 0
        k = 0
        i = 0
        self.M = numpy.array([]).astype(numpy.uint32)
        sumofweights = self.datagen.sumofweights
        valuesize = self.datagen.values.size
        CAPACITY = int(self.datagen.capacity)

        start = time.time()
        for k in range(0, valuesize, 1):

            weight_k = self.datagen.weights.take(k)
            value_k = self.datagen.values.take(k)

            sumofweights = sumofweights - weight_k
            if CAPACITY - sumofweights > weight_k:
                cmax = CAPACITY - sumofweights
            else: cmax = weight_k
            total_elements = numpy.int32(CAPACITY - cmax + 1)
            if total_elements > 0:

                power = k % 32

                if i % 2 == 0:

                    self.kernel.set_args(self.f0_mem,
                                         self.f1_mem,
                                         self.m_d_mem,
                                         numpy.uint32(cmax),
                                         weight_k,
                                         value_k,
                                         numpy.uint32(total_elements),
                                         numpy.uint32(power))
                    cl.enqueue_nd_range_kernel(self.queue, self.kernel, (CAPACITY,), None)

                else:

                    self.kernel.set_args(self.f1_mem,
                                         self.f0_mem,
                                         self.m_d_mem,
                                         numpy.uint32(cmax),
                                         weight_k,
                                         value_k,
                                         numpy.uint32(total_elements),
                                         numpy.uint32(power))
                    cl.enqueue_nd_range_kernel(self.queue, self.kernel, (CAPACITY,), None)

                i += 1

            # every 32 cycle transfer back data from device to host
            if k >= 31 and k % 32 == 31:
                row += 1
                # writes synchronously back from device buffer to host
                cl.enqueue_read_buffer(self.queue, self.m_d_mem, self.datagen.m_d, 0, is_blocking=True)
                # Send back empty buffer to device
                cl.enqueue_copy(self.queue, self.m_d_mem, numpy.zeros(CAPACITY+1).astype(numpy.uint32), is_blocking=True)
                row += 1
                self.M = numpy.append(self.M, self.datagen.m_d)

        # if elements are not (mod 32) then we need an extra transfer
        if valuesize % 32 != 0:
            if self.verbose:
                print("Value size is less then 32 or is not a mod of 32")
            cl.enqueue_read_buffer(self.queue, self.m_d_mem, self.datagen.m_d, 0, is_blocking=True)
            self.M = numpy.append(self.M, self.datagen.m_d)

        stop = time.time()
        self.executiontime = stop-start

        print("Time: ", stop-start)


if __name__ == "__main__":

    # generates elements [500..10000] at pace 500
    elements = [elem for elem in range(100, 10**(4-1)+1, 100)]

    for i, nelem in enumerate(elements):
        print("**************************************\n", nelem)
        printonce = False
        if i == 0:
             printonce = True
             
        ksack = KnapsackOcl(nelem, printonce, verbose=False)
        for platform in ksack.generate_platforms():
            devices = platform.get_devices(cl.device_type.ALL)
            for device in devices:
                ksack.generate_context_build_program(device)
                for x in range(1):
                    ksack.generate_memory_buffers_transfer_to_device()
                    ksack.execute_on_device()

                printresult.printresults(ksack.M,
                                         ksack.datagen.weights,
                                         ksack.datagen.values,
                                         ksack.datagen.capacity,
                                         ksack.datagen.values.size,
                                         ksack.executiontime)





