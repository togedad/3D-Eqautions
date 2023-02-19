#include <stdio.h>

void outputArr(char* string, int len) { //looops through ll charicters and prints tehm to screan 
    for (int i = 0; i < len; ++i) {
        printf("%c", string[i]);
    }
}

void removeString(char*& string, int& len, int start, int end) {
    int newLen = len - (end - start);//finds the new lengh of arrray

    char* newCharArr = string;//copying the array
    string = new char[newLen];//makeing the string a new array of corrcet lengh

    int add = 0;//add holds the number of time you have to skip so when you get to the charicters that are to be removed all you need to do is skip over them so the add gets changed
    for (int i = 0; i < newLen; i++) {
        if (i >= start && i < end) {//looking when to chaged the add number
            add = end - start;
        }

        string[i] = newCharArr[i + add];//adding charicter to the list
    }

    len = newLen;//changeing len of the string to correct value
}

void insertCharrArr(char*& string, int& len, char* insertStr, int insertLen, int location) {
    int newLen = len + insertLen;//finds the new lengh of arrray
    char* newCharArr = new char[len];

    if (location == len) // end of string
    {
        newCharArr = string;

        for (int n = 0; n < insertLen; n++) {//inserting new string 
            newCharArr[len + n] = insertStr[n];
        }
    }
    else {
        int add = 0;//add holds the number of time you have to skip so when you get to the charicters that have been inserted so the dont get written over straight away

        for (int i = 0; i < len; i++) {

            if (i == location) {//looking when to insert
                add = insertLen;
                for (int n = 0; n < insertLen; n++) {//inserting new string 
                    newCharArr[i + n] = insertStr[n];
                }
            }

            newCharArr[i + add] = string[i];
        }
    }

    string = newCharArr;
    len = newLen;//setting new lengh

}

void replaceString(char*& string, int& len, char* removeStr, int removeLen, char* replacetStr, int replacetLen) {
    for (int i = 0; i < len; i++) {
        if (string[i] == removeStr[0]) {//looking to see if charicter is same as first charicter of string to be replaced
            bool found = true;

            for (int n = 1; n < removeLen; n++) {//checking too see if it is the sub string that is to be removed
                if (string[i + n] != removeStr[n]) {//sinmple loop chaeck too see if it is the correct sub string
                    found = false;
                    break;
                }
            }

            if (found) {
                removeString(string, len, i, i + removeLen);//removes string
                insertCharrArr(string, len, replacetStr, replacetLen, i);//inserts new string 
                i += replacetLen;//skips i over the new string, as in some cases new string could include replacement string so could create infinate loop
            }
        }
    }
}

void removeAllChar(char*& string, int& len, char* removeStr, int removeLen) {
    for (int i = 0; i < len; i++) {
        if (string[i] == removeStr[0]) {//looking to see if charicter is same as first charicter of string to be replaced
            bool found = true;

            for (int n = 1; n < removeLen; n++) {//checking too see if it is the sub string that is to be removed
                if (string[i + n] != removeStr[n]) {//sinmple loop check too see if it is the correct sub string
                    found = false;
                    break;
                }
            }

            if (found) {
                removeString(string, len, i, i + removeLen);//removes string
            }
        }
    }
}


void pushToBack(char*& string, int& len, char charToPush) {
    len++;//finds the new lengh of arrray
    string[len - 1] = charToPush;//adding flaots form insert string to string   
}
//

void removeFloats(float*& floatArr, int& len, int start, int end) {
    int newLen = len - (end - start);//finds the new lengh of arrray

    float* newCharArr = floatArr;//copying the array
    floatArr = new float[newLen];//makeing the string a new array of corrcet lengh

    int add = 0;//add holds the number of time you have to skip so when you get to the charicters that are to be removed all you need to do is skip over them so the add gets changed
    for (int i = 0; i < newLen; i++) {
        if (i >= start && i < end) {//looking when to chaged the add number
            add = end - start;
        }

        floatArr[i] = newCharArr[i + add];//adding charicter to the list
    }

    len = newLen;//changeing len of the string to correct value
}

void insertFlaot(float*& floatArr, int& len, float insertFloat, int location) {
    int newLen = len + 1;//finds the new lengh of arrray

    float* newCharArr = floatArr;//copying the array
    floatArr = new float[newLen];//makeing the string a new array of corrcet lengh

    int add = 0;//add holds the number of time you have to skip so when you get to the charicters that have been inserted so the dont get written over straight away
    for (int i = 0; i < len; i++) {
        if (i == location) {//looking when to insert
            add = 1;
            //inserting new float 
            floatArr[i] = insertFloat;
        }

        floatArr[i + add] = newCharArr[i];//adding cahricters to string   
    }
    len = newLen;//setting new lengh
}

void pushBackFloat(float*& numbers, int& len, float floatToPush) {
    len++;//finds the new lengh of arrray
    numbers[len - 1] = floatToPush;//adding flaots form insert string to string   
}

void replaceFloat(float*& floatArr, int& len, char removeFloat, char replacetmentFloat) {
    for (int i = 0; i < len; i++) {

        if (floatArr[i] == removeFloat) {//looking to see if charicter is same as first charicter of string to be replaced
            bool found = true;

            if (found) {
                removeFloats(floatArr, len, i, i + 1);//removes string
                insertFlaot(floatArr, len, replacetmentFloat, removeFloat);//inserts new string 
                i += 1;//skips i over the new string, as in some cases new string could include replacement string so could create infinate loop
            }
        }
    }
}

void removeAllFloats(float*& floatArr, int& len, float removeFlaot) {
    for (int i = 0; i < len; i++) {
        if (floatArr[i] == removeFlaot) {//looking to see if flaot is same as float
            bool found = true;

            if (found) {
                removeFloats(floatArr, len, i, i + 1);//removes float
            }
        }
    }
}

void removeSectionChar(char*& string, int& len, int start, int end) {
    int removeLengh = end - start;
    int newLen = len - removeLengh;//finds the new lengh of arrray

    char* newCharArr = string;//copying the array

    if (removeLengh + start != len) {//this happens when it wants to remove end of string 
        for (int i = end; i < len; i++) {

            string[i - removeLengh] = newCharArr[i];//adding charicter to the list
        }
    }

    len = newLen;//changeing len of the string to correct value
}

void reverse(char*& string, int& len) {
    char* copyCharArr = new char[len];

    for (int i = 0; i < len; i++) {
        char letter = string[len - 1 - i];
        copyCharArr[i] = string[len - 1 - i];//taking alst element pos and taking i to reverse the array
    }

    string = copyCharArr;
}