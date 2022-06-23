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

    fullGridSize = w * h;

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

    // Particle centers (Center of each grid)
    particleX = (float *) malloc(sizeof(float) * fullGridSize);
    particleY = (float *) malloc(sizeof(float) * fullGridSize);

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
    free(particleX)
    free(particleY)
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

void FluidsSolver::updateEdges(float *values, int flag) {

    // General case
    if (flag == 0) {
        for (int i = 1; i <= w - 2; i++) {
            values[cIdx(i, 0)] = values[cIdx(i, 1)];
            values[cIdx(i, h - 1)] = values[cIdx(i, h - 2)];
        }
        for (int j = 1; j <= h - 2; j++) {
            values[cIdx(0, j)] = values[cIdx(1, j)];
            values[cIdx(w - 1, j)] = values[cIdx(w - 2, j)];
        }
    }

    // x-axis directional component
    if (flag == 1) {
        for (int i = 1; i <= w - 2; i++) {
            values[cIdx(i, 0)] = values[cIdx(i, 1)];
            values[cIdx(i, h - 1)] = values[cIdx(i, h - 2)];
        }
        for (int j = 1; j <= h - 2; j++) {
            // Invert x velocity at left and right borders
            values[cIdx(0, j)] = -values[cIdx(1, j)];
            values[cIdx(w - 1, j)] = -values[cIdx(w - 2, j)];
        }
    }

    // y-axis directional component
    if (flag == 2) {
        for (int i = 1; i <= w - 2; i++) {
            // Invert x velocity at top and bottom borders
            values[cIdx(i, 0)] = -values[cIdx(i, 1)];
            values[cIdx(i, h - 1)] = -values[cIdx(i, h - 2)];
        }
        for (int j = 1; j <= h - 2; j++) {
            values[cIdx(0, j)] = values[cIdx(1, j)];
            values[cIdx(w - 1, j)] = values[cIdx(w - 2, j)];
        }
    }

    // Update 4 corners (Average of adjacent cells)
    values[cIdx(0, 0)] = (values[cIdx(0, 1)] + values[cIdx(1, 0)]) / 2;
    values[cIdx(w - 1, 0)] = (values[cIdx(w - 2, 0)] + values[cIdx(w - 1, 1)]) / 2;
    values[cIdx(0, h - 1)] = (values[cIdx(0, h - 2)] + values[cIdx(1, h - 1)]) / 2;
    values[cIdx(w - 1, h - 1)] = (values[cIdx(w - 2, h - 1)] + values[cIdx(w - 1, h - 2)]) / 2;
}

void FluidsSolver::diffusion(float *values, float *previousValues, float diffusionRate, int flag) {
    // Reset all new values to 0
    for (int i = 0; i < fullGridSize; i++)
        values[i] = 0.0f;


    float diffusionAmount = diffusionRate * timeStep;

    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= rowSize - 2; i++) {
            for (int j = 1; j <= colSize - 2; j++) {
                // Grid square value takes the average of all adjacent grid squares (Top, left, right, bottom)
                values[cIdx(i, j)] = (previousValues[cIdx(i, j)] +
                        diffusionAmount *(values[cIdx(i + 1, j)] +
                        values[cIdx(i - 1, j)] + values[cIdx(i, j + 1)] +values[cIdx(i, j - 1)]))
                                /(4.0f * a + 1.0f);
            }
        }
        updateEdges(values, flag);
    }
}

void FluidsSolver::advection(float *values, float *previousValues, float *velocityX, float *velocityY, int flag) {
    float sourceX;
    float sourceY;
    int leftGridPointX;
    int rightGridPointX;
    int bottomGridPointY;
    int topGridPointY;
    float weightLeft;
    float weightRight;
    float weightBottom;
    float weightTop;

    for (int i = 1; i <= rowSize - 2; i++) {
        for (int j = 1; j <= colSize - 2; j++) {
            // Get previous particle (source0 position
            sourceX = particleX[cIdx(i, j)] - velocityX[cIdx(i, j)] * timeStep;
            sourceY = particleY[cIdx(i, j)] - velocityY[cIdx(i, j)] * timeStep;

            // Check edges
            if (sourceX < minX) sourceX = minX;
            if (sourceX > maxX) sourceX = maxX;
            if (sourceY < minY) sourceY = minY;
            if (sourceY > maxY) sourceY = maxY;

            // Find 4 nearest grid points
            leftGridPointX = (int) (sourceX - 0.5f);
            bottomGridPointY = (int) (sourceY - 0.5f);
            rightGridPointX = leftGridPointX + 1;
            topGridPointY = bottomGridPointY + 1;

            // Interpolate and get weights for Left, Right, Bottom and Top grid points (if source particle is close to
            // the top right grid point, weights for top and right would be higher)
            // TODO: Recheck logic
            weightLeft = particleX[cIdx(rightGridPointX, bottomGridPointY)] - sourceX;
            weightRight = 1.0f - weightLeft;
            weightBottom = particleY[cIdx(leftGridPointX, topGridPointY)] - sourceY;
            weightTop = 1.0f - weightBottom;

            values[cIdx(i, j)] = weightBottom * (weightLeft * previousValues[cIdx(leftGridPointX, bottomGridPointY)] +
                                                 weightRight *
                                                 previousValues[cIdx(rightGridPointX, bottomGridPointY)]) +
                                 weightTop * (weightLeft * previousValues[cIdx(leftGridPointX, topGridPointY)] +
                                              weightRight * previousValues[cIdx(rightGridPointX, topGridPointY)]);
        }
    }

    updateEdges(values, flag);
}