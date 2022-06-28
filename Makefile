C=mpic++
OBJDIR=obj
SRCDIR=src
MHFiles=$(ls *.gch)
CFLAGS=
LFLAGS=

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

smolFEM.$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o
	$(C) $(LFLAGS) -o $@ $^

smolFEM.g$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o $(OBJDIR)/Assemble.o
	$(C) $(LFLAGS) -o $@ $^
