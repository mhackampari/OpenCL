/* 
 * File:   Knapsack.h
 * Author: terminator
 *
 * Created on February 21, 2015, 10:14 PM
 */

#ifndef KNAPSACK_H
#define	KNAPSACK_H



#endif	/* KNAPSACK_H */

#include <cstdlib>
#include <string>
#include<CL/cl.h>
#include<iostream>


class Knapsack{
    //variables
    
    const cl_platform_info platform_const_value[5] = {CL_PLATFORM_PROFILE, CL_PLATFORM_VERSION, CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_EXTENSIONS};
    const std::string platform_const_string[5] = {"CL_PLATFORM_PROFILE", "CL_PLATFORM_VERSION", "CL_PLATFORM_NAME", "CL_PLATFORM_VENDOR", "CL_PLATFORM_EXTENSIONS"};
    
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_int err;
    char *platform_info;
    size_t size;
    //initialize variables
    public:Knapsack(){
        
    }

    
    void queryOCLdevices ();
};