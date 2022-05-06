#include <iostream>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

int main(){

    // Heat equation:
    // dt T + k Delta T = 0 with initial condition
    // and convection at Tenv

    // input params
    float a = 0.0;
    float b = 1.0;
    float Tenv = 20.0;
    float conduc = 1.0;
    float convec = 1.0;
    float Tfinal = 1;

    float L = b - a;

    // space discretization
    int Nels = 5;
    int nnodes = Nels + 1;
    float h = L / Nels;

    // time discretization
    int nsteps = 10;
    float dt = Tfinal / nsteps;



    // Mesh
    //pnodes: positions of nodes
    float pnodes[nnodes] = {a};
    //lnodes: connectivity of nodes
    int   lnodes[nnodes][2];

    for (int i=1; i<= Nels; i++) {
        pnodes[i] = a + i * h;
        lnodes[i][0] = i-1;
        lnodes[i][1] = i;

        cout << "Element " << i << ": " << pnodes[i-1] << ", "
            << pnodes[i] << "\n";
    }

    //allocation of problem matrices
    MatrixXd M = MatrixXd::Zero(nnodes, nnodes);
    MatrixXd A = MatrixXd::Zero(nnodes, nnodes);
    //implicit initial condition
    VectorXd u = VectorXd::Zero(nnodes);
    VectorXd load = VectorXd::Zero(nnodes);

    //assembly
    for (int i=0; i< Nels; i++){
        float hloc = pnodes[i+1] - pnodes[i];

        //build local matrices minus scaling
        Matrix2d Mloc {{2, 1}, {1, 2}};
        Matrix2d Aloc {{1, -1}, {-1, 1}};

        //scale local matrices
        Mloc *= hloc/6;
        Aloc *= (conduc/hloc);

        // assemble
        M(seq(i, i+1), seq(i, i+1)) += Mloc;
        A(seq(i, i+1), seq(i, i+1)) += Aloc;
    }


    // BCs: convection
    A(0, 0) += -conduc*convec;
    A(nnodes-1, nnodes-1) += -conduc*convec;
    load(0) += -conduc*convec*Tenv;
    load(nnodes-1) += -conduc*convec*Tenv;

    //time loop
    float t = 0.0; 
    while(t<Tfinal){
      t += dt;
      //algebra for forward euler
      // M un+1 = Mun - dt Aun
      VectorXd RHS = VectorXd::Zero(nnodes);
      RHS += M*u + dt * (-A*u + load);

      //solve M*u = RHS
      u = M.partialPivLu().solve( RHS );
      cout << u << endl << endl;
    }

    //for (int j=0; j<nnodes; j++){
        //cout << "[ ";
        //for (int k=0; k<nnodes; k++){
            //cout << A[j][k] << ", ";
        //}
        //cout << "]\n";
    //}

}

