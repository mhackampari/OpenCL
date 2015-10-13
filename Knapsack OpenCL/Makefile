NAME = knapsack
CXX = g++
INCDIR = /opt/AMDAPPSDK-2.9-1/include/
CXXFLAGS = -Wall -g -O3 -std=c++11 -I$(INCDIR) 
LDFLAGS = -L/opt/AMDAPPSDK-2.9-1/lib/x86_64/ -lOpenCL

all: $(NAME)

$(NAME): knapsack_main.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(LDFLAGS) -lOpenCL

clean :	
	rm -f *.o 
	rm -f $(NAME)


#TARGET = $(shell find . -name "*.cpp" | head -n 1 | sed 's/\.cpp$$//')
#CXXFLAGS = -O3 -g -Wall -Werror -ansi



#%.o : %.cpp
#	$(CXX) $(CXXFLAGS) -Iinclude/ -c $< -o $@

#$(TARGET) : $(TARGET).o 
#	$(CXX) $(CXXFLAGS) $< -lOpenCL -lpng  -o $@

#clean :	
#	rm -f *.o $(TARGET)
