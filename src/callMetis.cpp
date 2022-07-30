#include <iostream>
#include <vector>
#include "mesh.h"
#include <metis.h>

void callMetis(vector<int> epart, vector<int> npart, Mesh m, int nparts){
  //out
  int objval;

  // aux
  int auxepart[epart.size()];
  int auxnpart[npart.size()];
  int eptr[m.nels+1];
  eptr[0] = 0;

  vector<int> veidx;

  int counter = 0;
  veidx.reserve( m.c[0].size() * m.nels );
  for (vector<int> elc : m.c) {
    counter++;
    eptr[counter] = eptr[counter-1] + elc.size();
    veidx.insert(veidx.end(), elc.begin(), elc.end());
  }
  //convert veidx (vector) to eidx (array)
  int eidx[veidx.size()];
  for (int i = 0; i < veidx.size(); i++){
    eidx[i] = veidx[i];
  }

  int ret = METIS_PartMeshNodal( &m.nels, &m.nnodes, eptr, eidx,
      NULL, NULL, &nparts, NULL, NULL, &objval, auxepart, auxnpart);
  //vector to array
  for (int i=0; i<epart.size(); i++){
    epart[i] = auxepart[i];
  }
  for (int i=0; i<npart.size(); i++){
    npart[i] = auxnpart[i];
  }

  for (int i = 0; i < m.nels; i++){
    printf("Element %d belongs to partition %d\n", i, epart[i]);
  }
  for (int i = 0; i < m.nnodes; i++){
    printf("Node %d belongs to partition %d\n", i, npart[i]);
  }
}
