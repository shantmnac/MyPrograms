 #include "listText.hpp"
#include "otherFunctions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>

extern char markedListSymbol;
extern unsigned int listIndent, screenWidth;

bool previousElmentWasNumer = false, emptyListElement = false;
unsigned int numListCounter = 1, **indentsLevel = NULL, sizeOfIndentsArray = 0;
extern char *tmpStrGlobal;

ListText::ListText(){
    list = NULL;
    counter = 0;
}

ListText::~ListText(){
    if (list != NULL) {
        for (unsigned int q = 0; q < counter; q++) free(list[q].text);
        free(list);
    }
}

void ListText::push(char *txt, unsigned int indentTmp, bool type){
    if (txt == NULL) {
        list = (ListElement*)realloc(list, (counter + 1) * sizeof(ListElement));
        list[counter].text = (char*)malloc(2);
        list[counter].text[0] = '\n';
        list[counter].text[1] = '\0';
    }
    else{
        list = (ListElement*)realloc(list, (counter + 1) * sizeof(ListElement));
        list[counter].text = (char*)malloc(strlen(txt) + 1);
        strcpy(list[counter].text, txt);
    }
    list[counter].indent = indentTmp;
    list[counter].listType = type;
    counter++;
}

unsigned long int ListText::countOfSymbols()const{
    unsigned int tmpCounter = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (list[i].text[j] != '\0') {
            if ((list[i].text[j] == '\xd0') || (list[i].text[j] == '\xd1')) {
                tmpCounter++;
                j += 2;
            }
            else{
                tmpCounter++;
                j++;
            }
        }
    }
    
    return tmpCounter;
}

unsigned long int ListText::countOfWords()const{
    unsigned int tmpCounter = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (list[i].text[j] != '\0') {
            while ((list[i].text[j] != '\n') && (list[i].text[j] != ' ') && (list[i].text[j] != '\0')) j++;
            tmpCounter++;
            if (list[i].text[j] == '\0') break;
            else j++;
        }
    }
    
    return tmpCounter;
}

void ListText::print(FILE *output)const{
    for (unsigned int q = 0; q < counter; q++) {
        if (list[q].indent >= screenWidth) continue;
        prepare(list[q]);
        if (!emptyListElement) fprintf(output, "%s\n", tmpStrGlobal);
        else fprintf(output, "%s", tmpStrGlobal);
        free(tmpStrGlobal);
        tmpStrGlobal = NULL;
    }
    for (unsigned int q = 0; q < sizeOfIndentsArray; q++) free(indentsLevel[q]);
    if (indentsLevel != NULL) {
        free(indentsLevel);
        indentsLevel = NULL;
    }
    sizeOfIndentsArray = 0;
    fprintf(output, "\n");
}

void ListText::read(FILE *input){
    bool betweenWords = false, previousSymbolWasBackSlashN = false, paragraphIsEmpty = true, numericalStr = true, strBegins = false, firstProb = true;
    unsigned int i = 0, strIndent = 0;
    char tmpChar = '\0', *tmpWord = NULL;
    
    do{
        if (!strBegins) {
            do{
                tmpChar = getc(input);
                switch (tmpChar) {
                    case '*':{
                        numericalStr = false;
                        strBegins = true;
                        break;
                    }
                    
                    case ' ':{
                        strIndent++;
                        break;
                    }
                        
                    case EOF:{
                        if (paragraphIsEmpty) {
                            ungetc(tmpChar, input);
                            throw EmptyParagraphERROR();
                        }
                        else{
                            ungetc(tmpChar, input);
                            return;
                        }
                    }
                        
                    case '\n':{
                        ungetc(tmpChar, input);
                        strBegins = true;
                        break;
                    }
                        
                    default:{
                        i = 0;
                        ungetc(tmpChar, input);
                        do{
                            if (isdigit(tmpChar)) {
                                tmpChar = getc(input);
                                tmpWord = (char*)realloc(tmpWord, i + 1);
                                tmpWord[i] = tmpChar;
                                i++;
                            }
                            else {
                                if (tmpChar != '.') {
                                    for (unsigned int q = 0; q < i; q++) {
                                        ungetc(tmpWord[i - q - 1], input);
                                    }
                                    if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                    numericalStr = true;
                                    strBegins = true;
                                }
                                else{
                                    if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                    numericalStr = true;
                                    strBegins = true;
                                }
                            }
                        }while(!strBegins);
                        break;
                    }
                }
                i = 0;
            }while(!strBegins);
        }
        
        tmpChar = getc(input);
        
        if (firstProb) {
            if (tmpChar == ' ') tmpChar = getc(input);
            firstProb = false;
        }
        
        switch (tmpChar) {
            case ' ':{
                previousSymbolWasBackSlashN = false;
                paragraphIsEmpty = false;
                if (!betweenWords) {
                    betweenWords = true;
                    tmpWord = (char*)realloc(tmpWord, i + 1);
                    tmpWord[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:{
                if (paragraphIsEmpty) {
                    ungetc(tmpChar, input);
                    throw EmptyParagraphERROR();
                }
                if (tmpWord != NULL) {
                    tmpWord = (char*)realloc(tmpWord, i + 1);
                    tmpWord[i] = '\0';
                    push(tmpWord, strIndent, numericalStr);
                    if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                }
                ungetc(tmpChar, input);
                return;
            }
                
            case '\n':{
                paragraphIsEmpty = false;
                previousSymbolWasBackSlashN = true;
                if (tmpWord != NULL) {
                    tmpWord = (char*)realloc(tmpWord, i + 1);
                    tmpWord[i] = '\0';
                    push(tmpWord, strIndent, numericalStr);
                    if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                }
                else push(NULL, strIndent, 1);
                break;
            }
                
            default:{
                paragraphIsEmpty = false;
                previousSymbolWasBackSlashN = false;
                betweenWords = false;
                tmpWord = (char*)realloc(tmpWord, i + 1);
                tmpWord[i] = tmpChar;
                i++;
                break;
            }
        }
        
        if (previousSymbolWasBackSlashN) {
            tmpChar = getc(input);
            if (tmpChar == EOF) {
                ungetc(tmpChar, input);
                return;
            }
            if (tmpChar == '\n') {
                return;
            }
            ungetc(tmpChar, input);
            strIndent = 0;
            strBegins = false;
            numericalStr = true;
            firstProb = true;
            i = 0;
        }
    }while(1);
}

void ListText::prints(FILE *out)const{
    fprintf(out, "^^list^^ ");
    
    unsigned int i = 0, counterList = 1;
    bool previousIsNum = false;
    
    for (unsigned int j = 0; j < counter; j++) {
        for (unsigned int q = 0; q < list[j].indent; q++) fprintf(out, " ");
        
        if (list[j].listType) {
            if (previousIsNum) {
                if (list[j].indent == list[j - 1].indent) {
                    counterList ++;
                }
                else counterList = 1;
            }
            previousIsNum = true;
            fprintf(out, "%i. ", counterList);
        }
        else {
            fprintf(out, "*");
            previousIsNum = false;
            counterList = 1;
        }
        
        i = 0;
        while (list[j].text[i] != '\0') {
            fprintf(out, "%c", list[j].text[i]);
            i++;
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void ListText::prepare(ListElement element)const{
    char *text = element.text, *num = NULL;
    unsigned int positionInTmpStr = 0, symbolsInCurrentStr = 0, counterOfPrintedSymbolsInTheWordInStr = 0, currentWordLength = 0, i = 0, extraIndent = 0;
    bool continuationOfWord = false;
    
    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
    symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
    
    if (element.listType) {
        if ((element.indent / 2) + 1 > sizeOfIndentsArray) {
            indentsLevel = (unsigned int**)realloc(indentsLevel, ((element.indent / 2) + 1) * sizeof(unsigned int*));
            if ((element.indent / 2) + 1 - sizeOfIndentsArray > 1) {
                for (unsigned int q = sizeOfIndentsArray; q < (element.indent / 2); q++) {
                    indentsLevel[q] = (unsigned int*)malloc(2 * sizeof(unsigned int));
                    indentsLevel[q][0] = q;
                    indentsLevel[q][1] = 0;
                }
            }
            indentsLevel[(element.indent / 2)] = (unsigned int*)malloc(2 * sizeof(unsigned int));
            indentsLevel[(element.indent / 2)][0] = (element.indent / 2);
            indentsLevel[(element.indent / 2)][1] = 1;
            sizeOfIndentsArray = (element.indent / 2) + 1;
        }
        else{
            for (unsigned int q = (element.indent / 2) + 1; q < sizeOfIndentsArray; q++) free(indentsLevel[q]);
            indentsLevel = (unsigned int**)realloc(indentsLevel, ((element.indent / 2) + 1) * sizeof(unsigned int*));
            indentsLevel[(element.indent / 2)][1]++;
            sizeOfIndentsArray = (element.indent / 2) + 1;
        }
        
        numListCounter = indentsLevel[(element.indent / 2)][1];
        num = giveNum();
        extraIndent = 2 + (unsigned int)strlen(num);
        for (unsigned int i = 0; i < strlen(num); i++) {
            if (symbolsInCurrentStr < screenWidth) {
                tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                tmpStrGlobal[positionInTmpStr] = num[i];
                positionInTmpStr++;
                symbolsInCurrentStr++;
            }
            else{
                tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                tmpStrGlobal[positionInTmpStr] = '\n';
                positionInTmpStr++;
                positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
                tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                tmpStrGlobal[positionInTmpStr] = num[i];
                positionInTmpStr++;
                symbolsInCurrentStr = 1;
            }
        }
        if (symbolsInCurrentStr < screenWidth) {
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        else{
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = '\n';
            positionInTmpStr++;
            positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
            symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        previousElmentWasNumer = true;
        free(num);
        num = NULL;
    }
    else{
        extraIndent = 2;
        for (unsigned int q = 0; q < sizeOfIndentsArray; q++) free(indentsLevel[q]);
        if (indentsLevel != NULL){
            free(indentsLevel);
            indentsLevel = NULL;
        }
        sizeOfIndentsArray = 0;
        
        if (symbolsInCurrentStr < screenWidth) {
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = markedListSymbol;
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        else{
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = '\n';
            positionInTmpStr++;
            positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
            symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = markedListSymbol;
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        if (symbolsInCurrentStr < screenWidth) {
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        else{
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = '\n';
            positionInTmpStr++;
            positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
            symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        previousElmentWasNumer = false;
    }
    
    
    do{
        if (continuationOfWord || (i == 0) || ((text[i] != ' ') && (text[i - 1] == ' '))){
            currentWordLength = wordLength(text, i);
            if (currentWordLength + symbolsInCurrentStr <= screenWidth) {
                symbolsInCurrentStr += currentWordLength;
                for (unsigned int q = 0; q < currentWordLength; q++) {
                    if ((text[i] == '\xd0') || (text[i] == '\xd1')){
                        tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                        tmpStrGlobal[positionInTmpStr] = text[i];
                        tmpStrGlobal[positionInTmpStr + 1] = text[i + 1];
                        positionInTmpStr += 2;
                        i += 2;
                    }
                    else{
                        tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                        tmpStrGlobal[positionInTmpStr] = text[i];
                        positionInTmpStr++;
                        i++;
                    }
                }
                continuationOfWord = false;
                continue;
            }
        }
        
        if ((text[i] == '\xd0') || (text[i] == '\xd1')){
            if (symbolsInCurrentStr < screenWidth - 1) {
                if ((counterOfPrintedSymbolsInTheWordInStr == 0) && (screenWidth - symbolsInCurrentStr == 2)) {
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = '\n';
                    positionInTmpStr++;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    tmpStrGlobal[positionInTmpStr + 1] = text[i + 1];
                    positionInTmpStr += 2;
                    i += 2;
                    counterOfPrintedSymbolsInTheWordInStr = 1;
                    symbolsInCurrentStr++;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    tmpStrGlobal[positionInTmpStr + 1] = text[i + 1];
                    positionInTmpStr += 2;
                    i += 2;
                    symbolsInCurrentStr++;
                    counterOfPrintedSymbolsInTheWordInStr++;
                }
            }
            else{
                if (counterOfPrintedSymbolsInTheWordInStr == 0) {
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = '\n';
                    positionInTmpStr++;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    tmpStrGlobal[positionInTmpStr + 1] = text[i + 1];
                    positionInTmpStr += 2;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = '-';
                    tmpStrGlobal[positionInTmpStr + 1] = '\n';
                    positionInTmpStr += 2;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    tmpStrGlobal[positionInTmpStr + 1] = text[i + 1];
                    positionInTmpStr += 2;
                }
                counterOfPrintedSymbolsInTheWordInStr = 1;
                i += 2;
                symbolsInCurrentStr = (((element.indent / 2) * listIndent) % screenWidth) + 1;
                continuationOfWord = true;
            }
        }
        else{
            if (text[i] == ' ') {
                counterOfPrintedSymbolsInTheWordInStr = 0;
                
                if (symbolsInCurrentStr < screenWidth) {
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = ' ';
                    positionInTmpStr++;
                    i++;
                    symbolsInCurrentStr++;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = '\n';
                    positionInTmpStr++;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = ' ';
                    positionInTmpStr++;
                    i++;
                    symbolsInCurrentStr++;
                }
                continue;
            }
            
            if (symbolsInCurrentStr < screenWidth - 1) {
                if ((counterOfPrintedSymbolsInTheWordInStr == 0) && (screenWidth - symbolsInCurrentStr == 2)) {
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = '\n';
                    positionInTmpStr++;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    symbolsInCurrentStr = ((element.indent / 2) * listIndent) % screenWidth;
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                    i++;
                    counterOfPrintedSymbolsInTheWordInStr = 1;
                    symbolsInCurrentStr++;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                    i++;
                    symbolsInCurrentStr++;
                    counterOfPrintedSymbolsInTheWordInStr++;
                }
            }
            else{
                if (counterOfPrintedSymbolsInTheWordInStr == 0) {
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = '\n';
                    positionInTmpStr++;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = '-';
                    tmpStrGlobal[positionInTmpStr + 1] = '\n';
                    positionInTmpStr += 2;
                    positionInTmpStr = insertIndent(positionInTmpStr, (element.indent / 2) * listIndent + extraIndent);
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                }
                counterOfPrintedSymbolsInTheWordInStr = 1;
                i++;
                symbolsInCurrentStr = (((element.indent / 2) * listIndent) % screenWidth) + 1;
                continuationOfWord = true;
            }
        }
    }while (i < strlen(text));
    
    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
    tmpStrGlobal[positionInTmpStr] = '\0';
    return;
}

unsigned int ListText::insertIndent(unsigned int positionInTmpStr, unsigned int indent)const{
    unsigned int i = 0;
    
    if (indent <= screenWidth) {
        tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + indent);
        for (unsigned int q = 0; q < indent; q++) {
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
        }
        return positionInTmpStr ;
    }
    else {
        while (indent != 0) {
            if (i == screenWidth) {
                tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                tmpStrGlobal[positionInTmpStr] = '\n';
                positionInTmpStr++;
                i = 0;
            }
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
            indent--;
            i++;
        }
    }
    
    return positionInTmpStr;
}

char* ListText::giveNum()const{
    char *integer = NULL;
    
    unsigned int tempInt = numListCounter;
    int ost = 0;
    int i = 0;
    
    while (tempInt > 0) {
        ost = tempInt % 10;
        tempInt /= 10;
        integer = (char*)realloc(integer, (i + 1) * sizeof(char));
        integer[i] = convertIntToChar(ost);
        i++;
    }
    integer = (char*)realloc(integer, (i + 2) * sizeof(char));
    integer[i] = '.';
    integer[i + 1] = '\0';
    
    return integer;
}

char ListText::convertIntToChar(int integer)const{return integer + '0';}