#pragma once

#include "vectorMaths.h"

#include<iostream>
#include <vector>

struct triangle
{
	vector3D corners[3];

	uint8_t R = 255;
	uint8_t G = 255;
	uint8_t B = 255;
	uint8_t A = 255;
};

struct mesh
{
	std::vector<triangle> tris;
	vector3D origin = {0,0,0};
};

void translateMesh(mesh& m, mesh& o, vector3D translation);

void rotateMeshZAxis(mesh& m, mesh& o, vector3D p, float angle);

void rotateMeshYAxis(mesh& m, mesh& o, vector3D p, float angle);

void rotateMeshXAxis(mesh& m, mesh& o, vector3D p, float angle);

void makeRotationMatrixZ(matrix4x4& o, float angle);

void makeRotationMatrixX(matrix4x4& o, float angle);

void makeRotationMatrixY(matrix4x4& o, float angle);

void applyMatrixToMesh(mesh& m, matrix4x4 mat);

void createPlaneMesh(mesh &o, float sideLengh, int noOfPollygonsOnSide, vector3D origin);


