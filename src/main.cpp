#include <iostream>
#include "readGmsh.h"
#include "InitializeMPI.h"
#include "problem.h"

using namespace std;

int main(int argc, char *argv[]){
  Problem p;

  int world_size;
  int world_rank;

  //initialize parallelization
  InitializeMPI(world_size, world_rank);

  string meshFile = argv[1];


  if(world_rank==0){
    //read mesh file into mesh object
    readGmsh(p.mesh, meshFile);
    //p.mesh.printNodes();
    //p.mesh.printEls();
    //p.mesh.printFaces();
  }

  p.Initialize();
  // ASSEMBLY
  // Allocate global problem matrices
  // Iterate over elements
  // LINEAR SYS SOLUTION
  
  // Finalize the MPI environment.
  MPI_Finalize();
}

