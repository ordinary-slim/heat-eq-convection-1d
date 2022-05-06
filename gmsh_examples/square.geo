// Gmsh project created on Mon May 02 17:56:04 2022
SetFactory("OpenCASCADE");
//+
Rectangle(1) = {-1, -1, 0, 2, 2, 0};
//+
Physical Curve("boundary", 5) = {4, 3, 2, 1};
//+
Physical Surface("interior", 6) = {1};
//+
MeshSize {4, 1, 2, 3} = 1;
