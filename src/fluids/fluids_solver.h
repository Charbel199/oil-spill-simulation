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

    // Add sources of velocity and density (Previous state)
    void addSources();

    // Clear previous velocity and density buffers
    void clearBuffer();

    // Update edges (Flag: 0 represents the general case,
    // 1 represents the x-axis directional component,
    // 2 represents the y-axis directional component)
    void updateEdges(float *values, int flag);

    // Apply diffusion
    void diffusion(float *values, float *previousValues, float diffusionRate, int flag);

    // Apply advection
    void advection(float *values, float *previousValues, float *velocityX, float *velocityY, int flag);

    // Use hemholtz decomposition method (Any vector field = curl-free vectors + divergence-free vectors)
    // We will extract the divergence free vectors
    void computeNewVelocities();

    // Update velocities
    void updateVelocities();

    // Update densities
    void updateDensities();


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
    int fullGridSize;

    // Variables

    // Velocity
    float *velocityX;
    float *velocityY;
    float *prevVelocityX;
    float *prevVelocityY;

    // Density
    float *density;
    float *prevDensity;

    // Particles
    float *particleX;
    float *particleY;

    // Projection
    float *divergence;
    float *divergenceFreeVelocityField;

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
