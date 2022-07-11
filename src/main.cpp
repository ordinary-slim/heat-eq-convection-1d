#include <iostream>
#include "readGmsh.h"
#include "metisInterface.h"
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
    //read mesh at rank 0
    readGmsh(p.mesh, meshFile);
    //call METIS for mesh paritionioning at rank 0
    if (world_size>1){
      callMetis(p.mesh, world_size);
    }
    //p.mesh.printNodes();
    //p.mesh.printEls();
    //p.mesh.printFaces();

    p.Initialize();//serial, can be parallelized
  }

  printf("Processor %d waiting for other processors\n", world_rank);
  MPI_Barrier(MPI_COMM_WORLD);

  // ASSEMBLY
  // Allocate global problem matrices
  // Iterate over elements
  // LINEAR SYS SOLUTION
  
  // Finalize the MPI environment.
  MPI_Finalize();
}

