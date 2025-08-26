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

void Model::scale(int width, int height){
    // Step 1: Find bounding box
    float minX = numeric_limits<float>::max();
    float minZ = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float maxX = numeric_limits<float>::lowest();
    float maxY = numeric_limits<float>::lowest();
    float maxZ = numeric_limits<float>::lowest();

    for (const auto& v : vertices) {
        if (v.x < minX) minX = v.x;
        if (v.x > maxX) maxX = v.x;
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
        if (v.z < minZ) minZ = v.z;
        if (v.z > maxZ) maxZ = v.z;
    }
     // Step 2: Compute scale factors
    float objWidth = maxX - minX;
    float objHeight = maxY - minY;
    float objDepth  = maxZ - minZ;

    float scaleX = (float)width / objWidth;
    float scaleY = (float)height / objHeight;
    float scale = std::min(scaleX, scaleY); // preserve aspect ratio

    for (int i = 0; i < n_vertices; i++) {
        float normX = (vertices[i].x - minX) * scale;
        float normY = (vertices[i].y - minY) * scale;
        float normZ = (vertices[i].z - minZ) * scale;

        // center x & y inside canvas; z is just normalized to scale
        normX += (width  - objWidth  * scale) / 2.0f;
        normY += (height - objHeight * scale) / 2.0f;
        // Z doesn’t need centering unless you want a fixed depth range
        vertices[i].x = normX;
        vertices[i].y = normY;
        vertices[i].z = normZ;
    }
}