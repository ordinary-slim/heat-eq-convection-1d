#ifndef problem_header
#define problem_header
#include "mesh.h"

class Problem {
  public:
    Mesh mesh;
    //Eigen::MatrixXd A;// global stiffness matrix
    //Eigen::VectorXd f;// global forcing vector
    int spatialDim;
    int unkDim;
    vector<vector<double>> unk;// unknown
    
    void Assemble();
    void Initialize();
};
#endif
