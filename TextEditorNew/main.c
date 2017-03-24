#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "techCmds.h"
#include "structs.h"
#include "assistFunctions.h"

int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd = 1, EOFFlag = 0, commentFlag = 0;
struct listOfStrings *pointerForStart, *pointerForEnd;
char *firstParametr = NULL, *secondParametr = NULL, *userString = NULL, fileNameGlobal[256];
struct winsize terminal;

int main(int argc, char **argv){
    int returningValue = 314;
    
    initilization();
    
    if (argc == 2){
        openF(argv[1], strlen(argv[1]));
    }
    
    if (argc > 2){
        fprintf(stderr, "main: неверное число параметров\n");
        return 0;
    }
    
    while(1){
        userString = malloc(1);
        
        if (userString == NULL) {
            exitForce();
            return 0;
        }
        
        userString[0] = '\0';
        sizeOfCmd = 1;
        
        returningValue = commandReading();
        free(userString);
        
        if (returningValue == 255) break;
        
        if (EOFFlag == 1){
            exitForce();
            break;
        }
        
        if (returningValue == -12) fprintf(stderr, "main: файл не сохранен\n");
        
        if (returningValue == -3) fprintf(stderr, "main: произошла ошибка\n");
        
        if ((returningValue == -2) || (returningValue == -1)) fprintf(stderr, "main: неверная команда\n");
    }
    return 0;
}


