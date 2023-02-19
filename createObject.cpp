#include "createObject.h"

struct verticy {
	float pos_x;
	float pos_y;
	float pos_z;

	float normal_x;
	float normal_y;
	float normal_z;

	float uv_x;
	float uv_y;
};

int findVector(float vec[3], std::vector<float> vecArray) {
	for (int i = 0; i < vecArray.size(); i += 3) {
		if (vec[0] == vecArray[i] && vec[1] == vecArray[i + 1] && vec[2] == vecArray[i + 2])
			return i;
	}

	//if fails returns -1
	return -1;
}

int findUV(float uv[2], std::vector<float> uvArray) {
	for (int i = 0; i < uvArray.size(); i += 2) {
		if (uv[0] == uvArray[i] && uv[1] == uvArray[i + 1])
			return i;
	}

	//if fails returns -1
	return -1;
}

int createMeshObjectFile(mesh* Mesh) {
	// Create and open a text file
	std::ofstream objFile("exports\\mesh.obj");

	// Write to the file
	objFile << "#equationMesh\n";
	//objFile << "mtllib matt.mtl\n";
	objFile << "o Equation\n"; 

	std::vector<verticy> verticys;

	for (int i = 0; i < Mesh->tris.size(); i++) {

		verticy v1;
		verticy v2;
		verticy v3;

//these (uv's) are not yet calculated

#pragma region uvs

		v1.uv_x = 0;
		v2.uv_x = 0;
		v3.uv_x = 0;

		v1.uv_y = 0;
		v2.uv_y = 0;
		v3.uv_y = 0;

#pragma endregion

#pragma region normal

		triangle triProjected;

		//finding cross product
		vector3D a, b, cross, normalCross;
		SubtractVecors3D(Mesh->tris[i].corners[1], Mesh->tris[i].corners[0], a);
		SubtractVecors3D(Mesh->tris[i].corners[2], Mesh->tris[i].corners[0], b);
		calculateCrossProduct(a, b, cross);

		normaliseVecotr(cross, normalCross);

		v1.normal_x = normalCross.x;
		v1.normal_y = normalCross.y;
		v1.normal_z = normalCross.z;

		v2.normal_x = normalCross.x;
		v2.normal_y = normalCross.y;
		v2.normal_z = normalCross.z;

		v3.normal_x = normalCross.x;
		v3.normal_y = normalCross.y;
		v3.normal_z = normalCross.z;

#pragma endregion

#pragma region verticy positions

		v1.pos_x = Mesh->tris[i].corners[0].x;
		v1.pos_y = Mesh->tris[i].corners[0].y;
		v1.pos_z = Mesh->tris[i].corners[0].z - 12;
			
		v2.pos_x = Mesh->tris[i].corners[1].x;
		v2.pos_y = Mesh->tris[i].corners[1].y;
		v2.pos_z = Mesh->tris[i].corners[1].z - 12;
			 
		v3.pos_x = Mesh->tris[i].corners[2].x;
		v3.pos_y = Mesh->tris[i].corners[2].y;
		v3.pos_z = Mesh->tris[i].corners[2].z - 12;

#pragma endregion

		verticys.push_back(v1);
		verticys.push_back(v2);
		verticys.push_back(v3);

	}

	std::vector<float> positons;
	std::vector<float> normals;
	std::vector<float> uvs;

	for (int i = 0; i < verticys.size(); i++) {
		
#pragma region add positions to vector

		float pos[3] = { verticys[i].pos_x, verticys[i].pos_y, verticys[i].pos_z };

		if (findVector(pos, positons) == -1) {
			positons.push_back(pos[0]);
			positons.push_back(pos[1]);
			positons.push_back(pos[2]);
		}

#pragma endregion

#pragma region add normals to vector
		float norm[3] = { verticys[i].normal_x, verticys[i].normal_y, verticys[i].normal_z };

		if (findVector(norm, normals) == -1) {
			normals.push_back(norm[0]);
			normals.push_back(norm[1]);
			normals.push_back(norm[2]);
		}
#pragma endregion

#pragma region add uv's' to vector
		float uv[2] = { verticys[i].uv_x, verticys[i].uv_y};

		if (findUV(uv, uvs) == -1) {
			uvs.push_back(uv[0]);
			uvs.push_back(uv[1]);
		}
#pragma endregion


	}

	for (int i = 0; i < positons.size(); i += 3)
	{
		objFile << "v " << positons[i] << " " << positons[i + 1] << " " << positons[i + 2] << "\n";
	}

	//for (int i = 0; i < normals.size(); i += 3)
	//{
	//	objFile << "vn " << normals[i] << " " << normals[i + 1] << " " << normals[i + 2] << "\n";
	//}

	//for (int i = 0; i < uvs.size(); i += 2)
	//{
	//	objFile << "vt " << uvs[i] << " " << uvs[i + 1] << "\n";
	//}

	//objFile << "usemtl Red\n";
	objFile << "s off\n";

#pragma region create and add face

	for (int i = 0; i < verticys.size(); i += 3)
	{
		objFile << "f ";

		std::string face = "";
		std::string temp = "";

		//positon
		float pos1[3] = { verticys[i].pos_x, verticys[i].pos_y, verticys[i].pos_z };
		int index1 = findVector(pos1, positons);

		float pos2[3] = { verticys[i+1].pos_x, verticys[i+1].pos_y, verticys[i+1].pos_z };
		int index2 = findVector(pos2, positons);

		float pos3[3] = { verticys[i+2].pos_x, verticys[i+2].pos_y, verticys[i+2].pos_z };
		int index3 = findVector(pos3, positons);

		index1 = index1 / 3;
		index2 = index2 / 3;
		index3 = index3 / 3;

		temp = std::to_string(index1 + 1) + ' ';
		face.insert(face.end(), temp.begin(), temp.end());
		temp = std::to_string(index2 + 1) + ' ';
		face.insert(face.end(), temp.begin(), temp.end());	
		temp = std::to_string(index3 + 1);
		face.insert(face.end(), temp.begin(), temp.end());

		//texture
		
		////not yet implimented

		//temp = "1/1/1 ";
		//face.insert(face.end(), temp.begin(), temp.end());

		////normal
		//float norm[3] = { verticys[i].normal_x, verticys[i].normal_y, verticys[i].normal_z };
		//index = findVector(norm, normals);
		//temp = std::to_string(index + 1) + '/';
		//face.insert(face.end(), temp.begin(), temp.end());
		//temp = std::to_string(index + 2) + '/';
		//face.insert(face.end(), temp.begin(), temp.end());
		//temp = std::to_string(index + 3) + ' ';
		//face.insert(face.end(), temp.begin(), temp.end());

		objFile << face << "\n";
	}

#pragma endregion

	// Close the file
	objFile.close();

	return 1;
}