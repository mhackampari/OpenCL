cmake_minimum_required(VERSION 3.3.0)
project(Knapsack)
#cmake -DCMAKE_BUILD_TYPE=Debug ..
#cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..

add_subdirectory(src bin) #src files and build files




message("ROOT DIRECTORY")
message("EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")

message("{PROJECT_BINARY_DIR}: " ${PROJECT_BINARY_DIR})


