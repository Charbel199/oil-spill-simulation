#include "parameters.h"
#include "iostream"
#include "simulation_parameters.h"
#include <stdlib.h>

Parameters::Parameters(int argc, char** argv){
    this->argc = argc;
    this->argv = argv;
}

void Parameters::show_usage(){
    std::cerr << "Usage: " << argv[0] << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-w,--wind-speed \tSpecify the wind speed"
              << "\t-wa,--wind-alignment \tSpecify the wind alignment"
              << std::endl;
}

void Parameters::parse_parameters(SimulationParameters& simulationParameters) {

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        // If '-' or '--' argument check that value is also provided
        if (arg[0] == '-'){
            if (i + 1 > argc){
                std::cerr << "--destination option requires one argument." << std::endl;
                return;
            }
        }

        // Parsing parameters
        if ((arg == "-h") || (arg == "--help")) {
            show_usage();
            return;
        } else if ((arg == "-w") || (arg == "--wind-speed")) {
            simulationParameters.wind_speed = atof(argv[++i]);
        }else if ((arg == "-wa") || (arg == "--wind-alignment")) {
            simulationParameters.wind_alignment = atof(argv[++i]);
        }

    }
}



