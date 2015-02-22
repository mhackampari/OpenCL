__kernel void downsample(__global uchar* image, __global uchar* outputImage, int width, int height, float factor_cols, float factor_rows){
    //image vector containing original RGB values
    //outputImage vector containing "downsampled" RGB mean values
	//factor - downsampling factor, downscaling the image by factor: 1024*1024 -> 1024/factor * 1024/factor
    int c = get_global_id(0);
    int r = get_global_id(1); //current coordinates
    int oWidth = get_global_size(0);

    int olc, ohc, olr, ohr; //coordinates of the original image used for bilinear interpolation
    int index; //linearized index of the point  
    uchar q11, q12, q21, q22;
    float accurate_c, accurate_r; //the exact scaled point
    int k;

    accurate_c = convert_float(c*factor_cols);
    olc=convert_int(accurate_c);
    ohc=olc+1;

    if(!(ohc<width))
    ohc=olc;

    accurate_r = convert_float(r*factor_rows);
    olr=convert_int(accurate_r);
    ohr=olr+1;
    
    if(!(ohr<height))
    ohr=olr;
            
    index= (c + r*oWidth)*3; //3 bytes per pixel
	//Compute RGB values: take a central mean RGB values among four points 
    for(k=0; k<3; k++){
        q11=image[(olc + olr*width)*3+k];
        q12=image[(olc + ohr*width)*3+k];
        q21=image[(ohc + olr*width)*3+k];
        q22=image[(ohc + ohr*width)*3+k];
        outputImage[index+k] = convert_uchar((q11*(ohc - accurate_c)*(ohr - accurate_r) + 
                                              q21*(accurate_c - olc)*(ohr - accurate_r) + 
                                              q12*(ohc - accurate_c)*(accurate_r - olr) +
                                              q22*(accurate_c - olc)*(accurate_r - olr)));
      }


  
}
