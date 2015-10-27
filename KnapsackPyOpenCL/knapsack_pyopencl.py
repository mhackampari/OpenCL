__author__ = 'terminator'

import pyopencl as cl
import numpy
from myconstants import *
import printresult as myprint


print("values: ", values)
print("weights: ", weights)
print("sumW: ", sumW)

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

WG = devices[0].max_work_group_size

context = cl.Context(devices)
program = cl.Program(context, srcKernel)

try:
    program.build()
except:
    print("Build log:")
    print(program.get_build_info(devices[0],cl.program_build_info.LOG))
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
    kernel.get_work_group_info(cl.kernel_work_group_info.PREFERRED_WORK_GROUP_SIZE_MULTIPLE, devices[0])

print("Kernel: ", kernel.function_name)
print("preferred_multiple_size: ", preferred_multiple)

if WG % preferred_multiple:
    print("Number of workers not a preferred multiple (%d*N)." \
                    % (preferred_multiple))
    print("Performance may be reduced.")

#print (kernel.get_info())

cl.enqueue_copy(queue, f0_mem, f0, is_blocking=True)
cl.enqueue_copy(queue, f1_mem, f1, is_blocking=True)
cl.enqueue_copy(queue, m_d_mem, m_d, is_blocking=True)

row = 0
k = 0
f = numpy.zeros_like(f0)
M = numpy.zeros_like(m_d)

for k in range(0, values.size, 1):

    weight_k = weights.take(k)
    value_k = values.take(k)
    print(sumW,weight_k,k)
    sumW = sumW - weight_k
    print(sumW,weight_k,k)
    if CAPACITY - sumW > weight_k:
        cmax = CAPACITY - sumW
    else: cmax = weight_k

    power = k % 32

    for c in range(CAPACITY, int(cmax-1), -1):
        print("C: ",c)
        if f[c]<f[c - weight_k] + value_k:
            f[c] = f[c - weight_k] + value_k
            M[row * CAPACITY + c] += pow(2 ,power)
            print("M", M[c])

    if k >= 31 and k % 32 == 31:
        row += 1


"""
kernel.set_arg(0, )
#kernel.set_arg()
#cl.enqueue_nd_range_kernel()
cl.enqueue_read_buffer(queue, m_d_mem, m_d).wait()
"""


myprint.printresults(M)
