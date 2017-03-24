#include "otherFunctions.hpp"
#include "textViewer.hpp"
#include <iostream>
#include <cstring>

extern char *fileName, markedListSymbol;
extern unsigned int listIndent, screenWidth, redLineSize;
extern char *optarg;

void printHelp(){
    printf("-f - имя файла с разметкой\n-w - ширина экрана\n-t - отступ для второй и последующих строк списка (число пробелов)\n-m - символ-маркер ненумерованного списка\n-r - величина красной строки\n-h - выдача help с описанием поведения программы\n-v - отображение версии программы\nПараметры по ключам -w,-t,-m,-r - не обязательны. Если параметр не указан, то задаётся значение по умолчанию.\n");
}
           
void analizeArgument(char parametr){
    char *tmpStr = NULL;
    
    if (optarg[0] == '=') {
        tmpStr = (char*)malloc(strlen(optarg));
        tmpStr[strlen(optarg) - 1] = '\0';
        for (unsigned int i = 1; i < strlen(optarg); i++) {
            tmpStr[i - 1] = optarg[i];
        }
        
    }
    else {
        tmpStr = (char*)malloc(strlen(optarg) + 1);
        strcpy(tmpStr, optarg);
    }
    
    switch (parametr) {
        case 'f': fileName = (char*)malloc(strlen(tmpStr) + 1); strcpy(fileName, tmpStr); break;
        case 'w': screenWidth = atoi(tmpStr); break;
        case 't': listIndent = atoi(tmpStr); break;
        case 'm': markedListSymbol = tmpStr[0]; break;
        case 'r': redLineSize = atoi(tmpStr); break;
    }
    if (tmpStr != NULL) free(tmpStr);
}

unsigned int wordLength(char *str, unsigned int position){
    unsigned int i = position, length = 0;
    
    while ((str[i] != ' ') && (str[i] != '\n') && (str[i] != '\0')) {
        if ((str[i] == '\xd0') || (str[i] == '\xd1')) i += 2;
        else i++;
        length++;
    }
    
    return length;
}