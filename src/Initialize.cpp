#include "problem.h"
#include <algorithm>

void Problem::Initialize(){
  cout << "Initializing problem variables ..." << endl;
  mesh.p.resize(mesh.nnodes);//implicit assignment to 0, gotta correct
  //ad-hoc BC application: Dirichlet all boundary nodes
  for (int i=0; i < mesh.nfaces; i++){
    for (int j: mesh.b[i]) {
      mesh.p[j] = 1;
    }
  }
}
