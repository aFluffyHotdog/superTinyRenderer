#pragma once
#include <vector> 

struct vertex {
    double x;
    double y;
    double z;
};

class Model {
public:
    Model(const char* path);
    
    int n_faces;
    int n_vertices;
    std::vector<vertex> getFace(int i);
    std::vector<vertex> vertices;
    std::vector<std::vector<int>> faces;
};
