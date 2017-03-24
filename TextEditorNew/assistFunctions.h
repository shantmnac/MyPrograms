#ifndef assistFunctions_h
#define assistFunctions_h

#include <stdio.h>
#include "structs.h"

void initilization(void);
void clearBufffer(char);
int addCharToParametr(char**, int, char);
char readCommand(void);
void freeMemory(void);
void addString(void);
void addCharToString(struct listOfStrings*, char);
void initFile(FILE*);
void assistFunctionForWriteF(FILE*);
struct listOfChars* getChar(int, int);
int updateBuffer(int);
void printBuffer(int numOfStr);
void freeBuffer(void);
int deleteString(int);
void clearTTY(void);
void resetKeypress(void);
void setKeypress(void);
int deleteSymbol(int, int);
int replace(int *, int *, int, char*, int);
void assistFunctionForDeleteBraces(int*, int*);
int commandReading(void);

#endif
