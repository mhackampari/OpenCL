__author__ = 'terminator'

from myconstants import *

def printresults(M):
    c = CAPACITY
    bit32 = 32
    worth = 0
    capacita = 0

    print(M)
    
    for i in range(NUMOBJ-1, -1, -1):
        print("i",i)
        bit32 = 32
    
        print("**numobj%32**\n")
        #output_file.write("**numobj%32**\n")
    
        while bit32 > 0 and c > 0:
            print(c , bit32)
            m = M[i*CAPACITY + c]
            print("m",m)
            #print("M[%d][%d]: %d\n"%(i,c-1, M[i][c-1]))
            #x = int(math.ceil(math.log(m,2)))
    
            bit32pw = pow(2,(bit32-1))
            if bit32pw == (bit32pw & m):
    
                weight = weights[i*32+bit32-1]
                value = values[i*32+bit32-1]
    
                #print("M[%d]: %d\n"%(i*CAPACITY + c, M[i*CAPACITY + c]))
                print("[%d] value: %d \tweight: %d"%(i*32+bit32-1, value, weight))
                #output_file.write("[%d] value: %d \tweight: %d\n"%(i*32+bit32-1, value, weight))
    
                c -= weight
                capacita += weight
                worth += value
                print("KNAPSACK VALUE: ", worth)
    
            bit32 -= 1
    
    print("\nKnapsack Value: %d"%worth)
    print("Knapsack weight: %d"%capacita)
