#pragma once

#include "SDL.h"
#include <string.h>
#include <iostream>

struct renderPackage {
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Surface* screen;
};

class typeFace
{
public:
	typeFace(int charWidth, int charHeight); //path refers to a folder where a bitmap of each letter is found, letters not found will be left empty
	typeFace();
	~typeFace();

	void loadLettersBit(std::string pathToLetters, SDL_Renderer* renderer);
	void loadLettersFromSpriteSheet(std::string pathToSheet, SDL_Renderer* renderer);
	int drawImage(SDL_Renderer* renderer, char letter, SDL_Rect rect);
	void drawWord(renderPackage renderPack, SDL_Rect area, std::string text);

	int getCharWidth();
	int getCharHeight();
	int getAspectRatio();

private:
	SDL_Texture* letters[51];
	int charWidth;
	int charHeight;
	float aspectRatio;
};