#ifndef OIL_SPILL_SIMULATION_FLUIDS_SOLVER_H
#define OIL_SPILL_SIMULATION_FLUIDS_SOLVER_H

#define IX(i, j) ((i)+(N+2)*(j))
#define SWAP(x, y) {float * tmp=x;x=y;y=tmp;}

#include <string.h>
#include "iostream"

class FluidsSolver {
public:
    FluidsSolver(int w = 128, int h = 128, float viscosityCoefficient = 1.0f,
                 float vorticityCoefficient = 0.0f, float diffusionCoefficient = 0.02f, float timeStep = 1.0,
                 bool ignoreBorders = true);

    ~FluidsSolver();

    // Export any array to CSV file
    void exportArrayToCSV(float *values, std::string fileName);

    // Reset velocity and density arrays
    void reset();

    void resetVelocity();

    void resetDensity();

    // Add sources of velocity and density (Previous state)
    void addSource();

    // Add density spot
    void addDensitySpot(int xPosition, int yPosition, int radius = 25, int stepSize= 2);

    void fillCircle(int xPosition, int yPosition, int densityValue, int radius);

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

    // Update densities
    void updateNormalizedDensities();

    // Update vortex confinement
    void updateVortexConfinement();

    // Getters
    int getWidth() { return w; }

    int getHeight() { return h; }

    int getFullGridSize() { return fullGridSize; }

    float *getVelocityX() { return velocityX; }

    float *getVelocityY() { return velocityY; }

    float *getDensity() { return density; }

    float *getNormalizedDensity() { return normalizedDensity; }

    float *getParticlesX() { return particleX; }

    float *getParticlesY() { return particleY; }

    float getNormalizedDensityPoint(int i, int j) {
        return
                (normalizedDensity[cIdx(i - 1, j - 1)] + normalizedDensity[cIdx(i, j - 1)] +
                 normalizedDensity[cIdx(i - 1, j)] +
                 normalizedDensity[cIdx(i, j)]) / 4.0f;
    }

    //setter
    void setPreviousVelocityX(int i, int j, float value) { prevVelocityX[cIdx(i, j)] = value; }

    void setPreviousVelocityY(int i, int j, float value) { prevVelocityY[cIdx(i, j)] = value; }

    void setPreviousDensity(int i, int j, float value) { prevDensity[cIdx(i, j)] = value; }

    void changeViscosityCoefficient(bool increase) {
        increase ? viscosityCoefficient += 0.001 : viscosityCoefficient -= 0.001;
        if (viscosityCoefficient < 0)
            viscosityCoefficient = 0;
        std::cout << "Viscosity is now " << viscosityCoefficient << std::endl;
    }

    void changeDiffusionCoefficient(bool increase) {
        increase ? diffusionCoefficient += 0.001 : diffusionCoefficient -= 0.001;
        if (diffusionCoefficient < 0)
            diffusionCoefficient = 0;
        std::cout << "Diffusion is now " << diffusionCoefficient << std::endl;
    }

    void changeIgnoreBorders() {
        ignoreBorders = !ignoreBorders;
        std::cout << "Toggled ignore borders to " << ignoreBorders << std::endl;
    }

    void addWindX() {
        for (int i = 0; i < fullGridSize; i++) {
            velocityX[i] += 0.5f;
        }
        std::cout << "Add Wind X " << std::endl;
    }

    void addWindY() {
        for (int i = 0; i < fullGridSize; i++) {
            velocityY[i] += 0.5f;
        }
        std::cout << "Add Wind Y " << std::endl;
    }

    void subtractWindX() {
        for (int i = 0; i < fullGridSize; i++) {
            velocityX[i] -= 0.5f;
        }
        std::cout << "Subtract Wind X " << std::endl;
    }

    void subtractWindY() {
        for (int i = 0; i < fullGridSize; i++) {
            velocityY[i] -= 0.5f;
        }
        std::cout << "Subtract Wind Y " << std::endl;
    }

private:
    int w;
    int h;
    float viscosityCoefficient;
    float vorticityCoefficient;
    float diffusionCoefficient;
    float timeStep;
    bool ignoreBorders;


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
    float *normalizedDensity;
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
