#include "window.h"
#include "parameters.h"
#include "simulation_parameters.h"
#include "iostream"


int main(int argc, char **argv) {
    // Initialize parameters
    SimulationParameters simulationParameters;
    Parameters parameters = Parameters(argc, argv);


    // Parse and check parameters
    parameters.parse_parameters(simulationParameters);
    simulationParameters.check_parameters();

    Window *window = new Window(argc, argv, "Oil Spill Simulation", 1.0);
    window->run();

    return 0;
}