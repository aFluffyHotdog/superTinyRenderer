#include "bitmap.h"
#include "model.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <Eigen/Dense>

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
double triangleArea(vertex a, vertex b, vertex c){
    return 0.5 *((b.y-a.y)*(b.x+a.x) + (c.y-b.y)*(c.x+b.x) + (a.y-c.y)*(a.x+c.x));
}

void drawTriangle(vertex a, vertex b, vertex c, Bitmap &bitmap) {
    int bbminx = min(min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = min(min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = max(max(a.x, b.x), c.x);
    int bbmaxy = max(max(a.y, b.y), c.y);
    int painted_pixels = 0;
    double total_area = triangleArea(a, b, c);
    cout << total_area << endl;
    if (total_area<1) return; // backface culling + discarding triangles that cover less than a pixel
    unsigned int color = randomHexColor();
    for (int x=bbminx; x < bbmaxx; x++){
        for (int y=bbminy; y < bbmaxy; y++){
            vertex xy_pair = vertex{static_cast<double>(x),static_cast<double>(y), 0};
            double alpha = triangleArea(xy_pair, b, c) / total_area;
            double beta = triangleArea(xy_pair, c, a) / total_area;
            double gamma = triangleArea(xy_pair, a, b) / total_area;  
            if (alpha<0 || beta<0 || gamma<0) {
                continue; // point outside of triangle
            }
            unsigned char z = static_cast<unsigned char>(alpha * a.z + beta * b.z + gamma * c.z);
            if (bitmap.getPixel(x,y) <= z){
                bitmap.setPixel(x,y, z);
            }

            painted_pixels++;
        }
    }
}

vertex rotate(vertex v) {
    // rotation along the x-axis for now
    // TODO: maybe make refactor this method somewhere else
    Eigen::Matrix3d rotation_m;
    double theta = M_PI/4;
    // Y rotation
    rotation_m << 
    cos(theta), 0, sin(theta),
    0, 1, 0,
    -sin(theta), 0, cos(theta);

    // Z rotation
    // rotation_m << cos(M_PI/6), -sin(M_PI/6), 0,
    //  sin(M_PI/6), cos(M_PI/6), 0,
    //  0, 0, 1;

    // X rotation
    // rotation_m << 1,0,0,
    //  0, cos(M_PI/6), -sin(M_PI/6),
    //  0, sin(M_PI/6), cos(M_PI/6);

    Eigen::Vector3d v3(v.x, v.y, v.z);
    
    Eigen::Vector3d result = rotation_m * v3;
    return vertex{
        result[0], 
        result[1], 
        result[2]
    };
}

vertex perspective(vertex v){
    constexpr double c = 3.; // positive camera distance
    double factor = 1 - (v.z / c);
    return vertex{
        v.x / factor,
        v.y / factor,
        v.z / factor
    };
}


int main (int argc, char* argv[]) {

    // vertex v = {200.,200.,-3.};
    // vertex v_r = rotate(v);
    // cout << v_r.x << " " << v_r.y << " " << v_r.z << endl;
    // vertex v_p = perspective(v_r);
    // cout << v_p.x << " " << v_p.y << " " << v_p.z << endl;
    Bitmap testMap = Bitmap(1600, 1600);

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
    // amongus.vertices = testMap.scale(amongus);

    // rotate => perspective
    #pragma omp parallel for
    for (int i = 0; i < amongus.n_vertices; i++) {
        amongus.vertices[i] = testMap.project(perspective(rotate(amongus.vertices[i])));
    }

    #pragma omp parallel for
    for (int i = 0; i < amongus.n_faces; i++) {
        vector<vertex> temp_verts = amongus.getFace(i);
        drawTriangle(temp_verts[0], temp_verts[1], temp_verts[2], testMap);
    }
    testMap.writeToBmp(path.c_str());
}

