#include "headerText.hpp"
#include "otherFunctions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>

extern unsigned int screenWidth;

char *tmpStrGlobal = NULL;

HeaderText::HeaderText(){
    text = NULL;
    headerLevel = 0;
}

HeaderText::~HeaderText(){
    if (text != NULL) free(text);
}

unsigned long int HeaderText::countOfSymbols()const{
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

unsigned long int HeaderText::countOfWords()const{
    unsigned int tmpCounter = 0, j = 0;
    
    while (text[j] != '\0') {
        while ((text[j] != '\n') && (text[j] != ' ') && (text[j] != '\0')) j++;
        tmpCounter++;
        if (text[j] == '\0') break;
        else j++;
    }
    
    return tmpCounter;
}

void HeaderText::print(FILE *output)const{
    if (screenWidth < 2 * headerLevel + 4) return;
    
    try {
        prepare();
    } catch (TextProcessingERROR) {
        if (tmpStrGlobal != NULL) {
            free(tmpStrGlobal);
            tmpStrGlobal = NULL;
        }
        return;
    }
    fprintf(output, "%s\n\n", tmpStrGlobal);
    
    if (tmpStrGlobal != NULL) {
        free(tmpStrGlobal);
        tmpStrGlobal = NULL;
    }
}

void HeaderText::read(FILE *input){
    char tmpChar = '\0';
    bool betweenWords = false;
    unsigned int i = 0;
    
    tmpChar = getc(input);
    while (tmpChar == '#') {
        headerLevel++;
        tmpChar = getc(input);
    }
    
    if (tmpChar == ' ') betweenWords = true;
    else ungetc(tmpChar, input);
    
    do{
        tmpChar = getc(input);
        switch (tmpChar) {
            case ' ':{
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
                if (text != NULL) {
                    text = (char*)realloc(text, i + 1);
                    text[i] = '\0';
                    return;
                }
                else throw EmptyParagraphERROR();
            }
                
            default:{
                betweenWords = false;
                text = (char*)realloc(text, i + 1);
                text[i] = tmpChar;
                i++;
                break;
            }
        }
    }while(1);
}

void HeaderText::prints(FILE *out)const{
    fprintf(out, "^^header^^ ");
    
    for (unsigned int i = 0; i < headerLevel; i++) fprintf(out, "#");
    fprintf(out, " ");
    unsigned int i = 0;
    while (text[i] != '\0') {
        fprintf(out, "%c", text[i]);
        i++;
    }
    
    fprintf(out, "\n\n");
}

void HeaderText::prepare()const{
    unsigned int positionInTmpStr = 0, currentWordLength = 0, localScreenWidth = 0, counterForArrayOfWords = 0, k = 0, length = 0, q = 0, a = 0,  i = 0;
    char **arrayOfWords = NULL;
    bool tmpBool = false;
    
    while (text[i] != '\0') {
        if (text[i] == ' ') {
            arrayOfWords[counterForArrayOfWords] = (char*)realloc(arrayOfWords[counterForArrayOfWords], k + 1);
            arrayOfWords[counterForArrayOfWords][k] = '\0';
            counterForArrayOfWords++;
            i++;
            k = 0;
            if (text[i] != '\0') {
                arrayOfWords = (char**)realloc(arrayOfWords, (counterForArrayOfWords + 1) * sizeof(char*));
                arrayOfWords[counterForArrayOfWords] = NULL;
            }
        }
        else{
            if (i == 0) {
                arrayOfWords = (char**)realloc(arrayOfWords, (counterForArrayOfWords + 1) * sizeof(char*));
                arrayOfWords[counterForArrayOfWords] = NULL;
            }
            arrayOfWords[counterForArrayOfWords] = (char*)realloc(arrayOfWords[counterForArrayOfWords], k + 1);
            arrayOfWords[counterForArrayOfWords][k] = text[i];
            i++;
            k++;
        }
    }
    
    arrayOfWords[counterForArrayOfWords] = (char*)realloc(arrayOfWords[counterForArrayOfWords], k + 1);
    arrayOfWords[counterForArrayOfWords][k] = '\0';
    
    k = 0;
    i = 0;
    
    tmpStrGlobal = (char*)realloc(tmpStrGlobal, screenWidth + 1);
    for (unsigned int q = 0; q < screenWidth; q++) {
        tmpStrGlobal[q] = '#';
    }
    positionInTmpStr = screenWidth + 1;
    tmpStrGlobal[positionInTmpStr - 1] = '\n';
    
    localScreenWidth = screenWidth - 2 * headerLevel;
    
    
    do{
        positionInTmpStr = insertResh(positionInTmpStr);
        for (q = k; q < counterForArrayOfWords + 1; q++) {
            length += wordLength(arrayOfWords[q], 0) + 1;
            if (q == counterForArrayOfWords) length--;
            k++;
            
            if (length > localScreenWidth) {
                length -= wordLength(arrayOfWords[q], 0) + 2;
                k--;
                if (q == counterForArrayOfWords) length++;
                break;
            }
        }
        
        if (q == a) {
            if (arrayOfWords != NULL) {
                for (unsigned int z = 0; z < counterForArrayOfWords + 1; z++) {
                    free(arrayOfWords[z]);
                }
                free(arrayOfWords);
            }
            
            throw TextProcessingERROR();
        }
            
        for (unsigned int w = 0; w < (localScreenWidth - length)/2; w++) {
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
        }
        
        for (unsigned int t = a; t < q; t++) {
            currentWordLength = wordLength(arrayOfWords[t], 0);
            i = 0;
            if (t != a) {
                tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                tmpStrGlobal[positionInTmpStr] = ' ';
                positionInTmpStr++;
            }
            
            do {
                if ((arrayOfWords[t][i] == '\xd0') || (arrayOfWords[t][i] == '\xd1')){
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 2);
                    tmpStrGlobal[positionInTmpStr] = arrayOfWords[t][i];
                    tmpStrGlobal[positionInTmpStr + 1] = arrayOfWords[t][i + 1];
                    positionInTmpStr += 2;
                    i += 2;
                    if (i/2 == currentWordLength) break;
                }
                else{
                    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
                    tmpStrGlobal[positionInTmpStr] = arrayOfWords[t][i];
                    positionInTmpStr++;
                    i++;
                    if (i == currentWordLength) break;
                }
            }while(1);
        }
        
        for (unsigned int w = 0; w < (localScreenWidth - length)/2; w++) {
            tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
            tmpStrGlobal[positionInTmpStr] = ' ';
            positionInTmpStr++;
        }
        
        length = 0;
        
        positionInTmpStr = insertResh(positionInTmpStr);
        tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + 1);
        tmpStrGlobal[positionInTmpStr] = '\n';
        positionInTmpStr++;
        a = q;
    }while (q - 1 != counterForArrayOfWords);

    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + screenWidth + 1);
    for (unsigned int q = positionInTmpStr; q < positionInTmpStr + screenWidth; q++) {
        tmpStrGlobal[q] = '#';
    }
    positionInTmpStr += screenWidth;
    tmpStrGlobal[positionInTmpStr] = '\0';
    
    if (arrayOfWords != NULL) {
        for (unsigned int z = 0; z < counterForArrayOfWords + 1; z++) {
            free(arrayOfWords[z]);
        }
        free(arrayOfWords);
    }
    
    return;
}

unsigned int HeaderText::insertResh(unsigned int positionInTmpStr)const{
    unsigned int headerLvl = headerLevel;
    
    tmpStrGlobal = (char*)realloc(tmpStrGlobal, positionInTmpStr + headerLvl);
    for (unsigned int q = 0; q < headerLvl; q++) {
        tmpStrGlobal[positionInTmpStr] = '#';
        positionInTmpStr++;
    }
    return positionInTmpStr;
}