C=g++
OBJDIR=obj
SRCDIR=src
MHFiles=$(ls *.gch)

default: smolFEM.$C

clean:
	rm $(OBJDIR)/*

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(C) -c -o $@ $<

smolFEM.$C: $(OBJDIR)/main.o $(OBJDIR)/readGmsh.o
	$(C) -o $@ $^
