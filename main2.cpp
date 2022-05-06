#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "mesh.h"

using namespace std;

void ReadNodes( ifstream &file, Mesh &m );
void ReadElements( ifstream &file, Mesh &m );

int main(){
    string fileName = "gmsh_examples/square.msh";
    Mesh mesh;

    ifstream meshFile( fileName );
    string line;
    if (meshFile.is_open()){
        //reading loop
        while (getline( meshFile, line) ){
            regex rgx(R"(^\$(\w+))");
            smatch m;
            if (regex_search(line, m, rgx)){
                if (m.str(1)=="Nodes"){
                    cout << "Processing nodes!" << endl;
                    ReadNodes( meshFile, mesh );
                } else if (m.str(1)=="Elements"){
                    cout << "Processing elements!" << endl;
                    ReadElements( meshFile, mesh );
                }
            }
        }
    }
    cout << "My mesh is " << mesh.dim << "-dimensional." << endl;
    mesh.myprint();
    mesh.printNodes();
    meshFile.close();
}

void ReadNodes( ifstream &file, Mesh &mesh ){
    regex eoBlock( R"(^\$EndNodes)" );
    int a, b, c, d;
    int count = 0;
    double x, y, z, w;
    string line;
    getline( file, line );
    stringstream ss(line);
    //read first line
    if (ss >> a >> b >> c >> d){
        mesh.nnodes = b;
        mesh.xnodes.resize( mesh.nnodes );
        //read entities, node tags and node positions
        while (getline(file, line) ){
            if (regex_match(line, eoBlock)) {
                return;
            }
            ss.str(line);
            ss.clear();
            //try to read entity declaration
            if (ss >> a >> b >> c >> d){
                //update dimension of mesh
                mesh.dim = max( a, mesh.dim );
                continue;
            }
            ss.clear();
            ss.str(line);
            //try to read node coords
            if (ss >> x >> y >> z){
                mesh.xnodes[count] = {x, y, z};
                count += 1;
                continue;
            }
        }
    }

    //problem!
    cerr << "Error in node lecture!" << endl;
    return;
}

void ReadElements( ifstream &file, Mesh &mesh ){
    regex eoBlock( R"(^\$EndElements)" );
    int a, b, c, d;
    int count = 0;
    double x, y, z, w;
    //read first line
    string line;
    getline( file, line );
    stringstream ss(line);
    if (ss >> a >> b >> c >> d){
        mesh.nels = b;
        printf("My mesh has %4d els\n", mesh.nels);
    }
    //problem!
    cerr << "Error in element lecture!" << endl;
    return;
}

int readParams( stringstream &ss, int& params[] ){
    return 0;
}
