#include <fstream>
#include <sstream>
#include <vector>
#include "mesh.h"

void readGmsh(Mesh &mesh, string fileName, vector<int> *npart= NULL, vector<int> *epart= NULL);
