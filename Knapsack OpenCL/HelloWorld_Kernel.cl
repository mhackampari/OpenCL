__kernel void helloworld(__global int* in, __global int* out)
{
int num = get_global_id(0);
out[num] = in[num] + 1;
}
