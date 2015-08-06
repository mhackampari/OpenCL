
NAME = knapsack
BUILDIR = build
VPATH = src:build
SRCS := $(basename $(shell find ./src -name "*.cpp")) # := VS =
OBJS := $(SRCS:.cpp=.o)

CXX = g++ 
LDFLAGS = -fopenmp

MKDIR = mkdir -p
RM = rm -rf
RMFILES = $(RM) $(wildcard $(NAME)*) 
RMDIR = $(RM) $(BUILDIR)

##############################################

DBGNAME = $(addsuffix _dbg, $(NAME))
DBGDIR = $(addprefix $(BUILDIR)/, debug)
DBGOBJ = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCXXFLAGS = -g3 -O0 -DDEBUG -std=c++11

############################

RLSNAME = $(addsuffix _rls, $(NAME))
RLSDIR = $(addprefix $(BUILDIR)/, release)
RLSOBJ = $(addprefix $(RLSDIR)/, $(OBJS))
RLSCXXFLAGS = -O3 -std=c++11

#**********************************************************
.PHONY: all clean mkdir debug release help

all: debug release
	@echo "SETUP DIRECTORIES: OK"
	$(shell find . -type d -print)
	@echo "SOURCE FILES: " $(SRCS)
	

#**********************************************************
debug: mkdbgdir $(DBGNAME)

mkdbgdir:
	$(MKDIR) $(DBGDIR)

$(DBGNAME): $(DBGOBJ) #$(addprefix $(BUILDIR)/, $(OBJS))
	$(CXX) $(LDFLAGS) $^ -o $@
	
$(DBGDIR)/%.o: %.cpp
	$(CXX) -c $< $(DBGCXXFLAGS) $(LDFLAGS) -o $@

#**********************************************************
release: mkrlsdir $(RLSNAME)

mkrlsdir:
	$(MKDIR) $(RLSDIR)

$(RLSNAME): $(RLSOBJ) #$(addprefix $(BUILDIR)/, $(OBJS))
	$(CXX) $(LDFLAGS) $^ -o $@
	
$(RLSDIR)/%.o: %.cpp
	$(CXX) -c $< $(RLSCXXFLAGS) $(LDFLAGS) -o $@
	
#**********************************************************
clean: 
	$(RMFILES)
	$(RMDIR)

help:
	@echo "release - release version"
	@echo "debug - debug version"
	@echo "clean - removes build directory, binaries, and txt file"
	
	