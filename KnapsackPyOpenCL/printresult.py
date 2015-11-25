__author__ = 'terminator'

#from myconstants import *
from math import ceil
#TODO: write results to file

def printresults(M, weights, values, capacity, numelem, chrono, cycles):

    CAPACITY = capacity
    NUMOBJ = ceil(numelem/32)
    c = CAPACITY
    worth = []
    capacita = []

    #print(M, len(weights))
    
    for i in range(NUMOBJ-1, -1, -1):

        bit32 = 32
    
        #print("**total_elements%32**\n")
    
        while bit32 > 0 and c > 0:

            m = M[i*(CAPACITY+1) + c]
            bit32pw = pow(2, (bit32-1))

            if bit32pw == (bit32pw & m):

                weight = weights[i*32+bit32-1]
                value = values[i*32+bit32-1]
                c -= weight
                capacita.append(weight)
                worth.append(value)
                #print("KNAPSACK VALUE: ", worth)
    
            bit32 -= 1

    print("\n**********************************")
    print("Elapsed average time: ", chrono/cycles)
    print("Knapsack Value: %d" % sum(worth))
    print("Knapsack weight: %d" % sum(capacita))
    print("Knapsack capacity: %d" % CAPACITY)
    print("Worth aray:{0}\nWeight aray:{1}\n".format(worth, capacita))
