#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "stringEditCmds.h"
#include "structs.h"
#include "techCmds.h"
#include "assistFunctions.h"
#include "stringsInsertCmds.h"

extern int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd, EOFFlag, commentFlag;
extern struct winsize terminal;
extern struct listOfStrings *pointerForStart, *pointerForEnd;
extern char *firstParametr, *secondParametr, *userString, fileNameGlobal[256];

int insertAfter(int numOfStr, char* s, int size_s){
    int i, j, k;
    char c;
    struct listOfStrings *tmpString, *newString, *tmpString2;
    
    isFileSaved = 0;
    
    if ((numberOfStrings == 0) && ((numOfStr == 0)||(numOfStr == -1))){
        tmpString2 = malloc(sizeof(struct listOfStrings));
        if (tmpString2 == NULL) return exitForce();
        newString = tmpString2;
        newString -> curStringSize = 0;
        newString -> prev = newString -> next = NULL;
        newString -> firstChar = newString -> lastChar = NULL;
        pointerForStart = pointerForEnd = newString;
        numberOfStrings = 1;
        j = 1;
        i = 1;
        for (k = 0; k < size_s; k++){
            c = s[k];
            if (c == '\n'){
                insertSymbol(j,i,c);
                j++;
                i = 1;
            }
            else{
                insertSymbol(j,i, c);
                i++;
            }
        }
        free(s);
        return 0;
    }
    if (numOfStr >= numberOfStrings){
        free(s);
        return -3;
    }
    
    if (numOfStr == 0){
        insertSymbol(1, 1, '\n');
        j = 1;
        i = 1;
        for (k = 0; k < size_s; k++){
            c = s[k];
            if (c == '\n') {
                insertSymbol(j,i,c);
                j++;
                i = 1;
            }
            else{
                insertSymbol(j,i, c);
                i++;
            }
        }
        free(s);
        return 0;
    }
    
    if (numOfStr == -1){
        insertSymbol(numberOfStrings, pointerForEnd -> curStringSize + 1, '\n');
        j = numberOfStrings;
        i = 1;
        for (k = 0; k < size_s; k++){
            c = s[k];
            if (c == '\n') {
                insertSymbol(j,i,c);
                j++;
                i = 1;
            }
            else{
                insertSymbol(j,i, c);
                i++;
            }
        }
        
        free(s);
        return 0;
    }
    
    tmpString = pointerForStart;
    for (i = 0; i < numOfStr - 1; i++){
        tmpString = tmpString -> next;
    }
    
    insertSymbol(numOfStr, tmpString -> curStringSize + 1, '\n');
    j = numOfStr + 1;
    i = 1;
    
    for (k = 0; k < size_s; k++){
        c = s[k];
        if (c == '\n'){
            insertSymbol(j,i,c);
            j++;
            i = 1;
        }
        else{
            insertSymbol(j,i, c);
            i++;
        }
    }
    free(s);
    return 0;
}