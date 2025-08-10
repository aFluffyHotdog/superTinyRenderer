#include "bitmap.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void drawLine(int x0, int y0, int x1, int y1, int color, Bitmap &bitmap){
    bool steep = false;
    if (abs(x0-x1)<abs(y0-y1)) { // if the line is too steep we transpose it
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    }

    if (x1 < x0) { // Force line to be drawn from left to right
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    }

    for (int x = x0; x < x1; x++) {
        float t = (x - x0) / static_cast<float>(x1 - x0);
        int y = round(y0 + (y1-y0) * t);

        if(steep){
            bitmap.setPixel(y, x, color);
        }
        else {
            bitmap.setPixel(x, y, color);
        }
    }
}

int main() {
    Bitmap testMap = Bitmap(400,400);
    drawLine(0,0, 50, 50, 0xFFFFFF, testMap);
    testMap.writeToBmp("test.bmp");

    ifstream objFile("Among_Us.obj");
    if (!objFile.is_open()) {
        throw std::runtime_error("Could not open file: " );
    }

    string line;
    bool verticesFound;
    float x,y;
    // skip lines until it starts with "v" (vertex)
    while (getline(objFile, line)) {
       if (line[0] == 'v') {
            if (!verticesFound){
                verticesFound = true;
            }
            istringstream iss(line);

       }
       else if (verticesFound) {
            break;
       }
    }

    // draw a line from vx to vx+1

}