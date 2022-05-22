#include <stdio.h>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

class Mesh {
    public:
        int nnodes = 0, nels = 0;
        int dim = 0;
        
        vector<std::array<double, 3>> p;//nodes
        vector<vector<int>> c;//conectivity
        vector<int> nodeBCFlags;

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
        void printEls(){
            int count = 0;
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
