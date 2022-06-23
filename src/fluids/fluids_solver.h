#ifndef OIL_SPILL_SIMULATION_FLUIDS_SOLVER_H
#define OIL_SPILL_SIMULATION_FLUIDS_SOLVER_H

#define IX(i, j) ((i)+(N+2)*(j))
#define SWAP(x, y) {float * tmp=x;x=y;y=tmp;}

class FluidsSolver {
public:
    FluidsSolver();

    ~FluidsSolver() {};

    // Export any array to CSV file
    void exportArrayToCSV(float *values, std::string fileName);

    // Reset velocity and density arrays
    void reset();

    // Clear previous velocity and density buffers
    void clearBuffer();

    // Update boundaries (Flag: 0 represents the general case,
    // 1 represents the x-axis directional component,
    // 2 represents the y-axis directional component)
    void updateBoundaries(float *values, int flag);


private:
    int w;
    int h;
    float viscosityCoefficient;
    float vorticityCoefficient;
    float diffusionCoefficient;
    float timeStep;


    int cIdx(int i, int j) { return j * h + i; }


    float minX;
    float maxX;
    float minY;
    float maxY;


    // Variables

    // Velocity
    float *velocityX;
    float *velocityY;
    float *prevVelocityX;
    float *prevVelocityY;

    // Density
    float *density;
    float *prevDensity;

    // Pressure
    float *pressureX;
    float *pressureY;
    float *div;
    float *pressure;

    // Vorticity
    float *vorticity;
    float *absVorticity;
    float *gradVorticityX;
    float *gradVorticityY;
    float *lenGradient;
    float *voriticityfx;
    float *vorticityfy;
};

#endif //OIL_SPILL_SIMULATION_FLUIDS_SOLVER_H
