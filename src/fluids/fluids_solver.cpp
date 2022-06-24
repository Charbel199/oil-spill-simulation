#include "fluids_solver.h"
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <cmath>

FluidsSolver::FluidsSolver(int w, int h, float viscosityCoefficient, float vorticityCoefficient,
                           float diffusionCoefficient, float timeStep, bool ignoreBorders) {
    this->w = w;
    this->h = h;
    this->viscosityCoefficient = viscosityCoefficient;
    this->vorticityCoefficient = vorticityCoefficient;
    this->diffusionCoefficient = diffusionCoefficient;
    this->timeStep = timeStep;
    this->ignoreBorders = ignoreBorders;

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
    normalizedDensity = (float *) malloc(sizeof(float) * fullGridSize);
    prevDensity = (float *) malloc(sizeof(float) * fullGridSize);

    // Particle centers (Center of each grid)
    particleX = (float *) malloc(sizeof(float) * fullGridSize);
    particleY = (float *) malloc(sizeof(float) * fullGridSize);

    // Velocity field and divergence
    divergence = (float *) malloc(sizeof(float) * fullGridSize);
    divergenceFreeVelocityField = (float *) malloc(sizeof(float) * fullGridSize);

    // Vorticity
    vorticity = (float *) malloc(sizeof(float) * fullGridSize);
    absVorticity = (float *) malloc(sizeof(float) * fullGridSize);
    gradVorticityX = (float *) malloc(sizeof(float) * fullGridSize);
    gradVorticityY = (float *) malloc(sizeof(float) * fullGridSize);
    lenGradient = (float *) malloc(sizeof(float) * fullGridSize);
    voriticityfx = (float *) malloc(sizeof(float) * fullGridSize);
    vorticityfy = (float *) malloc(sizeof(float) * fullGridSize);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            particleX[cIdx(i, j)] = (float) i + 0.5f;
            particleY[cIdx(i, j)] = (float) j + 0.5f;
        }
    }
}

FluidsSolver::~FluidsSolver() {
    // Free all malloc
    // Velocity
    free(velocityX);
    free(velocityY);
    free(prevVelocityX);
    free(prevVelocityY);

    // Density
    free(density);
    free(normalizedDensity);
    free(prevDensity);

    // Pressure
    free(particleX);
    free(particleY);

    // Velocity field and divergence
    free(divergence);
    free(divergenceFreeVelocityField);

    // Vorticity
    free(vorticity);
    free(absVorticity);
    free(gradVorticityX);
    free(gradVorticityY);
    free(lenGradient);
    free(voriticityfx);
    free(vorticityfy);
}

void FluidsSolver::exportArrayToCSV(float *values, std::string fileName) {
    std::ofstream outputFile;
    fileName = fileName + ".csv";
    outputFile.open(fileName);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (j != h - 1) {
                outputFile << values[cIdx(i, j)] << ",";
            } else {
                outputFile << values[cIdx(i, j)];
            }

        }
        outputFile << std::endl;
    }
    outputFile.close();
}

void FluidsSolver::reset() {
    for (int i = 0; i < fullGridSize; i++) {
        velocityX[i] = 0.0f;
        velocityY[i] = 0.0f;
        density[i] = 0.0f;
    }
}

void FluidsSolver::resetVelocity() {
    for (int i = 0; i < fullGridSize; i++) {
        velocityX[i] = 0.0f;
        velocityY[i] = 0.0f;
    }
}

void FluidsSolver::resetDensity() {
    for (int i = 0; i < fullGridSize; i++) {
        density[i] = 0.0f;
    }
}

void FluidsSolver::addSource() {
    int index;
    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            index = cIdx(i, j);
            velocityX[index] += prevVelocityX[index];
            velocityY[index] += prevVelocityY[index];
            density[index] += prevDensity[index];
        }
    }

    updateEdges(density, 0);
    updateEdges(velocityX, 1);
    updateEdges(velocityY, 2);
}

void FluidsSolver::clearBuffer() {
    memset(prevVelocityX, 0, sizeof(float) * fullGridSize);
    memset(prevVelocityY, 0, sizeof(float) * fullGridSize);
    memset(prevDensity, 0, sizeof(float) * fullGridSize);
}

void FluidsSolver::updateEdges(float *values, int flag) {


    // Border cases
    if (!ignoreBorders) {
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
        for (int i = 1; i <= w - 2; i++) {
            for (int j = 1; j <= h - 2; j++) {
                // Grid square value takes the average of all adjacent grid squares (Top, left, right, bottom)
                values[cIdx(i, j)] = (previousValues[cIdx(i, j)] +
                                      diffusionAmount * (values[cIdx(i + 1, j)] +
                                                         values[cIdx(i - 1, j)] + values[cIdx(i, j + 1)] +
                                                         values[cIdx(i, j - 1)]))
                                     / (4.0f * diffusionAmount + 1.0f);
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

    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
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


void FluidsSolver::computeNewVelocities() {
    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            divergence[cIdx(i, j)] = 0.5f * (velocityX[cIdx(i + 1, j)] - velocityX[cIdx(i - 1, j)] +
                                             velocityY[cIdx(i, j + 1)] - velocityY[cIdx(i, j - 1)]);
            divergenceFreeVelocityField[cIdx(i, j)] = 0.0f;;
        }
    }
    updateEdges(divergence, 0);
    updateEdges(divergenceFreeVelocityField, 0);

    // Subtracting the divergence from the velocity field (To get a divergence free velocity matrix at the end)
    // Using the Gaus-Seidel method to solve the system of equations, keeping in mind that the divergenceFreeVelocityField is a scalar
    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= w - 2; i++) {
            for (int j = 1; j <= h - 2; j++) {
                divergenceFreeVelocityField[cIdx(i, j)] =
                        (divergenceFreeVelocityField[cIdx(i + 1, j)] + divergenceFreeVelocityField[cIdx(i - 1, j)] +
                         divergenceFreeVelocityField[cIdx(i, j + 1)] +
                         divergenceFreeVelocityField[cIdx(i, j - 1)] - divergence[cIdx(i, j)]) / 4.0f;
            }
        }
        updateEdges(divergenceFreeVelocityField, 0);
    }

    // Computing the gradient of the divergenceFreeVelocityField to get the velocities
    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            velocityX[cIdx(i, j)] -=
                    0.5f * (divergenceFreeVelocityField[cIdx(i + 1, j)] - divergenceFreeVelocityField[cIdx(i - 1, j)]);
            velocityY[cIdx(i, j)] -=
                    0.5f * (divergenceFreeVelocityField[cIdx(i, j + 1)] - divergenceFreeVelocityField[cIdx(i, j - 1)]);
        }
    }
    updateEdges(velocityX, 1);
    updateEdges(velocityY, 2);
}

void FluidsSolver::updateVelocities() {
    if (viscosityCoefficient > 0.0f) {
        SWAP(prevVelocityX, velocityX);
        SWAP(prevVelocityY, velocityY);
        diffusion(velocityX, prevVelocityX, viscosityCoefficient, 1);
        diffusion(velocityY, prevVelocityY, viscosityCoefficient, 2);
    }

    computeNewVelocities();

    SWAP(prevVelocityX, velocityX);
    SWAP(prevVelocityY, velocityY);
    advection(velocityX, prevVelocityX, prevVelocityX, prevVelocityY, 1);
    advection(velocityY, prevVelocityY, prevVelocityX, prevVelocityY, 2);

    computeNewVelocities();
}

void FluidsSolver::updateDensities() {
    updateNormalizedDensities();

    if (diffusionCoefficient > 0.0f) {
        SWAP(prevDensity, density);
        diffusion(density, prevDensity, diffusionCoefficient, 0);
    }
    SWAP(prevDensity, density);
    advection(density, prevDensity, velocityX, velocityY, 0);
}

void FluidsSolver::updateVortexConfinement() {
    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            vorticity[cIdx(i, j)] = 0.5f *
                                    (velocityY[cIdx(i + 1, j)] - velocityY[cIdx(i - 1, j)] - velocityX[cIdx(i, j + 1)] +
                                     velocityX[cIdx(i, j - 1)]);
            if (vorticity[cIdx(i, j)] >= 0.0f) absVorticity[cIdx(i, j)] = vorticity[cIdx(i, j)];
            else absVorticity[cIdx(i, j)] = -vorticity[cIdx(i, j)];
        }
    }
    updateEdges(vorticity, 0);
    updateEdges(absVorticity, 0);

    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            gradVorticityX[cIdx(i, j)] = 0.5f * (absVorticity[cIdx(i + 1, j)] - absVorticity[cIdx(i - 1, j)]);
            gradVorticityY[cIdx(i, j)] = 0.5f * (absVorticity[cIdx(i, j + 1)] - absVorticity[cIdx(i, j - 1)]);
            lenGradient[cIdx(i, j)] = sqrt(gradVorticityX[cIdx(i, j)] * gradVorticityX[cIdx(i, j)] +
                                           gradVorticityY[cIdx(i, j)] * gradVorticityY[cIdx(i, j)]);
            if (lenGradient[cIdx(i, j)] < 0.01f) {
                voriticityfx[cIdx(i, j)] = 0.0f;
                vorticityfy[cIdx(i, j)] = 0.0f;
            } else {
                voriticityfx[cIdx(i, j)] = gradVorticityX[cIdx(i, j)] / lenGradient[cIdx(i, j)];
                vorticityfy[cIdx(i, j)] = gradVorticityY[cIdx(i, j)] / lenGradient[cIdx(i, j)];
            }
        }
    }
    updateEdges(voriticityfx, 0);
    updateEdges(vorticityfy, 0);

    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            velocityX[cIdx(i, j)] += vorticityCoefficient * (vorticityfy[cIdx(i, j)] * vorticity[cIdx(i, j)]);
            velocityY[cIdx(i, j)] += vorticityCoefficient * (-voriticityfx[cIdx(i, j)] * vorticity[cIdx(i, j)]);
        }
    }

    updateEdges(velocityX, 1);
    updateEdges(velocityY, 2);
}

void FluidsSolver::updateNormalizedDensities() {
    float max = 30;
    float min = 0;
    float sum = 0;
    for (int i = 0; i < fullGridSize; i++) {
        sum += density[i];
        if (density[i] > 30) {
            normalizedDensity[i] = 10.0f;
        } else {
            normalizedDensity[i] = ((density[i] - min) / (max - min)) * 10;
        }
    }
    std::cout<<"Density sum is "<<sum<<std::endl;
}

void FluidsSolver::addDensitySpot(int xPosition, int yPosition) {
    int radius = 25;
    for (int i = 1; i <= 10; i++) {
        fillCircle(xPosition, yPosition, i, radius);
        radius -= 2;
    }
    updateNormalizedDensities();
}

void FluidsSolver::fillCircle(int xPosition, int yPosition, int densityValue, int radius) {
    for (int i = 1; i <= w - 2; i++) {
        for (int j = 1; j <= h - 2; j++) {
            if ((std::pow(i - xPosition,2) + std::pow(j - yPosition,2)) < std::pow(radius,2))
            {
                density[cIdx(i, j)] = densityValue * 3;
            }
        }
    }
}

