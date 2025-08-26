#include "bitmap.h"
#include "model.h"
#include <iostream>
#include <iomanip>

using namespace std;

double triangleArea(vertex a, vertex b, vertex c){
    return 0.5 *((b.y-a.y)*(b.x+a.x) + (c.y-b.y)*(c.x+b.x) + (a.y-c.y)*(a.x+c.x));
}


void drawTriangle(vertex a, vertex b, vertex c, Bitmap &bitmap) {
    int bbminx = min(min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = min(min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = max(max(a.x, b.x), c.x);
    int bbmaxy = max(max(a.x, b.y), c.y);
    double total_area = triangleArea(a, b, c);

    #pragma omp parallel for
    for (int x=bbminx; x < bbmaxx; x++){
        for (int y=bbminy; y < bbmaxy; y++){
            vertex xy_pair = vertex{x,y, 0};
            double alpha = triangleArea(xy_pair, b, c) / total_area;
            double beta = triangleArea(xy_pair, c, a) / total_area;
            double gamma = triangleArea(xy_pair, a, b) / total_area;  
            if (alpha<0 || beta<0 || gamma<0) continue; // point outside of triangle
            // unsigned int r = static_cast<unsigned int>(alpha * a.z * 1.1);
            // unsigned int g = static_cast<unsigned int>(beta * a.z * 1.1);
            // unsigned int b = static_cast<unsigned int>(gamma * a.z * 1.1);
            // unsigned int color = r << 16;
            // color ^= g << 8;
            // color ^= b;
            // cout << setfill('0') << setw(8) << hex << color << '\n';
            // bitmap.setPixel(x,y,color);
            unsigned char z = static_cast<unsigned char>(alpha * a.z + beta * b.z + gamma * c.z);
            if (bitmap.getPixel(x,y) < z){
                bitmap.setPixel(x,y, z);
            }
            
        }
    }
}

int main (int argc, char* argv[]) {
    Bitmap testMap = Bitmap(3200, 3200);
    Model amongus = Model("Socrates.obj");

    string path = "output/";
    if (argc = 2){
        path.append(argv[1]);
    }
    else {
        path.append("temp.bmp");
    }

    vector<vertex> temp_verts;

    #pragma omp parallel for
    for (int i = 0; i < amongus.n_faces; i++) {
        temp_verts = amongus.getFace(i);
        for (int j = 0; j < 3; j++) {
            temp_verts[j] = testMap.project(temp_verts[j]);
        }
        drawTriangle(temp_verts[0],temp_verts[1],temp_verts[2],testMap);
    }
    testMap.writeToBmp(path.c_str());
}

