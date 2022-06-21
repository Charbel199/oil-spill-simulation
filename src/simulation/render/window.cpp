#include "window.h"
#include <GL/glut.h>
#include <cmath>


void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(1.0, 0, 0);
    glVertex3f(0.5, 0.5, 1);
    glColor3f(0, 1.0, 0);
    glVertex3f(0.5, 0, 1);
    glColor3f(0, 0, 1.0);
    glVertex3f(0, 0.5, 1);
    glVertex3f(0.5, 0.5, 1);

    glEnd();

    glFlush();
}
Window::Window(int argc, char** argv, std::string title, float translationSpeed) {
    // Initialize OpenGL
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow(title.c_str());
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);



}

void Window::run() {
    timeElapsedForSetup = glutGet(GLUT_ELAPSED_TIME);


    // Run simulation
    glutDisplayFunc(display);
    glutMainLoop();
}
