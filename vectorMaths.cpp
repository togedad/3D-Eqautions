#include "vectorMaths.h"
#include <iostream>

void AddVecors3D(vector3D& v1, vector3D& v2, vector3D& o) {
	o.x = v1.x + v2.x;
	o.y = v1.y + v2.y;
	o.z = v1.z + v2.z;
}

void SubtractVecors3D(vector3D& v1, vector3D& v2, vector3D& o) {
	o.x = v1.x - v2.x;
	o.y = v1.y - v2.y;
	o.z = v1.z - v2.z;
}

void calculateCrossProduct(vector3D& a, vector3D& b, vector3D& o) {
	//inputs into this fucniton should take into note that the vecters are asuumebs to be orginiating from the origin
	o.x = a.y * b.z - a.z * b.y;
	o.y = a.z * b.x - a.x * b.z;
	o.z = a.x * b.y - a.y * b.x;
}

float calculateDotProduct(vector3D& a, vector3D& b) {
	//vectors should originate from the origin when sueing this 

	float o = a.x * b.x + a.y * b.y + a.z * b.z;
	return o;
}

void normaliseVecotr(vector3D& i, vector3D& o) {
	float len = sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);
	o.x = i.x / len;
	o.y = i.y / len;
	o.z = i.z / len;
}

void multiplyMatrixVector(vector3D& i, vector3D& o, matrix4x4& m) {

	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

	//if you want to effect then make sure that the projection array 4th coloum is 0 or 1 (0 prefrably)
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
}

float calculateLenghOfVector(vector3D& i) {
	return sqrt(i.x * i.x + i.y * i.y + i.z * i.z);
}

void makeProjectionMatrix(matrix4x4& projectionMatrix, float aspectRaatio, float fovRadain, float zNear, float zFar) {
	projectionMatrix.m[0][0] = aspectRaatio * fovRadain;
	projectionMatrix.m[1][1] = fovRadain;
	projectionMatrix.m[2][2] = zFar / (zFar - zNear);
	projectionMatrix.m[3][2] = (-zFar * zNear) / (zFar - zNear);
	projectionMatrix.m[2][3] = 1.0f;
	projectionMatrix.m[3][3] = 0.0f;
}

void makeOrthagraphicMatrix(matrix4x4& projectionMatrix, float leftClip, float rightClip, float camBottom, float camTop, float zNear, float Zfar) {
	projectionMatrix.m[0][0] = 2/(float)(rightClip - leftClip);
	projectionMatrix.m[1][1] = 2/(float)(camTop-camBottom);
	projectionMatrix.m[2][2] = -2/(float)(Zfar-zNear);
	projectionMatrix.m[3][3] = 1;

	projectionMatrix.m[3][0] = -((rightClip + leftClip) / (float)(rightClip - leftClip));
	projectionMatrix.m[3][1] = -((camTop + camBottom) / (float)(camTop - camBottom));
	projectionMatrix.m[3][2] = -((Zfar + zNear) / (float)(Zfar - zNear));
}

//this fucntion is only to make themultiply matrix fucntion easer to understand and work with
float findDotProductOfRowColom(matrix4x4 m1, matrix4x4 m2, int row, int colum) {
	float v1[4] = { m1.m[row][0], m1.m[row][1], m1.m[row][2], m1.m[row][3] };
	float v2[4] = { m2.m[0][colum], m2.m[1][colum] , m2.m[2][colum] , m2.m[3][colum] };

	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
}

float findDotProductOfRowColomVec3(matrix4x4 m1, matrix4x4 m2, int row, int colum) {
	float v1[3] = { m1.m[row][0],   m1.m[row][1],   m1.m[row][2]};
	float v2[3] = { m2.m[0][colum], m2.m[1][colum], m2.m[2][colum]};

	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


void multiplyMatrix(matrix4x4& out, matrix4x4 m1, matrix4x4 m2) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			out.m[y][x] = findDotProductOfRowColom(m1, m2, x, y);
		}
	}
}

void multiplyMatrixAs3x3(matrix4x4& out, matrix4x4 m1, matrix4x4 m2) {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			out.m[y][x] = findDotProductOfRowColomVec3(m1, m2, x, y);
		}
	}
}


void fillMatrix(matrix4x4& mat, float fillVal) {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			mat.m[x][y] = fillVal;
}