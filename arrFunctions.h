#pragma once

void outputArr(char* string, int len);

void removeString(char*& string, int& len, int start, int end);

void insertCharrArr(char*& string, int& len, char* insertStr, int insertLen, int location);

void replaceString(char*& string, int& len, char* removeStr, int removeLen, char* replacetStr, int replacetLen);

void removeAllChar(char*& string, int& len, char* removeStr, int removeLen);

void pushToBack(char*& string, int& len, char charToPush);

void removeSectionChar(char*& string, int& len, int start, int end);

//

void removeFloats(float*& floatArr, int& len, int start, int end);

void pushBackFloat(float*& numbers, int& len, float floatToPush);

void reverse(char*& string, int& len);