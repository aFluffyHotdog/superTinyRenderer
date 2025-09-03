#include "bitmap.h"
#include "model.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

void initRandom() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Generate random hex color 0xRRGGBB
unsigned int randomHexColor() {
    unsigned int r = std::rand() % 256;  // 0-255
    unsigned int g = std::rand() % 256;
    unsigned int b = std::rand() % 256;

    return (r << 16) | (g << 8) | b;
}
double triangleArea(vertex_int a, vertex_int b, vertex_int c){
    return 0.5 *((b.y-a.y)*(b.x+a.x) + (c.y-b.y)*(c.x+b.x) + (a.y-c.y)*(a.x+c.x));
}

void drawTriangle(vertex_int a, vertex_int b, vertex_int c, Bitmap &bitmap) {
    int bbminx = min(min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = min(min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = max(max(a.x, b.x), c.x);
    int bbmaxy = max(max(a.y, b.y), c.y);
    int painted_pixels = 0;
    double total_area = triangleArea(a, b, c);
    
    if (total_area<1) return; // backface culling + discarding triangles that cover less than a pixel
    
    unsigned int color = randomHexColor();
    for (int x=bbminx; x < bbmaxx; x++){
        for (int y=bbminy; y < bbmaxy; y++){
            vertex_int xy_pair = vertex_int{x,y, 0};
            double alpha = triangleArea(xy_pair, b, c) / total_area;
            double beta = triangleArea(xy_pair, c, a) / total_area;
            double gamma = triangleArea(xy_pair, a, b) / total_area;  
            if (alpha<0 || beta<0 || gamma<0) {
                continue; // point outside of triangle
            }
            unsigned int z = static_cast<unsigned int>(alpha * a.z + beta * b.z + gamma * c.z);
            if (bitmap.getPixel(x,y) <= z){
                bitmap.setPixel(x,y, z);
            }

            painted_pixels++;
        }
    }
}

int main (int argc, char* argv[]) {
    Bitmap testMap = Bitmap(1600, 1600);
    
    vertex_int a = {377, 445, 143};
    vertex_int b = {378, 439, 143};
    vertex_int c = {392, 439, 157};
    drawTriangle(a,b,c, testMap);

    string model_path;
    string path = "output/";
    if (argc = 4){
        path.append(argv[1]);
        model_path = argv[2];
    }
    else {
        path.append("temp.bmp");
        model_path = "Among_Us.obj";
    }

    Model amongus = Model(model_path.c_str());
    vector<vertex_int> scaled_verts = testMap.scale(amongus);


    #pragma omp parallel for
    for (int i = 0; i < amongus.n_faces; i++) {
        drawTriangle(
            scaled_verts[amongus.faces[i][0]],
            scaled_verts[amongus.faces[i][1]],
            scaled_verts[amongus.faces[i][2]],
            testMap);
    }
    testMap.writeToBmp(path.c_str());
}

