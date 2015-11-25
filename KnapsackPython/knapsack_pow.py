import math

f = open('data500', 'r')

tmpArray = f.readline()
#take first line and extract second element of the array
myCap = int(tmpArray.split()[1])

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

NUMOBJ = int(math.ceil(num_obj()/32.0))
ROWS = NUMOBJ
COLS = capacity() + 1

#initialize Memory Matrix [#obj][C]
#for k 1..n and c 1...C
M = [0]*(COLS*ROWS)

f = [0 for c in range(capacity()+1)]
#print("\nfunction: ", f)



row = 0
k = 0

for k in range(0, len(items), 1):
    weight_k = items[k][1]
    value_k = items[k][0]

    sumW = sumW - weight_k
    if capacity()- sumW > weight_k:
        cmax = capacity() - sumW
    else: cmax = weight_k
    total_elements = capacity()-cmax+1

    if capacity()-cmax+1 > 0:
        print("\nK: {0}\n".format(k))
        print("sumW: {0} weight_k: {1} value_k: {2} k: {3} total_elements: {4}".format(sumW, weight_k, value_k, k, total_elements))

        power = k % 32
        i = 0
        for c in range(capacity(), cmax-1, -1):
            if f[c] < f[c - weight_k] + value_k:
                f[c] = f[c - weight_k] + value_k
                M[row * COLS + c] += pow(2 , power)
            i += 1
        #print("m_d: ", M)
        #print("f0: ", f)

    if k >= 31 and k % 32 == 31:
        row += 1



output_file = open("knapsack_pow.txt", "w")

#for x in range(len(items)):
#    print('%d: '%x , M[x])

c = capacity()
bit32 = 32
worth = []
capacita = []




for i in range(NUMOBJ-1, -1, -1):
    bit32 = 32
    print("**numobj%32**\n")
    output_file.write("**numobj%32**\n")

    while bit32 > 0 and c > 0:
        print(c , bit32)
        m = M[i*COLS + c]
        bit32pw = pow(2,(bit32-1))

        if bit32pw == (bit32pw & m):
            weight = items[i*32+bit32-1][1]
            value = items[i*32+bit32-1][0]
            #print("[%d] value:  \tweight: "%(i*32+bit32-1))
            #print("M[%d]: %d\n"%(i*COLS + c, M[i*COLS + c]))
            output_file.write("[%d] value: %d \tweight: %d\n"%(i*32+bit32-1, value, weight))
            c -= weight
            capacita.append(weight)
            worth.append(value)

        bit32 -= 1

print("\nKnapsack Value: ",sum(worth))
print("Knapsack weight: ",sum(capacita))
print("Worth aray:{0}\nWeight aray:{1}\n".format(worth,capacita))

output_file.write("\nKnapsack Value: %s \nKnapsack weight: %s"%(str(sum(worth)), str(sum(capacita))))
output_file.close
