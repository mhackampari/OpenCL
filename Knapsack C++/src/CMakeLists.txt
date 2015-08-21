
#auto-discovers the files inside the directory
file(GLOB SRC *.cpp)
#include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(../include)
#add_definitions(-std=c++11) works
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall") 


#find_package( OpenCL REQUIRED )
#find_package(OpenCL)
#find_package(OpenCL)
#include_directories($OpenCL/include)
#link_directories($OpenCL/x86_64/lib)
#target_link_libararies(ese libopencl)

find_package( OpenCL REQUIRED )
message("OpenCL LIB dir: ${OpenCL_LIBRARY}")
find_package( OpenMP REQUIRED )
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


add_executable(ese ${SRC} )

# OK - target_include_directories(ese PUBLIC "${PROJECT_SOURCE_DIR}/include/")


#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})



message("SOURCE DIRECTORY")
message("Exe dir: ${EXECUTABLE_OUTPUT_PATH}")
message("SRC dir: ${PROJECT_SOURCE_DIR}")
message("{PROJECT_BINARY_DIR}: " ${PROJECT_BINARY_DIR})
message("{PROJECT_BINARY_DIR}: " ${PROJECT_BINARY_DIR})