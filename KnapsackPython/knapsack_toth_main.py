__author__ = "terminator"
__date__ = "$Feb 16, 2015 7:03:15 PM$"

f = open('datafile', 'r')

tmpArray = f.next()
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
M = [[0 for x in range(capacity())] for x in range(num_obj())] 
#print("M initialization:\n", M)

f = [0 for c in range(capacity()+1)]
#print("\nfunction: ", f)

for k in range(len(items)):
    sumW = sumW - items[k][1]
    if capacity()- sumW > items[k][1]:
        cmax = capacity()- sumW
    else: cmax = items[k][1]
    #print("\n---------------------------")
    for c in range(capacity(), cmax-1, -1):
        #print("cmax: ", cmax, "c: ", c)
        if f[c]<f[c-items[k][1]]+items[k][0]:
            #print("f[%d]: "%(c), f[c], "f[%d]: "%(c-1-items[k][1]), 
            #f[c-items[k][1]], "weight: ", items[k][1], "value: ", items[k][0])
            
            f[c] = f[c-items[k][1]]+items[k][0]
            #print("f[%d] = f[%d]+%d: "%(c, c-1-items[k][1], items[k][0] ), f[c])
            M[k][c-1]=1
    #for c in range(capacity()):
        #print("\n=============\nupdated function: ", f)
        
#print(M)
mc = 0
mv = 0

#Backtracking of the results
c = capacity()-1
for y in range(len(items)-1, -1, -1):
    for x in range(c, 0, -1):
        if(M[y][x]!=0):
            print("selected items: ", items[y])
            c = c - items[y][1]
            mc += items[y][1]
            mv += items[y][0]
            break
        if(M[y][x]==0):
            x=0
            break
            
print mv 
print mc










