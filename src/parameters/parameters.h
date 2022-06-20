#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP
#include "simulation_parameters.h"
class Parameters {
    public:
        Parameters(int argc, char** argv);


        void parse_parameters(SimulationParameters& simulationParameters);
    private:
        void show_usage();

        int argc;
        char** argv;
};
#endif