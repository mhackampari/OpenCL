/*
The erosion filter is used to reduce the noise in the image produced by the motion detection algorithm
*/

__kernel void erosion( __global uchar* inputImage,__global uchar* outputImage)
{
    int cols = get_global_id(0);
	int rows = get_global_id(1);
	int width = get_global_size(0);
	int height = get_global_size(1);
	int r1, c1;

	float tsh = 0.7;
	int erosionSize = 7; //matrix 7X7

    //uchar elem = inputImage[i];
    //float felem = convert_float(elem);
    int marker=0;
    int count=0;

      //for each pixel, we count how many pixels in the neighbor positions (we consider a 7x7 surrounding matrix) belong to the foreground (we have to accurately manage pixels on the borders)
    for(r1=(rows-(erosionSize-1)/2>0)?(rows-(erosionSize-1)/2):0; r1<rows+(erosionSize-1)/2 && r1<height; r1++){
        
        for(c1=(cols-(erosionSize-1)/2>0)?cols-(erosionSize-1)/2:0; c1<cols+(erosionSize-1)/2 && c1<width; c1++){
           
           count++;
           marker+=(convert_int(inputImage[r1*width+c1])==255)? 255:0;
           }
        
        } 
	         
     //if the percentage of foreground pixels is lower than a given threshold the pixel is set as background
     if(convert_float(marker/count)>tsh*255.0){
        outputImage[rows*width+cols]=convert_uchar(255);
		} else {
        outputImage[rows*width+cols]=convert_uchar(0);  
		}        
   
}

