#include "TextRendering.h"
#include <string>


typeFace::typeFace() {}
typeFace::~typeFace(){}

typeFace::typeFace(int charWidth, int charHeight) {
	this->charHeight = charHeight;
	this->charWidth = charWidth;
	this->aspectRatio = charHeight / (float)charWidth;
}

int typeFace::getCharWidth() { return charWidth; };
int typeFace::getCharHeight() { return charHeight; };
int typeFace::getAspectRatio() { return aspectRatio; };

void typeFace::loadLettersBit(std::string pathToLetters, SDL_Renderer* renderer){
	for (int i = 0; i < 9; i++) {
		std::string path = pathToLetters;

		int askiVal = 40 + i;

		path.push_back('/');

		std::string str = std::to_string(askiVal);

		for (auto chr : str) { path.push_back(chr); }

		path.push_back('.');
		path.push_back('b');
		path.push_back('m');
		path.push_back('p');

		SDL_Surface* image = SDL_LoadBMP(path.c_str());

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

		letters[i] = texture;

	}

	for (int i = 8; i < 51; i++) { // do not start at 0 as if i start there the code will look for files iwth * in it and that is a wild card
		std::string path = pathToLetters;

		int askiVal = 40 + i;

		path.push_back('/');
		path.push_back(askiVal);
		path.push_back('.');
		path.push_back('b');
		path.push_back('m');
		path.push_back('p');
		
		SDL_Surface* image = SDL_LoadBMP(path.c_str());

		if (image != NULL) {
			std::string path = pathToLetters;

			path.push_back('/');
			
			std::string str = std::to_string(askiVal);

			for (auto chr : str) { path.push_back(chr); }

			path.push_back('.');
			path.push_back('b');
			path.push_back('m');
			path.push_back('p');

			SDL_Surface* image = SDL_LoadBMP(path.c_str());
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

		letters[i] = texture;
		
		//SDL_LoadBMP("C:\\Users\\twhil\\OneDrive\\Desktop\\programing projects\\PrimittiveEngeion\\0.bmp");

		//letters[i] = SDL_LoadBMP("C:\\Users\\twhil\\OneDrive\\Desktop\\programing projects\\PrimittiveEngeion\\0.bmp");
	}
}

void typeFace::loadLettersFromSpriteSheet(std::string pathToLetters, SDL_Renderer* renderer) {
	
	SDL_Surface* image = SDL_LoadBMP(pathToLetters.c_str());

	SDL_Rect basicCharRect;

	basicCharRect.h = image->h;
	basicCharRect.w = image->w/51;

	for (int i = 0; i < 51; i++) {

		SDL_Rect charRect = basicCharRect;
		charRect.x = i * basicCharRect.w;
		charRect.y = 0;

		SDL_Surface* temp = SDL_CreateRGBSurface(0, charRect.w, charRect.h, 32, 0, 0, 0, 0);

		SDL_BlitSurface(image, &charRect, temp, NULL);

		SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255,255,255));

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);

		letters[i] = texture;

	}
}

int typeFace::drawImage(SDL_Renderer* renderer, char letter, SDL_Rect rect ) {
	int index = letter - 40;
	if (index < 0) {
		return 0;
	}
	if (index > 50) {
		return 0;
	}
	if (letters[index] != NULL) {
		SDL_RenderCopy(renderer, letters[index], NULL, &rect);
		return 1;
	}

	return 0;
}

void typeFace::drawWord(renderPackage renderPack, SDL_Rect area, std::string text){
	SDL_Rect tempRect;

	tempRect.x = area.x;
	tempRect.y = area.y;

	tempRect.w = area.w / ((float)text.size());
	tempRect.h = tempRect.w * aspectRatio;

	//if the rectangle has a larger height thatn the box it is to fit in 
	//then it has its hight set ot the size of the rectangle
	if (tempRect.h > area.h) { 
		tempRect.h = area.h;
	}
	else
	{
		tempRect.y += (area.h - tempRect.h) / 2 + 1;
	}

	for (auto chr : text) {
		drawImage(renderPack.renderer, chr, tempRect);
		tempRect.x += tempRect.w;
	}
}