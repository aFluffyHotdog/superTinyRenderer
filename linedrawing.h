#pragma once
#include "bitmap.h"

struct vertex {
    int x;
    int y;
    int z;
};

unsigned int normalizeCoords(float n);

void drawLine(int x0, int y0, int x1, int y1, unsigned int color, Bitmap &bitmap);