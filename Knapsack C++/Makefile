
NAME = knapsack++ 

CXX=g++ 
CFLAGS = -Wall -std=c++11
DEBUG ?= 1
ifeq ($(DEBUG), 1)
CFLAGS += -g -O0 -DDEBUG
else
CFLAGS += -O3
endif
LDFLAGS = -fopenmp
all: $(NAME)

$(NAME): main.o
	$(CXX) $(LDFLAGS) $^ -o $@ 
%.o: %.cpp
	$(CXX) -c $< $(CFLAGS) $(LDFLAGS)

clean: 
	rm *.o
	rm $(NAME)


