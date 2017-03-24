#include "syntacticalAnalysis.hpp"
#include "lexicalAnalysis.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>

extern Lexica tmp;
SyntaxERROR errorsArray;
Terminal terminals;
Grammatic *grammatic;
bool firstPercent = false, inComment = false, inGramatic = false, endOfFile = false, tokenFlag = false, alternativesFlag = false, afterComment = false;
unsigned int counterOfPercents = 0, counterOfGrammatic = 0, tmpCounterOfAlternatives = 0;


Terminal::Terminal(){
    arrayOfTerminals = NULL;
    counter = 0;
}

Terminal::~Terminal(){
    if (arrayOfTerminals != NULL) {
        for (unsigned int i = 0; i < counter; i++) {
            free(arrayOfTerminals[i]);
        }
        free(arrayOfTerminals);
    }
}

unsigned int Terminal::getCounter(){return counter;}

void Terminal::push(char *toPush) {
    arrayOfTerminals = (char**)realloc(arrayOfTerminals, (counter + 1) * sizeof(char*));
    arrayOfTerminals[counter] = NULL;
    arrayOfTerminals[counter] = (char*)realloc(arrayOfTerminals[counter], strlen(toPush) + 1);
    strcpy(arrayOfTerminals[counter], toPush);
    counter++;
}

bool Terminal::checkTerminal(char *toCmp){
    if (strlen(toCmp) == 3) {
        if ((toCmp[0] == '\'') && (toCmp[2] == '\'')) {
            for (unsigned int i = 0; i < counter; i++) {
                if (strcmp(toCmp, arrayOfTerminals[i])) {
                    push(toCmp);
                }
            }
            return true;
        }
    }
    else {
        if (strlen(toCmp) == 4) {
            if ((toCmp[0] == '\'') && (toCmp[3] == '\'') && (toCmp[1] == '\\')) {
                for (unsigned int i = 0; i < counter; i++) {
                    if (strcmp(toCmp, arrayOfTerminals[i])) {
                        push(toCmp);
                    }
                }
                return true;
            }
        }
    }
    
    for (unsigned int i = 0; i < counter; i++) {
        if (strcmp(toCmp, arrayOfTerminals[i])) {
            return true;
        }
    }
    return false;
}


SyntaxisToken::SyntaxisToken(){
    content = NULL;
}

Syntaxis::Syntaxis(){
    tokens = NULL;
    counter = 0;
}

Syntaxis::~Syntaxis(){
    if (tokens != NULL){
        for (unsigned int i = 0; i < counter; i++) {
            if (tokens[i].content != NULL) free(tokens[i].content);
        }
    }
    free(tokens);
}


Alternative::Alternative(){
    name = NULL;
}

Alternative::~Alternative(){
    if (name != NULL) {
        free(name);
    }
}


Grammatic::Grammatic(){
    alternatives = NULL;
    countOfAlternatives = 0;
    countOfMembersInAlternative = NULL;
}

Grammatic::~Grammatic(){
    if (alternatives != NULL) {
        for (unsigned int i = 0; i < countOfAlternatives; i++) {
            free(alternatives[i]);
        }
        free(alternatives);
    }
}

void Grammatic::newAlternative(){
    alternatives = (Alternative**)realloc(alternatives, countOfAlternatives + 1);
    alternatives[countOfAlternatives] = NULL;
    countOfAlternatives++;
    countOfMembersInAlternative = 0;
}

void Grammatic::pushToAlternatives(char *toPush){
    
    alternatives[countOfAlternatives] = (Alternative*)realloc(alternatives[countOfAlternatives], (countOfMembersInAlternative[countOfAlternatives] + 1) * sizeof(Alternative));
    alternatives[countOfAlternatives][countOfMembersInAlternative[countOfAlternatives]].name = (char*)realloc(alternatives[countOfAlternatives][countOfMembersInAlternative[countOfAlternatives]].name, strlen(toPush) + 1);
    strcpy(alternatives[countOfAlternatives][countOfMembersInAlternative[countOfAlternatives]].name, toPush);
    
    if (terminals.checkTerminal(toPush)) {
        alternatives[countOfAlternatives][countOfMembersInAlternative[countOfAlternatives]].type = NOT_TERMINAL;
    }
    else alternatives[countOfAlternatives][countOfMembersInAlternative[countOfAlternatives]].type = TERMINAL;
}

void Grammatic::setCountOfMembersInAlternative(unsigned int toSet){
    countOfMembersInAlternative = (unsigned int*)realloc(countOfMembersInAlternative, countOfAlternatives * sizeof(unsigned int));
    countOfMembersInAlternative[countOfAlternatives] = toSet;
}

void Syntaxis::push(SyntaxisToken tmpToken){
    tokens = (SyntaxisToken*)realloc(tokens, (counter + 1) * sizeof(Lexeme));
    tokens[counter].type = tmpToken.type;
    if (tmpToken.content != NULL) {
        tokens[counter].content = (char*)malloc(strlen(tmpToken.content) + 1);
        strcpy(tokens[counter].content, tmpToken.content);
    }
    counter++;
}

unsigned int Syntaxis::parsing(){
    unsigned int counterOfInputLexica = 0;
    
    while (counterOfInputLexica < tmp.counter) {
        switch (tmp.lexeme[counterOfInputLexica].type) {
                
            case PERCENT:
                counterOfInputLexica = analyzePercent(counterOfInputLexica);
                break;
                
            case FORWARD_SLASH:
                counterOfInputLexica = analyzeForwardSlash(counterOfInputLexica);
                break;
                
            case SOME_TEXT:
                if (inGramatic) {
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                }
                else {
                    errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
                    counterOfInputLexica++;
                }
                break;
                
            case OPEN_BRACE:
                if (inGramatic) {
                    counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
                }
                else{
                    errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
                    counterOfInputLexica++;
                }
                
            default:
                errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
                counterOfInputLexica++;
                break;
        }
        if (counterOfInputLexica == -1) {
            //обработать конец файла
        }
        if(endOfFile) return 0;
        firstPercent = false;
        inComment = false;
        tokenFlag = false;
        afterComment = false;
        alternativesFlag = false;
    }
    
    return 0;
}

unsigned int Syntaxis::analyzePercent(unsigned int counterOfInputLexica){
    counterOfInputLexica++;
    switch (tmp.lexeme[counterOfInputLexica].type) {
            
        case PERCENT:
            if (inGramatic) endOfFile = true;
            else inGramatic = true;
            counterOfInputLexica++;
            break;
            
        case TOKEN:
            if (inGramatic) {
                errorsArray.push(TOKEN_IN_GRAMATIC, tmp.lexeme[counterOfInputLexica].position);
                counterOfInputLexica++;
            }
            else{
                counterOfInputLexica = analyzeToken(counterOfInputLexica);
            }
            break;
            
        case OPEN_BRACE:
            counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
            break;
            
        default:
            break;
    }
    
    return counterOfInputLexica;
}

unsigned int Syntaxis::analyzeToken(unsigned int counterOfInputLexica){
    counterOfInputLexica++;
    switch (tmp.lexeme[counterOfInputLexica].type) {
        case SOME_TEXT:
            tokenFlag = true;
            counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
            break;
            
        default:
            errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
            counterOfInputLexica++;
            break;
    }
    
    return counterOfInputLexica;
}

unsigned int Syntaxis::analyzeOpenBrace(unsigned int counterOfInputLexica){
    afterComment = true;
    
    if (inGramatic) {
        do{
            counterOfInputLexica++;
            if (counterOfInputLexica < tmp.counter) {
                switch (tmp.lexeme[counterOfInputLexica].type) {
                        
                    case CLOSE_BRACE:
                        counterOfInputLexica++;
                        return counterOfInputLexica;
                        
                    default:
                        break;
                }
            }
        }while(counterOfInputLexica < tmp.counter);
    }
    else{
        do{
            counterOfInputLexica++;
            if (counterOfInputLexica + 1 < tmp.counter) {
                switch (tmp.lexeme[counterOfInputLexica].type) {
                        
                    case PERCENT:
                        if (tmp.lexeme[counterOfInputLexica + 1].type == CLOSE_BRACE) {
                            counterOfInputLexica += 2;
                            return counterOfInputLexica;
                        }
                        
                    default:
                        break;
                }
            }
        }while(counterOfInputLexica + 1 < tmp.counter);
    }
    
    errorsArray.push(EXPCOMMENTCLOSE, tmp.lexeme[counterOfInputLexica].position);
    
    return -1; // при окончании файла
}

unsigned int Syntaxis::analyzePipe(unsigned int counterOfInputLexica){
    
    counterOfInputLexica++;
    switch (tmp.lexeme[counterOfInputLexica].type) {
        case OPEN_BRACE:
            counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
            counterOfInputLexica--;
            counterOfInputLexica = analyzePipe(counterOfInputLexica);
            break;
            
        case SEMICOLON:
            return counterOfInputLexica++;
            break;
            
        case SOME_TEXT:
            afterComment = false;
            counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
            break;
            
        default:
            errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
            break;
    }
    
    return counterOfInputLexica;
}

unsigned int Syntaxis::analyzeColon(unsigned int counterOfInputLexica){
    counterOfInputLexica++;
    alternativesFlag = true;
    
    switch (tmp.lexeme[counterOfInputLexica].type) {
        case OPEN_BRACE:
            counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
            counterOfInputLexica--;
            counterOfInputLexica = analyzeColon(counterOfInputLexica);
            break;
        
        case FORWARD_SLASH:
            counterOfInputLexica = analyzeForwardSlash(counterOfInputLexica);
            counterOfInputLexica--;
            counterOfInputLexica = analyzeColon(counterOfInputLexica);
            break;
            
        case PIPE:
            counterOfInputLexica = analyzePipe(counterOfInputLexica);
            break;
            
        case SEMICOLON:
            counterOfInputLexica = analyzeSemicolon(counterOfInputLexica);
            break;
            
        case SOME_TEXT:
            counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
            break;
            
        default:
            errorsArray.push(UNDEFINED, tmp.lexeme[counterOfInputLexica].position);
            break;
    }
    
    return counterOfInputLexica;
}

unsigned int Syntaxis::analyzeSemicolon(unsigned int counterOfInputLexica){
    return ++counterOfInputLexica;
}

unsigned int Syntaxis::analyzeSomeText(unsigned int counterOfInputLexica){
    
    if (tokenFlag) {
        terminals.push(tmp.lexeme[counterOfInputLexica].content);
        
        counterOfInputLexica++;
        switch (tmp.lexeme[counterOfInputLexica].type) {
            case SEMICOLON:
                counterOfInputLexica = analyzeSemicolon(counterOfInputLexica);
                return counterOfInputLexica;
            
            default:
                errorsArray.push(EXPSEMICOLON, tmp.lexeme[counterOfInputLexica].position);
                return counterOfInputLexica;
        }
    }
    else{
        if (alternativesFlag) {
            counterOfInputLexica++;
            switch (tmp.lexeme[counterOfInputLexica].type) {
                case OPEN_BRACE:
                    counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
                    counterOfInputLexica--;
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                    break;
                    
                case FORWARD_SLASH:
                    counterOfInputLexica = analyzeForwardSlash(counterOfInputLexica);
                    counterOfInputLexica--;
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                    break;
                    
                case PIPE:
                    counterOfInputLexica = analyzePipe(counterOfInputLexica);
                    break;
                    
                case SEMICOLON:
                    counterOfInputLexica = analyzeSemicolon(counterOfInputLexica);
                    break;
                    
                case SOME_TEXT:
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                    break;
                    
                default:
                    break;
            }
        }
        else{
            counterOfInputLexica++;
            switch (tmp.lexeme[counterOfInputLexica].type) {
                case OPEN_BRACE:
                    counterOfInputLexica = analyzeOpenBrace(counterOfInputLexica);
                    counterOfInputLexica--;
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                    break;
                    
                case FORWARD_SLASH:
                    counterOfInputLexica = analyzeForwardSlash(counterOfInputLexica);
                    counterOfInputLexica--;
                    counterOfInputLexica = analyzeSomeText(counterOfInputLexica);
                    break;
                    
                case COLON:
                    counterOfInputLexica = analyzeColon(counterOfInputLexica);
                    break;
                    
                    
                default:
                    errorsArray.push(EXPSEMICOLON, tmp.lexeme[counterOfInputLexica].position);
                    return counterOfInputLexica;
                    break;
            }
        }
    }
    return counterOfInputLexica;
}

unsigned int Syntaxis::analyzeForwardSlash(unsigned int counterOfInputLexica){
    unsigned int slashRow = 0;
    afterComment = true;
    
    counterOfInputLexica++;
    switch (tmp.lexeme[counterOfInputLexica].type) {
        case STAR:
            do{
                counterOfInputLexica++;
                if (counterOfInputLexica + 1 < tmp.counter) {
                    switch (tmp.lexeme[counterOfInputLexica].type) {
                            
                        case STAR:
                            if (tmp.lexeme[counterOfInputLexica + 1].type == FORWARD_SLASH) {
                                counterOfInputLexica += 2;
                                return counterOfInputLexica;
                            }
                            
                        default:
                            break;
                    }
                }
            }while(counterOfInputLexica + 1 < tmp.counter);
            errorsArray.push(EXPCOMMENTCLOSE, tmp.lexeme[counterOfInputLexica].position);
            return -1;
            
        case FORWARD_SLASH:
            slashRow = tmp.lexeme[counterOfInputLexica].position.row;
            while (1) {
                counterOfInputLexica++;
                if (tmp.lexeme[counterOfInputLexica + 1].position.row != slashRow) {
                    return counterOfInputLexica;
                }
            }
            break;
            
        default:
            errorsArray.push(EXPCOMMENTOPEN, tmp.lexeme[counterOfInputLexica].position);
            return counterOfInputLexica++;
    }
}