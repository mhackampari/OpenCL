
void kernel knapsack(int capacity, int sumW, global int *value, global int *weight,  global int* M, global int *f){
    size_t gid = get_global_id(0);
    int cmax = 0;
    
    sumW = sumW - weight[gid];
    
    if(capacity - sumW > weight[gid])
        cmax = capacity - sumW;
        else cmax = weight[gid];
        
    for(int c = capacity; c > cmax-1; c--){
        if(f[c] < f[c - weight[gid]] + value[gid]){
            f[c] = f[c - weight[gid]] + value[gid];
            M[c-1+capacity*gid] = 1;
        }
    }
}