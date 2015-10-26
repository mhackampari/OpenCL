import math

f = open('datafile64', 'r')

tmpArray = f.next()
#take first line and extract second element of the array
myCap = int(tmpArray.split()[1])

print myCap

items = []

for line in f:
    tmpArray = line.split()
    items.append([int(tmpArray[-1]),int(tmpArray[1])])

#items=[[35,4],[4,3],[5,4],[5,2],[7,5]] #[valore, peso]
print("ITEMS:", items)

def capacity(): return myCap#9 #constant value
def num_obj(): return len(items) #total number of objects
print("\nCAPACITY: ", capacity())
print("\nNUM OBJECTS: ", num_obj())

def weight_tot(item=[]):
    sumW = 0
    for x in item:
        sumW += x[1]
    return sumW

sumW = weight_tot(items)
print("\nTotal weight: ", sumW)
c = 0 #temp var for capacity
k=1 #temp var for obj
wmax = 0


#initialize Memory Matrix [#obj][C]
#for k 1..n and c 1...C
numobj=int(math.ceil(num_obj()/32.0))
print(numobj)
M = [[0 for x in range(capacity())] for x in range(numobj)] 
#print("M initialization:\n", M)

f = [0 for c in range(capacity()+1)]
#print("\nfunction: ", f)

i = 0
k = 0

for y in range(len(items)):
    sumW = sumW - items[y][1]
    if capacity()- sumW > items[y][1]:
        cmax = capacity()- sumW
    else: cmax = items[y][1]
    
    i = y % 32
    if capacity() - cmax > 0:
        print("Main arguments k = %d power = %d value: %d weight: %d"%(y,i,items[y][0],items[y][1])) 
        print("total_threads: %d" %(capacity()-cmax+1))
    for c in range(capacity(), cmax-1, -1):
        if f[c]<f[c-items[y][1]]+items[y][0]:
            f[c] = f[c-items[y][1]]+items[y][0]
            M[k][c-1] += pow(2,i)
    
    if y >= 31 and y % 32 == 31:
        print M[k]
        print f
        k += 1


print M
print f
output_file = open("knapsack_pow.txt", "w")

#for x in range(len(items)):
#    print('%d: '%x , M[x])

c = capacity()
bit32 = 32
worth = 0
capacita = 0

for i in range(numobj-1, -1, -1):
    bit32 = 32 - 1
    
    print("**numobj%32**\n")
    output_file.write("**numobj%32**\n")

    while bit32 > 0 and c > 0:

        m = M[i][c-1]
        print("M[%d][%d]: %d\n"%(i,c-1, M[i][c-1]))
        #x = math.ceil(math.log(m,2))

        bit32pw = pow(2,(bit32-1))
        if bit32pw == (bit32pw & m):

            print("M[%d][%d]: %d\n"%(i,c-1, M[i][c-1]))
            print("[%d] value: %d \tweight: %d"%(i*32+bit32-1,items[i*32+bit32-1][0],items[i*32+bit32-1][1]))
            output_file.write("[%d] value: %d \tweight: %d\n"%(i*32+bit32-1, items[i*32+bit32-1][0],items[i*32+bit32-1][1]))

            weight = items[i*32+bit32-1][1]
            value = items[i*32+bit32-1][0]
            c -= weight
            capacita += weight
            worth += value

        bit32 -= 1

print("\nKnapsack Value: %d"%worth)
print("Knapsack weight: %d"%capacita)

output_file.write("\nKnapsack Value: %d \nKnapsack weight: %d"%(worth, capacita))
output_file.close