void kernel knapsack(global int *input_f, global int *output_f, global int *m_d,  int cmax, int weightk, int pk, int maxelem){
    
    int c = get_global_id(0)+cmax;
        
    if(get_global_id(0)<maxelem){
        if(input_f[c] < input_f[c - weightk] + pk){
            output_f[c] = input_f[c - weightk] + pk;
            m_d[c-1] = 1;
        } 
        else{
        output_f[c] = input_f[c];
        /*m_d[c-1] = 1;*/
        }   
    }
}