#ifndef OIL_SPILL_SIMULATION_SIMULATION_PARAMETERS_H
#define OIL_SPILL_SIMULATION_SIMULATION_PARAMETERS_H


class SimulationParameters {
public:
    SimulationParameters();

    bool check_parameters();

    float wind_speed;
    float wind_alignment;
};


#endif //OIL_SPILL_SIMULATION_SIMULATION_PARAMETERS_H
