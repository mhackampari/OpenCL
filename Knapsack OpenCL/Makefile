NAME = knapsack
CXX = g++
INCDIR = $(AMDAPPSDKROOT)/include/
LDFLAGS = -L$(AMDAPPSDKROOT)/lib/x86_64/ -lOpenCL
CXXFLAGS = -Wall -std=c++11
#run debug: DEBUG=1 make
DEBUG ?= 1
ifeq ($(DEBUG),1)
CXXFLAGS += -g -DDEBUG
	#@echo "ciao"
else
CXXFLAGS += -O3
endif

all: $(NAME)

$(NAME): Chrono.o knapsack_main.o knapsack_class.o TestData.o Timer.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) -I$(INCDIR)

clean :	
	rm -f *.o 
	rm -f $(NAME)

