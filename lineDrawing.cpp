#include "bitmap.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct vertex {
    float x;
    float y;
    float z;
};

unsigned int normalizeCoords(float n) 
{
    n *= 20;
    unsigned int result = static_cast<int>(n);
    result += 400;  // TODO: change this magic number this is for 800 x 800
    return result;
}
void drawLine(int x0, int y0, int x1, int y1, unsigned int color, Bitmap &bitmap)
{
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    { // if the line is too steep we transpose it
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x1 < x0)
    { // Force line to be drawn from left to right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x < x1; x++)
    {
        float t = (x - x0) / static_cast<float>(x1 - x0);
        int y = round(y0 + (y1 - y0) * t);

        if (steep)
        {
            bitmap.setPixel(y, x, color);
        }
        else
        {
            bitmap.setPixel(x, y, color);
        }
    }
}

int main()
{
    Bitmap testMap = Bitmap(800, 1600);

    ifstream objFile("Among_Us.obj");
    if (!objFile.is_open())
    {
        throw std::runtime_error("Could not open file: ");
    }

    string line;
    bool vertsFound;
    float x, y, z;
    vector<vertex> vertices;

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
            vertices.push_back(vertex{y,z});
            continue;
        }
        else if (vertsFound) {
            break;
        }
    }

    cout << "size of vertices " << vertices.size() << "\n";

    // read the faces (lines that start with "vf")
    char dummy;
    vector<int> vertexIndices;
    string token;
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
            vertexIndices.clear(); // reset array
            iss >> dummy; // skip the letter f

            for (int i = 0; i < 3; i++) {
                iss >> token;
                size_t pos = token.find("//");
                tempIndex = stoi(token.substr(0, pos));
                tempIndex -= 1; // obj file's indiecs are 1 indexed, not 0;
                vertexIndices.push_back(tempIndex);
            }

            for (int i = 0; i < 3; i++) {
                int j = (i + 1) % 3;
                x0 = normalizeCoords(vertices[vertexIndices[i]].x);
                y0 = normalizeCoords(vertices[vertexIndices[i]].y);
                x1 = normalizeCoords(vertices[vertexIndices[j]].x);
                y1 = normalizeCoords(vertices[vertexIndices[j]].y);
                drawLine(x0, y0, x1, y1, 0xFFFFFF, testMap);
            }
        }
        else if (facesFound) {
            break;
        }
    }
    
    testMap.writeToBmp("amongus.bmp");
}