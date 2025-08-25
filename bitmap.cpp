#include "bitmap.h"
#include "model.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring> // for memcpy
#include <iostream>
#include <cmath>


using namespace std;

Bitmap::Bitmap(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    pixels.assign(width * height, 0);
}

unsigned int rgbToUint(int r, int g, int b){
    unsigned int color = 0x0;
    color |= (r & 0xFF) << 16; // Red
    color |= (g & 0xFF) << 8;  // Green
    color |= (b & 0xFF);       // Blue
    return color;
}

void Bitmap::setPixel(int x, int y, unsigned int color){
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }

    pixels[y * width + x] = color;
}

void Bitmap::writeToBmp(const char* path){

    ofstream out(path, std::ios::binary);
    
    unsigned char fileHeader[FILE_HEADER_SIZE];
    // File Header
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    // File Size
    int rowStride = width * 3;
    int padding = (4 - (rowStride % 4)) % 4; // calculate padding
    int fileSize = FILE_HEADER_SIZE + DIB_HEADER_SIZE + (rowStride + padding) * height;
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    // Unused reserved bits
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    // Offset 
    fileHeader[10] = FILE_HEADER_SIZE + DIB_HEADER_SIZE;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char dibHeader[DIB_HEADER_SIZE];
    // Header Size
    dibHeader[0] = DIB_HEADER_SIZE;
    dibHeader[1] = 0;
    dibHeader[2] = 0;
    dibHeader[3] = 0;

    // Image Width
    dibHeader[4] = width;
    dibHeader[5] = width >> 8;
    dibHeader[6] = width >> 16;
    dibHeader[7] = width >> 24;
    
    // Image Height
    dibHeader[8] = height;
    dibHeader[9] = height >> 8;
    dibHeader[10]= height >> 16;
    dibHeader[11] = height >> 24;

    // Planes
    dibHeader[12] = 1;
    dibHeader[13] = 0;

    // Bits per pixel (for RGB)
    dibHeader[14] = 24;
    dibHeader[15] = 0;

    // the rest of the bits are not needed
    std::memset(&dibHeader[16], 0, sizeof(dibHeader) - 16);
    
    // write the headers
    if (!out) {
        cout << "Error opening file";
    }

    out.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
    out.write(reinterpret_cast<char*>(dibHeader), DIB_HEADER_SIZE);

    // Write each pixel from bottom to top
    unsigned char pad[3] = {0,0,0};
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            unsigned int color = pixels[y * width + x];
            unsigned char r = (color >> 16) & 0xFF;
            unsigned char g = (color >> 8)  & 0xFF;
            unsigned char b = (color)       & 0xFF;
            unsigned char pixel[3] = { b, g, r }; // BMP uses BGR order
            out.write(reinterpret_cast<char*>(pixel), 3);
        }
        out.write(reinterpret_cast<char*>(pad), padding);
    }

    out.close();

}

double scale(double n) {
    n *= 20;
    unsigned int result = static_cast<int>(n);
    result += 400;  // TODO: change this magic number this is for 800 x 800
    return result;
}
vertex Bitmap::project(vertex v) {
    return { scale(v.x), scale(v.y), scale(v.z)};
}