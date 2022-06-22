#ifndef OIL_SPILL_SIMULATION_WINDOW_H
#define OIL_SPILL_SIMULATION_WINDOW_H

#define WIDTH  1600
#define HEIGHT 900

#include "iostream"

namespace Window{




    void initializeWindow(int argc, char** argv, std::string title, float translationSpeed);
    void renderScene();

    void processSpecialKeys(int key, int x, int y);
    void processNormalKeys(unsigned char key, int x, int y);
    void keyDown(unsigned char, int, int);
    void keyUp(unsigned char, int, int);
    void mouseMove(int, int);
    void changeSize(int, int);

    void run();
    void quit();



    void drawFps();
};


#endif //OIL_SPILL_SIMULATION_WINDOW_H
