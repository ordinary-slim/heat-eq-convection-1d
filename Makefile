C=mpic++
OBJDIR=obj
SRCDIR=src

INCLUDE_PATH=$(HOME)/.local/include
LINK_PATH=$(HOME)/.local/lib
METIS_LINK=-lmetis

MHFiles=$(ls *.gch)
CFLAGS=-I$(INCLUDE_PATH)
LFLAGS=-L$(LINK_PATH) $(METIS_LINK)

default: smolFEM.$C

debug: CFLAGS+= -g
debug: LFLAGS+= -g
debug: smolFEM.g$C

clean:
	rm $(OBJDIR)/*

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(C) -c $(CFLAGS) -o $@ $<

smolFEM.$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o $(OBJDIR)/Initialize.o
	$(C) $(LFLAGS) -o $@ $^

smolFEM.g$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o $(OBJDIR)/Initialize.o
	$(C) $(LFLAGS) -o $@ $^

print:
	@echo $(METIS_INCLUDE)
