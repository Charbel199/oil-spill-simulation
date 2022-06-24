#ifndef OIL_SPILL_SIMULATION_FLUIDS_WINDOW_H
#define OIL_SPILL_SIMULATION_FLUIDS_WINDOW_H


#define WIDTH  1600
#define HEIGHT 900

#include "iostream"

namespace FluidsWindow {

    void renderScene();

    void drawVelocity();

    void drawDensity();

    void densityToColor(float density);

    void idleRenderScene();

    void initializeWindow(int argc, char **argv, std::string title);

    void processNormalKeys(unsigned char key, int x, int y);


    void mouseMove(int x, int y);

    void mouseActivate(int button, int state, int x, int y);

    void run();

    void getInput();

    void changeSize(int w, int h);


};


#endif //OIL_SPILL_SIMULATION_FLUIDS_WINDOW_H
