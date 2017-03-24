#include "errors.hpp"
#include "lexicalAnalysis.hpp"
#include <iostream>

SyntaxERROR::SyntaxERROR(){
    errors = NULL;
    counter = 0;
}

SyntaxERROR::~SyntaxERROR(){
    if (errors != NULL) {
        free(errors);
    }
}

void SyntaxERROR::push(ErrorTypes type, PositionOfLexeme position){
    errors = (ErrorStruct*)realloc(errors, (counter + 1) * sizeof(ErrorStruct));
    errors[counter].position = position;
    errors[counter].type = type;
    counter++;
}

void SyntaxERROR::print(){
    unsigned int i = 0;
    for(i = 0; i < counter; i++){
        fprintf(stderr, "Ошибка в строке %i столбце %i", errors[i].position.row + 1, errors[i].position.column + 1);
        switch (errors[i].type) {
            case UNDEFINED:
                fprintf(stderr, ": встречено недопустимое слово\n");
                break;
                
            case EXPCOLON:
                fprintf(stderr, ": ожидалось двоеточие\n");
                break;
                
            case EXPSEMICOLON:
                fprintf(stderr, ": ожидалась точка с запятой\n");
                break;
                
            case TOKEN_IN_GRAMATIC:
                fprintf(stderr, ": встречено недопустимое слово\n");
                break;
                
            case EXPCOMMENTCLOSE:
                fprintf(stderr, ": ожидалось закрытие комментария\n");
                break;
            
            case EXPCOMMENTOPEN:
                fprintf(stderr, ": ожидалось открытие комментария\n");
                break;
            
            default:
                break;
        }
    }
    if (i == 0) fprintf(stderr, "Ошибок нет\n");
}