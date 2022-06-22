#include "camera.h"

Camera::Camera(float X, float Y, float Z, float psi, float theta, float rotation_speed,
               float translation_speed, int window_width, int window_height) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
    this->mouse_x = window_width / 2;
    this->mouse_y = window_height / 2;
    this->psi = psi;
    this->theta = theta;
    this->rotation_speed = rotation_speed;
    this->translation_speed = translation_speed;
}

void Camera::rotation(int x, int y) {
    theta -= static_cast<float>(x - mouse_x) * rotation_speed;
    psi += static_cast<float>(y - mouse_y) * rotation_speed;
    if (psi <= 0.1) psi = 0.1;
    else if (psi >= 0.95 * M_PI) psi = 0.95 * M_PI;
    mouse_x = x;
    mouse_y = y;
}

void Camera::translation(int key) {
    switch (key) {
        case 97 : // A
            X -= sin(theta - M_PI / 2) * sin(psi) * translation_speed ;
            Z -= cos(theta - M_PI / 2) * sin(psi) * translation_speed ;
            break;
        case 100 : // D
            X -= sin(theta + M_PI / 2) * sin(psi) * translation_speed ;
            Z -= cos(theta + M_PI / 2) * sin(psi) * translation_speed ;
            break;
        case 119 : // W
            X += sin(theta) * sin(psi) * translation_speed ;
            Y += cos(psi) * translation_speed ;
            Z += cos(theta) * sin(psi) * translation_speed ;
            break;
        case 115 : // S
            X -= sin(theta) * sin(psi) * translation_speed ;
            Y -= cos(psi) * translation_speed ;
            Z -= cos(theta) * sin(psi) * translation_speed ;
            break;
    }

}
