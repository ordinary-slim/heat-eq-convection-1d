#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "mesh.h"

using namespace std;

void ReadPhysicalNames( ifstream &file, Mesh &m );
void ReadNodes( ifstream &file, Mesh &m );
void ReadElements( ifstream &file, Mesh &m );
int readParams( stringstream &ss, double params[] );

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
                if (m.str(1)=="PhysicalNames") {
                    cout << "Processing physical tags!" << endl;
                    ReadPhysicalNames( meshFile, mesh );
                } else if (m.str(1)=="Nodes"){
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

void ReadPhysicalNames( ifstream &file, Mesh &mesh ){
    double params[50];
    int level = 0, paramCount = 0;
    int numPhysicalTags = 0;
    regex eoBlock( R"(^\$EndPhysicalNames)" );
    string line;
    stringstream ss;

    while (getline(file, line) ){
        if (regex_search(line, eoBlock)) {
            return;
        }
        ss.str(line);
        ss.clear();

        paramCount = readParams( ss, params );

        if (level==0){
          numPhysicalTags = params[0];
          level++;
        } else {
          paramCount = readParams( ss, params );
          //params[0] is dim o tag params[1] is tag #
        }
    }
}
void ReadNodes( ifstream &file, Mesh &mesh ){
    regex eoBlock( R"(^\$EndNodes)" );
    double params[50];
    int level = 0;
    int paramCount, tnodeCount = 0, bnodeCount = 0;
    int counter = 0;
    string line;
    getline( file, line );
    stringstream ss(line);
    //read first line

    paramCount = readParams( ss, params);
    mesh.nnodes = params[1];
    mesh.xnodes.resize( mesh.nnodes );

    //read entities, node tags and node positions
    while (getline(file, line) ){
        if (regex_search(line, eoBlock)) {
            return;
        }

        ss.str(line);
        ss.clear();

        paramCount = readParams( ss, params );

        if (level==0){
          //read entity
          mesh.dim = max( int(params[0]), mesh.dim );
          bnodeCount = params[3];
          counter = params[3];
          level++;
        } else if( level==1){
          counter--;
          if (counter==0){
            level = 2;
          }
        } else if( level==2){
          mesh.xnodes[tnodeCount] = {params[0], params[1], params[2]};
          tnodeCount++;
          counter++;
          //printf("read node (%2.2f , %2.2f , %2.2f)\n", params[0], params[1], params[2]);
          if (counter == bnodeCount) {
            level = 0;
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

int readParams( stringstream &ss, double params[] ){
    int count = 0;
    while( ss >> params[count] ){
      count++;
    }
    return count;
}
