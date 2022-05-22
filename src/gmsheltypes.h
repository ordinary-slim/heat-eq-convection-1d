
using namespace std;

enum gmsheltypes {
  Line1 = 1,
  Triangle1 = 2,
  Quadrangle1 = 3,
  Tetrahedron1 = 4,
  Hexahedron1 = 5,
};

map<int, int> gmshNodesPerEl {{1, 2}, {2, 3}, {3, 4}, {4, 4}};
