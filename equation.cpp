#include <iostream>
#include <string>
#include <regex>

using namespace std;

#include "equation.h"
#include "arrFunctions.h"

const int operationsLen = 8;
const int oneInputOperationsLen = 3;
const int towInputOperationsLen = 4;

const int numberToAddOnForAskiiRefrance = 94; // this will need to cahnge eventually

char operations[operationsLen] = { '*', '/', '+', '-', 's', 'c', 't', '(' };

char oneInputOperations[oneInputOperationsLen] = { 's', 'c', 't' };

char towInputOperations[towInputOperationsLen] = { '*', '/', '+', '-' };
//device funtions

/*
void validateEquation(Equation& equation) {

    removeAllChar(equation.equation, equation.equationLen, new char[1] { ' ' }, 1); //removeing balck space
    replaceString(equation.equation, equation.equationLen, new char[3] { 's', 'i', 'n' }, 3, new char[1] { 'S' }, 1);
    replaceString(equation.equation, equation.equationLen, new char[3] { 'c', 'o', 's' }, 3, new char[1] { 'C' }, 1);
    replaceString(equation.equation, equation.equationLen, new char[3] { 't', 'a', 'n' }, 3, new char[1] { 'T' }, 1);

    bool lastElementWasAnOperation = false;
    for (int i = 0; i < equation.equationLen - 1; i++) {
        bool addTimesSymbol = true;
        char letter = equation.equation[i];
        char NextLetter = equation.equation[i + 1];

        for (int n = 0; n < sizeof(operations) / sizeof(char); n++)//checking too see if letter is an operation
        {
            if (operations[n] == letter || operations[n] == NextLetter) {
                addTimesSymbol = false;
                break;
            }
        }

        if ('(' == letter || ')' == NextLetter) {
            addTimesSymbol = false;
        }

        if (((int)letter >= 48 && (int)letter <= 57) || letter == '.') {
            if (((int)NextLetter >= 48 && (int)NextLetter <= 57) || NextLetter == '.') {
                addTimesSymbol = false;
            }
        }

        if (addTimesSymbol) {
            insertCharrArr(equation.equation, equation.equationLen, new char[1] { '*' }, 1, i + 1);
            i++;
        }
    }

    char* tempArr1 = new char[1] {'x'};
    char* tempArr2 = new char[2] {'~', (int8_t)(1 + numberToAddOnForAskiiRefrance)};//starts at one as 0 is no charicter
    replaceString(equation.equation, equation.equationLen, tempArr1, 1, tempArr2, 2);

    tempArr1 = new char[1] {'y'};
    tempArr2 = new char[2] {'~', (int8_t)(2 + numberToAddOnForAskiiRefrance)};
    replaceString(equation.equation, equation.equationLen, tempArr1, 1, tempArr2, 2);

    tempArr1 = new char[1] {'z'};
    tempArr2 = new char[2] {'~', (int8_t)(3 + numberToAddOnForAskiiRefrance)};
    replaceString(equation.equation, equation.equationLen, tempArr1, 1, tempArr2, 2);

    for (int i = 0; i < equation.equationLen; i++) {
        if (((int)equation.equation[i] >= 48 && (int)equation.equation[i] <= 57) || equation.equation[i] == '.') {//looking too see if it is number if it is then finding the whole number 
            std::string tempString = "";;//makeing temperary string to hold the number
            int numLen = 0; //helds the numbre of charicters in nuber

            bool isEnd = false;
            while (!isEnd) {//runs unitll the end of the tring is found

                tempString.push_back(equation.equation[i + numLen]);

                if (i + numLen == equation.equationLen) {
                    break;
                }

                isEnd = !((int)equation.equation[i + numLen + 1] >= 48 && (int)equation.equation[i + numLen + 1] <= 57 || (int)equation.equation[i + numLen + 1] == '.'); //this is a boll that checks if it is the end of a number

                for (int n = 0; n < sizeof(operations) / sizeof(char); n++)//checking too see if letter is an operation
                {
                    if (operations[n] == equation.equation[i + numLen + 1]) {
                        isEnd = true;
                        break;
                    }
                }

                numLen++;
            }

            float num = std::stof(tempString);

            char askiiVal = equation.equationVariblesLen + numberToAddOnForAskiiRefrance; // as len will be the index of the back of the array +1 and the plus is needed as it is added to back
            char* numRefrance = new char[2] {'~', askiiVal};//holds the refrance to teh numbre in the float array 

            pushBackFloat(equation.equationVaribles, equation.equationVariblesLen, num);
            removeString(equation.equation, equation.equationLen, i, i + numLen);
            insertCharrArr(equation.equation, equation.equationLen, numRefrance, 2, i);
            i += 2;
        }
    }

}
*/

double privateReturnVal(string equationPart) {

    if (equationPart[0] == '(') {
        int depth = 1;
        int i = 1;
        std::string tempStr = "";

        while (depth > 0) {
            if (equationPart[i] == '(') {
                depth++;
            }
            else if (equationPart[i] == ')') {
                depth--;
            }

            if (depth > 0) {
                tempStr.push_back(equationPart[i]);
            }
            i++;//referts to teh for loop i if it seems out of place 
        }

        double val = privateReturnVal(tempStr);

        equationPart.erase(equationPart.begin(), equationPart.begin() + i);
        equationPart.insert(0, to_string(val));
    }

    //looking at one input equations e.g. sin cos tan ect
    int depth = 0;
    string tempStr = "";
    for (int i = 0; i < equationPart.size(); i++) {
        char letter = equationPart[i];
        if (letter == '(') {
            depth++;
        }
        else if (letter == ')') {
            depth--;
        }
        else {
            if ((depth == 0)) {
                int currentIndex = i;
                for (int n = 0; n < sizeof(oneInputOperations) / sizeof(char); n++) {//divided by size of char as it is not the amount of elemets but the size of all the data (types) of elemets so dividing it by the data type makes it the right size
                    //cout << oneInputOperations[n] << " " << n << endl;
                    if (letter == oneInputOperations[n]) {
                        //cout << '\n' << letter << " " << oneInputOperations[n] << " " << n << endl;
                        i += 2;
                        depth++; //as you skip over a a open bracket that needs to be acounted for

                        while (depth > 0) {
                            if (equationPart[i] == '(') {
                                depth++;
                            }
                            else if (equationPart[i] == ')') {
                                depth--;
                            }

                            if (depth > 0) {
                                tempStr.push_back(equationPart[i]);
                            }
                            i++;//referts to teh for loop i if it seems out of place 
                        }
                        //int sub = currentIndex - tempStr.size() - 1;
                        equationPart.erase(equationPart.begin() + currentIndex, equationPart.begin() + i);

                        double val;

                        switch (letter) {
                        case 's':
                            val = sin(privateReturnVal(tempStr));
                            break;
                        case 'c':
                            val = cos(privateReturnVal(tempStr));
                            break;
                        case 't':
                            val = tan(privateReturnVal(tempStr));
                            break;
                        }


                        equationPart.insert(currentIndex, to_string(val));
                        tempStr = "";
                        i = currentIndex + 8; //8 is len of float turned to string
                        break;
                    }
                }
            }
        }

        //looking for times and multiplication

    }

    //looking at times and divide operations
    depth = 0;
    tempStr = "";
    for (int i = 0; i < equationPart.size(); i++) {
        char letter = equationPart[i];
        if (letter == '(') {
            depth++;
        }
        else if (letter == ')') {
            depth--;
        }

        if (depth == 0 && (letter == '*' || letter == '/')) {

            //value infront of the symbol
            double value1;//value on right of symbol
            double value2;//value on left of symbol

            // for removeing the necerry information
            int end; // needs to be subtracted from str.end()
            int start; // needs to be added to str.begin()

            //looking infront of the symbol
            i++;
            if (equationPart[i] == '(') {
                int currentIndex = i;
                i++;
                depth++; //as you skip over a a open bracket that needs to be acounted for

                while (depth > 0) {
                    if (equationPart[i] == '(') {
                        depth++;
                    }
                    else if (equationPart[i] == ')') {
                        depth--;
                    }

                    if (depth > 0) {
                        tempStr.push_back(equationPart[i]);
                    }
                    i++;//referts to teh for loop i if it seems out of place 
                }

                value1 = privateReturnVal(tempStr);
                tempStr = "";

                end = i;
                i = currentIndex;
                //break;
            }
            else
            {
                bool endOfMinus = (equationPart[i] != '-');
                int curretnIndex = i;
                while (((int)equationPart[i] >= 48 && (int)equationPart[i] <= 57) || (equationPart[i] == '.') || (equationPart[i] == '-'))
                {

                    if (equationPart[i] == '-') {
                        if (endOfMinus)
                            break;
                        if (((int)equationPart[i + 1] >= 48 && (int)equationPart[i + 1] <= 57)) {
                            endOfMinus = true;
                        }
                    }

                    tempStr.push_back(equationPart[i]);
                    i++;

                }

                if (tempStr.size() > 2) {
                    if (tempStr[0] == '-' && tempStr[1] == '-') {
                        tempStr.erase(tempStr.begin(), tempStr.begin() + 2);
                    }
                }

                end = i;
                i = curretnIndex;
                value1 = stod(tempStr);
                tempStr = "";
            }

            //looking behind of the symbol
            i--;
            if (equationPart[i] == ')') {
                i--;
                depth--; //as you skip over a a open bracket that needs to be acounted for

                while (depth < 0) {
                    if (equationPart[i] == '(') {
                        depth++;
                    }
                    else if (equationPart[i] == ')') {
                        depth--;
                    }

                    if (depth < 0) {
                        tempStr.push_back(equationPart[i]);
                    }
                    i--;//referts to teh for loop i if it seems out of place 
                }

                reverse(tempStr.begin(), tempStr.end());

                value2 = privateReturnVal(tempStr);
                tempStr = "";

                start = i;
                //break;
            }
            else
            {
                i--;
                bool startOn0 = (i == 0);

                while (((int)equationPart[i] >= 48 && (int)equationPart[i] <= 57) || (equationPart[i] == '.'))
                {
                    tempStr.push_back(equationPart[i]);
                    i--;

                    if (i < 1) {
                        i = 0;
                        if (!startOn0) {
                            tempStr.push_back(equationPart[i]);
                        }
                        i--;
                        break;
                    }
                }

                i++;

                start = i;

                reverse(tempStr.begin(), tempStr.end());

                if (tempStr.size() > 2) {
                    if (tempStr[0] == '-' && tempStr[1] == '-') {
                        tempStr.erase(tempStr.begin(), tempStr.begin() + 2);
                    }
                }

                value2 = stod(tempStr);
                tempStr = "";
            }

            if (letter == '*') {
                value2 = value2 * value1;
            }
            else {
                if (value1 != 0) {
                    value2 = value2 / value1;
                }
                else
                {
                    value2 = 0.f;
                }
            }
            string temp = equationPart;
            equationPart.erase(equationPart.begin() + start, equationPart.begin() + end);
            equationPart.insert(start, to_string(value2));

        }

    }

    //looking at add and subtract operations
    depth = 0;
    tempStr = "";
    for (int i = 0; i < equationPart.size(); i++) {
        char letter = equationPart[i];
        if (letter == '(') {
            depth++;
        }
        else if (letter == ')') {
            depth--;
        }

        if (depth == 0 && (letter == '+' || letter == '-') && i > 0) {

            //value infront of the symbol
            double value1;//value on right of symbol
            double value2;//value on left of symbol

            // for removeing the necerry information
            int end; // needs to be subtracted from str.end()
            int start; // needs to be added to str.begin()

            //looking behind of the symbol
            i++;
            if (equationPart[i] == '(') {
                int currentIndex = i;
                i++;
                depth++; //as you skip over a a open bracket that needs to be acounted for

                while (depth > 0) {
                    if (equationPart[i] == '(') {
                        depth++;
                    }
                    else if (equationPart[i] == ')') {
                        depth--;
                    }

                    if (depth > 0) {
                        tempStr.push_back(equationPart[i]);
                    }
                    i++;//referts to teh for loop i if it seems out of place 
                }

                value1 = privateReturnVal(tempStr);
                tempStr = "";

                end = i;
                i = currentIndex;
                //break;
            }
            else
            {
                bool endOfMinus = (equationPart[i] != '-');
                int curretnIndex = i;
                while (((int)equationPart[i] >= 48 && (int)equationPart[i] <= 57) || (equationPart[i] == '.') || (equationPart[i] == '-'))
                {

                    if (equationPart[i] == '-') {
                        if (endOfMinus)
                            break;
                        if (((int)equationPart[i + 1] >= 48 && (int)equationPart[i + 1] <= 57)) {
                            endOfMinus = true;
                        }
                    }

                    tempStr.push_back(equationPart[i]);
                    i++;

                }

                if (tempStr.size() > 2) {
                    if (tempStr[0] == '-' && tempStr[1] == '-') {
                        tempStr.erase(tempStr.begin(), tempStr.begin() + 2);
                    }
                }

                end = i;
                i = curretnIndex;
                value1 = stod(tempStr);
                tempStr = "";
            }

            //looking infront of the symbol
            i--;
            if (equationPart[i] == ')') {
                i--;
                depth--; //as you skip over a a open bracket that needs to be acounted for

                while (depth < 0) {
                    if (equationPart[i] == '(') {
                        depth++;
                    }
                    else if (equationPart[i] == ')') {
                        depth--;
                    }

                    if (depth < 0) {
                        tempStr.push_back(equationPart[i]);
                    }
                    i--;//referts to teh for loop i if it seems out of place 
                }

                reverse(tempStr.begin(), tempStr.end());

                value2 = privateReturnVal(tempStr);
                tempStr = "";

                start = i;
                //break;
            }
            else
            {
                start = 0;
                i--;
                if (i < 0) {
                    value2 = 0;
                }
                else
                {
                    while (((int)equationPart[i] >= 48 && (int)equationPart[i] <= 57) || (equationPart[i] == '.') || (equationPart[i] == '-'))
                    {
                        tempStr.push_back(equationPart[i]);
                        i--;

                        if (i < 1) {
                            i = 0;
                            tempStr.push_back(equationPart[i]);
                            i--;
                            break;
                        }
                    }

                    i++;

                    start = i;

                    reverse(tempStr.begin(), tempStr.end());


                    if (tempStr.size() > 2) {
                        if (tempStr[0] == '-' && tempStr[1] == '-') {
                            tempStr.erase(tempStr.begin(), tempStr.begin() + 2);
                        }
                    }

                    value2 = stod(tempStr);
                    tempStr = "";
                }
            }

            if (letter == '+') {
                value2 = value2 + value1;
            }
            else {
                value2 = value2 - value1;
            }

            equationPart.erase(equationPart.begin() + start, equationPart.begin() + end);
            equationPart.insert(start, to_string(value2));
            //i += 8;
        }
    }


    return stod(equationPart);
};

float returnEquiationValue(std::string stringEquation, vector3D vector) {

    stringEquation = regex_replace(stringEquation, regex("X"), to_string(vector.x));
    stringEquation = regex_replace(stringEquation, regex("Y"), to_string(vector.y));
    stringEquation = regex_replace(stringEquation, regex("Z"), to_string(vector.z));

    double temp = privateReturnVal(stringEquation);

    return (float)temp;
}

#pragma endregion