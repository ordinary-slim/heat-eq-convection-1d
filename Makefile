C=g++
OBJDIR=obj
SRCDIR=src
MHFiles=$(ls *.gch)

default: smolFEM.$C

clean:
	rm $(OBJDIR)/*

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(C) -c -o $@ $<

smolFEM.$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o
	$(C) -o $@ $^
