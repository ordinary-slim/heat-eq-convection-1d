C=mpic++
OBJDIR=obj
SRCDIR=src
MHFiles=$(ls *.gch)
CFLAGS=
LFLAGS=
METIS_INCLUDE="$(HOME)/bin/METIS/include/"

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
