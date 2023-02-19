#include "mesh.h"

void translateMesh(mesh& m, mesh& o, vector3D translation) {

	AddVecors3D(m.origin, translation, o.origin);

	for (int i = 0; i < m.tris.size(); i++)
	{
		AddVecors3D(m.tris[i].corners[0], translation, o.tris[i].corners[0]);
		AddVecors3D(m.tris[i].corners[1], translation, o.tris[i].corners[1]);
		AddVecors3D(m.tris[i].corners[2], translation, o.tris[i].corners[2]);
	}
}

void rotateMeshZAxis(mesh& m, mesh& o, vector3D p, float angle) {
	mesh translatedMesh = m;
	translateMesh(m, translatedMesh, vector3D{ -p.x, -p.y, -p.z });

	matrix4x4 matRotZ;

	// Rotation Z
	matRotZ.m[0][0] = cosf(angle);
	matRotZ.m[0][1] = sinf(angle);
	matRotZ.m[1][0] = -sinf(angle);
	matRotZ.m[1][1] = cosf(angle);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	for (int i = 0; i < translatedMesh.tris.size(); i++)
	{
		triangle tri = translatedMesh.tris[i];
		triangle tempTri = translatedMesh.tris[i];

		// Rotate in Z-Axis
		multiplyMatrixVector(tri.corners[0], tempTri.corners[0], matRotZ);
		multiplyMatrixVector(tri.corners[1], tempTri.corners[1], matRotZ);
		multiplyMatrixVector(tri.corners[2], tempTri.corners[2], matRotZ);

		translatedMesh.tris[i] = tempTri;
	}

	translateMesh(translatedMesh, translatedMesh, p);
	o = translatedMesh;
}

void rotateMeshYAxis(mesh& m, mesh& o, vector3D p, float angle) {
	mesh translatedMesh = m;
	translateMesh(m, translatedMesh, vector3D{ -p.x, -p.y, -p.z });

	//// Set up rotation matrices
	matrix4x4 matRotY;

	// Rotation X
	matRotY.m[0][0] = cosf(angle);
	matRotY.m[0][2] = sinf(angle);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = -sinf(angle);
	matRotY.m[2][2] = cosf(angle);
	matRotY.m[3][3] = 1;

	for (int i = 0; i < translatedMesh.tris.size(); i++)
	{
		triangle tri = translatedMesh.tris[i];
		triangle tempTri = translatedMesh.tris[i];

		// Rotate in X-Axis
		multiplyMatrixVector(tri.corners[0], tempTri.corners[0], matRotY);
		multiplyMatrixVector(tri.corners[1], tempTri.corners[1], matRotY);
		multiplyMatrixVector(tri.corners[2], tempTri.corners[2], matRotY);

		translatedMesh.tris[i] = tempTri;
	}

	translateMesh(translatedMesh, translatedMesh, p);
	o = translatedMesh;
}

void rotateMeshXAxis(mesh& m, mesh& o, vector3D p, float angle) {
	mesh translatedMesh = m;
	translateMesh(m, translatedMesh, vector3D{ -p.x, -p.y, -p.z });

	//// Set up rotation matrices
	matrix4x4 matRotX;

	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(angle);
	matRotX.m[1][2] = sinf(angle);
	matRotX.m[2][1] = -sinf(angle);
	matRotX.m[2][2] = cosf(angle);
	matRotX.m[3][3] = 1;

	for (int i = 0; i < translatedMesh.tris.size(); i++)
	{
		triangle tri = translatedMesh.tris[i];
		triangle tempTri = translatedMesh.tris[i];

		// Rotate in X-Axis
		multiplyMatrixVector(tri.corners[0], tempTri.corners[0], matRotX);
		multiplyMatrixVector(tri.corners[1], tempTri.corners[1], matRotX);
		multiplyMatrixVector(tri.corners[2], tempTri.corners[2], matRotX);

		translatedMesh.tris[i] = tempTri;
	}

	translateMesh(translatedMesh, translatedMesh, p);
	o = translatedMesh;
}

void makeRotationMatrixX(matrix4x4& o, float angle) {
	o.m[0][0] = cosf(angle);
	o.m[0][1] = sinf(angle);
	o.m[1][0] = -sinf(angle);
	o.m[1][1] = cosf(angle);
	o.m[2][2] = 1;
	o.m[3][3] = 1;
}

void makeRotationMatrixY(matrix4x4& o, float angle) {
	o.m[0][0] = cosf(angle);
	o.m[0][2] = sinf(angle);
	o.m[1][1] = 1;
	o.m[2][0] = -sinf(angle);
	o.m[2][2] = cosf(angle);
	o.m[3][3] = 1;
}

void makeRotationMatrixZ(matrix4x4& o, float angle) {
	o.m[0][0] = 1;
	o.m[1][1] = cosf(angle);
	o.m[1][2] = sinf(angle);
	o.m[2][1] = -sinf(angle);
	o.m[2][2] = cosf(angle);
	o.m[3][3] = 1;
}

void applyMatrixToMesh(mesh& m, matrix4x4 mat) {
	for (int i = 0; i < m.tris.size(); i++)
	{
		triangle tri = m.tris[i];
		triangle tempTri = m.tris[i];

		// Rotate in X-Axis
		multiplyMatrixVector(tri.corners[0], tempTri.corners[0], mat);
		multiplyMatrixVector(tri.corners[1], tempTri.corners[1], mat);
		multiplyMatrixVector(tri.corners[2], tempTri.corners[2], mat);

		m.tris[i] = tempTri;
	}
}

void createPlaneMesh(mesh& o, float sideLengh, int noOfPollygonsOnSide, vector3D origin) {
	o.origin = origin;

	float halfLen = sideLengh / (float)2;
	float triSideLen = sideLengh / (float)noOfPollygonsOnSide;

	float z = origin.z - halfLen;
	while ( z < origin.z + halfLen) {
		float x = origin.x - halfLen;
		while ( x < origin.x + halfLen ) {
			triangle triOne = {
				x + triSideLen, origin.y, z + triSideLen,
				x, origin.y, z,
				x, origin.y, z + triSideLen
			};

			triangle triTwo = {
				x + triSideLen, origin.y, z + triSideLen,
				x + triSideLen, origin.y, z,
				x, origin.y, z
			};
			
			o.tris.push_back(triOne);
			o.tris.push_back(triTwo);

			x += triSideLen;
		}

		z += triSideLen;
	}
}