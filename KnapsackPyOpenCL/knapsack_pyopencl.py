ya__author__ = 'terminator'


import pyopencl as cl
import time
import timeit
from myconstants import *
import printresult as myprint
from chronometer import Chronometer

platforms = cl.get_platforms()

print("PLATFORM_NAME: {0}".format(platforms[0].name))
print("PLATFORM_VERSION: {0}".format(platforms[0].version))
print("PLATFORM_EXTENSIONS: {0}".format(platforms[0].extensions))

devices = platforms[0].get_devices(cl.device_type.ALL)

for device in devices:
    print("DEVICE_NAME: {0}".format(device.name))
    print("DEVICE_GLOBAL_MEM_SIZE: {0}".format(device.global_mem_size//1024//1024), 'MB')
    print("MAX_WORK_GROUP_SIZE: ",device.max_work_group_size)
    print("MAX_WORK_ITEM_SIZES: ", device.max_work_item_sizes)

    WG = device.max_work_group_size
    context = cl.Context([device])
    program = cl.Program(context, srcKernel)

    try:
        program.build(["-cl-no-signed-zeros"])
    except:
        print("Build log:")
        print(program.get_build_info(device,cl.program_build_info.LOG))
        raise
    else:
        print("Program has been built!")

    queue = cl.CommandQueue(context)
    mem_flags = cl.mem_flags

    f0_mem = cl.Buffer(context, cl.mem_flags.READ_WRITE | cl.mem_flags.COPY_HOST_PTR, size=f0.nbytes, hostbuf=f0)
    f1_mem = cl.Buffer(context, cl.mem_flags.READ_WRITE | cl.mem_flags.COPY_HOST_PTR, size=f1.nbytes, hostbuf=f1)
    m_d_mem = cl.Buffer(context, cl.mem_flags.WRITE_ONLY | cl.mem_flags.COPY_HOST_PTR, m_d.nbytes, hostbuf=m_d)

    kernel = cl.Kernel(program, "knapsack")
    global_size=(NUMELEM,)
    local_size=(WG,)
    preferred_multiple = \
        kernel.get_work_group_info(cl.kernel_work_group_info.PREFERRED_WORK_GROUP_SIZE_MULTIPLE, device)

    print("Kernel: ", kernel.function_name)
    print("preferred_multiple_size: ", preferred_multiple)

    if WG % preferred_multiple:
        print("Number of workers not a preferred multiple (%d*N)." \
              % (preferred_multiple))
        print("Performance may be reduced.")

    cl.enqueue_copy(queue, f0_mem, f0, is_blocking=True)
    cl.enqueue_copy(queue, f1_mem, f1, is_blocking=True)
    cl.enqueue_copy(queue, m_d_mem, m_d, is_blocking=True)

    row = 0
    k = 0
    i = 0
    f = numpy.zeros_like(f0)
    M = numpy.array([]).astype(numpy.uint32)

    start = time.time()
    chrono = Chronometer().start()

    for k in range(0, values.size, 1):

        weight_k = weights.take(k)
        value_k = values.take(k)

        sumW = sumW - weight_k
        if CAPACITY - sumW > weight_k:
            cmax = CAPACITY - sumW
        else: cmax = weight_k
        total_elements = CAPACITY - cmax + 1
        if total_elements > 0:

            power = k % 32

            if i%2 == 0:
                kernel.set_args(f0_mem,f1_mem, m_d_mem, numpy.uint32(cmax), weight_k, value_k, numpy.uint32(total_elements), numpy.uint32(power))
                cl.enqueue_nd_range_kernel(queue, kernel, (CAPACITY,),None)
            else:
                kernel.set_args(f1_mem, f0_mem, m_d_mem, numpy.uint32(cmax), weight_k, value_k, numpy.uint32(total_elements), numpy.uint32(power))
                cl.enqueue_nd_range_kernel(queue, kernel, (CAPACITY,),None)



            i += 1
        if k >= 31 and k % 32 == 31:
            row += 1
            cl.enqueue_read_buffer(queue, m_d_mem, m_d, 0, is_blocking=True)
            #Send back empty buffer to device
            cl.enqueue_copy(queue, m_d_mem, numpy.zeros(CAPACITY+1).astype(numpy.uint32), is_blocking=True)
            row += 1
            M = numpy.append(M,m_d)

    if values.size%32 != 0:
        print("Value size is less then 32 or is not a mod of 32")
        cl.enqueue_read_buffer(queue, m_d_mem, m_d, 0, is_blocking=True)
        M = numpy.append(M,m_d)

    stop = time.time()
    chrono.stop()

    print("Chrono: ", chrono.elapsed)
    print("Time: ", stop-start)


    myprint.printresults(M.tolist(), chrono.elapsed)
