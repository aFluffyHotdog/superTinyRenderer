#include "bitmap.h"
#include "linedrawing.h"

using namespace std;

void drawTriangle(vertex a, vertex b, vertex c, unsigned int color, Bitmap &bitmap) {
    // sort the vertices, a,b,c in ascending y order (bubblesort yay!)
    if (a.y>b.y) { swap(a.x, b.x); swap(a.y, b.y); }
    if (a.y>c.y) { swap(a.x, c.x); swap(a.y, c.y); }
    if (b.y>c.y) { swap(b.x, c.x); swap(b.y, c.y); }

    drawLine(a.x, a.y, b.x, b.y, color, bitmap);
    drawLine(b.x, b.y, c.x, c.y, color, bitmap);
    drawLine(c.x, c.y, a.x, a.y, color, bitmap);
}

int main()
{
    Bitmap testBmp = Bitmap(400,400);
    vertex a = {100, 100};
    vertex b = {200,200};
    vertex c = {100,200};
    drawTriangle(a,b,c, 0xFFFFFF, testBmp);
    testBmp.writeToBmp("traingle1.bmp");
}