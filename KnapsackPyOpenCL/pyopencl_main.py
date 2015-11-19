import numpy
import pyopencl as cl
import myconstants as myconst
import DataGen as datagen

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
    printonce = 0

    def __init__(self, numelem, printonce=False, verbose=False):
        print()
        self.printonce = printonce
        self.verbose = verbose

        self.datagen = datagen.Data(numelem, verbose=self.verbose)

    def generate_platforms(self):

        platforms = cl.get_platforms()

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
        if self.verbose or self.printonce:
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

        if self.verbose  or self.printonce:
            print("Building kernel from source code: ")
            print("***********************************")
            print(myconst.srcKernel)

        self.program = cl.Program(self.context, myconst.srcKernel)
        """associates kernel source code to program"""

        try:
            self.program.build(["-cl-no-signed-zeros"])
            """build program with the specific flag """
        except:
            print("Build log:")
            print(self.program.get_build_info(device, cl.program_build_info.LOG))
            """"in case of failure stdouts error """
            raise
        else:
            print("Program has been built!")

        self.queue = cl.CommandQueue(self.context)
        """associates context to queue"""
        self.kernel = cl.Kernel(self.program, "knapsack")
        """creates kernel from built program"""

    def generate_memory_buffers_transfer_to_device(self):
        print(self.datagen.f0.nbytes)

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

        cl.enqueue_copy(self.queue, self.f0_mem, self.datagen.f0, is_blocking=True)
        cl.enqueue_copy(self.queue, self.f1_mem, self.datagen.f1, is_blocking=True)
        cl.enqueue_copy(self.queue, self.m_d_mem, self.datagen.m_d, is_blocking=True)


if __name__ == "__main__":
    # generates elements [500..10000] at pace 500
    elements = [elem for elem in range(500, 10**4+1, 500)]

    for i, num_elements in enumerate(elements):
        print("**************************************\n", elements)
        if i == 0:
             printonce = True
             
        ksack = KnapsackOcl(num_elements, printonce, verbose=False)
        for platform in ksack.generate_platforms():
            devices = platform.get_devices(cl.device_type.ALL)
            for device in devices:
                ksack.generate_context_build_program(device)
                for x in range(100):
                    ksack.generate_memory_buffers_transfer_to_device()





