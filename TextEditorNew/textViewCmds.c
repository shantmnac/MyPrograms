#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "structs.h"
#include "assistFunctions.h"
#include "techCmds.h"
#include "textViewCmds.h"

extern int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd, EOFFlag, commentFlag;
extern struct winsize terminal;
extern struct listOfStrings *pointerForStart, *pointerForEnd;
extern char *firstParametr, *secondParametr, *userString, fileNameGlobal[256];


int setTabWidth(int size){
    if (size <= 0) return -3;
    tabWidth = size;
    return 0;
}

int printRange(int startR, int endR){
    int startRTmp = startR, endRTmp = endR, localRow, localCol, i, j, k, stringCount, tmp;
    char c = 0;
    struct listOfStrings *tmpString, *tmpString2;
    struct listOfChars *tmpChar;
    
    if (TTYRedirect){
        assistFunctionForWriteF(stdout);
        return 0;
    }
    
    clearTTY();
    
    if (pointerForStart == NULL){
        setKeypress();
        
        while (1){
            c = getchar();
            if (c == 'q')
            {
                resetKeypress();
                system("setterm -cursor on");
                putchar('\n');
                return 0;
            }
        }
    }
    
    tmpString = pointerForStart;
    
    ioctl(0, TIOCGWINSZ, &terminal);
    localRow = terminal.ws_row-1;
    localCol = terminal.ws_col;
    
    if (startR > numberOfStrings) return -3;
    if (startR == -1) startRTmp = 1;
    if ((endR == -1) || (endR > numberOfStrings)) endRTmp = numberOfStrings;
    
    for (i = 0; i < startRTmp - 1; i++) {
        tmpString = tmpString -> next;
    }
    
    if (!wrapMod){
        tmp = numberOfStrings;
        numberOfStrings = endRTmp - startRTmp + 1;
        tmpString2 = pointerForStart;
        pointerForStart = tmpString;
        j = 1;
        startRTmp= 1;
        
        if(updateBuffer(startRTmp)){
            freeBuffer();
            return exitForce();
        }
        
        while (1){
            printBuffer(j);
            setKeypress();
            
            while (1){
                c = getchar();
                if (c == 'q')
                {
                    resetKeypress();
                    printf("\n");
                    freeBuffer();
                    numberOfStrings = tmp;
                    pointerForStart = tmpString2;
                    return 0;
                }
                
                if (c == '>'){
                    resetKeypress();
                    clearTTY();
                    j++;
                    break;
                }
                
                if ((c == '<') && (j >= 2)){
                    resetKeypress();
                    clearTTY();
                    j--;
                    break;
                }
                
                if ((c == ' ') && (startRTmp + localRow <= numberOfStrings)){
                    resetKeypress();
                    clearTTY();
                    startRTmp = startRTmp + localRow;
                    freeBuffer();
                    
                    if(updateBuffer(startRTmp)){
                        freeBuffer();
                        return exitForce();
                    }
                    
                    break;
                }
            }
        }
    }
    else
    {
        localRow -= 2;
        system("setterm -cursor off");
        i = startRTmp;
        stringCount = 1;
        
        while (i <= endRTmp){
            tmpChar = tmpString -> firstChar;
            putchar('>');
            j = 1;
            
            while (tmpChar != NULL){
                if ((j == localCol) && (tmpChar -> next != NULL)){
                    putchar('\n');
                    stringCount++;
                    if (stringCount > localRow){
                        setKeypress();
                        
                        while (1){
                            c = getchar();
                            if (c == 'q'){
                                resetKeypress();
                                system("setterm -cursor on");
                                printf("\n");
                                return 0;
                            }
                            
                            if (c == ' '){
                                stringCount = 1;
                                resetKeypress();
                                clearTTY();
                                break;
                            }
                        }
                    }
                    j = 0;
                }
                if (tmpChar -> curChar == '\t'){
                    for (k = 0; k < tabWidth; k++){
                        if (j == localCol){
                            putchar('\n');
                            stringCount++;
                            if (stringCount > localRow){
                                setKeypress();
                                
                                while (1){
                                    c = getchar();
                                    if (c == 'q'){
                                        resetKeypress();
                                        system("setterm -cursor on");
                                        putchar('\n');
                                        return 0;
                                    }
                                    if (c == ' '){
                                        stringCount = 1;
                                        resetKeypress();
                                        clearTTY();
                                        break;
                                    }
                                }
                            }
                            j = 0;
                        }
                        putchar(' ');
                        j++;
                    }
                }
                else putchar(tmpChar -> curChar);
                j++;
                tmpChar = tmpChar -> next;
            }
            i++;
            stringCount++;
            
            if (stringCount > localRow){
                setKeypress();
                
                while (1){
                    c = getchar();
                    if (c == 'q'){
                        resetKeypress();
                        system("setterm -cursor on");
                        printf("\n");
                        return 0;
                    }
                    
                    if (c == ' '){
                        stringCount = 1;
                        resetKeypress();
                        clearTTY();
                        tmpString = tmpString -> next;
                        break;
                    }
                }
                continue;
            }
            tmpString = tmpString -> next;
            putchar('\n');
        }
        setKeypress();
        
        while (1){
            c = getchar();
            if (c == 'q'){
                resetKeypress();
                printf("\n");
                break;
            }
        }
    }
    system("setterm -cursor on");
    return 0;
}

int printPages(void){
    return printRange(1, numberOfStrings);
}

int setWrap(int newWrapMod){
    if (newWrapMod == 1) wrapMod = 1;
    else wrapMod = 0;
    return 0;
}