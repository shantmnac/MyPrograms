#include "codeText.hpp"
#include "errors.hpp"
#include <iostream>
#include <stdlib.h>
#include <cstring>

extern unsigned int screenWidth;

CodeText::CodeText(){
    text = NULL;
    counter = 0;
}

CodeText::~CodeText(){
    if (text != NULL) {
        for (unsigned int q = 0; q < counter; q++) free(text[q]);
        free(text);
    }
}

void CodeText::push(const char *txt){
    if (txt == NULL) {
        text = (char**)realloc(text, (counter + 1) * sizeof(char*));
        text[counter] = (char*)malloc(2);
        text[counter][0] = '\n';
        text[counter][1] = '\0';
    }
    else{
        text = (char**)realloc(text, (counter + 1) * sizeof(char*));
        text[counter] = (char*)malloc(strlen(txt) + 1);
        strcpy(text[counter], txt);
    }
    counter++;
}

unsigned long int CodeText::countOfSymbols()const{
    unsigned int tmpCounter = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (text[i][j] != '\0') {
            if ((text[i][j] == '\xd0') || (text[i][j] == '\xd1')) {
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

unsigned long int CodeText::countOfWords()const{
    unsigned int tmpCounter = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (text[i][j] != '\0') {
            while ((text[i][j] != '\n') && (text[i][j] != ' ') && (text[i][j] != '\0')) j++;
            tmpCounter++;
            if (text[i][j] == '\0') break;
            else j++;
        }
    }
    
    return tmpCounter;
}

void CodeText::print(FILE *output)const{
    bool firstTime = true;
    
    for (unsigned int i = 0; i < counter; i++) {
        for (unsigned int j = 0; (j < strlen(text[i])) && (j < screenWidth - 4); j++) {
            if (firstTime) {
                fprintf(output, "@   %c", text[i][j]);
                firstTime = false;
            }
            else fprintf(output, "%c", text[i][j]);
        }
        firstTime = true;
        if ((strlen(text[i]) != 1) && (text[i][0] != '\n')) fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void CodeText::read(FILE *input){
    bool betweenWords = false, previousSymbolWasBackSlashN = false, paragraphIsEmpty = true;
    unsigned int i = 0;
    char tmpChar = '\0', *tmpWord = NULL;
    
    do{
        tmpChar = getc(input);
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
                    push(tmpWord);
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
                    push(tmpWord);
                    if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                }
                else push(NULL);
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
            if (tmpChar != '\t') {
                if (tmpChar != ' ') {
                    ungetc(tmpChar, input);
                    throw TextProcessingERROR();
                }
                else {
                    for (unsigned int q = 0; q < 3; q++) {
                        tmpChar = getc(input);
                        if (tmpChar != ' ') {
                            ungetc(tmpChar, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            throw TextProcessingERROR();
                        }
                    }
                }
            }
            i = 0;
        }
    }while(1);
}

void CodeText::prints(FILE *output)const{
    fprintf(output, "^^code^^ ");
    
    bool firstTime = true;
    
    for (unsigned int i = 0; i < counter; i++) {
        for (unsigned int j = 0; j < strlen(text[i]); j++) {
            if (firstTime) {
                fprintf(output, "@   %c", text[i][j]);
                firstTime = false;
            }
            else fprintf(output, "%c", text[i][j]);
        }
        firstTime = true;
        if ((strlen(text[i]) != 1) && (text[i][0] != '\n')) fprintf(output, "\n");
    }
    fprintf(output, "\n");
}