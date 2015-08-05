#	http://www.gnu.org/software/make/manual/make.html
#	http://makefiletutorial.com/
#
#	target: dependecies
#		command

#	$@: the target filename.
#	$*: the target filename without the file extension.
#	$<: the first prerequisite filename.
#	$^: the filenames of all the prerequisites, separated by spaces, discard duplicates.
#	$+: similar to $^, but includes duplicates.
#	$?: the names of all prerequisites that are newer than the target, separated by spaces.
##################################################

NAME = knapsack++ 
CXX = g++ 
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -fopenmp
SRCS = main.cpp
# $(shell find . -name "*.cpp") finds all cpp files in the current directory
# $(notdir files ...) removes all "/" and path
SRCS = $(notdir $(shell find . -name "*.cpp"))
OBJS = $(SRCS:.cpp=.o)
MKDIR = mkdir -p ./obj
RMDIR = rm -rf ./obj
RMFILES = rm -f $(NAME) $(OBJS) *.txt


#	?= only sets variables if they have not yet been defined
DEBUG ?= 1
ifeq ($(DEBUG), 1)
DEBUG_FLAGS += -g3 -O0 -DDEBUG
CXXLAGS += $(DEBUG_FLAGS) 
else
RELEASE += -O3
CXXFLAGS += $(RELEASE)
endif

####################################################
all: mkdir $(NAME)

mkdir:
	$(MKDIR)
	echo $(SRCS)

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ 
%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(LDFLAGS)

clean: 
	$(RMFILES)
	$(RMDIR)


