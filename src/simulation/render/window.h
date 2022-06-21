#ifndef OIL_SPILL_SIMULATION_WINDOW_H
#define OIL_SPILL_SIMULATION_WINDOW_H

#define WIDTH  600
#define HEIGHT 600

#include "iostream"

class Window{
    public:
        Window(int argc, char** argv, std::string title, float translationSpeed);

        void draw();
        void keyDown(unsigned char, int, int);
        void keyUp(unsigned char, int, int);
        void mouseMove(int, int);
        void reshape(int, int);

        void run();
        void quit();

        int timeElapsedForSetup;

};


#endif //OIL_SPILL_SIMULATION_WINDOW_H