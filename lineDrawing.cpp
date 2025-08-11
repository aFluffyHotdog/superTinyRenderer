#include "bitmap.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void drawLine(int x0, int y0, int x1, int y1, int color, Bitmap &bitmap)
{
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    { // if the line is too steep we transpose it
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x1 < x0)
    { // Force line to be drawn from left to right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x < x1; x++)
    {
        float t = (x - x0) / static_cast<float>(x1 - x0);
        int y = round(y0 + (y1 - y0) * t);

        if (steep)
        {
            bitmap.setPixel(y, x, color);
        }
        else
        {
            bitmap.setPixel(x, y, color);
        }
    }
}

int main()
{
    Bitmap testMap = Bitmap(400, 400);

    ifstream objFile("Among_Us.obj");
    if (!objFile.is_open())
    {
        throw std::runtime_error("Could not open file: ");
    }

    string line, nextLine;
    bool verticesFound;
    float x0, y0, x1, y1;
    // skip lines until it starts with "v" (vertex)
    if (getline(objFile, line)) 
    {
        istringstream iss(line);
        iss.ignore(2, ' '); // ignore the v
        iss >> x0 >> y0;
        while (getline(objFile, nextLine))
        {
            if (line[0] == 'v')
            {
                if (!verticesFound)
                {
                    verticesFound = true;
                }
                istringstream iss2(nextLine);
                iss2.ignore(2, ' '); // ignore the v
                iss2 >> x1 >> y1;

                // Multiply to match scale then draw line!.
                x0 *= 100; x1 *= 100; y0 *= 100; y1 *= 100;
                drawLine(x0, y0, x1, y1, 0xFFFFFF, testMap);
                cout << x0;
                // reassign variables for next loop
                line = nextLine;
                x0 = x1;
                y0 = y1;

            }
            else if (verticesFound)
            {
                break;
            }
        }
    }
    testMap.writeToBmp("amongus.bmp");
}