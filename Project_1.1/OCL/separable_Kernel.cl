kernel void separable(global uchar* inputImage, global float* mask, int maskSize, global uchar* outputImage, global uchar* temp)
{
    int col = get_global_id(1); //get_global_id(0) performances decrease
	int row = get_global_id(0);
	int width = get_global_size(0);
	int height = get_global_size(1);

    
	int r1, c1;

    //uchar elem = inputImage[i];
    //float felem = convert_float(elem);

	
    float sum=0;
     
    int w, h, i;
    global int *k;
    
  
      

    sum=0;
    
    //if(col >= maskSize && col <= width - maskSize && row >= maskSize && row <= height - maskSize){ 
    for(i=0; i < 2*maskSize+1; i++){

		if((col + i - maskSize) < 0) w = i;
		else if((width - 1) - (col + i + maskSize) < 0) w =  width - 1 - i;
		else w = col + i - maskSize;

        sum += mask[i] * inputImage[row * width + w]; //col+i-maskSize
        
        }//columns
        temp[row*width + col] = sum;
        
        sum = 0;
    for(i=0; i < 2*maskSize+1; i++){

		if((row + i - maskSize) < 0) h = i;
		else if((width - 1) - (row + i + maskSize) < 0) h =  height - 1 - i;
		else h = row + i - maskSize;

        sum += mask[i] * temp[h * width + col];
        
        }//rows    
    outputImage[row*width+col]=convert_uchar(sum); 
   // }//end if    
  	
   
}/* Please Write the OpenCL Kernel(s) code here*/