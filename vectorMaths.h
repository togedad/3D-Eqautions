#pragma once

struct vector3D
{
	float x, y, z;
};

struct matrix4x4 {
	float m[4][4] = { 0 };
};

float calculateLenghOfVector(vector3D& i);

void AddVecors3D(vector3D& v1, vector3D& v2, vector3D& o);

void SubtractVecors3D(vector3D& v1, vector3D& v2, vector3D& o);

void calculateCrossProduct(vector3D& a, vector3D& b, vector3D& o);

float calculateDotProduct(vector3D& a, vector3D& b);

void normaliseVecotr(vector3D& i, vector3D& o);

void multiplyMatrixVector(vector3D& i, vector3D& o, matrix4x4& m);

void makeProjectionMatrix(matrix4x4& projectionMatrix, float aspectRaatio, float fovRadain, float zNear, float zFar);

void makeOrthagraphicMatrix(matrix4x4& projectionMatrix, float leftClip, float rightClip, float camBottom, float camTop, float zNear, float Zfar);

void multiplyMatrix(matrix4x4& out, matrix4x4 m1, matrix4x4 m2);

void multiplyMatrixAs3x3(matrix4x4& out, matrix4x4 m1, matrix4x4 m2);

void fillMatrix(matrix4x4& mat, float fillVal);