#ifndef POLYGONS_STADIUM_H
#define POLYGONS_STADIUM_H
#include "canvas.h"
#include "lines_pitch.h"
#include <vector>
#include <algorithm>

namespace Member3_Polygons {
    inline void fillPolygonScanline(Canvas& canvas, const std::vector<Point2D>& pts, Color c) {
        if (pts.size() < 3) return;
        double minY = pts[0].y, maxY = pts[0].y;
        for (const auto& p : pts) {
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
        for (int y = (int)std::floor(minY); y <= (int)std::ceil(maxY); ++y) {
            std::vector<double> nodeX;
            size_t j = pts.size() - 1;
            for (size_t i = 0; i < pts.size(); ++i) {
                if ((pts[i].y < y && pts[j].y >= y) || (pts[j].y < y && pts[i].y >= y)) {
                    nodeX.push_back(pts[i].x + (y - pts[i].y) / (pts[j].y - pts[i].y) * (pts[j].x - pts[i].x));
                }
                j = i;
            }
            std::sort(nodeX.begin(), nodeX.end());
            for (size_t i = 0; i + 1 < nodeX.size(); i += 2) {
                for (int x = (int)std::ceil(nodeX[i]); x <= (int)std::floor(nodeX[i + 1]); ++x) {
                    canvas.putPixel(x, y, c);
                }
            }
        }
    }

    inline void drawStadiumAndTurf(Canvas& canvas) {
        fillPolygonScanline(canvas, {{0, 220}, {800, 220}, {800, 360}, {0, 360}}, {32, 42, 68});
        fillPolygonScanline(canvas, {{0, 220}, {800, 220}, {800, 248}, {0, 248}}, {210, 45, 55});
        for (int i = 0; i < 10; ++i) {
            int x0 = i * 80, x1 = (i + 1) * 80;
            Color turf = (i % 2 == 0) ? Color{34, 139, 58} : Color{42, 158, 68};
            fillPolygonScanline(canvas, {{(double)x0, 0}, {(double)x1, 0}, {(double)x1, 220}, {(double)x0, 220}}, turf);
        }
        fillPolygonScanline(canvas, {{290, 500}, {510, 500}, {510, 555}, {290, 555}}, {20, 26, 38});
        Member1_Lines::drawLineBresenham(canvas, 290, 500, 510, 500, {245, 195, 45}, 2);
    }
}
#endif
