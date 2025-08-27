#pragma once
#include <vector>
#include "model.h"

const int FILE_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;

class Bitmap {
public:
    Bitmap(int newWidth, int newHeight);
    vertex project(vertex v);
    std::vector<vertex> scale(Model model);
    unsigned int getPixel(int x, int y);
    void setPixel(int x, int y, unsigned int color);
    void writeToBmp(const char *path);

    int width;
    int height;
    std::vector<unsigned int> pixels;
};

unsigned int rgbToUint(int r, int g, int b);