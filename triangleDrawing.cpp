#include "bitmap.h"
#include "model.h"
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
