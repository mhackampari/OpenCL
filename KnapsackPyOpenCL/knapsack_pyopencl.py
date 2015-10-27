__author__ = 'terminator'

import pyopencl as cl

srcKernel = '''
void kernel knapsack(global int *input_f, global int *output_f, global uint *m_d, int cmax, int weightk, int pk, int maxelem, int i){
                int c = get_global_id(0)+cmax;
                if(get_global_id(0)<maxelem){
                    if(input_f[c] < input_f[c - weightk] + pk){
                        output_f[c] = input_f[c - weightk] + pk;
                        m_d[c-1] += pown(2.0,i);
                    }
                    else{
                    output_f[c] = input_f[c];
                    }
                }
            }
'''

platforms = cl.get_platforms()

print("PLATFORM_NAME: {0}".format(platforms[0].name))
print("PLATFORM_VERSION: {0}".format(platforms[0].version))
print("PLATFORM_EXTENSIONS: {0}".format(platforms[0].extensions))

devices = platforms[0].get_devices(cl.device_type.ALL)

for i in devices:
    print("DEVICE_NAME: {0}".format(i.name))
    print("DEVICE_GLOBAL_MEM_SIZE: {0}".format(i.global_mem_size))
    #print(devices)

context = cl.Context(devices)

program = cl.Program(context, srcKernel).build()

queue = cl.CommandQueue(context)

mem_flags = cl.mem_flags

