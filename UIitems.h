#pragma once

#include "SDL.h"
#include <string>
#include "TextRendering.h"

#include <vector>

class menueBox {
public:
	menueBox(float x, float y, int width, int height, SDL_Color colour, int screanWidth, int screanHeight);
	void draw(renderPackage RenderPack);

	int getWidth();
	int getHeight();
	int getXPos();
	int getYPos();

protected:
	int height,
		width,
		x,
		y;

	SDL_Rect rect;
	SDL_Color color;
};

class menue
{
protected:
	SDL_Rect rect;
	SDL_Color colour = {255,255,255,255};

	menueBox* parentBox;

	SDL_Texture* corners;
	int cornerTextSideLen;

	void drawCorners(renderPackage RenderPack);

public:
	menue(float width, float height, float xPos, float yPos, menueBox &parentBox);
	menue();

	SDL_Rect getRect();
	float getXPos();
	float getYPos();

	bool checkMouseOver(int mouseX, int mouseY);

	void setRoundedCorners(SDL_Renderer* renderer);

	SDL_Color getColor();

	virtual void draw(renderPackage RenderPack);
	virtual void update();

	void setColour(SDL_Color colour);
	void setPosition(float xPos, float yPos);
};

class textBox : public menue {
public:
	textBox(float width, float height, float xPos, float yPos, std::string text, typeFace* type, menueBox& parentBox);
	virtual void draw(renderPackage RenderPack);
	virtual void setText(std::string text);

protected:
	typeFace* type;

private:
	std::string text;
};

class inputBox : public textBox {
public:
	inputBox(float width, float height, float xPos, float yPos, std::string message, typeFace* type, menueBox& parentBox);
	void draw(renderPackage RenderPack);
	
	void addStr( std::string input);


	void setText(std::string data);
	std::string getData();
	std::string getDisplayText();

protected:
	std::string message;
	std::string data;

};

class button : public menue {
public:
	button(float width, float height, float xPos, float yPos, menueBox &parentBox);
	//button();

	virtual void draw(renderPackage RenderPack);
	virtual void update(int mouseX, int mouseY);

	void setTriggerFunc(void (*triggerFunction)());

protected:
	void (*triggerFunction)() = nullptr;//this points to a fucniton that is to be when the button is clicked
};


class textButton : public button {
public:
	textButton(float width, float height, float xPos, float yPos, std::string text, typeFace* type, menueBox& parentBox);
	virtual void draw(renderPackage RenderPack);

protected:
	void (*triggerFunction)() = nullptr;//this points to a fucniton that is to be when the button is clicked
	typeFace* type;

private:
	std::string text;
};

