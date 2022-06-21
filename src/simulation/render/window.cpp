#include "window.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "open_gl_helper.h"
namespace Window{
    int timeElapsedForSetup;

    void drawFps() {
        Color color= {1, 0 ,0 ,0};
        Position pos = {0, 640,480 ,0};
        PositionXY posXY = {350, 460};
        OpenGLHelper::drawText(color, pos, posXY, "TEST");
    }

    void draw()
    {
        drawFps();
        std::cout<<"DRAWING VERTEX"<<std::endl;


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
    void initializeWindow(int argc, char** argv, std::string title, float translationSpeed) {
        // Initialize OpenGL
        glutInit(&argc, argv);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitWindowPosition(10, 10);
        glutCreateWindow(title.c_str());
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glEnable(GL_MULTISAMPLE);

    }


    void run() {
        timeElapsedForSetup = glutGet(GLUT_ELAPSED_TIME);

        // Set drawing function (It will loop while the simulation is running)
        glutDisplayFunc(draw);

        // Set background color
        glClearColor(1 ,1 ,1 , 1);
        glClear ( GL_COLOR_BUFFER_BIT ) ;

        // Run main loop
        glutMainLoop();
    }

}
