#include "fluids_window.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "open_gl_helper.h"
#include "fluids_solver.h"

namespace FluidsWindow {
    FluidsSolver *fluidsSolver;
    int timeElapsed;
    int currentW = WIDTH, currentH = HEIGHT;
    int mouseX;
    int mouseY;
    int oldMouseX;
    int oldMouseY;
    int mouseDown[3];
    int displayWindow = 1;

    void renderScene() {
        getInput();
//        fluidsSolver->vortConfinement();
        fluidsSolver->updateVelocities();
        fluidsSolver->updateDensities();

        glViewport(0, 0, currentW, currentH);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0f, (float) (fluidsSolver->getRowSize()), 0.0f, (float) (fluidsSolver->getColSize()));

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (displayWindow == 0) drawDensity();
        if (displayWindow == 1) drawVelocity();

        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < fluidsSolver->getTotSize(); i++) {
            glVertex2f(fluidsSolver->getPX()[i], fluidsSolver->getPY()[i]);
        }
        glEnd();

        glutSwapBuffers();

    }

    void idleRenderScene() {
        glutPostRedisplay();

    }

    void drawVelocity() {
        float *px = fluidsSolver->getPX();
        float *py = fluidsSolver->getPY();
        float *vx = fluidsSolver->getVX();
        float *vy = fluidsSolver->getVY();

        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(1.0f);

        glBegin(GL_LINES);
        for (int i = 0; i < fluidsSolver->getTotSize(); i++) {
            glVertex2f(px[i], py[i]);
            glVertex2f(px[i] + vx[i] * 10.0f, py[i] + vy[i] * 10.0f);
        }
        glEnd();
    }

    void drawDensity() {
        float x;
        float y;
        float d00;
        float d01;
        float d10;
        float d11;

        int rowSize = fluidsSolver->getRowSize();
        int colSize = fluidsSolver->getColSize();

        glBegin(GL_QUADS);
        for (int i = 1; i <= rowSize - 2; i++) {
            x = (float) i;
            for (int j = 1; j <= colSize - 2; j++) {
                y = (float) j;

                d00 = fluidsSolver->getDens(i, j);
                d01 = fluidsSolver->getDens(i, j + 1);
                d10 = fluidsSolver->getDens(i + 1, j);
                d11 = fluidsSolver->getDens(i + 1, j + 1);

                glColor3f(1.0f - d00, 1.0f, 1.0f - d00);
                glVertex2f(x, y);
                glColor3f(1.0f - d10, 1.0f, 1.0f - d10);
                glVertex2f(x + 1.0f, y);
                glColor3f(1.0f - d11, 1.0f, 1.0f - d11);
                glVertex2f(x + 1.0f, y + 1.0f);
                glColor3f(1.0f - d01, 1.0f, 1.0f - d01);
                glVertex2f(x, y + 1.0f);
            }
        }
        glEnd();
    }

    void initializeWindow(int argc, char **argv, std::string title) {
        fluidsSolver = new FluidsSolver();
        fluidsSolver->reset();

        // Initialize OpenGL
        glutInit(&argc, argv);
        glutInitWindowSize(currentW, currentH);
        glutInitWindowPosition(0, 0);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutCreateWindow(title.c_str());
    }

    void processNormalKeys(unsigned char key, int x, int y) {

        switch (key) {
            case 'v':
            case 'V':
                displayWindow = (displayWindow + 1) % 2;
                break;

            case 'c':
            case 'C':
                fluidsSolver->reset();
                break;
            case 27: // escape
                exit(0);
                break;
        }

    }

    void getInput() {
        fluidsSolver->clearBuffer();

        //int totSize = fluidsSolver->getTotSize();
        int rowSize = fluidsSolver->getRowSize();
        int colSize = fluidsSolver->getColSize();

        int xPos;
        int yPos;

        if (mouseDown[0] || mouseDown[2]) {
            xPos = (int) ((float) (oldMouseX) / currentW * (rowSize));
            yPos = (int) ((float) (HEIGHT - oldMouseY) / currentH * (colSize));

            if (xPos > 0 && xPos < rowSize - 1 && yPos > 0 && yPos < colSize - 1) {
                if (mouseDown[0]) {
                    fluidsSolver->setVX0(xPos, yPos, 1.0f * (mouseX - oldMouseX));
                    fluidsSolver->setVY0(xPos, yPos, 1.0f * (oldMouseY - mouseY));
                }

                if (mouseDown[2]) {
                    fluidsSolver->setD0(xPos, yPos, 10.0f);
                }

                oldMouseX = mouseX;
                oldMouseY = mouseY;
            }

            fluidsSolver->addSource();
        }
    }

    void mouseMove(int x, int y) {
        mouseX = x;
        mouseY = y;
    }

    void mouseActivate(int button, int state, int x, int y) {
        oldMouseX = x;
        oldMouseY = y;

        mouseX = x;
        mouseY = y;

        mouseDown[button] = state == GLUT_DOWN;
    }

    void run() {
        timeElapsed = glutGet(GLUT_ELAPSED_TIME);

        // Register rendering callbacks
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);
        glutIdleFunc(idleRenderScene);


        // Register keyboard and mouse callbacks
        glutKeyboardFunc(processNormalKeys);
        glutMouseFunc(mouseActivate);
        glutMotionFunc(mouseMove);



        // Run main loop
        glutMainLoop();
    }

    void changeSize(int w, int h) {

        glutReshapeWindow(w, h);

        currentW = w;
        currentH = h;
    }


}
