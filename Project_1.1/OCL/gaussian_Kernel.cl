
__kernel void gaussian(  __global uchar* inputImage,  __global float* mask, int masksize,  __global uchar* outputImage)
{
    int cols = get_global_id(1); //get_global_id(0) performances decrease to 1.6 s
	int rows = get_global_id(0);
	int width = get_global_size(0);
	int height = get_global_size(1);
    int maskSize = masksize;
	int r1, c1;

    //uchar elem = inputImage[i];
    //float felem = convert_float(elem);

	
    float sum=0;
     
    int w, h;
    
    w = cols - maskSize>=0 ? 0 :  maskSize - cols;
    h = rows-maskSize>=0 ? 0 : maskSize - rows;
    
    for(r1=(rows-maskSize>=0) ? rows-maskSize:0; r1<=rows+maskSize && r1<height && h<2*maskSize+1; r1++){
        w = cols - maskSize>=0 ? 0 : maskSize - cols;
      
        for(c1=(cols-maskSize>=0) ? cols-maskSize:0; c1<=cols+maskSize && c1<width && w<2*maskSize+1; c1++){
            
            sum += mask[w + (2*maskSize+1)*h]*convert_int(inputImage[r1*width+c1]);
            w++;
        }
        
        h++;
      }
      //if the percentage of foreground pixels is lower than a given threshold the pixel is set as background
      
    outputImage[rows*width+cols]=convert_uchar(sum);  
		
   
}
// TODO: Add OpenCL kernel code here.