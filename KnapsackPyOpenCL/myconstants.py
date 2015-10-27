__author__ = 'terminator'

import numpy
import math

NUMELEM = int(10)
CAPACITY = int(10)
DATA_POINTS = int(10)
NUMOBJ = int(math.ceil(NUMELEM/32.0))

f0 = numpy.zeros(CAPACITY+1).astype(numpy.uint32)
f1 = numpy.empty_like(f0)
m_d = numpy.empty_like(f0)
values = numpy.random.random_integers(1,20,size=NUMELEM).astype(numpy.uint32)
weights = numpy.random.random_integers(1,5,size=NUMELEM).astype(numpy.uint32)
sumW = weights.sum()

srcKernel = '''
void kernel knapsack(global uint *input_f, global uint *output_f, global uint *m_d, int cmax, uint weightk, uint pk, uint maxelem, uint i){
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
