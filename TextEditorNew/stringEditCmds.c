#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "structs.h"
#include "techCmds.h"
#include "assistFunctions.h"

extern int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd, EOFFlag, commentFlag;
extern struct winsize terminal;
extern struct listOfStrings *pointerForStart, *pointerForEnd;
extern char *firstParametr, *secondParametr, *userString, fileNameGlobal[256];

int deleteRange(int startR, int endR){
    int i;
    isFileSaved = 0;
    
    if (numberOfStrings == 0){
        return 0;
    }
    
    if (startR > numberOfStrings) return -3;
    if ((endR == -1) || (endR > numberOfStrings)){
        endR = numberOfStrings;
    }
    
    for (i = startR; i <= endR; i++){
        deleteString(startR);
    }
    return 0;
}

int editString(int numOfStr, int numOfChar, char replacingChar){
    int i;
    struct listOfStrings *tmpString, *newString, *tmpString2;
    struct listOfChars *tmpChar;
    isFileSaved = 0;
    
    if (numOfStr > numberOfStrings) return -3;
    tmpString = pointerForStart;
    
    for (i = 0; i < numOfStr-1; i++){
        tmpString = tmpString -> next;
    }
    
    if (numOfChar > tmpString -> curStringSize) return -3;
    
    tmpChar = tmpString -> firstChar;
    
    for (i = 0; i < numOfChar-1; i++){
        tmpChar = tmpChar -> next;
    }
    
    if (replacingChar == '\n'){
        tmpString2 = malloc(sizeof(struct listOfStrings));
        if (tmpString2 == NULL) return exitForce();
        newString = tmpString2;
        if (numOfChar == 1) {
            if (tmpString != pointerForStart) tmpString -> prev -> next = newString;
            else pointerForStart = newString;
            newString -> prev = tmpString -> prev;
            newString -> curStringSize = 0;
            newString -> firstChar = newString -> lastChar = NULL;
            newString -> next = tmpString;
            newString -> next -> prev = newString;
            tmpString -> firstChar = tmpChar -> next;
            
            if (tmpChar == tmpString -> lastChar) tmpString -> lastChar = tmpChar -> prev;
            else tmpChar -> next -> prev = NULL;
            
            tmpString -> curStringSize--;
            free(tmpChar);
            numberOfStrings++;
            return 0;
        }
        
        if (numOfChar == tmpString -> curStringSize){
            newString -> next = tmpString->next;
            tmpString -> next = newString;
            newString -> firstChar = NULL;
            newString -> lastChar = NULL;
            newString -> curStringSize = 0;
            tmpString -> lastChar = tmpChar -> prev;
            newString -> prev = tmpString;
            if (tmpString -> lastChar != NULL) tmpString -> lastChar->next = NULL;
            
            tmpString -> curStringSize--;
            if (newString -> next != NULL) newString -> next -> prev = newString;
            else pointerForEnd = newString;
            numberOfStrings++;
            free(tmpChar);
        }
        else
        {
            newString -> next = tmpString -> next;
            tmpString -> next = newString;
            newString -> lastChar = tmpString -> lastChar;
            tmpString -> lastChar = tmpChar -> prev;
            newString -> curStringSize = tmpString -> curStringSize - numOfChar;
            tmpString -> curStringSize = numOfChar - 1;
            if (tmpString -> curStringSize != 0) tmpString -> lastChar -> next = NULL;
            newString -> prev = tmpString;
            newString -> firstChar = tmpChar -> next;
            newString -> firstChar -> prev = NULL;
            
            if (newString -> next != NULL) newString -> next -> prev = newString;
            else pointerForEnd = newString;
            numberOfStrings++;
            free(tmpChar);
        }
    }
    else
    {
        tmpChar -> curChar = replacingChar;
    }
    return 0;
}

int insertSymbol(int numOfStr, int numOfChar, char insertingChar){
    int i;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar, *newChar, *tmpChar2, *tmpChar3;
    isFileSaved = 0;
    
    if (numOfStr > numberOfStrings) return -3;
    
    tmpString = pointerForStart;
    
    for (i = 0; i < numOfStr-1; i++){
        tmpString = tmpString -> next;
    }
    
    if (numOfChar > tmpString -> curStringSize){
        tmpChar2  = malloc(sizeof(struct listOfChars));
        if (tmpChar2 == NULL) return exitForce();
        
        tmpChar = tmpChar2;
        
        if (tmpString -> lastChar == NULL) tmpString -> firstChar = tmpChar;
        else tmpString -> lastChar -> next = tmpChar;
        
        tmpChar -> prev = tmpString -> lastChar;
        tmpString -> lastChar = tmpChar;
        tmpChar -> curChar = '0';
        tmpChar -> next = NULL;
        tmpString -> curStringSize++;
        editString(numOfStr, tmpString -> curStringSize, insertingChar);
        return 0;
    }
    
    if (numOfChar <= 1){
        tmpChar2  = malloc(sizeof(struct listOfChars));
        if (tmpChar2 == NULL) return exitForce();
        
        tmpChar = tmpChar2;
        
        if (tmpString -> firstChar == NULL) tmpString -> lastChar = tmpChar;
        else tmpString -> firstChar -> prev = tmpChar;
        
        tmpChar -> prev = NULL;
        tmpChar -> next = tmpString -> firstChar;
        tmpString -> firstChar = tmpChar;
        tmpChar -> curChar = '0';
        tmpString -> curStringSize++;
        editString(numOfStr, 1, insertingChar);
        return 0;
    }
    
    tmpChar = tmpString -> firstChar;
    
    for (i = 0; i < numOfChar-1; i++){
        tmpChar = tmpChar -> next;
    }
    
    tmpChar3 = malloc(sizeof(struct listOfChars));
    
    if (tmpChar3 == NULL) return exitForce();
    newChar = tmpChar3;
    
    newChar -> prev = tmpChar -> prev;
    newChar -> next = tmpChar;
    newChar -> curChar = 0;
    tmpChar -> prev -> next = newChar;
    tmpChar -> prev = newChar;
    tmpString -> curStringSize++;
    editString(numOfStr, numOfChar, insertingChar);
    return 0;
}

int replaceSubstring(int startR, int endR, char* findThisString,int sizeOfFindingString, char* replaceWhitThisString, int sizeOfReplacingString){
    int i, j, k, m, dif, dif2;
    int tmp;
    char c;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar;
    isFileSaved = 0;
    
    if (numberOfStrings == 0){
        free(findThisString);
        free(replaceWhitThisString);
        return 0;
    }
    
    if (startR == -1) startR = 1;
    if (endR == -1) endR = numberOfStrings;
    
    tmpString = pointerForStart;
    
    for (i = 0; i < startR - 1; i++){
        tmpString = tmpString -> next;
    }
    
    if (sizeOfFindingString == -2){
        tmp = 1;
        dif = 0;
        
        for ( i = startR; i <= endR+dif; i++){
            dif += replace(&i, &tmp, 0, replaceWhitThisString, sizeOfReplacingString);
            tmp = 1;
        }
        
        free(findThisString);
        free(replaceWhitThisString);
        return 0;
    }
    
    if (sizeOfFindingString == -1){
        dif = 0;
        for ( i = startR; i <= endR+dif; i++){
            tmp = tmpString -> curStringSize + 1;
            dif2 = replace(&i, &tmp, 0, replaceWhitThisString, sizeOfReplacingString);
            for (m = 0; m < dif2; m++){
                tmpString = tmpString -> next;
            }
            dif+=dif2;
            tmpString = tmpString -> next;
        }
        free(findThisString);
        free(replaceWhitThisString);
        return 0;
    }
    
    k = 0;
    dif = 0;
    for ( i = startR; i <= endR + dif; i++){
        for (j = 1; j <= tmpString -> curStringSize + 1; j++){
            if (k == sizeOfFindingString){
                dif2 = replace(&i, &j, sizeOfFindingString, replaceWhitThisString, sizeOfReplacingString);
                for (m = 0; m < dif2; m++){
                    tmpString = tmpString -> next;
                }
                dif+=dif2;
                k = 0;
            }
            if (j > tmpString -> curStringSize) break;
            
            tmpChar = getChar(i, j);
            c = tmpChar -> curChar;
            
            if (c == findThisString[k])
                k++;
            else
                k = 0;
        }
        k = 0;
        tmpString = tmpString -> next;
    }
    free(findThisString);
    free(replaceWhitThisString);
    return 0;
}

int deleteBraces(void){
    int i, j, k;
    char c;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar;
    
    isFileSaved = 0;
    tmpString = pointerForStart;
    
    if (numberOfStrings == 0) return 0;

    for (i = 1; i <= numberOfStrings; i++){
        for (j = 1; j <= tmpString -> curStringSize; j++){
            tmpChar = getChar(i, j);
            c = tmpChar -> curChar;
            if (c == '{'){
                assistFunctionForDeleteBraces(&i, &j);
                tmpString = pointerForStart;
                for (k = 0; k < i-1; k++){
                    tmpString = tmpString -> next;
                }
            }
            
        }
        tmpString = tmpString -> next;
    }
    return 0;
}