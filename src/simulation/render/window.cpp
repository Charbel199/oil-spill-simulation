#include "window.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "open_gl_helper.h"
namespace Window{
    int timeElapsedForSetup;
    int frame = 0;
    void drawFps() {
        Color color= {1, 0 ,0 ,0};
        Position pos = {0, 640,480 ,0};
        PositionXY posXY = {350, 460};
        OpenGLHelper::drawText(color, pos, posXY, "TEST");
    }

    void draw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex3f(-2,-2,-5.0);
        glVertex3f(2,0.0,-5.0);
        glVertex3f(0.0,2,-5.0);
        glEnd();
        //Draw
        drawFps();

//        glFluh();
        glutSwapBuffers();
//        glutPostRedisplay();
        frame++;
        std::cout<<frame<<std::endl;

    }
    void initializeWindow(int argc, char** argv, std::string title, float translationSpeed) {
        // Initialize OpenGL
        glutInit(&argc, argv);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitWindowPosition(10, 10);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutCreateWindow(title.c_str());
        glEnable(GL_MULTISAMPLE);

    }


    void run() {
        timeElapsedForSetup = glutGet(GLUT_ELAPSED_TIME);
        // Set background color
        glClearColor(1 ,1 ,1 , 1);
        glClear ( GL_COLOR_BUFFER_BIT ) ;

        // Set drawing function (It will loop while the simulation is running)
        glutDisplayFunc(draw);

        glutReshapeFunc(changeSize);
        // Run main loop
        glutMainLoop();
    }
    void changeSize(int w, int h) {

        // Prevent a divide by zero, when window is too short
        // (you cant make a window of zero width).
        if(h == 0)
            h = 1;
        float ratio = 1.0* w / h;

        // Use the Projection Matrix
        glMatrixMode(GL_PROJECTION);

        // Reset Matrix
        glLoadIdentity();

        // Set the viewport to be the entire window
        glViewport(0, 0, w, h);

        // Set the correct perspective.
        gluPerspective(45,ratio,1,1000);

        // Get Back to the Modelview
        glMatrixMode(GL_MODELVIEW);
    }

}
