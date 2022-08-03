C=mpic++
OBJDIR=obj
SRCDIR=src

INCLUDE_PATH=$(HOME)/.local/include
LINK_PATH=$(HOME)/.local/lib
METIS_LINK=-lmetis -lGKlib

MHFiles=$(ls *.gch)
CFLAGS=-I$(INCLUDE_PATH)
LFLAGS=-L$(LINK_PATH) $(METIS_LINK)

default: smolFEM.$C test

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

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(C) -c $(CFLAGS) -o $@ $<

smolFEM.$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o $(OBJDIR)/Initialize.o $(OBJDIR)/callMetis.o
	$(C) -o $@ $^ $(LFLAGS) 

smolFEM.g$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o $(OBJDIR)/Initialize.o $(OBJDIR)/callMetis.o
	$(C) -o $@ $^ $(LFLAGS) 

print:
	@echo $(METIS_INCLUDE)
