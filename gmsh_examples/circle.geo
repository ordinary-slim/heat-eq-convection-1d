//+
SetFactory("OpenCASCADE");
Circle(1) = {0, 0, 0, 1, 0, 2*Pi};
//+
Curve Loop(1) = {1};
//+
Plane Surface(1) = {1};
//+
Physical Surface("boundary", 2) = {1};
//+
Physical Surface(" boundary", 2) -= {1};
//+
Physical Curve("boundary", 3) = {1};
