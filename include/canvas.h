#ifndef CANVAS_H
#define CANVAS_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

struct Color { unsigned char r, g, b; };
struct Point2D { double x, y; };

class Canvas {
private:
    int width, height;
    std::vector<unsigned char> rgbBuffer;
public:
    Canvas(int w, int h, Color bg = {18, 28, 52}) : width(w), height(h), rgbBuffer(w * h * 3) {
        clear(bg);
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const unsigned char* getRawBuffer() const { return rgbBuffer.data(); }
    
    void clear(Color bg) {
        for (int i = 0; i < width * height; ++i) {
            rgbBuffer[i * 3 + 0] = bg.r;
            rgbBuffer[i * 3 + 1] = bg.g;
            rgbBuffer[i * 3 + 2] = bg.b;
        }
    }
    
    void putPixel(int x, int y, Color c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = (y * width + x) * 3;
            rgbBuffer[idx + 0] = c.r;
            rgbBuffer[idx + 1] = c.g;
            rgbBuffer[idx + 2] = c.b;
        }
    }
    
    void savePPM(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (int y = height - 1; y >= 0; --y) {
            ofs.write(reinterpret_cast<const char*>(&rgbBuffer[y * width * 3]), width * 3);
        }
        ofs.close();
        std::cout << "[Saved Screenshot] " << filename << "\n";
    }
};
#endif
