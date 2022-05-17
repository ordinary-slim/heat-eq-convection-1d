#include <stdio.h>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

class Mesh {
    public:
        int nnodes = -1, nels = -1;
        int dim = 0;
        
        vector<std::array<double, 3>> p;//nodes
        vector<vector<int>> c;//conectivity

        //tbd
        vector<int> bc;

        void myprint(){
            cout << "Hello from mesh!" << endl;
        }
        void printNodes(){
            int count = 0;
            printf("nnodes = %d, dim = %d\n", nnodes, dim);
            for (int i =0; i < p.size(); i++){
                printf("Node #%2d:   ", i);
                for (int j =0; j < 3; j++){
                    printf("%8.2f", p[i][j]);
                }
                cout << endl;
            }
        }
};
