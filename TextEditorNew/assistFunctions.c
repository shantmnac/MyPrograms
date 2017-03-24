#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "assistFunctions.h"
#include "stringEditCmds.h"
#include "textViewCmds.h"
#include "stringsInsertCmds.h"
#include "techCmds.h"
#include "structs.h"

extern int numberOfStrings, enterFlag, TTYRedirect, tabWidth, wrapMod, isFileSaved, sizeOfCmd, EOFFlag, commentFlag;
extern struct winsize terminal;
extern struct listOfStrings *pointerForStart, *pointerForEnd;
extern char *firstParametr, *secondParametr, *userString, fileNameGlobal[256];

static struct termios storedSettings;
int bufStringSize = 0;
struct bufOfString *buffer;

void initilization(void){
    
    pointerForStart = NULL;
    pointerForEnd = NULL;
    numberOfStrings = 0;
    enterFlag = 0;
    TTYRedirect = !isatty(1);
    tabWidth = 8;
    wrapMod = 1;
    isFileSaved = 1;
    
}

void clearBufffer(char bufChars){
    
    if (EOFFlag == 1) return;
    
    while (bufChars != '\n'){
        
        if (bufChars == EOF){
            EOFFlag = 1;
            return;
        }
        
        bufChars =getchar();
    }
}

int addCharToParametr(char **array, int size, char inputChar){
    char* tmp = NULL;
    
    tmp = realloc(*array, size+1);
    
    if (tmp == NULL) return -1;
    
    tmp[size] = inputChar;
    *array = tmp;
    
    return size+1;
}

char readCommand(void){
    char *tmp, c;
    
    c = getchar();
    
    while ((c != '\n') && (c != ' ') && (c != '#')){
        if (c == EOF){
            EOFFlag = 1;
            return 0;
        }
        
        sizeOfCmd++;
        tmp = realloc(userString, sizeOfCmd);
        if (tmp == NULL) return -1;
        
        userString = tmp;
        userString[sizeOfCmd-2] = c;
        userString[sizeOfCmd-1] = '\0';
        c = getchar();
    }
    return c;
}

void freeMemory(void){
    int i, j, size;
    struct listOfStrings *temp1 = pointerForEnd;
    struct listOfChars *temp2;
    
    for(i = 0; i < numberOfStrings; i++){
        size = temp1 -> curStringSize;
        temp2 = temp1 -> lastChar;
        
        for (j = 0; j < size; j++){
            if (temp2 -> prev != NULL){
                temp2 = temp2 -> prev;
                free(temp2 -> next);
            }
            else{
                free(temp2);
            }
        }
        
        if (temp1 -> prev != NULL){
            temp1 = temp1 -> prev;
            free(temp1 ->next);
        }
        else{
            free(temp1);
        }
    }
    
    pointerForStart = NULL;
    pointerForEnd = NULL;
    numberOfStrings = 0;
}

void addString(void){
    struct  listOfStrings *tmp;
    
    if (numberOfStrings == 0){
        tmp = malloc(sizeof(struct listOfStrings));
        if (tmp == NULL){
            fprintf(stderr, "addString: недостаточно памяти\n");
            freeMemory();
            exit(1);
        }
        
        pointerForStart = tmp;
        pointerForEnd = tmp;
        
        pointerForStart -> firstChar = NULL;
        pointerForStart -> lastChar = NULL;
        pointerForStart -> curStringSize = 0;
        pointerForStart -> prev = NULL;
        pointerForStart -> next = NULL;
    }
    else{
        tmp = malloc(sizeof(struct listOfStrings));
        if (tmp == NULL){
            fprintf(stderr, "addString: недостаточно памяти\n");
            freeMemory();
            exit(1);
        }
        
        tmp -> curStringSize = 0;
        tmp -> firstChar = NULL;
        tmp -> lastChar = NULL;
        tmp -> prev = pointerForEnd;
        tmp -> next = NULL;
        pointerForEnd -> next = tmp;
        pointerForEnd = tmp;
    }
    
    numberOfStrings++;
}

void addCharToString(struct listOfStrings *tmpString, char addingChar){
    struct listOfChars *tmp;
    
    if (tmpString -> curStringSize == 0){
        tmp = malloc(sizeof(struct listOfChars));
        if (tmp == NULL){
            fprintf(stderr, "addCharToString: недостаточно памяти\n");
            freeMemory();
            exit(1);
        }
        
        tmpString -> firstChar = tmp;
        tmpString -> lastChar = tmp;
        
        tmp -> curChar = addingChar;
        tmp -> next = NULL;
        tmp -> prev = NULL;
    }
    else{
        tmp = malloc(sizeof(struct listOfChars));
        if (tmp == NULL){
            fprintf(stderr, "addCharToString: недостаточно памяти\n");
            freeMemory();
            exit(1);
        }
        
        pointerForEnd -> lastChar -> next = tmp;
        tmp -> curChar = addingChar;
        tmp -> next = NULL;
        tmp -> prev = tmpString -> lastChar;
        tmpString -> lastChar = tmp;
    }
    
    tmpString -> curStringSize++;
}

void initFile(FILE* input){
    unsigned char temp = 0;
    int test = -1;
    
    freeMemory();
    
    while(!feof(input)){
        test = (int)fread(&temp, 1, 1, input);
        
        if(test == 0){
            if (test == '\n') enterFlag = 1;
            return;
        }
        
        addString();
        
        while (temp != '\n'){
            addCharToString(pointerForEnd, temp);
            
            if(feof(input)) return;
        
            test = (int)fread(&temp, 1, 1, input);
            
            if(!test){
                if (temp == '\n') enterFlag = 1;
                return;
            }
        }
    }
    
    if (temp == '\n')
        enterFlag = 1;
}

void assistFunctionForWriteF(FILE *output){
    int i, j, sizeOfString;
    char enter = '\n';
    struct listOfStrings *tmp1 = pointerForStart;
    struct listOfChars *tmp2;
    
    for(i = 0; i < numberOfStrings; i++){
        sizeOfString = tmp1 -> curStringSize;
        tmp2 = tmp1 -> firstChar;
        
        for(j = 0; j < sizeOfString; j++){
            fwrite(&(tmp2 -> curChar), 1, 1, output);
            tmp2 = tmp2 -> next;
        }
        
        tmp1 = tmp1 -> next;
        
        if ((i != numberOfStrings-1) || (enterFlag != 0)) fwrite(&enter, 1, 1, output);
    }
}

struct listOfChars* getChar(int numOfStr, int numOfChar){
    struct listOfStrings *tmpString = pointerForStart;
    struct listOfChars *tmpChar;
    int i, j;
    
    for(i = 0; i < numOfStr-1; i++){
        tmpString = tmpString -> next;
        
        if (tmpString == NULL) return NULL;
    }
    
    tmpChar = tmpString -> firstChar;
    
    for(j = 0; j < numOfChar-1; j++){
        if (tmpChar == NULL) return NULL;
        
        tmpChar = tmpChar -> next;
    }
    
    return tmpChar;
}

int updateBuffer(int numOfStr){
    int i = 0, j = 0, b, localRow;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar;
    struct bufOfString *tmpBuffer;
    char *tmp, *tmp2;
    
    localRow = terminal.ws_row-1;
    tmpString = pointerForStart;
    
    for (i = 0; i < numOfStr - 1; i++){
        tmpString = tmpString -> next;
    }
    
    if (localRow > (numberOfStrings - numOfStr + 1)) b = numberOfStrings;
    else b = numOfStr + localRow - 1;
    
    for (i = numOfStr; i <= b; i++){
        bufStringSize++;
        
        tmpBuffer = realloc(buffer, sizeof(struct bufOfString) * bufStringSize);
        
        if (tmpBuffer == NULL) return -1;
        
        buffer = tmpBuffer;
        tmpChar = tmpString -> firstChar;
        buffer[bufStringSize-1].size = 0;
        
        tmp = malloc(1);
        if (tmp == NULL) return -1;
        
        buffer[bufStringSize-1].string = tmp;
        while (tmpChar != NULL){
            buffer[bufStringSize-1].size++;
            
            tmp2 = realloc(buffer[bufStringSize - 1].string, buffer[bufStringSize - 1].size);
            if (tmp2 == NULL) return -1;
            buffer[bufStringSize-1].string = tmp2;
            
            if (tmpChar -> curChar == '\t'){
                for (j = 0; j < tabWidth; j++){
                    buffer[bufStringSize - 1].string[buffer[bufStringSize - 1].size - 1] = ' ';
                    if (j != tabWidth - 1){
                        buffer[bufStringSize-1].size++;
                        
                        tmp2 = realloc(buffer[bufStringSize - 1].string, buffer[bufStringSize - 1].size);
                        if (tmp2 == NULL) return -1;
                        buffer[bufStringSize-1].string = tmp2;
                    }
                }
                tmpChar = tmpChar -> next;
            }
            else{
                buffer[bufStringSize - 1].string[buffer[bufStringSize - 1].size - 1] = tmpChar -> curChar;
                tmpChar = tmpChar -> next;
            }
        }
        tmpString = tmpString -> next;
    }
    return 0;
}

void printBuffer(int numOfStr){
    int i, j, localCol, counter = 0;
    
    localCol = terminal.ws_col;
    
    for (i = 0; i < bufStringSize; i++){
        j = numOfStr;
        
        while (1){
            if (counter >= localCol){
                break;
            }
            
            if (j > buffer[i].size){
                break;
            }
            
            putchar(buffer[i].string[j-1]);
            j++;
            counter++;
        }
        counter = 0;
        putchar('\n');
    }
}

void freeBuffer(void){
    int i;
    
    for (i = 0; i < bufStringSize; i++){
        free(buffer[i].string);
    }
    
    free(buffer);
    buffer = NULL;
    bufStringSize = 0;
}

int deleteString(int stringNum){
    int i;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar;
    
    tmpString = pointerForStart;
    
    for (i = 0; i < stringNum - 1; i++){
        tmpString = tmpString -> next;
    }
    
    tmpChar = tmpString -> lastChar;
    
    if (tmpChar != NULL){
        while (1){
            tmpChar = tmpChar -> prev;
            if (tmpChar == NULL){
                free(tmpString -> firstChar);
                break;
            }
            free(tmpChar -> next);
        }
    }
    
    if (pointerForStart == pointerForEnd){
        free(tmpString);
        
        pointerForStart = pointerForEnd = NULL;
        numberOfStrings = 0;
        return 0;
    }
    
    if (pointerForStart == tmpString){
        pointerForStart = tmpString -> next;
        tmpString -> next -> prev = NULL;
        free(tmpString);
        numberOfStrings--;
        return 0;
    }
    
    if (pointerForEnd == tmpString){
        pointerForEnd = tmpString -> prev;
        tmpString -> prev -> next = NULL;
        free(tmpString);
        numberOfStrings--;
        return 0;
    }
    
    tmpString -> next -> prev = tmpString -> prev;
    tmpString -> prev -> next = tmpString -> next;
    free(tmpString);
    numberOfStrings--;
    return 0;
}

void clearTTY(void){
    printf("\033[2J");
    printf("\033[0;0f");
}

void resetKeypress(void){
    tcsetattr(0,TCSANOW,&storedSettings);
    return;
}

void setKeypress(void){
    struct termios newSettings;
    
    tcgetattr(0,&storedSettings);
    
    newSettings = storedSettings;
    
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    newSettings.c_cc[VTIME] = 0;
    newSettings.c_cc[VMIN] = 1;
    
    tcsetattr(0,TCSANOW,&newSettings);
    return;
}

int deleteSymbol(int numOfStr, int numOfChar){
    int i;
    struct listOfStrings *tmpString, *tmpString2;
    struct listOfChars *tmpChar;
    isFileSaved = 0;
    
    if (numOfStr > numberOfStrings) return -4;
    tmpString = pointerForStart;
    
    for (i = 0; i < numOfStr-1; i++){
        tmpString = tmpString -> next;
    }
    
    if (numOfChar > tmpString -> curStringSize){
        if (numOfStr == numberOfStrings)
            return -4;
        if (tmpString -> curStringSize == 0) {
            if (pointerForStart == tmpString){
                pointerForStart = tmpString -> next;
                pointerForStart -> prev = NULL;
                free(tmpString);
                numberOfStrings--;
                return 0;
            }
            tmpString -> prev -> next = tmpString -> next;
            tmpString -> next -> prev = tmpString -> prev;
            free(tmpString);
            numberOfStrings--;
            return 0;
        }
        tmpString -> lastChar -> next = tmpString -> next -> firstChar;
        
        if (tmpString -> next -> firstChar != NULL){
            tmpString -> next -> firstChar -> prev = tmpString -> lastChar;
        }
        
        if (tmpString -> next -> lastChar != NULL){
            tmpString -> lastChar = tmpString -> next -> lastChar;
        }
        
        tmpString -> curStringSize += tmpString -> next -> curStringSize;
        tmpString2 = tmpString -> next;
        tmpString -> next = tmpString -> next -> next;
        
        if (pointerForEnd == tmpString2){
            pointerForEnd = tmpString;
        }
        
        if (tmpString -> next != NULL){
            tmpString -> next -> prev = tmpString;
        }
        
        free(tmpString2);
        numberOfStrings--;
        return 0;
    }
    
    tmpChar = tmpString -> firstChar;
    for (i = 0; i < numOfChar-1; i++){
        tmpChar = tmpChar -> next;
    }
    if (tmpChar == tmpString -> firstChar){
        tmpString -> curStringSize--;
        tmpString -> firstChar = tmpChar -> next;
        if (tmpString -> lastChar == tmpChar){
            tmpString -> lastChar = tmpChar -> prev;
        }
        if (tmpChar -> next != NULL){
            tmpChar -> next -> prev = NULL;
        }
        free(tmpChar);
        return 0;
    }
    if (tmpChar == tmpString -> lastChar){
        tmpString -> curStringSize--;
        tmpString -> lastChar = tmpChar -> prev;
        if (tmpString -> firstChar == tmpChar){
            tmpString -> firstChar = tmpChar -> next;
        }
        if (tmpChar -> prev != NULL){
            tmpChar -> prev -> next = NULL;
        }
        free(tmpChar);
        return 0;
    }
    
    tmpString -> curStringSize--;
    tmpChar -> prev -> next = tmpChar -> next;
    tmpChar -> next -> prev = tmpChar -> prev;
    free(tmpChar);
    
    return 0;
}

int replace(int *i, int *j,int size, char* s1, int size_s1){
    int k, res = 0;
    char c;
    
    isFileSaved = 0;
    
    *j -= size;
    for (k = 0; k < size; k++){
        deleteSymbol(*i, *j);
    }
    for (k = 0; k < size_s1; k++)
    {
        c = s1[k];
        if (c == '\n')
        {
            insertSymbol(*i, *j, '\n');
            *j = 1;
            *i = *i + 1;
            res++;
        }
        else
        {
            insertSymbol(*i, *j, c);
            *j = *j + 1;
        }
    }
    return res;
    
}

void assistFunctionForDeleteBraces(int *i, int *j){
    int k, countOfBraces = 0;
    char c;
    struct listOfStrings *tmpString;
    struct listOfChars *tmpChar;
    
    tmpString = pointerForStart;
    
    for (k = 0; k < *i-1; k++){
        tmpString = tmpString  ->  next;
    }
    
    tmpChar = tmpString -> firstChar;
    
    for (k = 0; k < *j-1; k++){
        tmpChar = tmpChar -> next;
    }
    
    while (1){
        if ((*i == numberOfStrings) && (*j > tmpString -> curStringSize)){
            break;
        }
        
        if (*j > tmpString -> curStringSize){
            deleteSymbol(*i,*j);
            tmpChar = getChar(*i, *j);
            tmpString = pointerForStart;
            
            for (k = 0; k < *i-1; k++){
                tmpString = tmpString -> next;
            }
        }
        
        if (tmpChar == NULL){
            while (tmpChar == NULL){
                if ((*i == numberOfStrings) && (*j > tmpString -> curStringSize)){
                    *j = *j -1;
                    return;
                }
                deleteSymbol(*i,*j);
                tmpChar = getChar(*i, *j);
                tmpString = pointerForStart;
                
                for (k = 0; k < *i-1; k++){
                    tmpString = tmpString -> next;
                }
            }
        }
        c = tmpChar -> curChar;
        
        switch (c){
            case '{': {
                countOfBraces++;
                deleteSymbol(*i, *j);
                tmpChar = getChar(*i, *j);
                break;
            }
                
            case '}': {
                countOfBraces--;
                deleteSymbol(*i, *j);
                tmpChar = getChar(*i, *j);
                break;
            }
                
            default:{
                deleteSymbol(*i, *j);
                tmpChar = getChar(*i, *j);
                break;
            }
        }
        
        if (countOfBraces == 0){
            break;
        }
    }
    
    *j = *j -1;
}

int commandReading(void){
    
    int size1 = 0, size2 = 0, count = 0;
    char temp = 0, tempPrev = 0, temp2 = 0;
    char parametr3[20] = "0";
    char parametr4[20] = "0";
    
    firstParametr = NULL;
    secondParametr = NULL;
    
    
    if (isatty(0)) fprintf(stderr, "Editor: ");
    
    temp = getchar();
    
    if (temp == EOF) {
        EOFFlag = 1;
        return 666;
    }
    while ((temp == ' ') || (temp == '\t')){
        temp = getchar();
    }
    
    if ((temp == '\n') || (temp == '#')){
        clearBufffer(temp);
        return -10;
    }
    
    ungetc(temp, stdin);
    temp2 = readCommand();
    
    if (temp2 == -1){
        free(userString);
        return exitForce();
    }
    
    ungetc(temp2, stdin);
    
    if (!strcmp(userString, "exit")){
        
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return exitF();
        }
        
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(1);
        
        if (userString == NULL) return exitForce();
        
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        
        if (temp2 == -1){
            free(userString);
            return exitForce();
        }
        
        ungetc(temp2, stdin);
        if (!strcmp(userString, "force")){
            temp= getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return exitForce();
            }
        }
        
        clearBufffer(temp2);
        return -1;
    }
    
    if (!strcmp(userString, "read")){
        temp = getchar();
        
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if (temp != '"'){
            clearBufffer(temp);
            return -1;
        }
        
        size1 = 0;
        
        while (1){
            temp = getchar();
            
            if ((temp == '"') && (tempPrev == '\\')){
                tempPrev = '"';
                firstParametr[size1 - 1] = '"';
                continue;
            }
            
            if ((temp == '\\') && (tempPrev == '\\')){
                tempPrev = 0;
                continue;
            }
            
            if ((temp == 'n') && (tempPrev == '\\')){
                tempPrev = '\n';
                firstParametr[size1-1] = '\n';
                continue;
            }
            
            if ((temp == 'r') && (tempPrev == '\\')){
                tempPrev = '\r';
                firstParametr[size1-1] = '\r';
                continue;
            }
            
            if ((temp == 't') && (tempPrev == '\\')){
                tempPrev = '\t';
                firstParametr[size1-1] = '\t';
                continue;
            }
            
            if ((temp == '"') && (tempPrev != '\\')) break;
            
            if (temp == '\n') {
                clearBufffer(temp);
                return -1;
            }
            
            size1 = addCharToParametr(&firstParametr, size1, temp);
            
            if (size1 == -1){
                free(firstParametr);
                free(userString);
                return exitForce();
            }
            tempPrev = temp;
        }
        
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp != '\n') && (temp != '#')){
            clearBufffer(temp);
            return -1;
        }
        
        clearBufffer(temp);
        return readF(firstParametr, size1);
    }
    
    if (!strcmp(userString, "open")){
        temp = getchar();
        
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if (temp != '"'){
            clearBufffer(temp);
            return -1;
        }
        
        size1 = 0;
        
        while (1){
            temp = getchar();
            
            if ((temp == '"') && (tempPrev == '\\')){
                tempPrev = '"';
                firstParametr[size1 - 1] = '"';
                continue;
            }
            
            if ((temp == '\\') && (tempPrev == '\\')){
                tempPrev = 0;
                continue;
            }
            
            if ((temp == 'n') && (tempPrev == '\\')){
                tempPrev = '\n';
                firstParametr[size1-1] = '\n';
                continue;
            }
            
            if ((temp == 'r') && (tempPrev == '\\')){
                tempPrev = '\r';
                firstParametr[size1-1] = '\r';
                continue;
            }
            
            if ((temp == 't') && (tempPrev == '\\')){
                tempPrev = '\t';
                firstParametr[size1-1] = '\t';
                continue;
            }
            
            if ((temp == '"') && (tempPrev != '\\')) break;
            
            if (temp == '\n'){
                clearBufffer(temp);
                return -1;
            }
            
            size1 = addCharToParametr(&firstParametr, size1, temp);
            
            if (size1 == -1){
                free(firstParametr);
                free(userString);
                return exitForce();
            }
            
            tempPrev = temp;
        }
        
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp != '\n') && (temp != '#')){
            clearBufffer(temp);
            return -1;
        }
        
        clearBufffer(temp);
        return openF(firstParametr,size1);
    }
    
    if (!strcmp(userString, "write")){
        temp = getchar();
        
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return writeF("", size1);
        }
        
        if (temp != '"'){
            clearBufffer(temp);
            return -1;
        }
        
        size1 = 0;
        while (1){
            temp = getchar();
            
            if ((temp == '"') && (tempPrev == '\\')){
                tempPrev = '"';
                firstParametr[size1 - 1] = '"';
                continue;
            }
            
            if ((temp == '\\') && (tempPrev == '\\')){
                tempPrev = 0;
                continue;
            }
            
            if ((temp == 'n') && (tempPrev == '\\')){
                tempPrev = '\n';
                firstParametr[size1-1] = '\n';
                continue;
            }
            
            if ((temp == 'r') && (tempPrev == '\\')){
                tempPrev = '\r';
                firstParametr[size1-1] = '\r';
                continue;
            }
            
            if ((temp == 't') && (tempPrev == '\\')){
                tempPrev = '\t';
                firstParametr[size1-1] = '\t';
                continue;
            }
            
            if ((temp == '"') && (tempPrev != '\\')) break;
            
            if (temp == '\n') {
                clearBufffer(temp);
                return -1;
            }
            
            
            size1 = addCharToParametr(&firstParametr, size1, temp);
            
            if (size1 == -1){
                free(firstParametr);
                free(userString);
                return exitForce();
            }
            
            tempPrev = temp;
        }
        
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp != '\n') && (temp != '#')){
            clearBufffer(temp);
            return -1;
        }
        
        clearBufffer(temp);
        return writeF(firstParametr, size1);
    }
    
    if (!strcmp(userString, "help")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp != '\n') && (temp != '#')){
            clearBufffer(temp);
            return -1;
        }
        
        clearBufffer(temp);
        return help();
    }
    
    if (!strcmp(userString, "set")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {
            temp = getchar();
        }
        
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return -1;
        }
        
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(1);
        
        if (userString == NULL) return exitForce();
        
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        
        if (temp2 == -1){
            free(userString);
            return exitForce();
        }
        
        ungetc(temp2, stdin);
        if (!strcmp(userString, "tabwidth")){
            temp = getchar();
            
            while ((temp == ' ') || (temp == '\t')) {
                temp = getchar();
            }
            
            ungetc(temp, stdin);
            free(userString);
            userString = malloc(1);
            
            if (userString == NULL) return exitForce();
            
            userString[0] = '\0';
            sizeOfCmd = 1;
            temp2 = readCommand();
            
            if (temp2 == -1){
                free(userString);
                return exitForce();
            }
            ungetc(temp2, stdin);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return setTabWidth(atoi(userString));
        }
        
        if (!strcmp(userString, "wrap"))
        {
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            free(userString);
            userString = malloc(sizeof(char));
            if (userString == NULL)
                return exitForce();
            userString[0] = '\0';
            sizeOfCmd = 1;
            temp2 = readCommand();
            if (temp2 == -1){
                free(userString);
                return exitForce();
            }
            ungetc(temp2, stdin);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            if (!strcmp(userString, "yes"))
                return setWrap(1);
            if (!strcmp(userString, "no"))
                return setWrap(0);
            
            clearBufffer(temp);
            return -1;
        }
        
        if (!strcmp(userString, "name"))
        {
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return setName("", 0);
            }
            if (temp != '"')
            {
                clearBufffer(temp);
                return -1;
            }
            
            size1 = 0;
            while (1){
                temp = getchar();
                if ((temp == '"') && (tempPrev == '\\')){
                    tempPrev = '"';
                    firstParametr[size1 - 1] = '"';
                    continue;
                }
                
                if ((temp == '\\') && (tempPrev == '\\')){
                    tempPrev = 0;
                    continue;
                }
                
                if ((temp == 'n') && (tempPrev == '\\')){
                    tempPrev = '\n';
                    firstParametr[size1-1] = '\n';
                    continue;
                }
                
                if ((temp == 'r') && (tempPrev == '\\')){
                    tempPrev = '\r';
                    firstParametr[size1-1] = '\r';
                    continue;
                }
                
                if ((temp == 't') && (tempPrev == '\\')){
                    tempPrev = '\t';
                    firstParametr[size1-1] = '\t';
                    continue;
                }
                
                if ((temp == '"') && (tempPrev != '\\')) break;
                
                if (temp == '\n'){
                    clearBufffer(temp);
                    return -1;
                }

                size1 = addCharToParametr(&firstParametr, size1, temp);
                
                if (size1 == -1){
                    free(firstParametr);
                    free(userString);
                    return exitForce();
                }
                tempPrev = temp;
            }
            
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            
            if ((temp != '\n') && (temp != '#')){
                clearBufffer(temp);
                return -1;
            }
            
            clearBufffer(temp);
            return setName(firstParametr,size1);
        }
        
        clearBufffer(temp2);
        return -1;
    }
    
    if (!strcmp(userString, "print")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return -1;
        }
        
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(sizeof(char));
        
        if (userString == NULL) return exitForce();
        
        userString[0] = '\0';
        sizeOfCmd = 1;
        
        temp2 = readCommand();
        
        if (temp2 == -1){
            free(userString);
            return exitForce();
        }
        
        ungetc(temp2, stdin);
        if (!strcmp(userString, "pages")){
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#')){
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return printPages();
        }
        
        if (!strcmp(userString, "range")){
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return printPages();
            }
            
            ungetc(temp, stdin);
            scanf("%s", parametr3);
            temp = getchar();
            
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return printRange(atoi(parametr3), -1);
            }
            
            ungetc(temp, stdin);
            scanf("%s", parametr4);
            temp = getchar();
            
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#')){
                clearBufffer(temp);
                return -1;
            }
            
            clearBufffer(temp);
            return printRange(atoi(parametr3), atoi(parametr4));
        }
        
        clearBufffer(temp2);
        return -1;
    }
    
    if (!strcmp(userString, "delete")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return -1;
        }
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(sizeof(char));
        if (userString == NULL) return exitForce();
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        
        if (temp2 == -1){
            free(userString);
            return exitForce();
        }
        
        ungetc(temp2, stdin);
        if (!strcmp(userString, "braces")){
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#')){
                clearBufffer(temp);
                return -1;
            }
            
            clearBufffer(temp);
            return deleteBraces();
        }
        
        if (!strcmp(userString, "range")){
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            scanf("%s", parametr3);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return deleteRange(atoi(parametr3), -1);
            }
            
            ungetc(temp, stdin);
            scanf("%s", parametr4);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#')){
                clearBufffer(temp);
                return -1;
            }
            
            clearBufffer(temp);
            return deleteRange(atoi(parametr3), atoi(parametr4));
        }
        
        clearBufffer(temp2);
        return -1;
    }

    if (!strcmp(userString, "edit")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return -1;
        }
        
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(sizeof(char));
        if (userString == NULL) return exitForce();
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        if (temp2 == -1)
        {
            free(userString);
            return exitForce();
        }
        ungetc(temp2, stdin);
        if (!strcmp(userString, "string"))
        {
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            scanf("%s", parametr3);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            scanf("%s", parametr4);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return -1;
            }
            if (temp == '\\'){
                tempPrev = temp;
                temp = getchar();
                switch (temp) {
                    case 'n':
                        temp = '\n';
                        break;
                    case 'r':
                        temp = '\r';
                        break;
                    case 't':
                        temp = '\t';
                        break;
                    default:
                        temp = 0;
                        break;
                }
            }
            if (temp == 0)
            {
                clearBufffer(temp);
                return -1;
            }
            tempPrev = temp;
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return editString(atoi(parametr3), atoi(parametr4),tempPrev);
        }
        
        clearBufffer(temp2);
        return -1;
    }

    if (!strcmp(userString, "insert")){
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        if ((temp == '\n') || (temp == '#')){
            clearBufffer(temp);
            return -1;
        }
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(sizeof(char));
        if (userString == NULL)
            return exitForce();
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        if (temp2 == -1){
            free(userString);
            return exitForce();
        }
        ungetc(temp2, stdin);
        if (!strcmp(userString, "symbol")){
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            scanf("%s", parametr3);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return -1;
            }
            ungetc(temp, stdin);
            scanf("%s", parametr4);
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#')){
                clearBufffer(temp);
                return -1;
            }
            if (temp == '\\'){
                tempPrev = temp;
                temp = getchar();
                switch (temp) {
                    case 'n':
                        temp = '\n';
                        break;
                    case 'r':
                        temp = '\r';
                        break;
                    case 't':
                        temp = '\t';
                        break;
                    default:
                        temp = 0;
                        break;
                }
            }
            if (temp == 0){
                clearBufffer(temp);
                return -1;
            }
            tempPrev = temp;
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return insertSymbol(atoi(parametr3), atoi(parametr4),tempPrev);
        }
        
        if (!strcmp(userString, "after"))
        {
            parametr3[0] = '-';
            parametr3[1] = '1';
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            if (temp != '"')
            {
                ungetc(temp, stdin);
                scanf("%s", parametr3);
                temp = getchar();
                while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
                if ((temp == '\n') || (temp == '#'))
                {
                    clearBufffer(temp);
                    return -1;
                }
            }
            if (temp != '"')
            {
                clearBufffer(temp);
                return -1;
            }
            temp= getchar();
            if (temp != '"')
            {
                size1 = 0;
                size1 = addCharToParametr(&firstParametr, size1, temp);
                if (size1 == -1)
                {
                    free(firstParametr);
                    free(userString);
                    return exitForce();
                }
                tempPrev = temp;
                while (1)
                {
                    temp = getchar();
                    if ((temp == '"') && (tempPrev == '\\'))
                    {
                        tempPrev = '"';
                        firstParametr[size1 - 1] = '"';
                        continue;
                    }
                    if ((temp == '\\') && (tempPrev == '\\'))
                    {
                        tempPrev = 0;
                        continue;
                    }
                    if ((temp == 'n') && (tempPrev == '\\'))
                    {
                        tempPrev = '\n';
                        firstParametr[size1-1] = '\n';
                        continue;
                    }
                    if ((temp == 'r') && (tempPrev == '\\'))
                    {
                        tempPrev = '\r';
                        firstParametr[size1-1] = '\r';
                        continue;
                    }
                    if ((temp == 't') && (tempPrev == '\\'))
                    {
                        tempPrev = '\t';
                        firstParametr[size1-1] = '\t';
                        continue;
                    }
                    if ((temp == '"') && (tempPrev != '\\'))
                        break;
                    if (temp == '\n')
                    {
                        clearBufffer(temp);
                        return -1;
                    }
                    size1 = addCharToParametr(&firstParametr, size1, temp);
                    if (size1 == -1)
                    {
                        free(firstParametr);
                        free(userString);
                        return exitForce();
                    }
                    tempPrev = temp;
                }
            }
            else
            {
                temp = getchar();
                if (temp != '"')
                {
                    clearBufffer(temp);
                    return -1;
                }
                
                size1 = 0;
                while (1) {
                    temp = getchar();
                    if ((temp == '"') && (tempPrev == '\\'))
                    {
                        tempPrev = '"';
                        firstParametr[size1 - 1] = '"';
                        continue;
                    }
                    if ((temp == '\\') && (tempPrev == '\\'))
                    {
                        tempPrev = 0;
                        continue;
                    }

                    if ((temp == 'r') && (tempPrev == '\\'))
                    {
                        tempPrev = '\r';
                        firstParametr[size1-1] = '\r';
                        continue;
                    }
                    if ((temp == 't') && (tempPrev == '\\'))
                    {
                        tempPrev = '\t';
                        firstParametr[size1-1] = '\t';
                        continue;
                    }
                    if ((temp == '"') && (tempPrev != '\\'))
                    {
                        count++;
                        if (count == 3)
                        {
                            size1 -= 2;
                            break;
                        }
                    }
                    else
                        count = 0;
                    size1 = addCharToParametr(&firstParametr, size1, temp);
                    if (size1 == -1)
                    {
                        free(firstParametr);
                        free(userString);
                        return exitForce();
                    }
                    tempPrev = temp;
                }
            }
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return insertAfter(atoi(parametr3), firstParametr, size1);
            
        }
        clearBufffer(temp2);
        return -1;
    }

    if (!strcmp(userString, "replace"))
    {
        temp = getchar();
        while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
        if ((temp == '\n') || (temp == '#'))
        {
            clearBufffer(temp);
            return -1;
        }
        ungetc(temp, stdin);
        free(userString);
        userString = malloc(sizeof(char));
        if (userString == NULL)
            return exitForce();
        userString[0] = '\0';
        sizeOfCmd = 1;
        temp2 = readCommand();
        if (temp2 == -1)
        {
            free(userString);
            return exitForce();
        }
        ungetc(temp2, stdin);
        if (!strcmp(userString, "substring"))
        {
            temp = getchar();
            
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp == '\n') || (temp == '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            if ((temp != '"') && (temp != '$') && (temp != '^'))
            {
                ungetc(temp, stdin);
                scanf("%s", parametr3);
                
                temp = getchar();
                while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
                if (temp == '\n')
                {
                    clearBufffer(temp);
                    return -1;
                }
                if ((temp != '"') && (temp != '$') && (temp != '^'))
                {
                    ungetc(temp, stdin);
                    scanf("%s", parametr4);
                    temp = getchar();
                    while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
                    if ((temp != '"') && (temp != '$') && (temp != '^'))
                    {
                        clearBufffer(temp);
                        return -1;
                    }
                }
                else
                {
                    parametr4[0] = '-';
                    parametr4[1] = '1';
                }
            }
            else
            {
                parametr3[0] = '-';
                parametr3[1] = '1';
                parametr4[0] = '-';
                parametr4[1] = '1';
            }
            if ((temp == '$') || (temp == '^'))
            {
                switch (temp)
                {
                    case '$':
                        size1 = -1;
                        break;
                    case '^':
                        size1 = -2;
                    default:
                        break;
                }
            }
            else
            {
                size1 = 0;
                while (1) {
                    temp = getchar();
                    if ((temp == '"') && (tempPrev == '\\'))
                    {
                        tempPrev = '"';
                        /*tmp[size - 1] = '"';*/
                        firstParametr[size1 - 1] = '"';
                        continue;
                    }
                    if ((temp == '\\') && (tempPrev == '\\'))
                    {
                        tempPrev = 0;
                        continue;
                    }
                    if ((temp == 'n') && (tempPrev == '\\'))
                    {
                        tempPrev = '\n';
                        /*tmp[size -1] = '\n';*/
                        firstParametr[size1-1] = '\n';
                        continue;
                    }
                    if ((temp == 'r') && (tempPrev == '\\'))
                    {
                        tempPrev = '\r';
                        /*tmp[size -1] = '\r';*/
                        firstParametr[size1-1] = '\r';
                        continue;
                    }
                    if ((temp == 't') && (tempPrev == '\\'))
                    {
                        tempPrev = '\t';
                        /*tmp[size -1] = '\t';*/
                        firstParametr[size1-1] = '\t';
                        continue;
                    }
                    if ((temp == '"') && (tempPrev != '\\'))
                        break;
                    if (temp == '\n')
                    {
                        clearBufffer(temp);
                        return -1;
                    }
                    /*tmp[size++] = c;*/
                    size1 = addCharToParametr(&firstParametr, size1, temp);
                    if (size1 == -1)
                    {
                        free(firstParametr);
                        free(userString);
                        return exitForce();
                    }
                    tempPrev = temp;
                }
            }
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if (temp != '"')
            {
                clearBufffer(temp);
                return -1;
            }
            
            size2 = 0;
            while (1)
            {
                temp = getchar();
                if ((temp == '"') && (tempPrev == '\\'))
                {
                    tempPrev = '"';
                    /*tmp[size - 1] = '"';*/
                    secondParametr[size2 - 1] = '"';
                    continue;
                }
                if ((temp == '\\') && (tempPrev == '\\'))
                {
                    tempPrev = 0;
                    continue;
                }
                if ((temp == 'n') && (tempPrev == '\\'))
                {
                    tempPrev = '\n';
                    /*tmp[size -1] = '\n';*/
                    secondParametr[size2-1] = '\n';
                    continue;
                }
                if ((temp == 'r') && (tempPrev == '\\'))
                {
                    tempPrev = '\r';
                    /*tmp[size -1] = '\r';*/
                    secondParametr[size2-1] = '\r';
                    continue;
                }
                if ((temp == 't') && (tempPrev == '\\'))
                {
                    tempPrev = '\t';
                    /*tmp[size -1] = '\t';*/
                    secondParametr[size2-1] = '\t';
                    continue;
                }
                if ((temp == '"') && (tempPrev != '\\'))
                    break;
                if (temp == '\n') {
                    clearBufffer(temp);
                    return -1;
                }
                /*tmp[size++] = c;*/
                size2 = addCharToParametr(&secondParametr, size2, temp);
                if (size2 == -1)
                {
                    free(firstParametr);
                    free(secondParametr);
                    free(userString);
                    return exitForce();
                }
                tempPrev = temp;
            }
            temp = getchar();
            while ((temp == ' ') || (temp == '\t')) {temp = getchar();}
            if ((temp != '\n') && (temp != '#'))
            {
                clearBufffer(temp);
                return -1;
            }
            clearBufffer(temp);
            return replaceSubstring(atoi(parametr3), atoi(parametr4), firstParametr, size1, secondParametr, size2);
            
        }
        
        clearBufffer(temp2);
        return -1;
    }
    clearBufffer(temp2);
    return -2;
}