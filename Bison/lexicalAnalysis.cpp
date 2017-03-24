#include "lexicalAnalysis.hpp"
#include <iostream>
#include <cctype>
#include <cstring>

PositionOfLexeme::PositionOfLexeme(){
    column = row = 0;
}

Lexeme::Lexeme(){
    content = NULL;
}

Lexica::Lexica(){
    lexeme = NULL;
    counter = 0;
}

Lexica::~Lexica(){
    if (lexeme != NULL){
        for (unsigned int i = 0; i < counter; i++) {
            if (lexeme[i].content != NULL) free(lexeme[i].content);
        }
    }
    free(lexeme);
}

void Lexica::analyzeFile(FILE *input){
    char tmpChar = EOF, *tmpWord = NULL;
    unsigned int column = 0, i = 0;
    Lexeme tmpLexeme;
    PositionOfLexeme tmpPosition;
    
    do{
        tmpChar = getc(input);
        
        switch (tmpChar) {
            case EOF: break;
            
            case ' ':
                tmpPosition.column++;
                break;
                
            case '%':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = PERCENT;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case '*':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = STAR;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case '/':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = FORWARD_SLASH;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
            
            case '\n': tmpPosition.row++; tmpPosition.column = 0; break;
                
            case '{':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = OPEN_BRACE;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
            
            case '}':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = CLOSE_BRACE;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case '|':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = PIPE;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case ':':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = COLON;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case ';':
                tmpLexeme.position = tmpPosition;
                tmpLexeme.type = SEMICOLON;
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                push(tmpLexeme);
                tmpPosition.column++;
                break;
                
            case 't':
                column = tmpPosition.column;
                tmpWord = (char*)malloc(i + 1);
                tmpWord[i] = tmpChar;
                i++;
                while ((tmpChar != ' ') && (tmpChar != '\n') && (tmpChar != EOF) && (tmpChar != '%') && (tmpChar != '{') && (tmpChar != '}') && (tmpChar != '|') && (tmpChar != ';') && (tmpChar != ':')) {
                    tmpChar = getc(input);
                    if ((tmpChar != ' ') && (tmpChar != '\n') && (tmpChar != EOF) && (tmpChar != '%') && (tmpChar != '{') && (tmpChar != '}') && (tmpChar != '|') && (tmpChar != ';') && (tmpChar != ':')) {
                        tmpWord = (char*)realloc(tmpWord, i + 1);
                        tmpWord[i] = tmpChar;
                        i++;
                        column++;
                    }
                }
                ungetc(tmpChar, input);
                column--;
                tmpWord = (char*)realloc(tmpWord, i + 1);
                tmpWord[i] = '\0';
                if (!strcmp(tmpWord, "token")) {
                    tmpLexeme.type = TOKEN;
                    tmpLexeme.position = tmpPosition;
                    push(tmpLexeme);
                    if (tmpLexeme.content != NULL) {
                        free(tmpLexeme.content);
                        tmpLexeme.content = NULL;
                    }
                }
                else{
                    tmpLexeme.type = SOME_TEXT;
                    tmpLexeme.content = (char*)malloc(strlen(tmpWord) + 1);
                    strcpy(tmpLexeme.content, tmpWord);
                    tmpLexeme.position = tmpPosition;
                    push(tmpLexeme);
                    if (tmpLexeme.content != NULL) {
                        free(tmpLexeme.content);
                        tmpLexeme.content = NULL;
                    }
                }
                tmpPosition.column = column;
                if (tmpWord != NULL) {
                    free(tmpWord);
                    tmpWord = NULL;
                }
                i = 0;
                break;
                
            default:
                column = tmpPosition.column;
                tmpWord = (char*)malloc(i + 1);
                tmpWord[i] = tmpChar;
                i++;
                while ((tmpChar != ' ') && (tmpChar != '\n') && (tmpChar != EOF) && (tmpChar != '%') && (tmpChar != '{') && (tmpChar != '}') && (tmpChar != '|') && (tmpChar != ';') && (tmpChar != ':')) {
                    tmpChar = getc(input);
                    if ((tmpChar != ' ') && (tmpChar != '\n') && (tmpChar != EOF) && (tmpChar != '%') && (tmpChar != '{') && (tmpChar != '}') && (tmpChar != '|') && (tmpChar != ';') && (tmpChar != ':')) {
                        tmpWord = (char*)realloc(tmpWord, i + 1);
                        tmpWord[i] = tmpChar;
                        i++;
                        column++;
                    }
                }
                ungetc(tmpChar, input);
                column--;
                tmpWord = (char*)realloc(tmpWord, i + 1);
                tmpWord[i] = '\0';
                tmpLexeme.type = SOME_TEXT;
                tmpLexeme.content = (char*)malloc(strlen(tmpWord) + 1);
                strcpy(tmpLexeme.content, tmpWord);
                tmpLexeme.position = tmpPosition;
                push(tmpLexeme);
                if (tmpLexeme.content != NULL) {
                    free(tmpLexeme.content);
                    tmpLexeme.content = NULL;
                }
                
                tmpPosition.column = column;
                if (tmpWord != NULL) {
                    free(tmpWord);
                    tmpWord = NULL;
                }
                i = 0;
                break;
        }
    }while(tmpChar != EOF);}

unsigned int Lexica::getCounter(){return counter;}

Lexeme* Lexica::getLexeme(){return lexeme;}

void Lexica::push(Lexeme tmpLexeme){
    lexeme = (Lexeme*)realloc(lexeme, (counter + 1) * sizeof(Lexeme));
    lexeme[counter].type = tmpLexeme.type;
    if (tmpLexeme.content != NULL) {
        lexeme[counter].content = (char*)malloc(strlen(tmpLexeme.content) + 1);
        strcpy(lexeme[counter].content, tmpLexeme.content);
    }
    else{
        lexeme[counter].content = NULL;
    }
    lexeme[counter].position = tmpLexeme.position;
    counter++;
}

void Lexica::print(){
    for (unsigned int i = 0; i < counter; i++) {
        printf("%i", lexeme[i].type);
    }
}