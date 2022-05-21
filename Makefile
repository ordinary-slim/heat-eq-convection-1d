C=g++
OFiles=$(ls *.o)
MHFiles=$(ls *.gch)

default: main.$(C)

clean:
	rm $(OFiles) $(MHFiles)

mesh.h.gch: mesh.h
	$(C) -c mesh.h

readGmsh.o: mesh.h.gch
	$(C) -c readGmsh.h
	$(C) -c readGmsh.cpp

main.$C: readGmsh.o
	$(C) -o main main.cpp $^
