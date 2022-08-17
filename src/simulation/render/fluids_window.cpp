#include "fluids_window.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "open_gl_helper.h"
#include "fluids_solver.h"
#include <string>
#include <filesystem>
#include "iostream"
#include <experimental/filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>

namespace FluidsWindow {
    FluidsSolver *fluidsSolver;
    int timeElapsed;
    int currentW = WIDTH, currentH = HEIGHT;
    int mouseX;
    int mouseY;
    int oldMouseX;
    int oldMouseY;
    int mouseDown[5];
    int displayWindow = 1;
    int radius = 25;
    int stepSize = 2;
    std::string output_path;

    void renderScene() {
        getInput();
        fluidsSolver->updateVortexConfinement();
        fluidsSolver->updateVelocities();
        fluidsSolver->updateDensities();

        glViewport(0, 0, currentW, currentH);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0f, (float) (fluidsSolver->getWidth()), 0.0f, (float) (fluidsSolver->getHeight()));

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (displayWindow == 0) drawDensity();
        if (displayWindow == 1) drawVelocity();

        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < fluidsSolver->getFullGridSize(); i++) {
            glVertex2f(fluidsSolver->getParticlesX()[i], fluidsSolver->getParticlesY()[i]);
        }
        glEnd();

        glutSwapBuffers();

    }

    void idleRenderScene() {
        glutPostRedisplay();

    }

    void drawVelocity() {
        float *px = fluidsSolver->getParticlesX();
        float *py = fluidsSolver->getParticlesY();
        float *vx = fluidsSolver->getVelocityX();
        float *vy = fluidsSolver->getVelocityY();

        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(1.0f);

        glBegin(GL_LINES);
        for (int i = 0; i < fluidsSolver->getFullGridSize(); i++) {
            glVertex2f(px[i], py[i]);
            glVertex2f(px[i] + vx[i] * 10.0f, py[i] + vy[i] * 10.0f);
        }
        glEnd();
    }

    void densityToColor(float density) {
        float normalizedDensity = density / 10;
        if (normalizedDensity < (0.25)) {
            glColor3f(0.0f, 4 * normalizedDensity, 1.0f);
        } else if (normalizedDensity < 0.5) {
            glColor3f(0.0f, 1.0f, 1 + 4 * (0.25 - normalizedDensity));
        } else if (normalizedDensity < 0.75) {
            glColor3f(4 * (normalizedDensity - 0.5), 1.0f, 0.0f);
        } else {
            glColor3f(1.0f, 1 + 4 * (0.75 - normalizedDensity), 0.0f);
        }
    }

    void drawDensity() {
        float x;
        float y;
        float d00;
        float d01;
        float d10;
        float d11;

        int rowSize = fluidsSolver->getWidth();
        int colSize = fluidsSolver->getHeight();

        glBegin(GL_QUADS);
        for (int i = 1; i <= rowSize - 2; i++) {
            x = (float) i;
            for (int j = 1; j <= colSize - 2; j++) {
                y = (float) j;

                d00 = fluidsSolver->getNormalizedDensityPoint(i, j);
                d01 = fluidsSolver->getNormalizedDensityPoint(i, j + 1);
                d10 = fluidsSolver->getNormalizedDensityPoint(i + 1, j);
                d11 = fluidsSolver->getNormalizedDensityPoint(i + 1, j + 1);


                densityToColor(d00);
                glVertex2f(x, y);
                densityToColor(d10);
                glVertex2f(x + 1.0f, y);
                densityToColor(d11);
                glVertex2f(x + 1.0f, y + 1.0f);
                densityToColor(d01);
                glVertex2f(x, y + 1.0f);
            }
        }
        glEnd();
    }

    void printInstructions() {
        std::cout << "\nHow to use this demo:\n\n";
        std::cout << "=================================\n";
        std::cout << "\nMouse controls:\n";
        std::cout << "RIGHT MOUSE CLICK\tAdd densities\n";
        std::cout << "LEFT MOUSE CLICK\tAdd velocities\n";
        std::cout << "MIDDLE MOUSE CLICK\tAdd density spot\n";
        std::cout << "SCROLL WHEEL\tChange density spot radius\n";
        std::cout << "\nKeyboard controls:\n";
        std::cout << "'Q'\t\t\tIncrease density spot step size\n";
        std::cout << "'E'\t\t\tDecrease density spot step size\n";
        std::cout << "\n";
        std::cout << "'R'\t\t\tIncrease viscosity\n";
        std::cout << "'F'\t\t\tDecrease viscosity\n";
        std::cout << "\n";
        std::cout << "'T'\t\t\tIncrease diffusion\n";
        std::cout << "'G'\t\t\tDecrease diffusion\n";
        std::cout << "\n";
        std::cout << "'V'\t\t\tToggle density/velocity display\n";
        std::cout << "'B'\t\t\tToggle ignore borders\n";
        std::cout << "\n";
        std::cout << "'W'\t\t\tWind direction: Top\n";
        std::cout << "'A'\t\t\tWind direction: Left\n";
        std::cout << "'S'\t\t\tWind direction: Bottom\n";
        std::cout << "'D'\t\t\tWind direction: Right\n";
        std::cout << "\n";
        std::cout << "'P'\t\t\tEnvironment to CSV\n";
        std::cout << "'X'\t\t\tClear density\n";
        std::cout << "'Z'\t\t\tClear velocity\n";
        std::cout << "'C'\t\t\tClear the entire simulation\n";
        std::cout << "'Esc'\t\t\tQuit\n";
        std::cout << "=================================\n";
    }

    void initializeWindow(int argc, char **argv, std::string title) {
        parse_config();
        fluidsSolver = new FluidsSolver(100, 100);
        fluidsSolver->reset();

        // Initialize OpenGL
        glutInit(&argc, argv);
        glutInitWindowSize(currentW, currentH);
        glutInitWindowPosition(0, 0);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutCreateWindow(title.c_str());

        // Print instructions
        printInstructions();
    }

    void parse_config() {
        // Parse config json file
        std::ifstream ifs("src/config/config.json");
        nlohmann::json js = nlohmann::json::parse(ifs);

        // Set variables
        output_path = js["output_path"];
    }

    std::string checkNextFileName(std::string directory, std::string prefix) {
        std::vector<int> file_index;
        for (const auto &entry: std::experimental::filesystem::directory_iterator(directory)) {
            std::string str = entry.path().filename().string();
            size_t i = 0;
            for (; i < str.length(); i++) { if (isdigit(str[i])) break; }

            // remove the first chars, which aren't digits
            str = str.substr(i, str.length() - i);
            // get file index
            int num = atoi(str.c_str());

            file_index.push_back(num);

        }
        int max_index = *max_element(file_index.begin(), file_index.end());
        return directory + "/" + prefix + std::to_string(max_index + 1);

    }

    void processNormalKeys(unsigned char key, int x, int y) {
        std::string file_path;
        switch (key) {
            case 'v':
            case 'V':
                displayWindow = (displayWindow + 1) % 2;
                break;
            case 'p':
            case 'P':
                file_path = checkNextFileName(output_path, "y");
                fluidsSolver->exportArrayToCSV(fluidsSolver->getNormalizedDensity(), file_path);
                break;
            case 'd':
            case 'D':
                fluidsSolver->addWindX();
                break;
            case 'w':
            case 'W':
                fluidsSolver->addWindY();
                break;
            case 'a':
            case 'A':
                fluidsSolver->subtractWindX();
                break;
            case 's':
            case 'S':
                fluidsSolver->subtractWindY();
                break;
            case 'b':
            case 'B':
                fluidsSolver->changeIgnoreBorders();
                break;
            case 'r':
            case 'R':
                fluidsSolver->changeViscosityCoefficient(true);
                break;
            case 'f':
            case 'F':
                fluidsSolver->changeViscosityCoefficient(false);
                break;
            case 't':
            case 'T':
                fluidsSolver->changeDiffusionCoefficient(true);
                break;
            case 'g':
            case 'G':
                fluidsSolver->changeDiffusionCoefficient(false);
                break;
            case 'x':
            case 'X':
                fluidsSolver->resetDensity();
                break;
            case 'z':
            case 'Z':
                fluidsSolver->resetVelocity();
                break;
            case 'c':
            case 'C':
                fluidsSolver->reset();
                break;
            case 'q':
            case 'Q':
                stepSize++;
                std::cout<<"Increased step size to "<<stepSize<<std::endl;
                break;
            case 'e':
            case 'E':
                stepSize = (stepSize > 1) ? stepSize - 1 : 1;
                std::cout<<"Decreased step size to "<<stepSize<<std::endl;
                break;
            case 27: // escape
                exit(0);
                break;
        }

    }

    void getInput() {
        fluidsSolver->clearBuffer();

        int rowSize = fluidsSolver->getWidth();
        int colSize = fluidsSolver->getHeight();

        int xPos;
        int yPos;

        if (mouseDown[0] || mouseDown[2]) {
            xPos = (int) ((float) (oldMouseX) / currentW * (rowSize));
            yPos = (int) ((float) (HEIGHT - oldMouseY) / currentH * (colSize));

            if (xPos > 0 && xPos < rowSize - 1 && yPos > 0 && yPos < colSize - 1) {
                if (mouseDown[0]) {
                    fluidsSolver->setPreviousVelocityX(xPos, yPos, 1.0f * (mouseX - oldMouseX));
                    fluidsSolver->setPreviousVelocityY(xPos, yPos, 1.0f * (oldMouseY - mouseY));
                }

                if (mouseDown[2]) {
                    fluidsSolver->setPreviousDensity(xPos, yPos, 30.0f);
                }

                oldMouseX = mouseX;
                oldMouseY = mouseY;
            }

            fluidsSolver->addSource();
        } else if (mouseDown[1]) {
            xPos = (int) ((float) (oldMouseX) / currentW * (rowSize));
            yPos = (int) ((float) (HEIGHT - oldMouseY) / currentH * (colSize));
            fluidsSolver->addDensitySpot(xPos, yPos, radius, stepSize);

            oldMouseX = mouseX;
            oldMouseY = mouseY;
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


        // Wheel reports as button 3(scroll up) and button 4(scroll down)
        if ((button == 3) || (button == 4)) // It's a wheel event
        {
            // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
            if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
            if (button == 3) radius++;
            if (button == 4) radius = (radius > 3) ? radius - 1 : 3;
        } else {
            mouseDown[button] = state == GLUT_DOWN;
        }
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
