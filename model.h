#pragma once
#include <vector> 

struct vertex {
    double x;
    double y;
    double z;
};

// we use integers for drawing, wish we could have a more elegant solution but here we go..
struct vertex_int {
    int x;
    int y;
    int z;
};


class Model {
public:
    Model(const char* path);
    
    int n_faces;
    int n_vertices;
    int min_x, max_x, min_y, max_y, min_z, max_z;
    
    void scale(int width, int height);
    std::vector<vertex> getFace(int i);
    std::vector<vertex> vertices;
    std::vector<std::vector<int>> faces;
};
