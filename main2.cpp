#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include "mesh.h"

using namespace std;

void ReadPhysicalNames( ifstream &file, Mesh &m, vector<int> & physicalTags );
void ReadEntities( ifstream &file, Mesh &m, vector<int> & physicalTags);
void ReadNodes( ifstream &file, Mesh &m );
void ReadElements( ifstream &file, Mesh &m );

template <class T>
int readParams( stringstream &ss, T params[] );

int main(int argc, char *argv[]){
    string fileName = argv[1];
    cout << "About to read: " << fileName << endl;
    Mesh mesh;
    vector<int> physicalTags;

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
                    cout << "-------------------------" << endl;
                    ReadPhysicalNames( meshFile, mesh, physicalTags );
                    for (vector<int>::iterator it = begin(physicalTags); it != end(physicalTags); it++){
                      cout << "physical tag: " << *it << endl;
                    }
                } else if (m.str(1)=="Entities"){
                    cout << "Processing entities:" << endl;
                    cout << "-------------------------" << endl;
                    ReadEntities( meshFile, mesh, physicalTags );
                } else if (m.str(1)=="Nodes"){
                    cout << "Processing nodes!" << endl;
                    cout << "-----------------" << endl;
                    ReadNodes( meshFile, mesh );
                } else if (m.str(1)=="Elements"){
                    cout << "Processing elements!" << endl;
                    ReadElements( meshFile, mesh );
                }
            }
        }
    }
    cout << "My mesh is " << mesh.dim << "-dimensional." << endl;
    //mesh.printNodes();
    //end 
    meshFile.close();
}

void ReadPhysicalNames( ifstream &file, Mesh &mesh, vector<int> & physicalTags ){
    double params[50];
    int level = 0, paramCount = 0, count = 0;
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

        paramCount = readParams<double>( ss, params );

        if (level==0){
          numPhysicalTags = params[0];
          physicalTags.resize(numPhysicalTags);
          level++;
        } else {
          //params[0] is dim o tag params[1] is tag #
          physicalTags[count] = params[1];
          count++;
        }
    }
}
void ReadEntities( ifstream &file, Mesh &mesh, vector<int> & physicalTags ){
    double params[50];
    int level = 0, paramCount = 0, counter = 0;
    int numPoints, numCurves, numSurfaces, numVolumes;
    int numPhysicalTags; //num physical tags in an entity
    vector<vector<int>> physicalGroups;
    regex eoBlock( R"(^\$EndEntities)" );
    string line;
    stringstream ss;

    while (getline(file, line) ){
        if (regex_search(line, eoBlock)) {
            return;
        }
        ss.str(line);
        ss.clear();

        paramCount = readParams<double>( ss, params );

        cout << line << endl;
        cout << "counter: " << counter << "level:" << level << endl;

        if (level==0){
          numPoints = params[0];
          numCurves = params[1];
          numSurfaces = params[2];
          numVolumes = params[3];
          //assuming for the moment ea group has a single! physical group
          //physicalGroups.resize(numEntities);
          level++;
        } else if( level==1 ){
          //reading points
          numPhysicalTags = params[4];
          if (numPhysicalTags>0){
            cout << params[5] << endl;
            physicalGroups.push_back({0, params[0], params[5]}); 
          }else{
            physicalGroups.push_back({0, params[0], -1}); 
          }
          counter++;
          if (counter==numPoints) {
            level++;
            counter = 0;
          }
        } else if( level==2 ){
          //reading curves
          numPhysicalTags = params[7];
          cout << "# pt: " << numPhysicalTags << endl;
          if (numPhysicalTags>0){
            physicalGroups.push_back({1, params[0], params[8]}); 
          }else{
            physicalGroups.push_back({1, params[0], -1}); 
          }
          counter++;
          if (counter==numCurves) {
            level++;
            counter = 0;
          }
        } else if( level==3 ){
          //reading surface
          numPhysicalTags = params[7];
          if (numPhysicalTags>0){
            physicalGroups.push_back({2, params[0], params[8]}); 
          }else{
            physicalGroups.push_back({2, params[0], -1}); 
          }
          counter++;
          if (counter==numSurfaces) {
            level++;
            counter = 0;
          }
        } else if( level==4 ){
          //reading volume
          numPhysicalTags = params[7];
          if (numPhysicalTags>0){
            physicalGroups.push_back({3, params[0], params[8]}); 
          }else{
            physicalGroups.push_back({3, params[0], -1}); 
          }
          counter++;
          if (counter==numVolumes) {
            level++;
            counter = 0;
          }
        }
        for (int i=0; i < physicalGroups.size(); i++){
          cout << "[";
          for (int j=0; j < physicalGroups[i].size(); j++){ 
            cout << physicalGroups[i][j] << ", ";
          }
          cout << "]" << endl;
        }
    }
}
void ReadNodes( ifstream &file, Mesh &mesh ){
    regex eoBlock( R"(^\$EndNodes)" );
    double params[50];
    int level = 0;
    int paramCount, tnodeCount = 0, bnodeCount = 0;
    int counter = 0;
    int entityTag = -1;
    string line;
    getline( file, line );
    stringstream ss(line);
    //read first line

    paramCount = readParams<double>( ss, params);
    mesh.nnodes = params[1];
    mesh.nodes.resize( mesh.nnodes );

    //read entities, node tags and node positions
    while (getline(file, line) ){
        if (regex_search(line, eoBlock)) {
            return;
        }

        ss.str(line);
        ss.clear();

        paramCount = readParams<double>( ss, params );

        if (level==0){
          //read entity
          mesh.dim = max( int(params[0]), mesh.dim );
          entityTag = params[1];
          bnodeCount = params[3];
          counter = params[3];
          level++;
        } else if( level==1){
          counter--;
          if (counter==0){
            level = 2;
          }
        } else if( level==2){
          mesh.nodes[tnodeCount] = {params[0], params[1], params[2]};
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

template <class T>
int readParams( stringstream &ss, T params[] ){
    int count = 0;
    while( ss >> params[count] ){
      count++;
    }
    return count;
}
