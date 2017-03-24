#include "regularText.hpp"
#include "otherFunctions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>

extern unsigned int screenWidth, redLineSize;

RegularText::RegularText(){
    text = NULL;
}

RegularText::~RegularText(){
    if (text != NULL) free(text);
}

unsigned long int RegularText::countOfSymbols()const{
    unsigned int tmpCounter = 0, j = 0;
    
    while (text[j] != '\0') {
        if ((text[j] == '\xd0') || (text[j] == '\xd1')) {
            tmpCounter++;
            j += 2;
        }
        else{
            tmpCounter++;
            j++;
        }
    }
    
    return tmpCounter;
}

unsigned long int RegularText::countOfWords()const{
    unsigned int tmpCounter = 0, j = 0;
    
    while (text[j] != '\0') {
        while ((text[j] != '\n') && (text[j] != ' ') && (text[j] != '\0')) j++;
        tmpCounter++;
        if (text[j] == '\0') break;
        else j++;
    }
    
    return tmpCounter;
}

void RegularText::print(FILE *output)const{
    char *tmpStr = prepare();
    
    fprintf(output, "%s\n", tmpStr);
    
    free(tmpStr);
}

void RegularText::read(FILE *input){
    bool betweenWords = false, previousSymbolWasBackSlashN = false;
    unsigned int i = 0;
    char tmpChar = '\0';
    
    do{
        tmpChar = getc(input);
        switch (tmpChar) {
            case ' ':{
                previousSymbolWasBackSlashN = false;
                if (!betweenWords) {
                    betweenWords = true;
                    text = (char*)realloc(text, i + 1);
                    text[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:{
                if (text != NULL) {
                    text = (char*)realloc(text, i + 1);
                    text[i] = '\0';
                    return;
                }
                else {
                    ungetc(tmpChar, input);
                    throw EmptyParagraphERROR();
                }
            }
                
            case '\n':{
                previousSymbolWasBackSlashN = true;
                text = (char*)realloc(text, i + 1);
                text[i] = '\n';
                i++;
                break;
            }
                
            default:{
                previousSymbolWasBackSlashN = false;
                betweenWords = false;
                text = (char*)realloc(text, i + 1);
                text[i] = tmpChar;
                i++;
                break;
            }
        }
        
        if (previousSymbolWasBackSlashN) {
            tmpChar = getc(input);
            if (tmpChar == EOF) {
                text = (char*)realloc(text, i + 1);
                text[i] = '\0';
                ungetc(tmpChar, input);
                return;
            }
            if (tmpChar == '\n') {
                text = (char*)realloc(text, i + 1);
                text[i] = '\0';
                return;
            }
            ungetc(tmpChar, input);
        }
    }while(1);
}

void RegularText::prints(FILE *out)const{
    fprintf(out, "^^regular^^ ");
    
    unsigned int i = 0;
    while (text[i] != '\0') {
        fprintf(out, "%c", text[i]);
        i++;
    }
    
    fprintf(out, "\n");
}

char* RegularText::prepare()const{
    char *tmpStr = NULL;
    unsigned int positionInTmpStr = 0, symbolsInCurrentStr = 0, counterOfPrintedSymbolsInTheWordInStr = 0, currentWordLength = 0, i = 0;
    bool continuationOfWord = false;
    
    for (unsigned int i = 0; i < redLineSize; i++) {
        if (symbolsInCurrentStr < screenWidth) {
            tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
            tmpStr[positionInTmpStr] = ' ';
            positionInTmpStr++;
            symbolsInCurrentStr++;
        }
        else{
            tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
            tmpStr[positionInTmpStr] = '\n';
            tmpStr[positionInTmpStr + 1] = ' ';
            positionInTmpStr += 2;
            symbolsInCurrentStr = 1;
        }
    }
    
    do{
        if (continuationOfWord || (i == 0) || (((text[i] != ' ') && (text[i] != '\n')) && ((text[i - 1] == ' ') || (text[i - 1] == '\n')))){
            currentWordLength = wordLength(text, i);
            if (currentWordLength + symbolsInCurrentStr <= screenWidth) {
                symbolsInCurrentStr += currentWordLength;
                for (unsigned int q = 0; q < currentWordLength; q++) {
                    if ((text[i] == '\xd0') || (text[i] == '\xd1')){
                        tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
                        tmpStr[positionInTmpStr] = text[i];
                        tmpStr[positionInTmpStr + 1] = text[i + 1];
                        positionInTmpStr += 2;
                        i += 2;
                    }
                    else{
                        tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
                        tmpStr[positionInTmpStr] = text[i];
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
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 3);
                    tmpStr[positionInTmpStr] = '\n';
                    tmpStr[positionInTmpStr + 1] = text[i];
                    tmpStr[positionInTmpStr + 2] = text[i + 1];
                    positionInTmpStr += 3;
                    i += 2;
                    counterOfPrintedSymbolsInTheWordInStr = 1;
                    symbolsInCurrentStr = 1;
                }
                else{
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
                    tmpStr[positionInTmpStr] = text[i];
                    tmpStr[positionInTmpStr + 1] = text[i + 1];
                    positionInTmpStr += 2;
                    i += 2;
                    symbolsInCurrentStr++;
                    counterOfPrintedSymbolsInTheWordInStr++;
                }
            }
            else{
                if (counterOfPrintedSymbolsInTheWordInStr == 0) {
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 3);
                    tmpStr[positionInTmpStr] = '\n';
                    tmpStr[positionInTmpStr + 1] = text[i];
                    tmpStr[positionInTmpStr + 2] = text[i + 1];
                    positionInTmpStr += 3;
                }
                else{
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 4);
                    tmpStr[positionInTmpStr] = '-';
                    tmpStr[positionInTmpStr + 1] = '\n';
                    tmpStr[positionInTmpStr + 2] = text[i];
                    tmpStr[positionInTmpStr + 3] = text[i + 1];
                    positionInTmpStr += 4;
                }
                counterOfPrintedSymbolsInTheWordInStr = 1;
                i += 2;
                symbolsInCurrentStr = 1;
                continuationOfWord = true;
            }
        }
        else{
            if ((text[i] == ' ') || (text[i] == '\n')) {
                counterOfPrintedSymbolsInTheWordInStr = 0;
                
                if (text[i] == '\n') {
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
                    tmpStr[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                    i++;
                    symbolsInCurrentStr = 0;
                }
                else{
                    if (symbolsInCurrentStr < screenWidth) {
                        tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
                        tmpStr[positionInTmpStr] = ' ';
                        positionInTmpStr++;
                        i++;
                        symbolsInCurrentStr++;
                    }
                    else{
                        tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
                        tmpStr[positionInTmpStr] = '\n';
                        tmpStr[positionInTmpStr + 1] = ' ';
                        positionInTmpStr += 2;
                        i++;
                        symbolsInCurrentStr = 1;
                    }
                }
                continue;
            }
            
            if (symbolsInCurrentStr < screenWidth - 1) {
                if ((counterOfPrintedSymbolsInTheWordInStr == 0) && (screenWidth - symbolsInCurrentStr == 2)) {
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
                    tmpStr[positionInTmpStr] = '\n';
                    tmpStr[positionInTmpStr + 1] = text[i];
                    positionInTmpStr += 2;
                    i++;
                    counterOfPrintedSymbolsInTheWordInStr = 1;
                    symbolsInCurrentStr = 1;
                }
                else{
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
                    tmpStr[positionInTmpStr] = text[i];
                    positionInTmpStr++;
                    i++;
                    symbolsInCurrentStr++;
                    counterOfPrintedSymbolsInTheWordInStr++;
                }
            }
            else{
                if (counterOfPrintedSymbolsInTheWordInStr == 0) {
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 2);
                    tmpStr[positionInTmpStr] = '\n';
                    tmpStr[positionInTmpStr + 1] = text[i];
                    positionInTmpStr += 2;
                }
                else{
                    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 3);
                    tmpStr[positionInTmpStr] = '-';
                    tmpStr[positionInTmpStr + 1] = '\n';
                    tmpStr[positionInTmpStr + 2] = text[i];
                    positionInTmpStr += 3;
                }
                counterOfPrintedSymbolsInTheWordInStr = 1;
                i++;
                symbolsInCurrentStr = 1;
                continuationOfWord = true;
            }
        }
    }while (i < strlen(text));
    
    tmpStr = (char*)realloc(tmpStr, positionInTmpStr + 1);
    tmpStr[positionInTmpStr] = '\0';
    
    return tmpStr;
}