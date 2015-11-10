__author__ = 'terminator'

import numpy
import math
import sys

NUMELEM = int(10)
CAPACITY = int(10)
NUMOBJ = int(math.ceil(NUMELEM/32.0))

f0 = numpy.array([0]).astype(numpy.uint32)
f1 = numpy.array([0]).astype(numpy.uint32)
m_d = numpy.array([0]).astype(numpy.uint32)
#the same as above
values = numpy.zeros(1).astype(numpy.uint32)
weights = numpy.zeros(1).astype(numpy.uint32)
sumW = int(0)


try:
    input_file = open('datafile', 'r')
    lines = input_file.readlines()
    #first line at position 1, extract capacity
    CAPACITY = int(lines[0].split()[1])
    NUMELEM = len(lines[1:])
    NUMOBJ = int(math.ceil(NUMELEM/32.0))
    f0 = numpy.zeros(CAPACITY+1).astype(numpy.uint32)
    f1 = numpy.zeros_like(f0)
    m_d = numpy.zeros_like(f0)

    weights = numpy.zeros(NUMELEM).astype(numpy.uint32)
    values = numpy.zeros(NUMELEM).astype(numpy.uint32)

    #reading weights and values from line 1;
    i = 0
    for line in lines[1:]:
        weights[i] = line.split()[1]
        values[i] = line.split()[-1]
        i += 1

    sumW = weights.sum()

except OSError as error:
    print("INPUT DATA FILE NOT FOUND: ", error)
    print("Generating random numbers...")
    f0 = numpy.zeros(CAPACITY+1).astype(numpy.uint32)
    f1 = numpy.zeros_like(f0)
    m_d = numpy.zeros_like(f0)
    values = numpy.random.random_integers(1,20,size=NUMELEM).astype(numpy.uint32)
    weights = numpy.random.random_integers(1,5,size=NUMELEM).astype(numpy.uint32)
    sumW = weights.sum()
else:
    print("File has been found: opening '{0}'".format(input_file.name))
    print("Capacity: ", CAPACITY)
    print("NUMOBJECTS: ", NUMELEM)
    print("values: ", values)
    print("weights: ", weights)
    print("sumW: ", sumW)
    input_file.close()






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
