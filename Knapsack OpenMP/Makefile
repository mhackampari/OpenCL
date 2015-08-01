#gcc -fopenmp filename.cpp
#For bash shell: export OMP_NUM_THREADS=4

# add option: -g after -Wall to have symbolic information for gdb
# -lm to compile with math lib
#g++ -Wall main.cpp -o knapsack
NAME = knapsack_openmp 

CXX=g++ 
CFLAGS = -Wall -O3 -std=c++11
LDFLAGS = -fopenmp
all: $(NAME)

$(NAME): knapsack_openmp.o
	$(CXX) $(LDFLAGS) $^ -o $@ 
%.o: %.cpp
	$(CXX) -c $< $(CFLAGS) -fopenmp

clean: 
	rm *.o
	rm $(NAME)


