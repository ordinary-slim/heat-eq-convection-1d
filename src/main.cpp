#include <iostream>
#include "readGmsh.h"
#include "InitializeMPI.h"

using namespace std;

int main(int argc, char *argv[]){
  int world_size;
  int world_rank;

  //initialize parallelization
  InitializeMPI(world_size, world_rank);

  Mesh mesh;
  string meshFile = argv[1];


  if(world_rank==0){
    //read mesh file into mesh object
    readGmsh(mesh, meshFile);
  }
  
  //print mesh
  //mesh.printNodes();
  //mesh.printEls();

  // Finalize the MPI environment.
  MPI_Finalize();
}
