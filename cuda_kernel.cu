//#include <stdio.h>
//#include <iostream>
//#include <iomanip>
//
//#include "SDL.h"
//#include <cuda_runtime.h>
//#include "cuda_kernel.cuh"
//#include "device_launch_parameters.h"
////#include "equation.cuh"
//
////other
//SDL_Surface* surf;
//int defaultSize = 75;
//
////char* temp = new char[len] { 'T', '(', 'S', '(', 'y', ')', ')'};
//char* GPUequations;
//
//int* equaitonLens;
//int* valusesInEquationLens;
//int* hits;
//
//float* vals;
//float* valusesInEquation;
//
//float resalution;
//
//int totalPoints;
//int blocks;
//
//int screenHeight;
//int screenWidth;
//int renderDistance;
//
//#pragma region Consts
//
//
//const int threadsBeingUsed = 256;
//
//const int operationsLen = 8;
//const int oneInputOperationsLen = 3;
//const int towInputOperationsLen = 4;
//
//const int numberToAddOnForAskiiRefrance = 91; // this will need to cahnge eventually
//
//__device__
//char operations[operationsLen] = { '*', '/', '+', '-', 'S', 'C', 'T', '(' };
//__device__
//char oneInputOperations[oneInputOperationsLen] = { 'S', 'C', 'T' };
//__device__
//char towInputOperations[towInputOperationsLen] = { '*', '/', '+', '-' };
////device funtions
//
//#pragma endregion
//
//
////to draw pixles to a surface
//void drawPixel(SDL_Surface* surface, int x, int y, uint8_t R, uint8_t G, uint8_t B) {
//    SDL_LockSurface(surface);
//    uint8_t* pixelArr = (uint8_t*)surface->pixels;
//    pixelArr[y * surface->pitch + (x * surface->format->BytesPerPixel)] = B;
//    pixelArr[y * surface->pitch + (x * surface->format->BytesPerPixel) + 1] = G;
//    pixelArr[y * surface->pitch + (x * surface->format->BytesPerPixel) + 2] = R;
//
//    SDL_UnlockSurface(surface);
//    //remmeber you need to update windop surface after use 
//
//}
//
////these are the same as in char functions but they have been re wirrten for use in the gpu
////
////
////if (threadIdx.x == 0 && blockIdx.x == 0) {
////    printf("\n out of sin");
////    outputEquation(equationPart, equationPartLen);
////    printf("\n");
////
////}
//
//void CPUoutputEquation(char* string, int len) { //looops through ll charicters and prints tehm to screan 
//    for (int i = 0; i < len; ++i) {
//        char temp = string[i];
//        printf("%c", temp);
//    }
//}
//
//#pragma region Equation
//
//__device__
//void outputEquation(char* string, int len) { //looops through ll charicters and prints tehm to screan 
//    for (int i = 0; i < len; ++i) {
//        char temp = string[i];
//        printf("%c", temp);
//    }
//}
//
//__device__
//void remove(char*& string, int& len, int start, int end) {
//    int removeLengh = end - start;
//    int newLen = len - removeLengh;//finds the new lengh of arrray
//
//    char* newCharArr = string;//copying the array
//
//    for (int i = end; i < len; i++) {
//
//        string[i - removeLengh] = newCharArr[i];//adding charicter to the list
//    }
//
//    len = newLen;//changeing len of the string to correct value
//}
//
//__device__
//void reverse(char*& string, int& len) {
//    char* copyCharArr = new char[len];
//
//    for (int i = 0; i < len; i++) {
//        char letter = string[len - 1 - i];
//        copyCharArr[i] = string[len - 1 - i];//taking alst element pos and taking i to reverse the array
//    }
//
//    string = copyCharArr;
//}
//
//__device__
//void insert(char*& string, int& len, char* insertStr, int insertLen, int location) {
//    int newLen = len + insertLen;//finds the new lengh of arrray
//    char* newCharArr = new char[len];
//
//    if (location == len) // end of string
//    {
//        newCharArr = string;
//
//        for (int n = 0; n < insertLen; n++) {//inserting new string 
//            newCharArr[len + n] = insertStr[n];
//        }
//    }
//    else {
//        int add = 0;//add holds the number of time you have to skip so when you get to the charicters that have been inserted so the dont get written over straight away
//
//        for (int i = 0; i < len; i++) {
//
//            if (i == location) {//looking when to insert
//                add = insertLen;
//                for (int n = 0; n < insertLen; n++) {//inserting new string 
//                    newCharArr[i + n] = insertStr[n];
//                }
//            }
//
//            newCharArr[i + add] = string[i];
//        }
//    }
//
//    string = newCharArr;
//    len = newLen;//setting new lengh
//
//}
//
//__device__
//void pushToBack(char*& string, int& len, char charToPush) {
//    len++;//finds the new lengh of arrray
//    string[len - 1] = charToPush;//adding flaots form insert string to string   
//}
//
//__device__
//void pushToBackFloat(float*& numbers, int& len, float floatToPush) {
//    len++;//finds the new lengh of arrray
//    numbers[len - 1] = floatToPush;//adding flaots form insert string to string   
//}
//
//__device__
//void replace(char*& string, int& len, char* removeStr, int removeLen, char* replacetStr, int replacetLen) {
//    for (int i = 0; i < len; i++) {
//        if (string[i] == removeStr[0]) {//looking to see if charicter is same as first charicter of string to be replaced
//            bool found = true;
//
//            for (int n = 1; n < removeLen; n++) {//checking too see if it is the sub string that is to be removed
//                if (string[i + n] != removeStr[n]) {//sinmple loop chaeck too see if it is the correct sub string
//                    found = false;
//                    break;
//                }
//            }
//
//            if (found) {
//                remove(string, len, i, i + removeLen);//removes string
//                insert(string, len, replacetStr, replacetLen, i);//inserts new string 
//                i += replacetLen;//skips i over the new string, as in some cases new string could include replacement string so could create infinate loop
//            }
//        }
//    }
//}
//
//__device__
//float returnEquationResult(char* equationPart, int equationPartLen, float* numbersInEquation, int& numbersLen) { //some funciton will be slow and i think are being called multiple times as it runns really slow when it is just renterng y
//
//    #pragma region one val ops
//
//    int depth = 0;
//    char* tempStr =new char[100];
//    int tempStrLen = 0;
//
//    for (int i = 0; i < equationPartLen; i++) {
//        char letter = equationPart[i];
//
//        if (letter == '(') {//seeing if it is in a set of brackets - the depth is howmany sets of brackes you are in for example ( (v) ) teh v is in a depth of 2 as it is 2 layers deep we only want to compute numbers on lowest level (0) as the numbers on loweelevels are what we are looking at teh brakets in highleves are fed in the prgram with recution 
//            depth++;
//        }
//        else if (letter == ')') {//seeing if it is leaveing a set of brackets
//            depth--;
//        }
//        else {
//            if ((depth == 0)) { //only runs if it is on right level look at comments above for depth in more detail
//
//                int start = i;
//
//                //outputEquation(equationPart, equationPartLen);
//
//                for (int n = 0; n < oneInputOperationsLen; n++) {//divided by size of char as it is not the amount of elemets but the size of all the data (types) of elemets so dividing it by the data type makes it the right size
//                    
//                    if (letter == oneInputOperations[n]) {
//                        
//
//                        i += 2;
//                        depth++; //as you skip over a a open bracket that needs to be acounted for
//
//                        while (depth > 0) {//needs an exit condition if no bracket -------------------------------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                            
//                            if (equationPart[i] == '(') {
//                                depth++;
//                            }
//                            else if (equationPart[i] == ')') {
//                                depth--;
//                            }
//
//                            if (depth > 0) {
//                                char tempLetter = equationPart[i];
//                                pushToBack(tempStr, tempStrLen, tempLetter);
//                            }
//
//                            i++;//referts to the for loop i if it seems out of place 
//                        }
//
//                        remove(equationPart, equationPartLen, start, i);
//
//                        double val;//declaring now so is=t can be useing in any of the cases below - this is BAD CODE as it uses 3 switch cases, if more opreations want to be added then a  better system should be devised but for now this is all that is needed
//                        val = returnEquationResult(tempStr, tempStrLen, numbersInEquation, numbersLen);
//
//                        switch (letter) {
//                        case 'S':
//                            val = sin(val);
//                            break;
//                        case 'C':
//                            val = cos(val);
//                            break;
//                        case 'T':
//                            val = tan(val);
//                            break;
//                        }
//
//                        char* temp = new char[2];
//                        temp[0] = '~';
//                        temp[1] = numbersLen + numberToAddOnForAskiiRefrance;//the value of the askicharicter is the index of nubers to look at
//
//                        pushToBackFloat(numbersInEquation, numbersLen, val);//push the float val to the back of this 
//                        insert(equationPart, equationPartLen, temp, 2, start);
//
//                        i = start + 2;
//                    }
//                }
//            }
//        }
//
//        //looking for times and multiplication
//    }
//
//    #pragma endregion
//
//    #pragma region mul divs
//
//    //looking at times and divide operations
//    depth = 0;
//    tempStr = new char[100];
//    tempStrLen = 0;
//
//    for (int i = 0; i < equationPartLen; i++) {
//        char letter = equationPart[i];
//
//        if (letter == '(') {//seeing if it is in a set of brackets - the depth is howmany sets of brackes you are in for example ( (v) ) teh v is in a depth of 2 as it is 2 layers deep we only want to compute numbers on lowest level (0) as the numbers on loweelevels are what we are looking at teh brakets in highleves are fed in the prgram with recution 
//            depth++;
//        }
//        else if (letter == ')') {//seeing if it is leaveing a set of brackets
//            depth--;
//        }
//
//        if (depth == 0 && (letter == '*' || letter == '/')) {
//
//
//            int currentIndex = i;
//
//            double value1;//value on right of symbol
//            double value2;//value on left of symbol
//
//            // for removeing the necerry information
//            int end;
//            int start;
//
//            //looking infront of the symbol
//            i++;
//            if (equationPart[i] == '(') {
//                int currentIndex = i;
//                i++;
//                depth++; //as you skip over a a open bracket that needs to be acounted for
//
//                while (depth > 0) {
//                    if (equationPart[i] == '(') {
//                        depth++;
//                    }
//                    else if (equationPart[i] == ')') {
//                        depth--;
//                    }
//
//                    if (depth > 0) {
//                        pushToBack(tempStr, tempStrLen, equationPart[i]);
//                    }
//                    i++;//referts to teh for loop i if it seems out of place 
//                }
//
//                value1 = returnEquationResult(tempStr, tempStrLen, numbersInEquation, numbersLen);
//                tempStrLen = 0;
//                end = i;
//                i = currentIndex;
//                //break;
//            }
//            else
//            {
//                end = i + 2;
//
//                char* stringVal = new char[2];
//                stringVal[0] = equationPart[i];
//                stringVal[1] = equationPart[i + 1];
//                value1 = returnEquationResult(stringVal, 2, numbersInEquation, numbersLen);
//
//                //printf("%i \n", stringVal[1] - numberToAddOnForAskiiRefrance);
//            }
//
//            //looking behind of the symbol
//            i--;
//            if (equationPart[i] == ')') {
//                i--;
//                depth--; //as you skip over a a open bracket that needs to be acounted for
//
//                while (depth < 0) {
//                    if (equationPart[i] == '(') {
//                        depth++;
//                    }
//                    else if (equationPart[i] == ')') {
//                        depth--;
//                    }
//
//                    if (depth < 0) {
//                        pushToBack(tempStr, tempStrLen, equationPart[i]);
//                    }
//                    i--;//referts to teh for loop i if it seems out of place 
//                }
//
//                reverse(tempStr, tempStrLen);
//
//                value2 = returnEquationResult(tempStr, tempStrLen, numbersInEquation, numbersLen);
//
//                tempStrLen = 0;
//                start = i;
//                //break;
//            }
//            else
//            {
//                start = i - 2;
//
//                char* stringVal = new char[2];
//                stringVal[0] = equationPart[i - 2];
//                stringVal[1] = equationPart[i - 1];
//
//                value2 = returnEquationResult(stringVal, 2, numbersInEquation, numbersLen);
//
//
//            }
//                        
//            remove(equationPart, equationPartLen, start, end);
//
//            char* temp = new char[2];
//            temp[0] = '~';
//            temp[1] = numbersLen + numberToAddOnForAskiiRefrance;//the value of the askicharicter is the index of nubers to look at
//
//            //printf("%i %g %g | %g %g | %i \n", temp[1] - numberToAddOnForAskiiRefrance, value2, value1, numbersInEquation[4], numbersInEquation[2], &numbersInEquation[4]);//value2, value1, equationPart[1] - numberToAddOnForAskiiRefrance, &numbersInEquation[4], temp[1] - numberToAddOnForAskiiRefrance, 'A');
//
//            if (letter == '*') {
//                value2 = (float)value2 * value1;
//            }
//            else if (letter == '/') {
//                value2 = (float)value2 / value1;
//            }
//
//            i = start;
//
//            pushToBackFloat(numbersInEquation, numbersLen, value2);//push the float val to the back of this 
//            insert(equationPart, equationPartLen, temp, 2, start);
//
//
//            tempStrLen = 0;
//        }
//    }
//
//    #pragma endregion
//
//    #pragma region add sub
//
//    //looking at subtract and add operations
//    depth = 0;
//    tempStr = new char[100];
//    tempStrLen = 0;
//
//    for (int i = 0; i < equationPartLen; i++) {
//        char letter = equationPart[i];
//
//        if (letter == '(') {//seeing if it is in a set of brackets - the depth is howmany sets of brackes you are in for example ( (v) ) teh v is in a depth of 2 as it is 2 layers deep we only want to compute numbers on lowest level (0) as the numbers on loweelevels are what we are looking at teh brakets in highleves are fed in the prgram with recution 
//            depth++;
//        }
//        else if (letter == ')') {//seeing if it is leaveing a set of brackets
//            depth--;
//        }
//
//        if (depth == 0 && (letter == '+' || letter == '-')) {
//
//            int currentIndex = i;
//
//            double value1;//value on right of symbol
//            double value2;//value on left of symbol
//
//            // for removeing the necerry information
//            int end;
//            int start;
//
//            //looking infront of the symbol
//            i++;
//            if (equationPart[i] == '(') {
//                int currentIndex = i;
//                i++;
//                depth++; //as you skip over a a open bracket that needs to be acounted for
//
//                while (depth > 0) {
//                    if (equationPart[i] == '(') {
//                        depth++;
//                    }
//                    else if (equationPart[i] == ')') {
//                        depth--;
//                    }
//
//                    if (depth > 0) {
//                        pushToBack(tempStr, tempStrLen, equationPart[i]);
//                    }
//                    i++;//referts to teh for loop i if it seems out of place 
//                }
//
//                value1 = returnEquationResult(tempStr, tempStrLen, numbersInEquation, numbersLen);
//                tempStrLen = 0;
//                end = i;
//                i = currentIndex;
//                //break;
//            }
//            else
//            {
//                end = i + 2;
//
//                char* stringVal = new char[2];
//                stringVal[0] = equationPart[i];
//                stringVal[1] = equationPart[i + 1];
//                value1 = returnEquationResult(stringVal, 2, numbersInEquation, numbersLen);
//            }
//
//            //looking behind of the symbol
//            i--;
//            if (equationPart[i] == ')') {
//                i--;
//                depth--; //as you skip over a a open bracket that needs to be acounted for
//
//                while (depth < 0) {
//                    if (equationPart[i] == '(') {
//                        depth++;
//                    }
//                    else if (equationPart[i] == ')') {
//                        depth--;
//                    }
//
//                    if (depth < 0) {
//                        pushToBack(tempStr, tempStrLen, equationPart[i]);
//                    }
//                    i--;//referts to teh for loop i if it seems out of place 
//                }
//
//                reverse(tempStr, tempStrLen);
//
//                value2 = returnEquationResult(tempStr, tempStrLen, numbersInEquation, numbersLen);
//
//                tempStrLen = 0;
//                start = i;
//                //break;
//            }
//            else
//            {
//                start = i - 2;
//
//                char* stringVal = new char[2];
//                stringVal[0] = equationPart[i - 2];
//                stringVal[1] = equationPart[i - 1];
//                value2 = returnEquationResult(stringVal, 2, numbersInEquation, numbersLen);
//            }
//
//            remove(equationPart, equationPartLen, start, end);
//
//            
//
//            if (letter == '+') {
//                value2 = (float)value2 + value1;
//            }
//            else if (letter == '-') {
//                value2 = (float)value2 - value1;
//            }
//
//            char* temp = new char[2];
//            temp[0] = '~';
//            temp[1] = numbersLen + numberToAddOnForAskiiRefrance;//the value of the askicharicter is the index of nubers to look at
//
//
//            i = start;
//            pushToBackFloat(numbersInEquation, numbersLen, value2);//push the float val to the back of this 
//            insert(equationPart, equationPartLen, temp, 2, start);
//
//            //#printf("-- %g %i\n", numbersInEquation[4], start);
//
//
//            tempStrLen = 0;
//        }
//    }
//
//    #pragma endregion
//
//    char endLetter = equationPart[0];
//    if (endLetter == '~') {
//        int index = equationPart[1];
//        index -= numberToAddOnForAskiiRefrance;
//
//        return numbersInEquation[index];
//    }
//    else {
//        //printf("failed");
//        return 500;
//    }
//
//
//};
////this should be called to get value of equation not return Eqation Result function as it needs the prper setup
//__device__
//float returnValSetup(char* stringEquation, int& len, float* numbersInEquation, int numbersLen, float x, float y, float z) {//x y z arrays are assumnesd to be lengh of float - 7 significant figures
//
//    float* copyNumberseInEquaiton = new float[numbersLen];
//    int copyNumLen = numbersLen;
//
//    memcpy(copyNumberseInEquaiton, &numbersInEquation[0], numbersLen * sizeof(float));
//
//    char* copyEquaiton = new char[len];
//    int copyEquLen = len;
//
//    memcpy(copyEquaiton, &stringEquation[0], len * sizeof(char));
//
//    copyNumberseInEquaiton[1] = x;//start at 1 as 0 in askii is no letter so you cna never convert it to a letter as there is nothing there 
//    copyNumberseInEquaiton[2] = y;
//    copyNumberseInEquaiton[3] = z;
//
//    return returnEquationResult(copyEquaiton, copyEquLen, copyNumberseInEquaiton, copyNumLen);
//}
//
//#pragma endregion
//
////global functions
//
//__global__
//void test(char* equation, int* len, float* numbersInEquation, int* numbersLen, float* vals, float screenXRotation, float screenYRotation, int screenWidth, int screenHeight, int renderDistance, float zoom, float resalution, int* hitMap)
//{
//    //int totalPoints = screenWidth * screenHeight * (renderDistance / resalution);
//    int gloabalThreadNumber = (blockIdx.x * blockDim.x) + threadIdx.x;
//
//    if (gloabalThreadNumber >= screenHeight * screenWidth) { return; }//this is to stop wrong parts of memory being overwritten. e.g. lests say you have 2 bloacks of 10 but only porsesing 15 elemets then there are 5 floats that are going to be written but these floats 
//
//    //NEEDS TO BE CHANGED TO BE HOW FAR ALONG Z AXIS AND THEN GIVIEN ITS ACTUAL VALUE <------------- TO DO
//    //FIND SOLUTION WHERE THESE VALUES ARE ACTUALY CALCULATED SO THAT IT DOESNT HAVE TO BE JSUT A SQUARE
//    float xWidth = renderDistance,
//          yWidth = renderDistance,
//
//    int totalZpoints = renderDistance / resalution;
//
//    int screenX = gloabalThreadNumber % screenWidth,
//        screenY = (gloabalThreadNumber % (screenHeight * screenHeight)) / screenWidth;
//
//     float z = (((gloabalThreadNumber / (screenWidth * screenHeight)) / (float)totalZpoints) * (renderDistance)) - ((renderDistance) / (float)2), // <----------- NOT WORKING ALLWAYS GIVE NEGATIVE A HALF
//           x = (((gloabalThreadNumber % screenWidth) / (float)screenWidth) * (zoom)) - ((zoom) / (float)2),
//           y = ((((gloabalThreadNumber % (screenHeight * screenHeight)) / screenWidth) / (float)screenHeight) * (zoom)) - ((zoom) / (float)2);
//
//    float xT = x * cos(screenXRotation) - z * sin(screenXRotation); //xT means it is the x location that has been transformed inot the corect postion
//    float zT = x * sin(screenXRotation) + z * cos(screenXRotation);
//
//    //ROTAION FOR UP AND DOWN SHALL GO HERE <------ TO DO
//    float yT = y;
//
//    x = xT; //these are so i do not need to create a new varbiles they just hold the lcoation of where the ray starts now.
//    y = yT;
//    z = zT;
//
//    float val = returnValSetup(equation, *len, numbersInEquation, *numbersLen, xT, yT, zT);
//
//
//    //THIS NEEDS TO BE CHANGED TO ONLY RUN ONCE, IF COLITION FOUND THAN CORRECT PLACE IN ARRAY IS INCREMETED BY ONE <------- TO DO
//
//
//    if (val >= -resalution/(float)2 && val <= resalution/(float)2) { //this checks for a colition, if a value is is closengough to zero then it is a coltion, this is determined by checking to see if the abs of the value is withing half of the resalution as that is the distance between points
//       hitMap[gloabalThreadNumber % (screenWidth * screenHeight)]++; //is incremented as i would like to knwo homany hits i get 
//    }
//}
//
//int setUpKernal(Equation equ, int _screenWidth, int _screenHeight, int _renderDistance, float _resalution, SDL_Surface* _surface) {
//    screenWidth = _screenWidth;
//    screenHeight = _screenHeight;
//    surf = _surface;
//    resalution = _resalution;
//    renderDistance = _renderDistance;
//
//    int amountOfImagePixels = screenHeight * screenWidth;
//
//
//    //VALUES NEED TO BE CHANGED TO THE CORRECT AMOUNT OF DATA ALOCATED 
//    cudaDeviceSetLimit(cudaLimitMallocHeapSize, (int)(0.5 * 125000000));
//    //cudaDeviceGetLimit(&size, cudaLimitMallocHeapSize);
//
//
//    cudaDeviceSetLimit(cudaLimitStackSize, 1025);
//    //cudaDeviceGetLimit(&size, cudaLimitStackSize);
//
//
//    //#pragma region alocating memoery for eqautions
//
//    cudaMalloc(&GPUequations, defaultSize * sizeof(char));
//    cudaMalloc(&equaitonLens, defaultSize * sizeof(int));
//
//    cudaMalloc(&valusesInEquation, defaultSize * sizeof(float));
//    cudaMalloc(&valusesInEquationLens, defaultSize * sizeof(int));
//
//    cudaMalloc(&vals, defaultSize * sizeof(float));
//
//    cudaMalloc(&hits, amountOfImagePixels * sizeof(int));
//
//
//    valusesInEquationLens[0] = equ.equationVariblesLen;
//    equaitonLens[0] = equ.equationLen;
//
//    cudaMemcpy(GPUequations, &equ.equation[0], defaultSize * sizeof(char), cudaMemcpyHostToDevice);
//    cudaMemcpy(valusesInEquation, &equ.equationVaribles[0], defaultSize * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemset(hits, 0, amountOfImagePixels);
//
//    //this calculates the tola points being checked, the rendDist / res is to find how many points to be checked on z axis
//    totalPoints = amountOfImagePixels * (renderDistance / resalution);
//    blocks = totalPoints / threadsBeingUsed;
//
//    return 0;
//
//}
//
//int freeEquaitonMemory() {
//    cudaFree(GPUequations);
//    cudaFree(equaitonLens);
//
//    cudaFree(vals);
//    cudaFree(hits);
//
//    cudaFree(valusesInEquationLens);
//    cudaFree(valusesInEquation);
//
//    return 0;
//}
//
//int renderKernal(float xRoatation, float yRoatation, float zoom)
//{
//
//    //clock_t start, finish;
//    //double totaltime;
//    //start = clock();
//
//    //////the threads being used now needs to include treads on the Z axis <----------------------- DONE I THINK
//
//    ////test <<<blocks, threadsBeingUsed>>>(GPUequations, equaitonLens, valusesInEquation, valusesInEquationLens, vals, xRoatation, yRoatation, screenWidth, screenHeight, renderDistance, zoom, resalution, hits);
//
//    //cudaDeviceSynchronize();
//
//    //int* hitsCopy;
//    //cudaMemcpy(hits, hitsCopy, screenWidth * screenHeight * sizeof(int), cudaMemcpyHostToDevice);
//
//    ////////NEEDS TO BE CHAGED INSTEAD OF VALUE BEING CEHCKED FOR TOLANCE NEEDS TO BE CHECKED TOO SEE IF ITS GREATER THAN 0 <--------------------------- TO DO
//    //for (int y = 0; y < screenHeight; y++) {
//    //    for (int x = 0; x < screenWidth; x++) {
//    //        int val = hitsCopy[(y * screenWidth) + x];
//    //        if (val > 0) {
//    //            drawPixel(surf, x, y, 255, 0, 0);
//    //        }
//    //        //else
//    //        //{
//    //        //    drawPixel(surface, x, y, 255, 255, 255);
//    //        //}
//    //    }
//    //}
//
//
//
//    //finish = clock();
//    //totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
//    //printf("%f", totaltime);
//
//    return 0;
//}
