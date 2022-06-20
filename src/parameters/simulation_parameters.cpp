#include "simulation_parameters.h"
#include "iostream"

SimulationParameters::SimulationParameters() {
    // Set default simulation values
    this->wind_alignment = 0;
    this->wind_speed = 0;
}

bool SimulationParameters::check_parameters() {
    if (wind_speed < 0) {
        std::cerr << "Wind speed needs to be positive" << std::endl;
        return false;
    }
    if (wind_alignment < 0) {
        std::cerr << "Wind alignment needs to be positive" << std::endl;
        return false;
    }
    return true;
}