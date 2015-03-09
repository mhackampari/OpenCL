items=[[3, 2], [4,4], [2,3], [1,1]] #[valore, peso]
print("ITEMS:", items)

def capacity(): return 7 #constant value
c = 0 #temp var for capacity
k=1 #temp var for obj
def num_obj(): return len(items) #total number of objects
print("CAPACITY:", capacity())
print("NUM OBJECTS:", num_obj())

#initialize Knapsack Matrix [#obj][C]
KM = [[0 for x in range(capacity())] for x in range(num_obj()+1)] 
print("KM initialization:\n", KM)

for k in range(num_obj()):
    valk = items[k][0]
    weightk = items[k][1]
    print("\nObject: ", k, "\tValue: ", valk, "\tWeight: ", weightk)
    for c in range(capacity()):
        if(c < weightk):
            KM[k+1][c] = KM[k][c]
        if(c >= weightk):
            if( KM[k][c-weightk]+ valk > KM[k][c]):
                KM[k+1][c] = KM[k][c] + valk
            else:
                KM[k+1][c] = KM[k][c]

print(KM)
                
         
        
    
    

