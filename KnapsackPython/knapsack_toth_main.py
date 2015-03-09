__author__ = "terminator"
__date__ = "$Feb 16, 2015 7:03:15 PM$"

items=[[1,2],[3,3],[2,2]] #[valore, peso]
print("ITEMS:", items)

def capacity(): return 5 #constant value
def num_obj(): return len(items) #total number of objects
print("\nCAPACITY: ", capacity())
print("\nNUM OBJECTS: ", num_obj())

def weightTot(item=[]):
    sumW = 0
    for x in item:
        sumW += x[1]
    return sumW

sumW = weightTot(items)
print("\nTotal weight: ", sumW)
c = 0 #temp var for capacity
k=1 #temp var for obj
wmax = 0


#initialize Memory Matrix [#obj][C]
#for k 1..n and c 1...C
M = [[0 for x in range(capacity())] for x in range(num_obj())] 
print("M initialization:\n", M)

f = [0 for c in range(capacity()+1)]
print("\nfunction: ", f)

for k in range(len(items)):
    sumW = sumW - items[k][1]
    if capacity()- sumW > items[k][1]:
        cmax = capacity()- sumW
    else: cmax = items[k][1]
    print("\n---------------------------")
    for c in range(capacity(), cmax-1, -1):
        print("cmax: ", cmax, "c: ", c)
        if f[c]<f[c-items[k][1]]+items[k][0]:
            print("f[%d]: "%(c), f[c], "f[%d]: "%(c-1-items[k][1]), 
            f[c-items[k][1]], "weight: ", items[k][0], "value: ", items[k][0])
            
            f[c] = f[c-items[k][1]]+items[k][0]
            print("f[%d] = f[%d]+%d: "%(c, c-1-items[k][1], items[k][0] ), f[c])
            M[k][c-1]=1
    for c in range(capacity()):
        print("\n=============\nupdated function: ", f)
        
print(M)




