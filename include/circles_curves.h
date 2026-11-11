#ifndef CIRCLES_CURVES_H
#define CIRCLES_CURVES_H
#include "canvas.h"
#include "lines_pitch.h"
#include "transforms_scene.h"

namespace Member2_CirclesAndCurves {
    inline void drawCircleMidpoint(Canvas& canvas, int xc, int yc, int r, Color c, bool filled = false) {
        int x = 0, y = r, d = 1 - r;
        auto plotOctants = [&](int px, int py) {
            if (filled) {
                for (int ix = xc - px; ix <= xc + px; ++ix) {
                    canvas.putPixel(ix, yc + py, c); canvas.putPixel(ix, yc - py, c);
                }
                for (int ix = xc - py; ix <= xc + py; ++ix) {
                    canvas.putPixel(ix, yc + px, c); canvas.putPixel(ix, yc - px, c);
                }
            } else {
                canvas.putPixel(xc + px, yc + py, c); canvas.putPixel(xc - px, yc + py, c);
                canvas.putPixel(xc + px, yc - py, c); canvas.putPixel(xc - px, yc - py, c);
                canvas.putPixel(xc + py, yc + px, c); canvas.putPixel(xc - py, yc + px, c);
                canvas.putPixel(xc + py, yc - px, c); canvas.putPixel(xc - py, yc - px, c);
            }
        };
        while (x <= y) {
            plotOctants(x, y);
            x++;
            if (d < 0) d += 2 * x + 1;
            else { y--; d += 2 * (x - y) + 1; }
        }
    }

    inline Point2D evalQuadraticBezier(Point2D P0, Point2D P1, Point2D P2, double t) {
        double u = 1.0 - t;
        return {
            u * u * P0.x + 2 * u * t * P1.x + t * t * P2.x,
            u * u * P0.y + 2 * u * t * P1.y + t * t * P2.y
        };
    }

    inline void drawQuadraticBezier(Canvas& canvas, Point2D P0, Point2D P1, Point2D P2, Color c, int steps = 400) {
        for (int i = 0; i <= steps; ++i) {
            if ((i / 8) % 2 == 0) {
                Point2D pt = evalQuadraticBezier(P0, P1, P2, (double)i / steps);
                canvas.putPixel((int)std::round(pt.x), (int)std::round(pt.y), c);
                canvas.putPixel((int)std::round(pt.x), (int)std::round(pt.y) + 1, c);
            }
        }
    }

    inline void drawCubicBezierArch(Canvas& canvas, Point2D P0, Point2D P1, Point2D P2, Point2D P3, Color c) {
        for (int i = 0; i <= 800; ++i) {
            double t = (double)i / 800.0, u = 1.0 - t;
            double x = u*u*u*P0.x + 3*u*u*t*P1.x + 3*u*t*t*P2.x + t*t*t*P3.x;
            double y = u*u*u*P0.y + 3*u*u*t*P1.y + 3*u*t*t*P2.y + t*t*t*P3.y;
            for (int dy = -2; dy <= 2; ++dy)
                canvas.putPixel((int)std::round(x), (int)std::round(y) + dy, c);
        }
    }

    inline void drawFootball(Canvas& canvas, Point2D center, double radius, double spinAngleDeg) {
        int cx = (int)std::round(center.x), cy = (int)std::round(center.y), r = (int)std::round(radius);
        drawCircleMidpoint(canvas, cx, cy, r, {250, 250, 250}, true);
        drawCircleMidpoint(canvas, cx, cy, r, {30, 30, 35}, false);

        Matrix3x3 rot = Matrix3x3::rotateAroundPivot(spinAngleDeg, center.x, center.y);
        for (int a = 0; a < 360; a += 72) {
            double rad = a * 3.14159265 / 180.0;
            Point2D spokeEnd = {center.x + (radius - 2) * std::cos(rad), center.y + (radius - 2) * std::sin(rad)};
            Point2D rotatedEnd = Matrix3x3::transform(rot, spokeEnd);
            Member1_Lines::drawLineBresenham(canvas, cx, cy, (int)rotatedEnd.x, (int)rotatedEnd.y, {40, 40, 45}, 1);
        }
        drawCircleMidpoint(canvas, cx, cy, 4, {25, 25, 30}, true);
    }
}
#endif
