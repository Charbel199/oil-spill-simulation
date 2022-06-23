#include "window.h"
#include "camera.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "open_gl_helper.h"


namespace Window {
    int timeElapsed;
    int currentW = WIDTH, currentH = HEIGHT;
    int frame = 0;
    float red = 0, green = 0, blue = 0;
    // angle of rotation of object
    float angle = 0.0;
    Camera *camera;

    void drawFps() {
        Color color = {1, 0, 0, 0};
        Position pos = {0, 640, 480, 0};
        PositionXY posXY = {350, 460};
        OpenGLHelper::drawText(color, pos, posXY, "TEST");
    }

    void renderScene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset transformations
        glLoadIdentity();

        // Set the camera
        gluLookAt(camera->getX(), camera->getY(), camera->getZ(), camera->getSightX(), camera->getSightY(),
                  camera->getSightZ(), 0, 1, 0);

        // Draw
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glColor3f(red, green, blue);
        glBegin(GL_TRIANGLES);
        glVertex3f(-2.0f, -2.0f, 0.0f);
        glVertex3f(2.0f, 0.0f, 0.0);
        glVertex3f(0.0f, 2.0f, 0.0);
        glEnd();
        angle += 0.1f;
        //Draw
        drawFps();


        // Swap between back buffer  (Where drawing is happening) and front buffer (Where drawing is shown)
        glutSwapBuffers();

        frame++;
//        std::cout << frame << std::endl;

    }

    void initializeWindow(int argc, char **argv, std::string title, float translationSpeed) {
        // Initialize OpenGL
        glutInit(&argc, argv);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitWindowPosition(0, 0);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutCreateWindow(title.c_str());
        glEnable(GL_MULTISAMPLE);

        // Initialize Camera
        camera = new Camera(-1, 1, -1, 4 * M_PI / 7, M_PI / 4, 0.01, translationSpeed, WIDTH, HEIGHT);

    }

    void processNormalKeys(unsigned char key, int x, int y) {

        if (key == 27)
            exit(0);
        camera->translation(key);

    }

    void processSpecialKeys(int key, int xx, int yy) {
        camera->translation(key);
    }

    void mouseMove(int x, int y) {
        camera->rotation(x, y);
    }

    void run() {
        timeElapsed = glutGet(GLUT_ELAPSED_TIME);
        // Set background color
        glClearColor(0, 1, 1, 0.5);
        glClear(GL_COLOR_BUFFER_BIT);

        // Register rendering callbacks
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);
        glutIdleFunc(renderScene);


        // Register keyboard and mouse callbacks
        glutKeyboardFunc(processNormalKeys);
        glutSpecialFunc(processSpecialKeys);
        glutPassiveMotionFunc(mouseMove);

        // OpenGL init
        glEnable(GL_DEPTH_TEST);

        // Run main loop
        glutMainLoop();
    }

    void changeSize(int w, int h) {

        // Prevent a divide by zero, when window is too short
        // (you cant make a window of zero width).
        if (h == 0)
            h = 1;
        float ratio = 1.0 * w / h;

        // Update current width and height
        currentW = w;
        currentH = h;

        // Use the Projection Matrix
        glMatrixMode(GL_PROJECTION);

        // Reset Matrix
        glLoadIdentity();

        // Set the viewport to be the entire window
        glViewport(0, 0, w, h);

        // Set the correct perspective.
        gluPerspective(45, ratio, 1, 1000);

        // Get Back to the Modelview
        glMatrixMode(GL_MODELVIEW);
    }


}
