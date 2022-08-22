MCXX=mpic++

SRCDIR=./src
OBJDIR=./obj

SRCS=$(shell find $(SRCDIR) -name *.cpp)
OBJS=$(SRCS:%=$(OBJDIR)/%.o) 
DEPS=$(OBJS:%.o=%.d)

INCLUDE_PATH=$(HOME)/.local/include
LINK_PATH=$(HOME)/.local/lib
METIS_LINK=-lmetis -lGKlib

CFLAGS=-I$(INCLUDE_PATH) -MMD -MP
LFLAGS=-L$(LINK_PATH) $(METIS_LINK)

default: smolFEM.$(MCXX)

debug: CFLAGS+= -g
debug: LFLAGS+= -g
debug: smolFEM.g$(MCXX)

clean:
	rm $(OBJDIR)/*

test:
	@echo "-------------------------------"
	@echo "Running tests: square.msh"
	mpirun -n 4 smolFEM.mpic++ gmsh_examples/square.msh
	@echo "-------------------------------"

$(OBJDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(MCXX) $(CFLAGS) -c $< -o $@

smolFEM.$(MCXX): $(OBJS)
	$(MCXX) -o $@ $^ $(LFLAGS) 

smolFEM.g$(MCXX): $(OBJS)
	$(MCXX) -o $@ $^ $(LFLAGS) 

-include $(DEPS)
