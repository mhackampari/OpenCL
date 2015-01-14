__kernel void motion_detection(__global uchar* bgImage, __global uchar* motionImage,__global uchar* outputImage)
{
    
    int i = get_global_id(0);
    int tsh = 7;
    uchar bgEl = bgImage[i];
    uchar motionEl = motionImage[i];
	
	
    float bg = convert_float(bgEl);
    float mo = convert_float(motionEl);
    float temp = fabs(bg-mo);

	if(temp<tsh){ 
    temp=255.0;
	}else
    {
    temp=0.0;
    }
	
    outputImage[i] = convert_uchar(temp);//convert_uchar(temp);
}


