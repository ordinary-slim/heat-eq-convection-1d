#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include "mesh.h"
#include "gmsheltypes.h"

using namespace std;

void ReadPhysicalNames( ifstream &file, Mesh &m, vector<int> & physicalTags );
void ReadEntities( ifstream &file, Mesh &m, vector<int> & physicalTags, vector<vector<int>> & physicalGroups);
void ReadNodes( ifstream &file, Mesh &m, vector<array<int, 2>> & nodeEntities );
void ReadElements( ifstream &file, Mesh &m, vector<array<int, 2>> & elementEntities  );
template <class T>
int readParams( stringstream &ss, T params[] );

int main(int argc, char *argv[]){
    string fileName = argv[1];
    cout << "About to read: " << fileName << endl;
    Mesh mesh;
    vector<int> physicalTags;
    vector<vector<int>> physicalGroups;
    vector<array<int, 2>> nodeEntities;
    vector<array<int, 2>> elementEntities;

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
                } else if (m.str(1)=="Entities"){
                    cout << "Processing entities:" << endl;
                    cout << "-------------------------" << endl;
                    ReadEntities( meshFile, mesh, physicalTags, physicalGroups);
                    cout << "Physical groups:" << endl;
                    for (int i=0; i < physicalGroups.size(); i++){
                      for (int j=0; j < physicalGroups[i].size(); j++){ 
                        printf("%3d, ", physicalGroups[i][j]);
                      }
                      cout << endl;
                    }
                } else if (m.str(1)=="Nodes"){
                    cout << "Processing nodes!" << endl;
                    cout << "-----------------" << endl;
                    ReadNodes( meshFile, mesh, nodeEntities );
                    cout << "Node entities:" << endl;
                    for (int i=0; i < nodeEntities.size(); i++){
                      printf("%3d: %3d, %3d\n", i, nodeEntities[i][0], nodeEntities[i][1]);
                    }
                } else if (m.str(1)=="Elements"){
                    cout << "Processing elements!" << endl;
                    ReadElements( meshFile, mesh, elementEntities );
                }
            }
        }
    }
    cout << "My mesh has " << mesh.nnodes << " nodes." << endl;
    cout << "My mesh is " << mesh.dim << "-dimensional." << endl;
    //mesh.printNodes();
    //end 
    meshFile.close();
}

void ReadPhysicalNames( ifstream &file, Mesh &mesh, vector<int> & physicalTags ){
    int params[50];
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

        paramCount = readParams<int>( ss, params );

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
void ReadEntities( ifstream &file, Mesh &mesh, vector<int> & physicalTags, vector<vector<int>> &  physicalGroups){
    double params[50];
    int level = 0, paramCount = 0, counter = 0;
    int numPoints, numCurves, numSurfaces, numVolumes;
    int numPhysicalTags; //num physical tags in an entity
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
            physicalGroups.push_back({0, int(params[0]), int(params[5])}); 
          }else{
            physicalGroups.push_back({0, int(params[0]), -1}); 
          }
          counter++;
          if (counter==numPoints) {
            level++;
            counter = 0;
          }
        } else if( level==2 ){
          //reading curves
          numPhysicalTags = int(params[7]);
          if (numPhysicalTags>0){
            physicalGroups.push_back({1, int(params[0]), int(params[8])}); 
          }else{
            physicalGroups.push_back({1, int(params[0]), -1}); 
          }
          counter++;
          if (counter==numCurves) {
            level++;
            counter = 0;
          }
        } else if( level==3 ){
          //reading surface
          numPhysicalTags = int(params[7]);
          if (numPhysicalTags>0){
            physicalGroups.push_back({2, int(params[0]), int(params[8])}); 
          }else{
            physicalGroups.push_back({2, int(params[0]), -1}); 
          }
          counter++;
          if (counter==numSurfaces) {
            level++;
            counter = 0;
          }
        } else if( level==4 ){
          //reading volume
          numPhysicalTags = int(params[7]);
          if (numPhysicalTags>0){
            physicalGroups.push_back({3, int(params[0]), int(params[8])}); 
          }else{
            physicalGroups.push_back({3, int(params[0]), -1}); 
          }
          counter++;
          if (counter==numVolumes) {
            level++;
            counter = 0;
          }
        }
    }
}
void ReadNodes( ifstream &file, Mesh &mesh, vector<array<int, 2>> & nodeEntities ){
    regex eoBlock( R"(^\$EndNodes)" );
    double params[50];
    int level = 0;
    int paramCount, tnodeCount = 0, bnodeCount = 0;
    int counter = 0;
    int entityDim, entityTag;
    string line;
    getline( file, line );
    stringstream ss(line);

    //read first line
    paramCount = readParams<double>( ss, params);
    mesh.nnodes = params[1];
    mesh.p.resize( mesh.nnodes );
    nodeEntities.resize(mesh.nnodes);

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
          entityDim = params[0];
          entityTag = params[1];
          bnodeCount = params[3];
          counter = params[3];

          //update mesh dim
          mesh.dim = max( entityDim, mesh.dim );

          level++;
        } else if( level==1){
          //reading node index, ignored
          counter--;
          if (counter==0){
            level = 2;
          }
        } else if( level==2){
          //reading actual node position
          mesh.p[tnodeCount] = {params[0], params[1], params[2]};
          nodeEntities[tnodeCount][0] = entityDim;
          nodeEntities[tnodeCount][1] = entityTag;

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

void ReadElements( ifstream &file, Mesh &mesh, vector<array<int, 2>> & elementEntities ){
    regex eoBlock( R"(^\$EndElements)" );
    int params[50];
    int level = 0;
    int paramCount, tElementCount = 0, bElementCounter = 0;
    int entityDim, entityTag;
    int elementType = -1;
    string line;
    getline( file, line );
    stringstream ss(line);

    //read first line
    paramCount = readParams<int>( ss, params);
    mesh.nels = params[1];
    mesh.c.resize( mesh.nels );
    elementEntities.resize(mesh.nels );

    //read entities, node tags and node positions
    while (getline(file, line) ){
        if (regex_search(line, eoBlock)) {
            return;
        }

        ss.str(line);
        ss.clear();

        paramCount = readParams<int>( ss, params );

        cout << line << endl;
        cout << "Current level " << level << endl;

        if (level==0){
          //read entity
          entityDim = params[0];
          entityTag = params[1];
          //we could do this with an enum
          elementType = params[2];
          bElementCounter = params[3];

          level++;
        } else if( level==1){
          //reading element
          //element type logic
          mesh.c[tElementCount] = {params[0], params[1], params[2]};
          elementEntities[tElementCount][0] = entityDim;
          elementEntities[tElementCount][1] = entityTag;

          tElementCount++;
          bElementCounter--;
          //printf("read node (%2.2f , %2.2f , %2.2f)\n", params[0], params[1], params[2]);
          if (bElementCounter == 0) {
            level = 0;
          }
        }
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
