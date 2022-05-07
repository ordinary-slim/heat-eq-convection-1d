#include <stdio.h>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

class Mesh {
    public:
        int nnodes;
        int nels;
        int dim = 0;
        vector<vector<int>> lnodes;
        vector<std::array<double, 3>> nodes;
        vector<int> bc_nodes;

        void myprint(){
            cout << "Hello from mesh!" << endl;
        }
        void printNodes(){
            int count = 0;
            printf("nnodes = %d, dim = %d\n", nnodes, dim);
            for (int i =0; i < nodes.size(); i++){
                printf("Node #%2d:   ", i);
                for (int j =0; j < 3; j++){
                    printf("%8.2f", nodes[i][j]);
                }
                cout << endl;
            }
        }
};
