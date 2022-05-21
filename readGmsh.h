#include <fstream>
#include <sstream>
#include <vector>
#include "mesh.h"


void ReadPhysicalNames( ifstream &file, Mesh &m, vector<int> & physicalTags );
void ReadEntities( ifstream &file, Mesh &m, vector<int> & physicalTags, vector<vector<int>> & physicalGroups);
void ReadNodes( ifstream &file, Mesh &m, vector<array<int, 2>> & nodeEntities );
void ReadElements( ifstream &file, Mesh &m, vector<array<int, 2>> & elementEntities  );
template <class T>
int readParams( stringstream &ss, T params[] );

int readGmsh(Mesh & mesh, string fileName);
