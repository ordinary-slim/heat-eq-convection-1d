#include <iostream>
#include <vector>
#include "mesh.h"
#include <metis.h>

void callMetis(Mesh m, int nparts){
  //out
  int epart[m.nels];
  int npart[m.nnodes];
  int objval;

  // aux
  int eptr[m.nels+1];
  eptr[0] = 0;

  vector<int> veidx;

  int counter1 = 0;
  veidx.reserve( m.c[0].size() * m.nels );
  for (vector<int> elc : m.c) {
    counter1++;
    eptr[counter1] = eptr[counter1-1] + elc.size();
    veidx.insert(veidx.end(), elc.begin(), elc.end());
  }
  //convert veidx (vector) to eidx (array)
  int eidx[veidx.size()];
  for (int i = 0; i < veidx.size(); i++){
    eidx[i] = veidx[i];
  }

  int ret = METIS_PartMeshNodal( &m.nels, &m.nnodes, eptr, eidx,
      NULL, NULL, &nparts, NULL, NULL,
      &objval, epart, npart);
  for (int i = 0; i < m.nels; i++){
    printf( "Element %d belongs to partition %d\n", i, epart[i]);
  }
  for (int i = 0; i < m.nnodes; i++){
    printf( "Node %d belongs to partition %d\n", i, npart[i]);
  }
}
