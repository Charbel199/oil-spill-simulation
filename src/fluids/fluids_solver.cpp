#include "fluids_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iostream"
#include <fstream>

FluidsSolver::FluidsSolver(int w = 128, int h = 128, float viscosityCoefficient = 0.0001f,
                           float vorticityCoefficient = 0.0f, float diffusionCoefficient = 0.0f, float timeStep = 1.0) {
    this->w = w;
    this->h = h;
    this->viscosityCoefficient = viscosityCoefficient;
    this->vorticityCoefficient = vorticityCoefficient;
    this->diffusionCoefficient = diffusionCoefficient;
    this->timeStep = timeStep;

    int fullGridSize = w * h;

    minX = 1.0f;
    maxX = w - 1.0f;
    minY = 1.0f;
    maxY = h - 1.0f;

    // Allocating memory, all vectors will be 1D vector for efficiency

    // Velocity
    velocityX = (float *) malloc(sizeof(float) * fullGridSize);
    velocityY = (float *) malloc(sizeof(float) * fullGridSize);
    prevVelocityX = (float *) malloc(sizeof(float) * fullGridSize);
    prevVelocityY = (float *) malloc(sizeof(float) * fullGridSize);

    // Density
    density = (float *) malloc(sizeof(float) * fullGridSize);
    prevDensity = (float *) malloc(sizeof(float) * fullGridSize);

    // Pressure
    pressureX = (float *) malloc(sizeof(float) * fullGridSize);
    pressureY = (float *) malloc(sizeof(float) * fullGridSize);
    div = (float *) malloc(sizeof(float) * fullGridSize);
    pressure = (float *) malloc(sizeof(float) * fullGridSize);

    // Vorticity
    vorticity = (float *) malloc(sizeof(float) * fullGridSize);
    absVorticity = (float *) malloc(sizeof(float) * fullGridSize);
    gradVorticityX = (float *) malloc(sizeof(float) * fullGridSize);
    gradVorticityY = (float *) malloc(sizeof(float) * fullGridSize);
    lenGradient = (float *) malloc(sizeof(float) * fullGridSize);
    voriticityfx = (float *) malloc(sizeof(float) * fullGridSize);
    vorticityfy = (float *) malloc(sizeof(float) * fullGridSize);
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

void FluidsSolver::exportArrayToCSV(float *values, std::string fileName) {
    ofstream outputFile;
    filename = fileName + ".csv";
    outputFile.open(filename);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (j != h - 1) {
                outputFile << values[cIdx(i, j)] << ",";
            } else {
                outputFile << values[cIdx(i, j)];
            }

        }
        outputFile << endl;
    }
    outputFile.close();
}

void FluidsSolver::reset() {
    for (int i = 0; i < totSize; i++) {
        velocityX[i] = 0.0f;
        velocityY[i] = 0.0f;
        density[i] = 0.0f;
    }
}

void FluidsSolver::clearBuffer() {
    memset(prevVelocityX, 0, sizeof(float) * totSize);
    memset(prevVelocityY, 0, sizeof(float) * totSize);
    memset(prevDensity, 0, sizeof(float) * totSize);
}