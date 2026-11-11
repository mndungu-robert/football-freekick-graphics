#ifndef TRANSFORMS_SCENE_H
#define TRANSFORMS_SCENE_H
#include "canvas.h"

struct Matrix3x3 {
    double m[3][3];
    static Matrix3x3 identity() {
        return {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    }
    static Matrix3x3 multiply(const Matrix3x3& A, const Matrix3x3& B) {
        Matrix3x3 R = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    R.m[i][j] += A.m[i][k] * B.m[k][j];
        return R;
    }
    static Point2D transform(const Matrix3x3& M, Point2D p) {
        return {
            M.m[0][0] * p.x + M.m[0][1] * p.y + M.m[0][2],
            M.m[1][0] * p.x + M.m[1][1] * p.y + M.m[1][2]
        };
    }
    static Matrix3x3 translate(double tx, double ty) {
        return {{{1, 0, tx}, {0, 1, ty}, {0, 0, 1}}};
    }
    static Matrix3x3 rotateAroundPivot(double deg, double px, double py) {
        double rad = deg * 3.141592653589793 / 180.0;
        double c = std::cos(rad), s = std::sin(rad);
        Matrix3x3 T1 = translate(-px, -py);
        Matrix3x3 R  = {{{c, -s, 0}, {s, c, 0}, {0, 0, 1}}};
        Matrix3x3 T2 = translate(px, py);
        return multiply(T2, multiply(R, T1));
    }
    static Matrix3x3 scaleAroundPivot(double sx, double sy, double px, double py) {
        Matrix3x3 T1 = translate(-px, -py);
        Matrix3x3 S  = {{{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}}};
        Matrix3x3 T2 = translate(px, py);
        return multiply(T2, multiply(S, T1));
    }
    static Matrix3x3 shearX(double shx, double yRef) {
        return {{{1, shx, -shx * yRef}, {0, 1, 0}, {0, 0, 1}}};
    }
};
#endif
