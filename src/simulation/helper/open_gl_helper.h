#ifndef OIL_SPILL_SIMULATION_OPEN_GL_HELPER_H
#define OIL_SPILL_SIMULATION_OPEN_GL_HELPER_H
#include <GL/glut.h>
#include <cstring>
#include "iostream"
struct Color {
    float r = 0, g = 0, b = 0, a = 0;
};
struct Position {
    int left = 0, right = 0, bottom = 0, top = 0;
};
struct PositionXY {
    int x = 0, y = 0;
};
namespace OpenGLHelper {
    void drawText(Color color, Position projectionPosition, PositionXY position, std::string text);
}
#endif //OIL_SPILL_SIMULATION_OPEN_GL_HELPER_H
