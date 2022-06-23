#include "fluids_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FluidsSolver::FluidsSolver(int w, int h, float viscosity, float vorticity, float diffusionCoefficient, float timeStep) {
    this->w = w;
    this->h = h;
    this->viscosityCoefficient = viscosity;
    this->vorticityCoefficient = vorticity;
    this->diffusionCoefficient = diffusionCoefficient;
    this->timeStep = timeStep;

    int fullGridSize = w*h;

    // Allocating memory, all vectors will be 1D vector for efficiency

    // Velocity
    velocityX = (float *)malloc(sizeof(float)*fullGridSize);
    velocityY = (float *)malloc(sizeof(float)*fullGridSize);
    prevVelocityX = (float *)malloc(sizeof(float)*fullGridSize);
    prevVelocityY = (float *)malloc(sizeof(float)*fullGridSize);

    // Density
    density = (float *)malloc(sizeof(float)*fullGridSize);
    prevDensity = (float *)malloc(sizeof(float)*fullGridSize);

    // Pressure
    pressureX = (float *)malloc(sizeof(float)*fullGridSize);
    pressureY = (float *)malloc(sizeof(float)*fullGridSize);
    div = (float *)malloc(sizeof(float)*fullGridSize);
    pressure = (float *)malloc(sizeof(float)*fullGridSize);

    // Vorticity
    vorticity = (float *)malloc(sizeof(float)*fullGridSize);
    absVorticity = (float *)malloc(sizeof(float)*fullGridSize);
    gradVorticityX = (float *)malloc(sizeof(float)*fullGridSize);
    gradVorticityY = (float *)malloc(sizeof(float)*fullGridSize);
    lenGradient = (float *)malloc(sizeof(float)*fullGridSize);
    voriticityfx = (float *)malloc(sizeof(float)*fullGridSize);
    vorticityfy = (float *)malloc(sizeof(float)*fullGridSize);
}

FluidsSolver::~FluidsSolver() {
    // Free all malloc
    // Velocity
    free(velocityX)
    free(velocityY)
    free(prevVelocityX)
    free(prevVelocityY)

    // Density
    free(density)
    free(prevDensity)

    // Pressure
    free(pressureX)
    free(pressureY)
    free(div)
    free(pressure)

    // Vorticity
    free(vorticity)
    free(absVorticity)
    free(gradVorticityX)
    free(gradVorticityY)
    free(lenGradient)
    free(voriticityfx)
    free(vorticityfy)
}