#include "simulation_parameters.h"
#include "parameters.h"
#include "iostream"

int main(int argc, char **argv) {
    // Initialize parameters
    SimulationParameters simulationParameters;
    Parameters parameters = Parameters(argc, argv);


    // Parse and check parameters
    parameters.parse_parameters(simulationParameters);
    simulationParameters.check_parameters();

    return 0;
}