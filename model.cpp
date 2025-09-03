#include "model.h"
#include "bitmap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

Model::Model(const char* path){
    std::ifstream in;
    double x,y,z;
    vector<int> temp_faces;
    char dummy;
    string token;
    vector<int> vert_idx;
    bool facesFound;
    int tempIndex;

    in.open(path, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            iss >> x >> y >> z;
            vertices.push_back(vertex{x,y,z});
        } else if (!line.compare(0, 2, "f ")) {
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
    }
    cout << "No. of vertices found: " << vertices.size() << "\n";
    n_vertices = vertices.size();
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
