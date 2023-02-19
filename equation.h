#pragma once

#include "vectorMaths.h"

typedef struct Equation {
    char* equation;
    int equationLen = 0;

    float* equationVaribles = new float[4];
    int equationVariblesLen = 4;
};

void validateEquation(Equation& equation);

float returnEquiationValue(std::string stringEquation, vector3D vector);