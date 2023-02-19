//this goes though all equations and updates the meshes and craeats and delets unessery ones
void updateMeshes() {

	meshes.clear();
	for (int h = 0; h < equationVector.size(); h++) {

		mesh plane;
		createPlaneMesh(plane, 5, meshRes, vector3D(0.0f, 0.0f, 0.0f));

		std::string equ = equationVector[h]->getData();

		for (int i = 0; i < plane.tris.size(); i++) {
			for (int n = 0; n < 3; n++) {
				float height = returnEquiationValue(equ, plane.tris[i].corners[n]);//((float)sinf(plane1.tris[i].corners[n].x));// *plane1.tris[i].corners[n].x))* cosf(plane1.tris[i].corners[n].z) / (float)2.0f;// *(float)sinf(plane1.tris[i].corners[n].z)* plane1.tris[i].corners[n].z / (float)4;
				plane.tris[i].corners[n].y += height;

				int temp = (255 * ((h + 1) / (float)equationVector.size()));

				plane.tris[i].B = temp;

				plane.tris[i].G = 255;// (255 * ((n + 1) / (float)equationVector.size()));;

				plane.tris[i].R = temp;// (255 * ((n + 1) / (float)equationVector.size()));
			}
		}

		translateMesh(plane, plane, { 0.0f, 0.0f, 12.0f });

		meshes.push_back(plane);
	}
}

void addEquation() {
	std::string str = "EQUATION:";
	equationVector.push_back(new inputBox(1.0f, 0.05f, 0.0f, 0.05f * equationVector.size(), str, &font, equationsBox));
}

void removeSelectedEquation() {
	if (selectedInput == nullptr) { return; }

	for (int i = 0; i < equationVector.size(); i++) {
		if (equationVector[i] == selectedInput) {
			equationVector.erase(equationVector.begin() + i);
			if (meshes.size() != 0) {

				meshes.erase(meshes.begin() + i);
			}
		}
	}

	for (int i = 0; i < equationVector.size(); i++) {
		equationVector[i]->setPosition(equationVector[i]->getXPos(), 0.05f * i);

	}


}

void lightenBackGroundColour() {
	BGColor.r += 10;
	BGColor.g += 10;
	BGColor.b += 10;
}

void drakenBackGroundColour() {
	BGColor.r -= 10;
	BGColor.g -= 10;
	BGColor.b -= 10;
}

void increseMeshRes() {
	meshRes += 1;
}

void decreasMeshRes() {
	meshRes -= 1;
}