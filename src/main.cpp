#include "canvas.h"
#include "transforms_scene.h"
#include "lines_pitch.h"
#include "circles_curves.h"
#include "polygons_stadium.h"

#ifdef USE_OPENGL
#include <GL/glut.h>
#endif

Canvas gCanvas(800, 600);
bool gIsKicking = false;
bool gNightMode = true;
double gT = 0.0;
Point2D gP0 = {165, 148};
Point2D gP1 = {410, 365};
Point2D gP2 = {675, 275};

void drawPlayer(Canvas& canvas, Point2D feetPos, Color jerseyColor, double bodyTiltDeg, double kickLegDeg, double jumpY) {
    Matrix3x3 T_jump = Matrix3x3::translate(0, jumpY);
    Matrix3x3 R_body = Matrix3x3::rotateAroundPivot(bodyTiltDeg, feetPos.x, feetPos.y + jumpY);
    Matrix3x3 M_body = Matrix3x3::multiply(R_body, T_jump);
    
    std::vector<Point2D> torso = {
        {feetPos.x - 10, feetPos.y + 32}, {feetPos.x + 10, feetPos.y + 32},
        {feetPos.x + 12, feetPos.y + 68}, {feetPos.x - 12, feetPos.y + 68}
    };
    for (auto& p : torso) p = Matrix3x3::transform(M_body, p);
    Member3_Polygons::fillPolygonScanline(canvas, torso, jerseyColor);

    Point2D headCenter = Matrix3x3::transform(M_body, {feetPos.x, feetPos.y + 80});
    Member2_CirclesAndCurves::drawCircleMidpoint(canvas, (int)headCenter.x, (int)headCenter.y, 10, {240, 200, 165}, true);

    Point2D hip = Matrix3x3::transform(M_body, {feetPos.x, feetPos.y + 32});
    Point2D standFoot = Matrix3x3::transform(M_body, {feetPos.x - 6, feetPos.y});
    Member1_Lines::drawLineBresenham(canvas, (int)hip.x, (int)hip.y, (int)standFoot.x, (int)standFoot.y, {25, 25, 35}, 3);

    Matrix3x3 R_leg = Matrix3x3::rotateAroundPivot(kickLegDeg, hip.x, hip.y);
    Point2D kickFootRaw = Matrix3x3::transform(M_body, {feetPos.x + 8, feetPos.y});
    Point2D kickFoot = Matrix3x3::transform(R_leg, kickFootRaw);
    Member1_Lines::drawLineBresenham(canvas, (int)hip.x, (int)hip.y, (int)kickFoot.x, (int)kickFoot.y, {25, 25, 35}, 3);

    Point2D shoulder = Matrix3x3::transform(M_body, {feetPos.x, feetPos.y + 64});
    Point2D leftHand = Matrix3x3::transform(M_body, {feetPos.x - 18, feetPos.y + 46});
    Point2D rightHand = Matrix3x3::transform(M_body, {feetPos.x + 18, feetPos.y + 52});
    Member1_Lines::drawLineBresenham(canvas, (int)shoulder.x, (int)shoulder.y, (int)leftHand.x, (int)leftHand.y, jerseyColor, 3);
    Member1_Lines::drawLineBresenham(canvas, (int)shoulder.x, (int)shoulder.y, (int)rightHand.x, (int)rightHand.y, jerseyColor, 3);
}

void renderCurrentScene(double tBall) {
    Color sky = gNightMode ? Color{18, 28, 52} : Color{115, 185, 245};
    gCanvas.clear(sky);
    double kickLegAngle = (tBall == 0.0) ? -25.0 : std::min(45.0, -25.0 + tBall * 140.0);
    double wallJumpY    = std::sin(tBall * 3.14159) * 42.0;
    double keeperDive   = tBall * 58.0;
    double netShearX    = (tBall >= 0.95) ? 0.18 : 0.0;

    Member1_Lines::drawFloodlights(gCanvas, gNightMode);
    Member2_CirclesAndCurves::drawCubicBezierArch(gCanvas, {40, 360}, {240, 590}, {560, 590}, {760, 360}, {220, 225, 235});
    Member3_Polygons::drawStadiumAndTurf(gCanvas);
    Member1_Lines::drawPitchLines(gCanvas);

    Member2_CirclesAndCurves::drawQuadraticBezier(gCanvas, gP0, gP1, gP2, {255, 230, 80});
    Member1_Lines::drawGoalAndNet(gCanvas, netShearX);
    Member1_Lines::drawTargetCrosshair(gCanvas, gP2);

    drawPlayer(gCanvas, {140, 140}, {225, 40, 50}, 0.0, kickLegAngle, 0.0);
    drawPlayer(gCanvas, {380, 140}, {40, 110, 230}, 0.0, 0.0, wallJumpY);
    drawPlayer(gCanvas, {412, 140}, {40, 110, 230}, 0.0, 0.0, wallJumpY);
    drawPlayer(gCanvas, {645, 140}, {245, 205, 35}, keeperDive, 0.0, tBall * 14.0);

    Point2D ballPos = Member2_CirclesAndCurves::evalQuadraticBezier(gP0, gP1, gP2, tBall);
    Member2_CirclesAndCurves::drawFootball(gCanvas, ballPos, 13.0 - tBall * 2.5, tBall * 720.0);
}

#ifdef USE_OPENGL
void displayCallback() {
    renderCurrentScene(gT);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glDrawPixels(gCanvas.getWidth(), gCanvas.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, gCanvas.getRawBuffer());
    glutSwapBuffers();
}

void timerCallback(int) {
    if (gIsKicking) {
        gT += 0.015;
        if (gT >= 1.0) { gT = 1.0; gIsKicking = false; }
        glutPostRedisplay();
    }
    glutTimerFunc(16, timerCallback, 0);
}

void keyboardCallback(unsigned char key, int, int) {
    if (key == ' ') { gT = 0.0; gIsKicking = true; }
    else if (key == 'r' || key == 'R') { gT = 0.0; gIsKicking = false; }
    else if (key == 'n' || key == 'N') { gNightMode = !gNightMode; }
    else if (key == 27) { exit(0); }
    glutPostRedisplay();
}

void specialKeysCallback(int key, int, int) {
    if (key == GLUT_KEY_UP)    gP1.y = std::min(520.0, gP1.y + 15.0);
    if (key == GLUT_KEY_DOWN)  gP1.y = std::max(180.0, gP1.y - 15.0);
    if (key == GLUT_KEY_LEFT)  gP2.y = std::min(300.0, gP2.y + 10.0);
    if (key == GLUT_KEY_RIGHT) gP2.y = std::max(155.0, gP2.y - 10.0);
    glutPostRedisplay();
}
#endif

int main(int argc, char** argv) {
#ifdef USE_OPENGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("2D Football Free-Kick Arena (Pure C++ Algorithms + OpenGL)");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(specialKeysCallback);
    glutTimerFunc(16, timerCallback, 0);
    std::cout << "Controls: [SPACE] Kick | [UP/DOWN] Curve Loft | [LEFT/RIGHT] Target Height | [N] Day/Night | [R] Reset\n";
    glutMainLoop();
#else
    renderCurrentScene(0.0);
    gCanvas.savePPM("output/cp1_static_setup.ppm");
    renderCurrentScene(0.52);
    gCanvas.savePPM("output/cp2_mid_flight.ppm");
    renderCurrentScene(1.0);
    gCanvas.savePPM("output/cp2_goal_impact.ppm");
#endif
    return 0;
}
