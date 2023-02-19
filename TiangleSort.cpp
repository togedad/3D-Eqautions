//#include <cuda_runtime.h>
//#include "cuda_kernel.cuh"
//#include "device_launch_parameters.h"

#include "TriangleSort.h"
#include "mesh.h"

bool onStrightLine(vector3D A, vector3D B, vector3D C) {
	//fi all three points are on the same line then then i do not want to shade 

	vector3D AB;
	vector3D AC;

	SubtractVecors3D(A, B, AB);
	SubtractVecors3D(A, C, AC);

	float scaleFactor;
	if (AB.x == 0 && AC.x == 0) {
		scaleFactor = 0;
	}
	else
	{
		scaleFactor = AB.x / (float)AC.x;
	}

	bool onLine = (scaleFactor == 0 || ((AB.y * scaleFactor == AC.y) && (AC.y * scaleFactor == AB.y)));

	return onLine;
}

//void sort(triangle* list, int len, bool* sorted) {
//	bool swap = false;
//
//	triangle* tri1 = &list[threadIdx.x * 2];
//	triangle* tri2 = &list[threadIdx.x * 2 + 1];
//
//	float triOneDist = tri1->corners[0].x * tri1->corners[0].x + tri1->corners[0].y * tri1->corners[0].y + tri1->corners[0].z * tri1->corners[0].z;
//	float triTwoDist = tri2->corners[0].x * tri2->corners[0].x + tri2->corners[0].y * tri2->corners[0].y + tri2->corners[0].z * tri2->corners[0].z;
//
//	if (triOneDist < triTwoDist) {
//		//swap
//		triangle* temp = &list[threadIdx.x * 2];
//
//		list[threadIdx.x * 2] = list[threadIdx.x * 2 + 1];
//		list[threadIdx.x * 2 + 1] = *temp;
//
//		swap = true;
//	}
//
//	if (threadIdx.x * 2 + 2 == len) { return; }
//
//	tri1 = &list[threadIdx.x * 2 + 1];
//	tri2 = &list[threadIdx.x * 2 + 2];
//
//	triOneDist = tri1->corners[0].x * tri1->corners[0].x + tri1->corners[0].y * tri1->corners[0].y + tri1->corners[0].z * tri1->corners[0].z;
//	triTwoDist = tri2->corners[0].x * tri2->corners[0].x + tri2->corners[0].y * tri2->corners[0].y + tri2->corners[0].z * tri2->corners[0].z;
//	
//	bool triOneOnLine = onStrightLine(tri1->corners[0], tri1->corners[1], tri1->corners[2]);
//	bool triTwoOnLine = onStrightLine(tri2->corners[0], tri2->corners[1], tri2->corners[2]);
//
//	if (triOneDist < triTwoDist) {
//		//swap
//		triangle* temp = &list[threadIdx.x * 2];
//
//		list[threadIdx.x * 2] = list[threadIdx.x * 2 + 1];
//		list[threadIdx.x * 2 + 1] = *temp;
//
//		swap = true;
//	}
//
//	//if (swap) {
//	//	sorted = false;
//	//}
//
//	return;
//}

float sqrCalculateDistnce(triangle i) {
	return i.corners[0].x * i.corners[0].x + i.corners[0].y * i.corners[0].y + i.corners[0].z * i.corners[0].z;
}

void quickSortTriangle (std::vector<triangle> i, std::vector<triangle>& o) {
	if (i.size() == 0) {
		return;
	}

	int pivetDist = sqrCalculateDistnce(i[0]);

	std::vector<triangle> closer;
	std::vector<triangle> further;

	for (int n = 1; n < i.size(); n++) { // i starts at 1 as we want to skip over the firdt value witch is the pivet
		bool onLine = onStrightLine(i[n].corners[0], i[n].corners[1], i[n].corners[2]);

		float dist = sqrCalculateDistnce(i[n]);
		if (dist < pivetDist || onLine) {
			closer.push_back(i[n]);
		}
		else
		{
			further.push_back(i[n]);
		}
	}

	std::vector<triangle> closeSort;
	quickSortTriangle(closer, closeSort);

	std::vector<triangle> furtherSort;
	quickSortTriangle(further, furtherSort);

	o = furtherSort;
	o.push_back(i[0]);
	o.insert(o.end(), closeSort.begin(), closeSort.end());
}

int sortTriangleDistToCameraKernal(std::vector<mesh> i, std::vector<triangle> &o) {
	if (i.size() == 0) { return 0; }

	#pragma region sorting out triangles

	o.clear();
	std::vector<triangle> unorderTriangles;
	for (auto mesh : i) {
		unorderTriangles.insert(unorderTriangles.end(), mesh.tris.begin(), mesh.tris.end());
	}
	int totalTriangles = unorderTriangles.size();
	o = std::vector<triangle>(totalTriangles);

	#pragma endregion

	quickSortTriangle(unorderTriangles, o);

	//this is not run on GPU as i can not figure it out so i hiw to do it so im doing it not on GPU as im lazy

	return 1;
}

//while (pileOneIndex < noTirsToSort / 2 && pileTwoIndex < noTirsToSort / 2)
//{
//	triangle* triOne = &pileOne[pileOneIndex];
//	float distOne = triOne->corners[0].x * triOne->corners[0].x + triOne->corners[0].y * triOne->corners[0].y + triOne->corners[0].z * triOne->corners[0].z;
//
//	triangle* triTwo = &pileTwo[pileTwoIndex];
//	float distTWo = triTwo->corners[0].x * triTwo->corners[0].x + triTwo->corners[0].y * triTwo->corners[0].y + triTwo->corners[0].z * triTwo->corners[0].z;
//
//	if (distOne > distTWo) {
//		finalPile[finalPileIndex] = pileOne[pileOneIndex];
//		pileOneIndex++;
//	}
//	else
//	{
//		finalPile[finalPileIndex] = pileTwo[pileTwoIndex];
//		pileTwoIndex++;
//	}
//
//	finalPileIndex++;
//}
//
//if (pileOneIndex < noTirsToSort / 2) {
//	int i = 1;
//	while (pileOneIndex < noTirsToSort / 2) {
//		finalPile[finalPileIndex + i] = pileOne[pileOneIndex];
//		i++;
//		pileOneIndex++;
//	}
//}
//else {
//	int i = 1;
//	while (pileTwoIndex < noTirsToSort / 2) {
//		finalPile[finalPileIndex + i] = pileOne[pileTwoIndex];
//		i++;
//		pileTwoIndex++;
//	}
//}
//
//for (int i = 0; i < noTirsToSort / 2; i++) {
//	finalPile[i] = pileOne[i];
//	finalPile[i + noTirsToSort / 2] = pileTwo[i];
//}