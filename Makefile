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

default: smolFEM.$C

debug: CFLAGS+= -g
debug: LFLAGS+= -g
debug: smolFEM.g$C

clean:
	rm $(OBJDIR)/*

test:
	@echo "-------------------------------"
	@echo "Running tests: square.msh"
	mpirun -n 4 smolFEM.mpic++ gmsh_examples/square.msh
	@echo "-------------------------------"

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.cpp.o: %.cpp | $(OBJDIR)
	$(MCXX) -c $(CFLAGS) -o $@ $<

smolFEM.$C: $(OBJS)
	$(MCXX) -o $@ $^ $(LFLAGS) 

smolFEM.g$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o $(OBJDIR)/Initialize.o $(OBJDIR)/callMetis.o
	$(MCXX) -o $@ $^ $(LFLAGS) 

-include $(DEPS)
