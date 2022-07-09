#ifndef mesh_header
#define mesh_header
#include <stdio.h>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

class Mesh {
    public:
        int nnodes = 0, nels = 0, nfaces = 0;
        int nfnodes = 0;
        int dim = 0;
        
        vector<std::array<double, 3>> x;//nodes
        vector<int> p;//node pointers; 0 free, 1 dirichlet, 2 neumann
        vector<vector<int>> c;//conectivity
        vector<vector<int>> b;//d-1 connectivity

        void myprint(){
            cout << "Hello from mesh!" << endl;
        }
        void printNodes(){
            printf("nnodes = %d, dim = %d\n", nnodes, dim);
            for (int i =0; i < x.size(); i++){
                printf("Node #%2d:   ", i);
                for (int j =0; j < 3; j++){
                    printf("%8.2f", x[i][j]);
                }
                cout << endl;
            }
        }
        void printFaces(){
            printf("---- PRINTING FACES -------------\n");
            printf("nfaces = %d, dim = %d\n", nfaces, dim-1);
            for (int i =0; i < b.size(); i++){
                printf("Element #%2d:   ", i);
                for (int j =0; j < b[i].size(); j++){
                    printf("%8.2d", b[i][j]);
                }
                cout << endl;
            }
        }
        void printEls(){
            printf("---- PRINTING ELEMENTS ---------\n");
            printf("nels = %d, dim = %d\n", nels, dim);
            for (int i =0; i < c.size(); i++){
                printf("Element #%2d:   ", i);
                for (int j =0; j < c[i].size(); j++){
                    printf("%8.2d", c[i][j]);
                }
                cout << endl;
            }
        }
};
#endif

