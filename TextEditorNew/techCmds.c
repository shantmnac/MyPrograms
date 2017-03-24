#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "techCmds.h"
#include "textViewCmds.h"
#include "assistFunctions.h"

extern int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd, EOFFlag, commentFlag;
extern struct listOfStrings *pointerForStart, *pointerForEnd;
extern char fileNameGlobal[256];

int exitF(void){
    freeMemory();
    
    if (!isFileSaved){
        return -12;
    }
    return 255;
}

int exitForce(void){
    freeMemory();
    return 255;
}

int readF(const char *fileName, int sizeOfFileName){
       
    FILE* tmpInput = fopen(fileName, "rb");
    if (tmpInput == NULL) 
        return -3;
    
    initFile(tmpInput);
    fclose(tmpInput);

    return 0;
}

int openF(char *fileName, int sizeOfFileName){
    char *tmpFileName;
    FILE *tmpInput;
    
    tmpFileName = (char*)realloc(fileName, sizeOfFileName + 1);
    
    if (tmpFileName == NULL) {
        free(fileName);
        return exitForce();
    }
    
    fileName = tmpFileName;
    
    fileName[sizeOfFileName] = '\0';
    tmpInput = fopen(fileName, "rb");
    
    if (tmpInput == NULL) return -3;
    
    memcpy(fileNameGlobal, fileName, sizeOfFileName + 1);
    initFile(tmpInput);
    fclose(tmpInput);
    free(fileName);
    return 0;
}

int writeF(char *fileName, int sizeOfFileName){
    char *tmpFileName;
    FILE *tmpInput;
    
    isFileSaved = 1;
    
    if (!sizeOfFileName){
        if (!strcmp(fileNameGlobal, "")) return -3;
        
        tmpInput = fopen(fileNameGlobal, "wb");
        assistFunctionForWriteF(tmpInput);
        fclose(tmpInput);
        return 0;
    }
    
    tmpFileName = realloc(fileName, sizeOfFileName + 1);
    if (tmpFileName == NULL) {
        free(fileName);
        return exitForce();
    }
    
    fileName = tmpFileName;
    
    fileName[sizeOfFileName] = '\0';
    tmpInput = fopen(fileName, "wb");
    
    if (tmpInput == NULL) return -3;
    assistFunctionForWriteF(tmpInput);
    fclose(tmpInput);
    free(fileName);
    return 0;
}

int setName(char *fileName, int sizeOfFileName){
    char *tmpFileName;
    
    if (sizeOfFileName == 0) {
        fileNameGlobal[0] = '\0';
        return 0;
    }
    
    tmpFileName = realloc(fileName, sizeOfFileName + 1);
    if (tmpFileName == NULL){
        free(fileName);
        return exitForce();
    }
    
    fileName = tmpFileName;
    fileName[sizeOfFileName] = '\0';
    memcpy(fileNameGlobal, fileName, sizeOfFileName + 1);
    free(fileName);
    return 0;
}



int help(void){
    char *tmpFileName="help.txt";
    int tmp;
    struct listOfStrings *tmpString1=NULL;
    struct listOfStrings *tmpString2=NULL;
    
    
    tmpString1 = pointerForStart;
    tmpString2 = pointerForEnd;
    tmp = numberOfStrings;
    
    pointerForStart = NULL;
    pointerForEnd = NULL;
    numberOfStrings = 0;
    
    if(readF(tmpFileName, 8) == -3) return -3;
    printPages();
    
    freeMemory();
    pointerForStart = tmpString1;
    pointerForEnd = tmpString2;
    numberOfStrings = tmp;
    
    return 0;
}
