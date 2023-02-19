#include "UIitems.h"

#include <algorithm>
#include <iostream>

menue::menue(float width, float height, float xPos, float yPos, menueBox& parentBox) {
    this->parentBox = &parentBox;


    //this->xPos = std::clamp(xPos, 0.0f, 1.0f);
    //this->yPos = std::clamp(yPos, 0.0f, 1.0f);
    xPos = (xPos * this->parentBox->getWidth()) + this->parentBox->getXPos();
    yPos = (yPos * this->parentBox->getHeight()) + this->parentBox->getYPos();

    rect.x = xPos;//(xPos * parentBox.getWidth()) + parentBox.getWidth();
    rect.y = yPos;// (xPos * parentBox.getHeight()) + parentBox.getHeight();
    rect.w = width * parentBox.getWidth();
    rect.h = height * parentBox.getHeight();

}

void menue::setRoundedCorners(SDL_Renderer* renderer) {
    //creating corners

    SDL_Surface* cornersTexture = SDL_LoadBMP("Textures\\corners.bmp"); 
    SDL_SetColorKey(cornersTexture, SDL_TRUE, SDL_MapRGB(cornersTexture->format, 0, 0, 0));
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, cornersTexture);
    corners = text;

    cornerTextSideLen = std::min(cornersTexture->w, cornersTexture->h);

    //SDL_Surface* topRight = SDL_CreateRGBSurface(0, cornerRect.w, cornerRect.h, 32, 0, 0, 0, 0);
    //cornerRect.x = cornersTexture->w / 2;
    //SDL_BlitSurface(cornersTexture, &cornerRect, topRight, NULL);
    //SDL_SetColorKey(topRight, SDL_TRUE, SDL_MapRGB(topRight->format, 0, 0, 0));
    //corners[1] = SDL_CreateTextureFromSurface(RenderPac.renderer, topRight);
                                                  
    //SDL_Surface* lowRight = SDL_CreateRGBSurface(0, cornerRect.w, cornerRect.h, 32, 0, 0, 0, 0);
    //cornerRect.y = cornersTexture->h / 2;
    //SDL_BlitSurface(cornersTexture, &cornerRect, lowRight, NULL);
    //SDL_SetColorKey(lowRight, SDL_TRUE, SDL_MapRGB(lowRight->format, 0, 0, 0));
    //corners[2] = SDL_CreateTextureFromSurface(RenderPac.renderer, lowRight);

    //SDL_Surface* lowLeft = SDL_CreateRGBSurface(0, cornerRect.w, cornerRect.h, 32, 0, 0, 0, 0);
    //cornerRect.x = 0;
    //SDL_BlitSurface(cornersTexture, &cornerRect, lowLeft, NULL);
    //SDL_SetColorKey(lowLeft, SDL_TRUE, SDL_MapRGB(lowLeft->format, 0, 0, 0));
    //corners[3] = SDL_CreateTextureFromSurface(RenderPac.renderer, lowLeft);
}

menue::menue() {
    rect = { 0,0,0,0 };
    parentBox = nullptr;
}

SDL_Color menue::getColor() { return colour; }
SDL_Rect menue::getRect() { return rect; }
float menue::getXPos() { return rect.x; };
float menue::getYPos() { return rect.y; };

void menue::setColour(SDL_Color colour) {
    this->colour = colour; 
};

void menue::setPosition(float xPos, float yPos) {
    xPos = (xPos * this->parentBox->getWidth()) + this->parentBox->getXPos();
    yPos = (yPos * this->parentBox->getHeight()) + this->parentBox->getYPos();

    rect.x = xPos;
    rect.y = yPos;
}

void menue::draw(renderPackage RenderPack) { //draws to the desigherd renderer

    SDL_SetRenderDrawColor(RenderPack.renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(RenderPack.renderer, &rect);
}

void menue::drawCorners(renderPackage RenderPack) {
    int sideLen = std::min(rect.w, rect.h) / 4;
    int halfOfTextlen = cornerTextSideLen/2;

    int sizeOfCornerText = cornerTextSideLen / 2;
   
    SDL_Rect topLeftLcoation{ rect.x, rect.y,sideLen,sideLen };
    SDL_Rect topLeftTexture { 0, 0, sizeOfCornerText, sizeOfCornerText };

    SDL_Rect topRightLcoation{ rect.x + rect.w - sideLen, rect.y, sideLen,sideLen };
    SDL_Rect topRightTexture{ sizeOfCornerText, 0, sizeOfCornerText, sizeOfCornerText };

    SDL_Rect bottomRightLcoation{ rect.x + rect.w - sideLen, rect.y + rect.h - sideLen, sideLen,sideLen };
    SDL_Rect bottomRightTexture{ sizeOfCornerText, sizeOfCornerText, sizeOfCornerText, sizeOfCornerText };

    SDL_Rect bottomleftcoation{ rect.x, rect.y + rect.h - sideLen, sideLen,sideLen };
    SDL_Rect bottomLeftTexture{ 0, sizeOfCornerText, sizeOfCornerText, sizeOfCornerText };

    SDL_RenderCopy(RenderPack.renderer, corners, &topLeftTexture, &topLeftLcoation);
    SDL_RenderCopy(RenderPack.renderer, corners, &topRightTexture, &topRightLcoation);
    SDL_RenderCopy(RenderPack.renderer, corners, &bottomRightTexture, &bottomRightLcoation);
    SDL_RenderCopy(RenderPack.renderer, corners, &bottomLeftTexture, &bottomleftcoation);
}

void menue::update() {}

bool menue::checkMouseOver(int mouseX, int mouseY) {
    if (!(mouseX > rect.x && mouseX < rect.x + rect.w)) {
        return false;
    }
    if (!(mouseY > rect.y && mouseY < rect.y + rect.h)) {
        return false;
    }

    std::cout << "clicked! ";

    return true;
}

textBox::textBox(float width, float height, float xPos, float yPos, std::string text, typeFace *type, menueBox& parentBox) : menue(width, height, xPos, yPos, parentBox) {
    this->text = text;
    this->type = type;
}

void textBox::setText(std::string text) {
    this->text = text;
}

void textBox::draw(renderPackage RenderPack){
    SDL_SetRenderDrawColor(RenderPack.renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(RenderPack.renderer, &rect);

/*    SDL_SetRenderDrawColor(RenderPack.renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(RenderPack.renderer, &rect);   */ 

    //if (topLeftCorner != nullptr) {
    //    drawCorners(RenderPack);
    //}

    type->drawWord(RenderPack, rect, text);
}

inputBox::inputBox(float width, float height, float xPos, float yPos, std::string message, typeFace* type, menueBox& parentBox) : textBox(width, height, xPos, yPos, message, type, parentBox) {
    this->message = message;
}

void inputBox::draw(renderPackage RenderPack) {
    SDL_SetRenderDrawColor(RenderPack.renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(RenderPack.renderer, &rect);

    //if (topLeftCorner != nullptr) {
    //    drawCorners(RenderPack);
    //}

    type->drawWord(RenderPack, rect, message + " " + data);

    
}

void inputBox::setText(std::string data) {
    this->data = data;
};

void inputBox::addStr(std::string input) {

    if (input[0] == 8) {
        if (data.size() == 0)
            return;
        data.pop_back();
    }
    else {
        data = data + input;
    }
    
}

std::string inputBox::getDisplayText() { return message; }

std::string inputBox::getData() { return data; }

button::button(float width, float height, float xPos, float yPos, menueBox& parentBox) : menue(width, height, xPos, yPos, parentBox) {}

void button::draw(renderPackage RenderPack) {
    SDL_SetRenderDrawColor(RenderPack.renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(RenderPack.renderer, &rect);

    //if (topLeftCorner != nullptr) {
    //    drawCorners(RenderPack);
    //}
}

void button::update(int mouseX, int mouseY) {
    if (!(mouseX > rect.x && mouseX < rect.x + rect.w)) {
        return;
    }
    if (!(mouseY > rect.y && mouseY < rect.y + rect.h)) {
        return;
    }   

    if (triggerFunction != nullptr){ triggerFunction(); }    
}

void button::setTriggerFunc(void (*triggerFunction)()) {
    this->triggerFunction = *triggerFunction;
}

menueBox::menueBox(float x, float y, int width, int height, SDL_Color colour, int screanWidth, int screanHeight) {
    this->color = colour;
    this->width = width;
    this->height = height;

    this->x = x * screanWidth;
    this->y = y * screanHeight;


    rect.w = width;
    rect.h = height;
    rect.x = x * screanWidth;
    rect.y = y * screanHeight;
}

void menueBox::draw(renderPackage RenderpPack) {
    SDL_SetRenderDrawColor(RenderpPack.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(RenderpPack.renderer, &rect);
}

int menueBox::getWidth() { return width; }
int menueBox::getHeight() { return height; }
int menueBox::getXPos() { return x; }
int menueBox::getYPos() { return y; }

textButton::textButton(float width, float height, float xPos, float yPos, std::string text, typeFace* type, menueBox& parentBox) : button(width,height, xPos,yPos ,parentBox) {
    this->text = text;
    this->type = type;
}

void textButton::draw(renderPackage RenderpPack) {
    SDL_SetRenderDrawColor(RenderpPack.renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(RenderpPack.renderer, &rect);

    if (corners != nullptr) {
        drawCorners(RenderpPack);
    }

    type->drawWord(RenderpPack, rect, text);
}

