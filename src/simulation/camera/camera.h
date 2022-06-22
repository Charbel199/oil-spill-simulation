#ifndef OIL_SPILL_SIMULATION_CAMERA_H
#define OIL_SPILL_SIMULATION_CAMERA_H

#include <cmath>
#include <map>

class Camera {

public:

    Camera(float X, float Y, float Z, float psi, float theta, float rotation_speed,
           float translation_speed, int window_width, int window_height);
    ~Camera() {}

    int   getMouseX() { return mouse_x; }
    int   getMouseY() { return mouse_y; }
    float getSightX() { return X + sin(theta)*sin(psi); }
    float getSightY() { return Y + cos(psi); }
    float getSightZ() { return Z + cos(theta)*sin(psi); }
    float getX()      { return X; }
    float getY()      { return Y; }
    float getZ()      { return Z; }

    void setMouse(int x, int y)         { mouse_x = x; mouse_y = y; }
    void setX(float p_X)                { X = p_X; }
    void setY(float p_Y)                { Y = p_Y; }
    void setZ(float p_Z)                { Z = p_Z; }

    void rotation(int, int);
    void translation(int);

private:
    float    X;                                     /* Camera X */
    float    Y;                                     /* Camera Y */
    float    Z;                                     /* Camera Z */
    int      mouse_x;                               /* Mouse x position */
    int      mouse_y;                               /* Mouse y position */
    float    psi;                                   /* psi angle in spherical coordinates */
    float    theta;                                 /* theta angle in spherical coordinates */
    float    rotation_speed;                        /* Mouse sensibility */
    float    translation_speed;                     /* Keyboard sensibility */

};

#endif //OIL_SPILL_SIMULATION_CAMERA_H
