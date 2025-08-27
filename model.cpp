#include "model.h"
#include "bitmap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

Model::Model(const char* path){
    ifstream objFile(path);
    if (!objFile.is_open())
    {
        throw std::runtime_error("Could not open file: ");
    }

    string line;
    bool vertsFound;
    float x, y, z;

    // read the vertices (lines that starts with 'v')
    while(getline(objFile, line)) 
    {
        if (line[0] == 'v')
        {   
            if (!vertsFound) 
            {
                vertsFound = true;
            }
            istringstream iss(line);
            iss.ignore(2, ' '); // ignore the v
            iss >> x >> y >> z;
            vertices.push_back(vertex{x,y,z});
            continue;
        }
        else if (vertsFound) {
            break;
        }
    }

    cout << "No. of vertices found: " << vertices.size() << "\n";
    n_vertices = vertices.size();

    // read the faces (lines that start with "vf")
    char dummy;
    string token;
    vector<int> vert_idx;
    bool facesFound;
    int tempIndex;
    int x0, y0, x1, y1;
    // parse and draw faces line by line

    while(getline(objFile, line)) {
        if (line[0] == 'f') {
            if(!facesFound) {
                facesFound = true;
            }
            std::istringstream iss(line);
            vert_idx.clear(); // reset array
            iss >> dummy; // skip the letter f

            for (int i = 0; i < 3; i++) {
                iss >> token;
                size_t pos = token.find("//");
                tempIndex = stoi(token.substr(0, pos));
                tempIndex -= 1; // obj file's indiecs are 1 indexed so we have to offset;
                vert_idx.push_back(tempIndex); // store the current 
            }   
            faces.push_back(vert_idx);
        }
        else if (facesFound) {
            break;
        }
    }

    cout << "No. of faces found: " << faces.size() << "\n";
    n_faces = faces.size();
}

vector<vertex> Model::getFace(int i) {
    vector<vertex> result;
    if (i >= n_faces){
        return result;
    }
    
    result.push_back(vertices[faces[i][0]]);
    result.push_back(vertices[faces[i][1]]);
    result.push_back(vertices[faces[i][2]]);

    return result;
}
