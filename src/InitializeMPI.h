#include <mpi.h>

int InitializeMPI(int &world_size, int &world_rank){
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Print off a hello world message
  if (world_rank==0){
    printf("Launched job on %d processors.", world_size);
  }

  return 1;
}
