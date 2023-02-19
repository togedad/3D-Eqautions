#include "SDL.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>

#include "vectorMaths.h"
#include "mesh.h"

#include "UIitems.h"
#include "MenuUI.h"

#include "TextRendering.h"
#include "equation.h"
#include "arrFunctions.h"

#include "TriangleSort.h"

#include "createObject.h"

#undef main
#define SDL_MAIN_HANDLE

#pragma region structs

struct vectorInputBox
{
	textBox* text;
	inputBox* x;
	inputBox* y;
	inputBox* z;
};

struct colourScheme {
	SDL_Color green = { 163, 210, 73, 255 };
	SDL_Color red = { 255, 51, 51, 255 };
	SDL_Color orange = { 255, 199, 33, 255 };
	SDL_Color lightBlue = { 166, 239, 255, 255 };
	SDL_Color pink = { 255, 202, 242, 255 };
	SDL_Color pastelPurple = { 170, 182, 255, 255 };

	SDL_Color lightOrange = { 247,227,187, 255 };
	SDL_Color darkOrange = { 199,80,56, 255 };

	SDL_Color selectColour = {255,255,22,255};

}colorPallet;

#pragma endregion


#pragma region setup

SDL_Color BGColor = { 100,100,100,255 };

bool isRunning;
bool fullscreen = false;

//functions definied up here for 
void handleEvents();//cahcks fro user inputs and SDL_Events
void update();//called every frame to update objects
void render();//called every frame to draw screen 
void start();//at teh beginging once before  the prgram starts

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* screen = NULL;

//const int width = 1920;
//const int height = 10800;

const int width = 1800;
const int height = 940;

//const int width = 800;
//const int height = 400;

float deltaTime;
int deltaA;
int deltaB;

//these are all varibless used to cotole how objects are trasformed into screan space
float aspectRaatio = height / (float)width;
float feildOfVeiw = 30.f;
float zNear = 0.01f; // how far from camrea does object need to be to be rendered
float zFar = 1000.0f; // this is the render distance
float fovRadain = 1.0f / tanf(feildOfVeiw * 0.5f / 180.0f * 3.14159f);
float orthagrpahicCamWidth = 8.5f;
bool camModePerspective = true;

#pragma region stats

//global varibles used to track certin key presses and staticsitics
bool rightClickDown = false;
int runTime = 0;
int yMousePotistion, xMousePotistion;
float zoom = 0.0f;
float xRotation = 0.0f;
float yRotation = 0.0f;

int meshOpacity = 255;
int meshRes = 35;
int meshSideLengh = 7;
bool fillTriangleOnDraw = false;
bool posClourTriangle = false;

SDL_Color lastColour;
vector3D camrea = { 0, 0, 1 };

#pragma endregion

#pragma region global varibles 

matrix4x4 projectionMatrix; 

std::vector<mesh> meshes;

inputBox* selectedInput = nullptr;
typeFace font;
renderPackage RenderPackage;

#pragma region makeing menue

std::vector<std::vector<menue*>> drawList;

menueBox equationsBox(0.0f, 0.0f, 250, height/3, { 0,255,0,255 }, width, height);
menueBox VectorsBox(0.0f, 0.6666666f, 250, height/3, {255,0,0,255}, width, height);
menueBox variblesBox(0.0f, 0.3333333f, 250, height/3, { 0,0,255,255 }, width, height);

menueBox buttonsBox(1.0f - (float)(250 / (float)width), 0.0f, 250, height, { 255,255,255,255 }, width, height);

textButton resetEquations(0.95f, 0.2f, 0.025f, 0.01f, " UPDATE OBJECTS ", &font, buttonsBox);

textButton addEquations(0.95f, 0.045f, 0.025f, 0.2225f, "ADD EQUATION", &font, buttonsBox);
textButton addVarible  (0.95f, 0.045f, 0.025f, 0.2725f, "ADD VARIABLE", &font, buttonsBox);
textButton addVector   (0.95f, 0.045f, 0.025f, 0.3225f," ADD VECTOR ", &font, buttonsBox);

textButton removeItem(0.95f, 0.0325f, 0.025f, 0.38f, " REMOVE SELECTED ITEM ", &font, buttonsBox);

textButton drawTriangles(0.95f, 0.05f, 0.025f, 0.4775f,        " TOGGLE DRAW TRIANGLES", &font, buttonsBox);
textButton postionColoursToggle(0.95f, 0.05f, 0.025f, 0.4225f, " TOGGLE POSITION COLOR", &font, buttonsBox);

textButton lightenEquation(0.45f, 0.05f, 0.05f, 0.625f, " LIGHTEN BACKDROP", &font, buttonsBox);
textButton darkenEquation(0.45f, 0.05f, 0.525f, 0.625f,  " DARKEN  BACKDROP", &font, buttonsBox);

textButton orthagraphicPerspectiveButton(0.45f, 0.05f, 0.05f, 0.55f, " ORTHAORAPHIC ", &font, buttonsBox);
textButton perspectiveButton(0.45f, 0.05f, 0.525f, 0.55f, " PERSPECTIVE ", &font, buttonsBox);

textBox cahngeMeshSizeText(1.0f, 0.05f, 0.0f, 0.68f, "CHANGE MESH SIZE", &font, buttonsBox);

textButton increaseMeshSize(0.25f, 0.05f, 0.15f, 0.73f, " + ", &font, buttonsBox); 
textButton decreaseMeshSize(0.25f, 0.05f, 0.6f, 0.73f, " - ", &font, buttonsBox);

textBox changemeshOpactyText(1.0f, 0.05f, 0.0f, 0.78f, "CHANGE MESH OPACITY", &font, buttonsBox);

textButton plusOpacity(0.25f, 0.05f, 0.15f, 0.83f, " + ", &font, buttonsBox);
textButton lowOpacity(0.25f, 0.05f, 0.6f, 0.83f, " - ", &font, buttonsBox);

textBox incresResText(1.0f, 0.05f, 0.0f, 0.88f, "CHANGE MESH RESALUTION", &font, buttonsBox);

textButton increaseResButton(0.25f, 0.05f, 0.15f, 0.93f, " + ", &font, buttonsBox);
textButton decreaseResButton(0.25f, 0.05f, 0.6f, 0.93f, " - ", &font, buttonsBox);

std::vector<inputBox*> equationVector;
std::vector<inputBox*> varibleVector;
std::vector<vectorInputBox*> vector3DVector;
std::vector<textButton*> textButtons = { &resetEquations, &addEquations, &addVarible, &addVector, &removeItem, &lightenEquation, &darkenEquation, &increaseResButton, &decreaseResButton, &drawTriangles, &postionColoursToggle, &plusOpacity, &lowOpacity, &orthagraphicPerspectiveButton, &perspectiveButton, &increaseMeshSize, &decreaseMeshSize};

menueBox matrixBox(0.67f, 0.7f, height / 3, height / 3, { 0,0,255,255 }, width, height);
inputBox b1(0.33, 0.33, 0.0 , 0.0, "", &font, matrixBox);
inputBox b2(0.33, 0.33, 0.33, 0.0, "", &font, matrixBox);
inputBox b3(0.33, 0.33, 0.66, 0.0, "", &font, matrixBox);

inputBox b4(0.33, 0.33, 0.0 , 0.33, "", &font, matrixBox);
inputBox b5(0.33, 0.33, 0.33, 0.33, "", &font, matrixBox);
inputBox b6(0.33, 0.33, 0.66, 0.33, "", &font, matrixBox);

inputBox b7(0.33, 0.33, 0.0,  0.66, "", &font, matrixBox);
inputBox b8(0.33, 0.33, 0.33, 0.66, "", &font, matrixBox);
inputBox b9(0.33, 0.33, 0.66, 0.66, "", &font, matrixBox);
std::vector<inputBox*> matrixVector = { &b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9};

matrix4x4 scew;

#pragma endregion

#pragma endregion

void setUp() {
	//SDL stuff

	int flags = 0;
	flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen) {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) { //initilising SDL
		std::cout << "Subsystems Initialized!\n";

		window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);//setingin what si teh window
		screen = SDL_GetWindowSurface(window);//seting the screan pointer 

		if (window) {
			std::cout << "Window Created!\n";
			SDL_SetWindowMinimumSize(window, 250, 175);
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			isRunning = true;
		}

	}
}

#pragma endregion

#pragma region functions

//this only draws the outline of a triangle
void drawTriangle(SDL_Renderer *renderer, triangle &triangle, SDL_Color color = {255, 255, 255, 255}) {

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLine(renderer, (int)triangle.corners[0].x, height - (int)triangle.corners[0].y, (int)triangle.corners[1].x, height - (int)triangle.corners[1].y);
	SDL_RenderDrawLine(renderer, (int)triangle.corners[1].x, height - (int)triangle.corners[1].y, (int)triangle.corners[2].x, height - (int)triangle.corners[2].y);
	SDL_RenderDrawLine(renderer, (int)triangle.corners[2].x, height - (int)triangle.corners[2].y, (int)triangle.corners[0].x, height - (int)triangle.corners[0].y);
}

//triangls have to be prded though hight  and on x axis too look at fill triangle use of this function
void fillFlatTopTriangle(SDL_Renderer* renderer, vector3D* v0, vector3D* v1, vector3D* v2) {
	//calculate gradiants
	float m0 = (v2->x - v0->x) / (float)(v2->y - v0->y); //this is run over rise to stop an infinate slop i.e a matshs error as you may divide by 0
	float m1 = (v2->x - v1->x) / (float)(v2->y - v1->y);

	//calulate start and end of scan lines
	const int yStart = (int)ceil(v0->y - 0.5f); //to center on grid
	const int yEnd = (int)ceil(v2->y - 0.5f); //the scan line  after the last line drawn

	for (int y = yStart; y < yEnd; y++) {
		//caculater start and end points (x-cords)
		//add 0.5f to y as we are calulating on per pixel basis
		const float x0 = m0 * (float(y) + 0.5f - v0->y) + v0->x;
		const float x1 = m1 * (float(y) + 0.5f - v1->y) + v1->x;

		//calculate start and end of pixels 
		const int xStart = (int)ceil(x0 + 0.5f);
		const int xEnd = (int)ceil(x1 + 0.5f);


		SDL_RenderDrawLine(renderer, xStart, height - y, xEnd, height - y);
	}
}

//triangls have to be prded though hight  and on x axis too look at fill triangle use of this function
void fillFlatBottomTriangle(SDL_Renderer* renderer, vector3D* v0, vector3D* v1, vector3D* v2) {
	//calculate gradiants
	float m0 = (v1->x - v0->x) / (float)(v1->y - v0->y); //this is run over rise to stop an infinate slop i.e a matshs error as you may divide by 0
	float m1 = (v2->x - v0->x) / (float)(v2->y - v0->y);

	//calulate start and end of scan lines
	const int yStart = (int)ceil(v0->y - 0.5f); //to center on grid
	const int yEnd = (int)ceil(v2->y - 0.5f); //the scan line  after the last line drawn

	for (int y = yStart; y < yEnd; y++) {
		//caculater start and end points (x-cords)
		//add 0.5f to y as we are calulating on per pixel basis
		const float x0 = m0 * (float(y) + 0.5f - v0->y) + v0->x;
		const float x1 = m1 * (float(y) + 0.5f - v0->y) + v0->x;

		//calculate start and end of pixels 
		const int xStart = (int)ceil(x0 + 0.5f);
		const int xEnd = (int)ceil(x1 + 0.5f);


		SDL_RenderDrawLine(renderer, xStart, height - y, xEnd, height - y);
	}
}

void fillTriangle(SDL_Renderer* renderer, triangle& Triangle, SDL_Color color = { 255, 255, 255, 255 }) {

	//set colour
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//make pointers
	vector3D* v0 = &Triangle.corners[0];
	vector3D* v1 = &Triangle.corners[1];
	vector3D* v2 = &Triangle.corners[2];

	//sort by height
	if (v1->y < v0->y) { std::swap(v0, v1); }
	if (v2->y < v1->y) { std::swap(v1, v2); }
	if (v1->y < v0->y) { std::swap(v0, v1); }



	if (v0->y == v1->y) { //natrally has a flat top
		//sort vericys by x
		if (v1->x < v0->x) std::swap(v0, v1); 
		fillFlatTopTriangle(renderer, v0,v1,v2);
		return;
	}

	if (v1->y == v2->y) { //naturaly has a flat bottom
		//sort vericys by x
		if (v2->x < v1->x) std::swap(v1, v2); 
		fillFlatBottomTriangle(renderer, v0,v1,v2);
		return;
	}

	//general tringal

	//find splitting vertex
	float alphaRatio = (v1->y - v0->y) / (float)(v2->y - v0->y);
	vector3D splitingPoint = {v0->x + (v2->x - v0->x) * alphaRatio, v0->y + (v2->y - v0->y) * alphaRatio}; //the point that is used to split the trianlge into a flat bottom and a flat top

	if (v1->x < splitingPoint.x) { //major right
		fillFlatBottomTriangle(renderer, v0, v1, &splitingPoint);
		fillFlatTopTriangle(renderer, v1, &splitingPoint, v2);
	}
	else { //major left
		fillFlatBottomTriangle(renderer, v0, &splitingPoint, v1);
		fillFlatTopTriangle(renderer, &splitingPoint, v1, v2);
	}

}

//draws all the trangls in a given vector 
void drawTriangleVector(matrix4x4 projectionMatrix, SDL_Renderer* renderer, std::vector<triangle> triangles, int zDitsFromCam, bool fill = true, bool drawBack = false, bool drawVecToRGB = false, int opacityLevel = 255) {
	
	if (opacityLevel > 255)
		opacityLevel = 255;

	triangle tri;
	for (int i = 0; i < triangles.size(); i++) {
		tri = triangles[i];

		if (!(tri.corners->z <= 0)) { //this solution will need to be changed for a more complex camrea but basicly this ckes is a triangle is behind the camrea

			triangle triProjected;

			//finding cross product
			vector3D a, b, cross, normalCross;
			SubtractVecors3D(tri.corners[1], tri.corners[0], a);
			SubtractVecors3D(tri.corners[2], tri.corners[0], b);
			calculateCrossProduct(a, b, cross);

			normaliseVecotr(cross, normalCross);

			vector3D tempVec;
			SubtractVecors3D(tri.corners[0], camrea, tempVec);

			float dotPooduct = calculateDotProduct(normalCross, tempVec);
			SDL_Color col = { tri.R, tri.G, tri.B, 255 };;

			//fi all three points are on the same line then then i do not want to shade them

			vector3D AB;
			vector3D AC;

			SubtractVecors3D(tri.corners[0], tri.corners[1], AB);
			SubtractVecors3D(tri.corners[0], tri.corners[2], AC);

			float scaleFactor;
			if (AB.x == 0 && AC.x == 0) {
				scaleFactor = 0;
			}
			else
			{
				scaleFactor = AB.x / (float)AC.x;
			}

			bool onLine = (scaleFactor == 0 || ( (AB.y * scaleFactor == AC.y) && (AC.y * scaleFactor == AB.y)));

			if (dotPooduct <= 0.0f || drawBack) {

				//aply prjection matrix
				multiplyMatrixVector(tri.corners[0], triProjected.corners[0], projectionMatrix);
				multiplyMatrixVector(tri.corners[1], triProjected.corners[1], projectionMatrix);
				multiplyMatrixVector(tri.corners[2], triProjected.corners[2], projectionMatrix);

				//checking too see if it is off screan as all values taht are less or greater than one have gone off screan
				if (abs(triProjected.corners[0].x) > 1.0f || abs(triProjected.corners[0].y) > 1.0) {//triProjected.corners[0].y += 1.0f) {
					continue;
				}
				if (abs(triProjected.corners[1].x) > 1.0f || abs(triProjected.corners[1].y) > 1.0) {//triProjected.corners[0].y += 1.0f) {
					continue;
				}
				if (abs(triProjected.corners[2].x) > 1.0f || abs(triProjected.corners[2].y) > 1.0) {//triProjected.corners[0].y += 1.0f) {
					continue;
				}

				// Scale into view
				triProjected.corners[0].x += 1.0f; triProjected.corners[0].y += 1.0f;
				triProjected.corners[1].x += 1.0f; triProjected.corners[1].y += 1.0f;
				triProjected.corners[2].x += 1.0f; triProjected.corners[2].y += 1.0f;

				triProjected.corners[0].x *= 0.5f * width;
				triProjected.corners[0].y *= 0.5f * height;
				triProjected.corners[1].x *= 0.5f * width;
				triProjected.corners[1].y *= 0.5f * height;
				triProjected.corners[2].x *= 0.5f * width;
				triProjected.corners[2].y *= 0.5f * height;

				if (!onLine) {

					if (drawVecToRGB) {
						float z = abs(tri.corners[0].z - zDitsFromCam);
						float x = abs(tri.corners[0].x);
						float y = abs(tri.corners[0].y);

						int r = (1.f - ((x / (float)(meshSideLengh * 0.8f)) * 0.6f + 0.4f)) * 255;
						int g = (1.f - ((y / (float)(meshSideLengh * 0.8f)) * 0.6f + 0.4f)) * 255;
						g *= 0.2f;
						int b = (1.f - ((z / (float)(meshSideLengh * 0.8f)) * 0.6f + 0.4f)) * 255;

						//r = 35 + r % 125;
						//g = 35 + g % 125;
						//b = 35 + b % 125;

						//r = 15 + r % 240;
						//g = 15 + g % 240;
						//b = 15 + b % 240;

						SDL_Color temp = { r, g, b, 255 };
						col = temp;

						//SDL_Color temp = { abs(normalCross.x * 255), abs(normalCross.y * 255), abs(normalCross.z * 255), 255 };
					}

				}
			}



			if (fill && !onLine)
			{

				if (!drawVecToRGB) {
					vector3D v = { 0.f,0.f, -1.f };
					float dotProd = calculateDotProduct(normalCross, v);

					float dot = abs(dotProd) * 0.8f + 0.2f;//((dotProd + 1) / (float)2

					col = { (unsigned char)(col.r * dot), (unsigned char)(col.g * dot), (unsigned char)(col.b * dot), 255 };

				}

				col.a = opacityLevel;


				fillTriangle(renderer, triProjected, col);
			}
			else
			{
				col.a = tri.A;
				drawTriangle(renderer, triProjected, col);
			}
		}
	}

	return;
}

//void create a vector equaion
mesh makeVectorMeshFromOrigin(float x1, float y1, float z1, float x2 = 0.0f, float y2 = 0.0f, float z2 = 0.0f) {
	mesh returnMesh;
	triangle temp;
	temp.corners[0] = { x2, y2, z2 };
	temp.corners[1] = { x2, y2, z2 };
	temp.corners[2] = { x2 + x1, y2 + y1, z2 + z1 };

	returnMesh.tris.push_back(temp);

	return returnMesh;
}

char twoValueOperations[] = { '+', '-', '*', '/' };
char oneValueOperations[] = { 's', 'c', 't' };
char miscSymbols[] = { '.' };

bool isOperation(char letter) {
	for (int m = 0; m < 3; m++)
		if (letter == oneValueOperations[m])
			return true;

	for (int n = 0; n < 4; n++)
		if (letter == twoValueOperations[n])
			return true;

	return false;
}

//if needed this can be improved but it works for now just a bit inivificent
void validateEquation(std::string& output, std::string input) {

	output = input;

	//reaplceing words
	output.erase(std::remove_if(output.begin(), output.end(), ::isspace), output.end());

	while (output.find("SIN") != std::string::npos)
		output.replace(output.find("SIN"), 3, "s");

	while (output.find("COS") != std::string::npos)
		output.replace(output.find("COS"), 3, "c");

	while (output.find("TAN") != std::string::npos)
		output.replace(output.find("TAN"), 3, "t");

	//reaplaceing all varibles (leteters)		

	bool valueParseFail = false;
	for (int i = 0; i < varibleVector.size(); i++) {

		try {
			//converting val to float 
			float val = std::stof(varibleVector[i]->getData());
			std::string letter = " ";

			//65 is teh letter 'A' and +i lets me cycle thorugh alphabet 
			letter[0] = varibleVector[i]->getDisplayText()[4];

			//this loops thought anf replaces all letters with the value
			while (output.find(letter) != std::string::npos)
				output.replace(output.find(letter), 1, std::to_string(val));
		}
		catch (...) {
			std::cout << "could not pars value to string: " + varibleVector[i]->getData() << std::endl;
			valueParseFail = true;
		}

		//char* check;
		//double value = strtod(varibleVector[i]->getData().c_str(), &check);

		//if (*check != 0 ) {
		//	// not a number - in this case will point at 'x'
		//	std::cout << "could not pars value to string: " + varibleVector[i]->getData() << std::endl;
		//	valueParseFail = true;
		//}
		//else
		//{
			//std::string letter = " ";
			//letter[0] = 65 + i;
			//while (output.find(letter) != std::string::npos)
			//	output.replace(output.find(letter), 1, varibleVector[i]->getData());
		//}
	}

	if (valueParseFail) {
		std::cout << "could not parse value so meshes were not redrawn fix error and update meshes then" << std::endl;
	}

	//looking for uncolsed brakets
	int depth = 0;
	for (int i = 0; i < output.size(); i++) {
		if (output[i] == '(')
			depth++;
		else if (output[i] == ')')
			depth--;
	}

	if (depth != 0) {
		output = "";
		return;
	}

	//checing to see if all charicters are vlid
	for (int i = 0; i < output.size(); i++) {
		//cheking char
		bool charOk = false;	
		if (output[i] >= 48 && output[i] <= 57)//cheking to see if it is number
			charOk = true;
		if (output[i] >= 88 && output[i] <= 90) {//cheking to see if it is XYZ all next to eachother in alphabet
			bool bothSideAreOk = true;
			if (i != 0)
				if (!isOperation(output[i - 1]))
					bothSideAreOk = false;
				else if(output[i - 1] == '(')
					bothSideAreOk = false;

			if (i - 1 != output.size() || i - 1 < 0)
				if (!isOperation(output[i + 1]))
					bothSideAreOk = false;
			if (output[i + 1] == ')')
				bothSideAreOk = false;
			charOk = true;
		}
		if (output[i] == ')')
			charOk = true;
		if (output[i] == '(')
			charOk = true;

		charOk = isOperation(output[i]) || charOk;

		//for (int m = 0; m < 3; m++) 
		//	if (output[i] == oneValueOperations[m])
		//		charOk = true;
		//
		//for (int n = 0; n < 4; n++)
		//	if (output[i] == twoValueOperations[n])
		//		charOk = true;

		for (int n = 0; n < 1; n++)
			if (output[i] == miscSymbols[n])
				charOk = true;

		if (!charOk) {
			output = "";
			return;
		}
	}

	//checing to see if tehre are any problem with contenets of opreations
	for (int i = 0; i < output.size(); i++) {
		//loop over all two value operations to check if there are prblems with wahts on eather side fo them
		for (int n = 0; n < 4; n++) {
			if (output[i] == twoValueOperations[n]) {

				//a two operation can not be last or first value in the equation
				if (i == 0 || i == output.size() - 1) {
					if (output[i] == '-')
						continue;
					output = "";
					return;
				}

				//cheking left of operation
				bool leftOk = false;
				if (output[i - 1] >= 48 && output[i - 1] <= 57 )//cehing to see if it is number
					leftOk = true;
				if (output[i - 1] >= 88 && output[i - 1] <= 90)//cheking to see if it is XYZ all next to eachother in alphabet
					leftOk = true;
				if (output[i - 1] == ')')
					leftOk = true;
				if (output[i] == '-' && isOperation(output[i - 1]))
					leftOk = true;	
				
				//cheing right of operation
				bool rightOk = false;
				if (output[i + 1] == '-')
					rightOk = true;
				if (output[i + 1] >= 48 && output[i + 1] <= 57)//cehing to see if it is number
					rightOk = true;
				if (output[i + 1] >= 88 && output[i + 1] <= 90)//cheking to see if it is XYZ all next to eachother in alphabet
					rightOk = true;
				if (output[i + 1] == '(')
					rightOk = true;

				//looping over one value input operations
				for (int m = 0; m < 3; m++) {
					if (output[i+1] == oneValueOperations[m])
						rightOk = true;
				}

				if (!rightOk || !leftOk) {
					output = "";
					return;
				}
			}
		}
	}

}

#pragma region buttonFunctions

bool validateNumberString(std::string stringNumber) {
	bool isParsable = true;

	try {
		//converting val to float see if it is possable
		std::stof(stringNumber);
	}
	catch (...) {
		std::cout << "could not pars value to string: " + stringNumber << std::endl;
		isParsable = false;
	}

	return isParsable;
}

//this goes though all equations and updates the meshes and craeats and delets unessery ones
void updateMeshes() {

	scew.m[0][0] = validateNumberString(b1.getData()) ? std::stod(b1.getData()) : 1;
	scew.m[0][1] = validateNumberString(b2.getData()) ? std::stod(b2.getData()) : 0;
	scew.m[0][2] = validateNumberString(b3.getData()) ? std::stod(b3.getData()) : 0;
				   
	scew.m[1][0] = validateNumberString(b4.getData()) ? std::stod(b4.getData()) : 0;
	scew.m[1][1] = validateNumberString(b5.getData()) ? std::stod(b5.getData()) : 1;
	scew.m[1][2] = validateNumberString(b6.getData()) ? std::stod(b6.getData()) : 0;
					 			
	scew.m[2][0] = validateNumberString(b7.getData()) ? std::stod(b7.getData()) : 0;
	scew.m[2][1] = validateNumberString(b8.getData()) ? std::stod(b8.getData()) : 0;
	scew.m[2][2] = validateNumberString(b9.getData()) ? std::stod(b9.getData()) : 1;

	meshes.clear();
	//updateing equations
	for (int h = 0; h < equationVector.size(); h++) {

		std::string equ = equationVector[h]->getData();

		validateEquation(equ, equ);		

		if (equ == "") {
			std::cout << "eqquation Fail" + h << std::endl;
			continue;
		}

		mesh plane;
		createPlaneMesh(plane, meshSideLengh, meshRes, vector3D(0.0f, 0.0f, 0.0f));

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

	for (int h = 0; h < vector3DVector.size(); h++) {
		mesh vector;
		
		//if all valuse in vector are nto parasble then it moves onto tthe next varible
		if (!(validateNumberString(vector3DVector[h]->x->getData()) && 
			  validateNumberString(vector3DVector[h]->y->getData()) && 
			  validateNumberString(vector3DVector[h]->z->getData()))) {

			continue;
		}

		float x = std::stof(vector3DVector[h]->x->getData());
		float y = std::stof(vector3DVector[h]->y->getData());
		float z = std::stof(vector3DVector[h]->z->getData());

		vector = makeVectorMeshFromOrigin(x,y,z);

		vector.tris[0].R = 255;
		vector.tris[0].G = 0;
		vector.tris[0].B = 0;

		translateMesh(vector, vector, { 0.0f, 0.0f, 12.0f });

		meshes.push_back(vector);
	}

	//makeing grid vectors 
	mesh tempVector;
	int vecOpacity = 60;

	tempVector = makeVectorMeshFromOrigin(0, 3, 0);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	tempVector = makeVectorMeshFromOrigin(0, -3, 0);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	tempVector = makeVectorMeshFromOrigin(3, 0, 0);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	tempVector = makeVectorMeshFromOrigin(-3, 0, 0);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	tempVector = makeVectorMeshFromOrigin(0, 0, 3);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	tempVector = makeVectorMeshFromOrigin(0, 0, -3);
	tempVector.tris[0].A = vecOpacity;
	translateMesh(tempVector, tempVector, { 0.0f, 0.0f, 12.0f });
	meshes.push_back(tempVector);

	createMeshObjectFile(&meshes[0]);
}

void addEquation() {
	// 10 is the max size as larger than 10 would mean that the boxes postion would be 1.1
	if (equationVector.size() < 10) {

		std::string text = "EQUATION:";
		equationVector.push_back(new inputBox(1.0f, 0.1f, 0.0f, 0.1f * equationVector.size(), text, &font, equationsBox));
	}
}

void resetBoxPositions(std::vector<inputBox*> vec) {
	for (int i = 0; i < vec.size(); i++) {
		vec[i]->setPosition(0.f, 0.1f * i);
	}
}

void addVaribleFunc() {	
	if (varibleVector.size() < 10) {

		char letter = (65 + varibleVector.size());
		int intsertLocation = varibleVector.size();

		//finding the location where to insert into the 
		//this chekcs to see if there is a varible in alphabetical order if not there is a 
		//letter in teh gap free so the insert location is set there and the letter is cahnged to the correct one
		for (int i = 0; i < varibleVector.size(); i++) {
			if (varibleVector[i]->getDisplayText()[4] != 65 + i) {
				letter = 65 + i;
				intsertLocation = i;
				break;
			}
		}

		std::string text = "VAR ";
		text.push_back(letter);
		text.push_back(':');
		text.push_back(' ');

		varibleVector.insert(varibleVector.begin() + intsertLocation, new inputBox(1.0f, 0.1f, 0.0f, 0.1f * varibleVector.size(), text, &font, variblesBox)); //new inputBox(1.0f, 0.1f, 0.0f, 0.1f * varibleVector.size(), text, &font, variblesBox));

		//order needs reseting espashaly if  box has been inserted in the middle it is easer to ccall every 
		//frma as it is a simple quick function and wont be called oftern
		resetBoxPositions(varibleVector);

	}
}

void updateSelectedINputBox(inputBox* newSelcted) {
	if (selectedInput != nullptr) {
		selectedInput->setColour(lastColour);
	}
	lastColour = newSelcted->getColor();
	selectedInput = newSelcted;
	selectedInput->setColour(colorPallet.selectColour);

}

void addVectorToList() {
	//more tehna 5 on a list casues overdraw
	if (vector3DVector.size() < 5) {

		std::string str = "   VECTOR   ";
		vector3DVector.push_back(new vectorInputBox);

		//just finding the last vector so i can edit it
		int end = vector3DVector.size() - 1;
		vector3DVector[end]->text = new textBox(1.0f, 0.1f, 0.0f, 0.2f * (vector3DVector.size() - 1), str, &font, VectorsBox);

		vector3DVector[end]->x = new inputBox(0.33f, 0.1f, 0.0f, 0.2f * (vector3DVector.size() - 1) + 0.1f, "X:", &font, VectorsBox);
		vector3DVector[end]->x->setColour({ 255,200,200,255 });

		vector3DVector[end]->y = new inputBox(0.33f, 0.1f, 0.33f, 0.2f * (vector3DVector.size() - 1) + 0.1f, "Y:", &font, VectorsBox);
		vector3DVector[end]->y->setColour({ 200,255,200,255 });

		vector3DVector[end]->z = new inputBox(0.33f, 0.1f, 0.66f, 0.2f * (vector3DVector.size() - 1) + 0.1f, "Z:", &font, VectorsBox);
		vector3DVector[end]->z->setColour({ 200,200,255,255 });

	}

}

void removeSelectedEquation() {
	if (selectedInput == nullptr) { return; }

	for (int i = 0; i < equationVector.size(); i++) {
		if (equationVector[i] == selectedInput) {
			equationVector.erase(equationVector.begin() + i);
			if (meshes.size() != 0) {
				meshes.erase(meshes.begin() + i);
				resetBoxPositions(equationVector);
				return;
			}
		}
	}

	for (int i = 0; i < varibleVector.size(); i++) {
		if (varibleVector[i] == selectedInput) {
			varibleVector.erase(varibleVector.begin() + i);
			resetBoxPositions(varibleVector);
			return;
		}
	}

	//since a there is only one triangle in a mesh but multiple in a vector asnd we add vectors after meshes in the mesh list
	//we can be reasonably confident that any mesh with one triangle is a vector so all meshes after the first mesh with one triangle is a vector.
	int indexOfFirstVector = 0;
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i].tris.size() == 1) {
			indexOfFirstVector = i;
			break;
		}
	}

	for (int i = 0; i < vector3DVector.size(); i++) {
		if (vector3DVector[i]->x == selectedInput || vector3DVector[i]->y == selectedInput || vector3DVector[i]->z == selectedInput) {
			vector3DVector.erase(vector3DVector.begin() + i);

			if (meshes.size() != 0) {
				meshes.erase(meshes.begin() + indexOfFirstVector + i);
			}
		}
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
	std::cout << meshRes << std::endl;
}
void increseMeshSizeFunc() {
	meshSideLengh += 1;
	std::cout << meshSideLengh << std::endl;
}

void decreseMeshSizeFunc() {
	if (meshSideLengh == 1) {
		return;
	}

	meshSideLengh -= 1;
	std::cout << meshSideLengh << std::endl;
}

void decreasMeshRes() {
	if (meshRes == 1) {
		return;
	}

	meshRes -= 1;
	std::cout << meshRes << std::endl;
}

void decreasOpacity() {
	meshOpacity -= 10;

	if (meshOpacity < 0)
		meshOpacity = 0;
}

void increseOpacity() {
	meshOpacity += 10;

	if (meshOpacity > 255)
		meshOpacity = 255;
}

void togglePositonColours() {
	posClourTriangle = !posClourTriangle;
}

void toddleFillTriangle() {
	fillTriangleOnDraw = !fillTriangleOnDraw;
}

void setPerspectiveProjection() {
	camModePerspective = true;

	projectionMatrix = matrix4x4{};
	makeProjectionMatrix(projectionMatrix, aspectRaatio, fovRadain, zNear, zFar);
}

void setOrthographicProjection() {
	camModePerspective = false;

	projectionMatrix = matrix4x4{};
	makeOrthagraphicMatrix(projectionMatrix, -orthagrpahicCamWidth/(float)2 / (float)aspectRaatio, orthagrpahicCamWidth / (float)2 / (float)aspectRaatio, -orthagrpahicCamWidth / (float)2, orthagrpahicCamWidth / (float)2, zNear, zFar);
}

void updateOrthagraphicCam() {
	projectionMatrix = matrix4x4{};
	makeOrthagraphicMatrix(projectionMatrix, -orthagrpahicCamWidth / (float)2 / (float)aspectRaatio, orthagrpahicCamWidth / (float)2 / (float)aspectRaatio, -orthagrpahicCamWidth / (float)2, orthagrpahicCamWidth / (float)2, zNear, zFar);
}

#pragma endregion

#pragma endregion

int main() {

	setUp();
	start();

	while (isRunning) {
		deltaA = SDL_GetTicks();
		deltaTime = deltaA - deltaB;


		if (deltaTime > 1000 / 18.0) //limiting frme rate
		{
			//std::cout << "fps: " << 1000 / deltaTime << std::endl;

			handleEvents();
			update();
			render();

			deltaB = deltaA;

		}

	}

	//frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	//error here
	SDL_Quit();

	return 0;
};

//called once before loop
void start() {

	deltaTime = SDL_GetTicks();

	//drawList.push_back(&equationVector);

#pragma region setting up UI 

	for (int i = 0; i < matrixVector.size(); i++) {
		matrixVector[i]->setColour(colorPallet.pastelPurple);
	}

	for (int i = 0; i < matrixVector.size(); i++) {
		matrixVector[i]->setText("0");
	}
	b1.setText("1");
	b5.setText("1");
	b9.setText("1");

	resetEquations.setColour(colorPallet.green);
	addEquations.setColour(colorPallet.lightBlue);
	addVarible.setColour(colorPallet.lightBlue);
	addVector.setColour(colorPallet.lightBlue);

	removeItem.setColour(colorPallet.red);

	lightenEquation.setColour(colorPallet.lightOrange);
	darkenEquation.setColour(colorPallet.darkOrange);

	orthagraphicPerspectiveButton.setColour(colorPallet.pink);
	perspectiveButton.setColour(colorPallet.pastelPurple);

	increaseResButton.setColour(colorPallet.green);
	decreaseResButton.setColour(colorPallet.red);

	drawTriangles.setColour(colorPallet.lightBlue);
	postionColoursToggle.setColour(colorPallet.lightBlue);

	plusOpacity.setColour(colorPallet.green);
	lowOpacity.setColour(colorPallet.red);

	increaseMeshSize.setColour(colorPallet.green);
	decreaseMeshSize.setColour(colorPallet.red);

	resetEquations.setTriggerFunc(&updateMeshes);
	addEquations.setTriggerFunc(&addEquation);
	addVarible.setTriggerFunc(&addVaribleFunc);
	addVector.setTriggerFunc(&addVectorToList);
	removeItem.setTriggerFunc(&removeSelectedEquation);

	lightenEquation.setTriggerFunc(&lightenBackGroundColour);
	darkenEquation.setTriggerFunc(&drakenBackGroundColour);

	increaseResButton.setTriggerFunc(&increseMeshRes);
	decreaseResButton.setTriggerFunc(&decreasMeshRes);

	drawTriangles.setTriggerFunc(&toddleFillTriangle);
	postionColoursToggle.setTriggerFunc(&togglePositonColours);

	plusOpacity.setTriggerFunc(&increseOpacity);
	lowOpacity.setTriggerFunc(&decreasOpacity);

	perspectiveButton.setTriggerFunc(&setPerspectiveProjection);
	orthagraphicPerspectiveButton.setTriggerFunc(&setOrthographicProjection);

	increaseMeshSize.setTriggerFunc(&increseMeshSizeFunc);
	decreaseMeshSize.setTriggerFunc(&decreseMeshSizeFunc);

	for (int i = 0; i < textButtons.size(); i++) {
		textButtons[i]->setRoundedCorners(renderer);
	}

#pragma endregion

	font = typeFace(40, 90);
	//font.loadLettersBit("Fonts/Nemeue", renderer);
	font.loadLettersFromSpriteSheet("Fonts/PNGFirstCompEquationFontMRK10.bmp", renderer);

	//defining prjection matrix
	makeProjectionMatrix(projectionMatrix, aspectRaatio, fovRadain, zNear, zFar);

	//float t = returnEquiationValue(equ, {3.0f, 3.0f, 3.0f});

	selectedInput = nullptr;

#pragma region create meshes


#pragma endregion

	RenderPackage ={
		renderer,
		window,
		screen
	};

	std::string text = "EQUATION:";

	inputBox defaultEquation(1.0f, 0.1f, 0.0f, 0.1f * equationVector.size(), text, &font, equationsBox);
	
	equationVector.push_back(new inputBox(1.0f, 0.1f, 0.0f, 0.1f * equationVector.size(), text, &font, equationsBox));

	equationVector[0]->setText("SIN(X*Z)");
	//equationVector[0]->setText("0");

	updateMeshes();

}

#pragma region functionsCalledEveryFrame

void handleEvents() {
	SDL_Event event;

	Uint32 buttons;
	int xPreMouseState = xMousePotistion; 
	int yPreMouseState = yMousePotistion;
	buttons = SDL_GetMouseState(&xMousePotistion, &yMousePotistion);

	char* textInput = new char[1];

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}

		if (event.type == SDL_TEXTINPUT) {
			if (selectedInput != nullptr) {

				int y = strlen(event.text.text);

				std::string str = event.text.text;
				
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);

				selectedInput->addStr(str);
			}
		}

		if (event.type == SDL_KEYDOWN) {
			char keyDown = event.key.keysym.scancode;
			if (selectedInput != nullptr) {
				if (keyDown == SDL_SCANCODE_BACKSPACE) {
					std::string str = { 8 };

					selectedInput->addStr(str);
				}
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN){
			if (event.button.button == SDL_BUTTON_RIGHT) {
				rightClickDown = true;
			}

			if (event.button.button == SDL_BUTTON_LEFT) {
				inputBox *tempInputEqu = nullptr;

				////checking to see if new input box selected
				//if (selectedInput != nullptr) {
				//	selectedInput->setColour({ 255,255,255,255 });
				//}

				bool selcted = false;

				for (int i = 0; i < equationVector.size(); i++) {
					if (equationVector[i]->checkMouseOver(xMousePotistion, yMousePotistion)) {
						updateSelectedINputBox(equationVector[i]);
						selcted = true;
						break;
					}
				}

				for (int i = 0; i < varibleVector.size(); i++) {
					if (varibleVector[i]->checkMouseOver(xMousePotistion, yMousePotistion)) {
						updateSelectedINputBox(varibleVector[i]);
						selcted = true;
						break;
					}
				}

				for (int i = 0; i < matrixVector.size(); i++) {
					if (matrixVector[i]->checkMouseOver(xMousePotistion, yMousePotistion)) {
						updateSelectedINputBox(matrixVector[i]);
						selcted = true;
						break;
					}
				}

				for (int i = 0; i < vector3DVector.size(); i++) {
					if (vector3DVector[i]->x->checkMouseOver(xMousePotistion, yMousePotistion)) {

						updateSelectedINputBox(vector3DVector[i]->x);
						selcted = true;
						break;
					}
					if (vector3DVector[i]->y->checkMouseOver(xMousePotistion, yMousePotistion)) {
						updateSelectedINputBox(vector3DVector[i]->y);
						selcted = true;
						break;
					}
					if (vector3DVector[i]->z->checkMouseOver(xMousePotistion, yMousePotistion)) {
						updateSelectedINputBox(vector3DVector[i]->z);
						selcted = true;
						break;
					}
				}

				//chacking for button click

				for (int i = 0; i < textButtons.size(); i++) {
					textButtons[i]->update(xMousePotistion, yMousePotistion);
				}

			}
		}

		if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_RIGHT) {
				rightClickDown = false;
			}
		}

		if (event.type == SDL_MOUSEWHEEL) {
			float zoomDist = 0.03f;
			if (event.wheel.y > 0) {
				if (camModePerspective) {
					//for (int i = 0; i < meshes.size(); i++) {
					//	translateMesh(meshes[i], meshes[i], vector3D{ 0.0f,0.0f, -zoomDist });
					//}

					zoom -= zoomDist;
				}
				else
				{
					orthagrpahicCamWidth += 0.5;
					setOrthographicProjection();
				}
			}
			else if (event.wheel.y < 0) {// && zoom < 0.0f
				if (camModePerspective) {
			/*		for (int i = 0; i < meshes.size(); i++) {
						translateMesh(meshes[i], meshes[i], vector3D{ 0.0f,0.0f, zoomDist });

					}*/
					zoom += zoomDist;
				}
				else
				{
					orthagrpahicCamWidth -= 0.5f;
					setOrthographicProjection();
				}
			}
		}
	}

	if (rightClickDown) {
		int xGap = xMousePotistion - xPreMouseState;
		xRotation += xGap / (float)500;

		int yGap = yMousePotistion - yPreMouseState;
		yRotation += yGap / (float)500;

		if (yRotation > 1.570796)
			yRotation = 1.570796;
		else if (yRotation < -1.570796)
			yRotation = -1.570796;

		std::cout << yRotation << std::endl;

	}

	delete[] textInput;

}

void update() {
	runTime++;

	//inputBox* temp = dynamic_cast<inputBox*>(UI[0]);
	//std::string str = std::to_string(zoom);
	//temp->setText(str);
}

void render() {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

#pragma region graphics

	std::vector<mesh> drawMeshes;
	for (int i = 0; i < meshes.size(); i++) {
		mesh temp;
		temp = meshes[i];

		translateMesh(temp, temp, { -meshes[i].origin.x , -meshes[i].origin.y , -meshes[i].origin.z });

		//rotateMeshXAxis(meshes[i], temp, meshes[i].origin, -yRotation);

		//rotateMeshYAxis(temp, temp, meshes[i].origin, xRotation);

		//std::cout << yRotation << std::endl;


		matrix4x4 m;
		matrix4x4 rotate;
		matrix4x4 yaw;
		matrix4x4 pitch;

		

		//if (yRotation > 0) {
		//	yRotation = -yRotation;
		//}

		makeRotationMatrixZ(pitch, yRotation);
		makeRotationMatrixY(yaw,  -xRotation);


		multiplyMatrix(m, pitch, yaw);
		//multiplyMatrix(rotate, m, roll);
		applyMatrixToMesh(temp, scew);
		applyMatrixToMesh(temp, m);

		translateMesh(temp, temp, { meshes[i].origin.x , meshes[i].origin.y , meshes[i].origin.z });

		void multiplyMatrix(matrix4x4 & out, matrix4x4 m1, matrix4x4 m2);

		drawMeshes.push_back(temp);
	}

	std::vector<triangle> tris;
	sortTriangleDistToCameraKernal(drawMeshes, tris);
	//int totalTriangles = 0;
	
	//std::vector<triangle> unorderTriangles;
	//for (auto mesh : meshes) {
	////	totalTriangles += mesh.tris.size();
	//	unorderTriangles.insert(unorderTriangles.end(), mesh.tris.begin(), mesh.tris.end());
	//}


	////starting matrix
	matrix4x4 idnetity;
	//fillMatrix(scew, 1.f);
	idnetity.m[0][0] = 1.f;
	idnetity.m[1][1] = 1.f;
	idnetity.m[2][2] = 1.f;
	idnetity.m[3][3] = 1.f;

	//transform matrix (mainly for zoom)
	matrix4x4 trnsform;
	//fillMatrix(scew, 1.f);
	trnsform.m[0][0] = 1.f;
	trnsform.m[1][1] = 1.f;
	trnsform.m[2][2] = 1.f;
	trnsform.m[3][3] = 1.f;
	
	trnsform.m[3][0] = 0.f;
	trnsform.m[3][1] = 0.f;
	trnsform.m[3][2] = zoom + 1.f;

	//multiplyMatrixAs3x3(trnsform, scew, trnsform);
	multiplyMatrixAs3x3(trnsform, idnetity, trnsform);

	//could add roation

	matrix4x4 p = projectionMatrix;

	//scew.m[2][0] = -5.f;
	//scew.m[2][1] =  2.f;
	matrix4x4 pv;

	if (camModePerspective) {
		multiplyMatrix(pv, trnsform, projectionMatrix);
	}
	else
	{
		multiplyMatrixAs3x3(pv, trnsform, projectionMatrix);
	}

	
	drawTriangleVector(pv ,renderer, tris, meshes[0].origin.z, fillTriangleOnDraw, true, posClourTriangle, meshOpacity);
	//drawTriangleArr(renderer, tris, totalTriangles, true, false);

	//delete[] tris;

#pragma endregion

#pragma region UI drawing

	buttonsBox.draw(RenderPackage);
	equationsBox.draw(RenderPackage);
	VectorsBox.draw(RenderPackage);
	variblesBox.draw(RenderPackage);
	matrixBox.draw(RenderPackage);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	for (int i = 0; i < equationVector.size(); i++) {
		//menue* temp = UI[i]
		equationVector[i]->draw(RenderPackage);
	}

	for (int i = 0; i < varibleVector.size(); i++) {
		//menue* temp = UI[i]
		varibleVector[i]->draw(RenderPackage);
	}

	for (int i = 0; i < vector3DVector.size(); i++) {
		//menue* temp = UI[i]
		vector3DVector[i]->text->draw(RenderPackage);
		vector3DVector[i]->x->draw(RenderPackage);
		vector3DVector[i]->y->draw(RenderPackage);
		vector3DVector[i]->z->draw(RenderPackage);
	}

	for (int i = 0; i < textButtons.size(); i++) {		
		textButtons[i]->draw(RenderPackage);
	}

	for (int i = 0; i < matrixVector.size(); i++) {
		//menue* temp = UI[i]
		matrixVector[i]->draw(RenderPackage);
	}

	incresResText.draw(RenderPackage);
	changemeshOpactyText.draw(RenderPackage);
	cahngeMeshSizeText.draw(RenderPackage);

#pragma endregion

	SDL_SetRenderDrawColor(renderer, BGColor.r, BGColor.g, BGColor.b, 255);
	SDL_RenderPresent(renderer);
}

#pragma endregion

