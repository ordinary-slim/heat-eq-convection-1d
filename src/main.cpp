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


  //read mesh at rank 0
  if(world_rank==0){
    //read mesh at rank 0
    readGmsh(p.mesh, meshFile);
  }
  //bcast size of mesh
  MPI_Bcast( &p.mesh.nnodes, 1, MPI_INT, 0, MPI_COMM_WORLD );
  MPI_Bcast( &p.mesh.nels, 1, MPI_INT, 0, MPI_COMM_WORLD );

  //partitioning
  if(world_size>1){
    vector<int> epart;//element partitioning
    vector<int> npart;//node partitioning
    epart.resize( p.mesh.nels );
    npart.resize( p.mesh.nnodes );
    //call METIS for mesh paritionioning at rank 0
    if (world_rank==0){
      callMetis(epart, npart, p.mesh, world_size);
    }
    //bcast partitionning
    MPI_Bcast( &epart[0], epart.size(), MPI_INT, 0, MPI_COMM_WORLD );
    MPI_Bcast( &npart[0], npart.size(), MPI_INT, 0, MPI_COMM_WORLD );

    /*
    if (world_rank==2){
      for (int i = 0; i < p.mesh.nels; i++){
        printf("Element %d belongs to partition %d\n", i, epart[i]);
      }
      for (int i = 0; i < p.mesh.nnodes; i++){
        printf("Node %d belongs to partition %d\n", i, npart[i]);
      }
    }
    */
    //Quick and dirty
    //Bcast whole mesh then throw out what is not needed
    // TODO

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

