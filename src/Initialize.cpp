#include "problem.h"
#include <algorithm>

void Problem::Initialize(){
  mesh.p.resize(mesh.nfaces);
  //ad-hoc BC application: Dirichlet all boundary nodes
  for (int i=0; i < mesh.nfaces; i++){
    /*
    std::for_each(mesh.b[i].begin(), mesh.b[i].end(),
        [](int * j){
          cout << j << endl;
        }
    );
    */
    /*
    for (int j: mesh.b[i]) {
      cout << j << ", ";
    }
    cout << endl;
    */
  }
}
