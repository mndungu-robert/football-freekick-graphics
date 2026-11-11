#ifndef LINES_PITCH_H
#define LINES_PITCH_H
#include "canvas.h"
#include "transforms_scene.h"
#include <cmath>
#include <algorithm>

namespace Member1_Lines {
    inline void drawLineBresenham(Canvas& canvas, int x0, int y0, int x1, int y1, Color c, int thickness = 1) {
        int dx = std::abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;
        while (true) {
            for (int tx = -thickness / 2; tx <= thickness / 2; ++tx)
                for (int ty = -thickness / 2; ty <= thickness / 2; ++ty)
                    canvas.putPixel(x0 + tx, y0 + ty, c);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    inline void drawLineDDA(Canvas& canvas, double x0, double y0, double x1, double y1, Color c, bool dashed = false) {
        double dx = x1 - x0, dy = y1 - y0;
        int steps = static_cast<int>(std::max(std::abs(dx), std::abs(dy)));
        if (steps == 0) { canvas.putPixel(static_cast<int>(x0), static_cast<int>(y0), c); return; }
        double xInc = dx / steps, yInc = dy / steps;
        double x = x0, y = y0;
        for (int i = 0; i <= steps; ++i) {
            if (!dashed || (i / 6) % 2 == 0) {
                canvas.putPixel(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), c);
            }
            x += xInc; y += yInc;
        }
    }

    inline void drawPitchLines(Canvas& canvas) {
        Color white = {235, 245, 235};
        drawLineBresenham(canvas, 0, 140, 760, 140, white, 2);
        drawLineBresenham(canvas, 550, 140, 620, 50, white, 2);
        drawLineBresenham(canvas, 620, 50, 780, 50, white, 2);
        drawLineBresenham(canvas, 650, 140, 745, 40, white, 3);
    }

    inline void drawFloodlights(Canvas& canvas, bool nightMode) {
        Color poleGray = {160, 170, 185};
        drawLineBresenham(canvas, 90, 360, 90, 540, poleGray, 4);
        drawLineBresenham(canvas, 70, 540, 110, 540, poleGray, 3);
        drawLineBresenham(canvas, 710, 360, 710, 540, poleGray, 4);
        drawLineBresenham(canvas, 690, 540, 730, 540, poleGray, 3);
        if (nightMode) {
            Color beamYellow = {255, 245, 160};
            drawLineDDA(canvas, 90, 540, 260, 200, beamYellow, true);
            drawLineDDA(canvas, 90, 540, 420, 200, beamYellow, true);
            drawLineDDA(canvas, 710, 540, 520, 200, beamYellow, true);
            drawLineDDA(canvas, 710, 540, 660, 200, beamYellow, true);
        }
    }

    inline void drawGoalAndNet(Canvas& canvas, double netShearX) {
        int gxFront = 650, gxBack = 735;
        int gyBottom = 140, gyTop = 310;
        Matrix3x3 netShear = Matrix3x3::shearX(netShearX, gyBottom);

        Color netColor = {205, 215, 225};
        for (int x = gxFront + 10; x <= gxBack; x += 12) {
            Point2D pBot = Matrix3x3::transform(netShear, {(double)x, (double)gyBottom});
            Point2D pTop = Matrix3x3::transform(netShear, {(double)x, (double)gyTop});
            drawLineBresenham(canvas, (int)pBot.x, (int)pBot.y, (int)pTop.x, (int)pTop.y, netColor, 1);
        }
        for (int y = gyBottom; y <= gyTop; y += 14) {
            Point2D pLeft  = Matrix3x3::transform(netShear, {(double)gxFront, (double)y});
            Point2D pRight = Matrix3x3::transform(netShear, {(double)gxBack,  (double)y});
            drawLineBresenham(canvas, (int)pLeft.x, (int)pLeft.y, (int)pRight.x, (int)pRight.y, netColor, 1);
        }

        Color postWhite = {255, 255, 255};
        Point2D backTop = Matrix3x3::transform(netShear, {(double)gxBack, (double)gyTop});
        drawLineBresenham(canvas, gxFront, gyBottom, gxFront, gyTop, postWhite, 5);
        drawLineBresenham(canvas, gxFront, gyTop, (int)backTop.x, (int)backTop.y, postWhite, 4);
        drawLineBresenham(canvas, (int)backTop.x, (int)backTop.y, gxBack, gyBottom, {185, 195, 205}, 3);
    }

    inline void drawTargetCrosshair(Canvas& canvas, Point2D target) {
        int tx = static_cast<int>(target.x), ty = static_cast<int>(target.y);
        Color aimRed = {255, 70, 70};
        drawLineBresenham(canvas, tx - 10, ty, tx + 10, ty, aimRed, 2);
        drawLineBresenham(canvas, tx, ty - 10, tx, ty + 10, aimRed, 2);
    }
}
#endif
