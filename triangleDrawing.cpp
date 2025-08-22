#include "bitmap.h"
#include "linedrawing.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// Call this once at program start to seed the RNG
void initRandom() {
    std::srand(static_cast<unsigned int>(time(nullptr)));
}

// Generate random hex color 0xRRGGBB
unsigned int randomHexColor() {
    unsigned int r = std::rand() % 256;  // 0-255
    unsigned int g = std::rand() % 256;
    unsigned int b = std::rand() % 256;

    return (r << 16) | (g << 8) | b;
}

double triangleArea(vertex a, vertex b, vertex c){
    return 0.5 *((b.y-a.y)*(b.x+a.x) + (c.y-b.y)*(c.x+b.x) + (a.y-c.y)*(a.x+c.x));
}

void drawTriangle(vertex a, vertex b, vertex c, unsigned int color, Bitmap &bitmap) {
    int bbminx = min(min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = min(min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = max(max(a.x, b.x), c.x);
    int bbmaxy = max(max(a.x, b.y), c.y);
    double total_area = triangleArea(a, b, c);
    cout << "total area:" << total_area << "\n";
     if (total_area<1) return;

    #pragma omp parallel for
    for (int x=bbminx; x < bbmaxx; x++){
        for (int y=bbminy; y < bbmaxy; y++){
            vertex xy_pair = vertex{x,y, 0};
            double alpha = triangleArea(xy_pair, b, c) / total_area;
            double beta = triangleArea(xy_pair, c, a) / total_area;
            double gamma = triangleArea(xy_pair, a, b) / total_area;  
            if (alpha<0 || beta<0 || gamma<0) continue; // point outside of triangle
            bitmap.setPixel(x,y, color);
        }
    }
}


int main()
{
    // Bitmap testBmp = Bitmap(400,400);
    // vertex a = {100, 100};
    // vertex b = {200,200};
    // vertex c = {100,200};
    // drawTriangle(a,b,c, 0xFFFFFF, testBmp);
    // testBmp.writeToBmp("traingle3.bmp");

    // Amongus!
    Bitmap testMap = Bitmap(800, 1600);

    ifstream objFile("Among_Us.obj");
    if (!objFile.is_open())
    {
        throw std::runtime_error("Could not open file: ");
    }

    string line;
    bool vertsFound;
    float x, y, z;
    int y_norm, z_norm;
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
            y_norm = normalizeCoords(y);
            z_norm = normalizeCoords(z);
            vertices.push_back(vertex{y_norm,z_norm});
            continue;
        }
        else if (vertsFound) {
            break;
        }
    }

    cout << "size of vertices " << vertices.size() << "\n";

    // read the faces (lines that start with "vf")
    char dummy;
    vector<int> vert_idx;
    string token;
    bool facesFound;
    int tempIndex;
    int x0, y0, x1, y1;
    unsigned int random_color;
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
                tempIndex -= 1; // obj file's indiecs are 1 indexed, not 0;
                vert_idx.push_back(tempIndex);
            }

            random_color = randomHexColor();
            drawTriangle(vertices[vert_idx[0]],
                vertices[vert_idx[1]],
                vertices[vert_idx[2]], 
                random_color, 
                testMap); 
                
        }
        else if (facesFound) {
            break;
        }
    }
    
    testMap.writeToBmp("amongus.bmp");
}