#include <iostream>
#include "readGmsh.h"

using namespace std;

int main(int argc, char *argv[]){
  Mesh mesh;
  string meshFile = argv[1];
  //read mesh file into mesh object
  readGmsh(mesh, meshFile);
  
  mesh.printNodes();
}
