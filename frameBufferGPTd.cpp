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
    // Find min and max Y for the triangle
    int minY = std::min({a.y, b.y, c.y});
    int maxY = std::max({a.y, b.y, c.y});
    unsigned int random_color = randomHexColor();
    // For each scanline between minY and maxY
    for (int y = minY; y <= maxY; ++y) {
        std::vector<int> nodes;
        // Check each edge for intersection with scanline y
        auto edgeIntersect = [&](vertex_int v1, vertex_int v2) {
            if ((v1.y < y && v2.y >= y) || (v2.y < y && v1.y >= y)) {
                int x = v1.x + (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
                nodes.push_back(x);
            }
        };
        edgeIntersect(a, b);
        edgeIntersect(b, c);
        edgeIntersect(c, a);

        // Sort intersection points
        sort(nodes.begin(), nodes.end());
        // Fill between pairs of intersections
        for (size_t i = 0; i + 1 < nodes.size(); i += 2) {
            for (int x = nodes[i]; x <= nodes[i + 1]; ++x) {
                bitmap.setPixel(x, y, random_color);
            }
        }
    }
    // Optionally, draw triangle edges
    auto drawLine = [&](vertex_int p0, vertex_int p1) {
        int x0 = p0.x, y0 = p0.y;
        int x1 = p1.x, y1 = p1.y;
        int dx = abs(x1 - x0), dy = abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            bitmap.setPixel(x0, y0, 0x000000); // black edge
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    };
    drawLine(a, b);
    drawLine(b, c);
    drawLine(c, a);
}


int main (int argc, char* argv[]) {
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

