#ifndef errors_hpp
#define errors_hpp

#include "lexicalAnalysis.hpp"
#include <stdio.h>

enum ErrorTypes{UNDEFINED, EXPSEMICOLON, EXPCOLON, TOKEN_IN_GRAMATIC, EXPCOMMENTCLOSE, EXPCOMMENTOPEN};

struct ErrorStruct{
    PositionOfLexeme position;
    ErrorTypes type;
};

class SyntaxERROR{
private:
    ErrorStruct *errors;
    unsigned int counter;
    
public:
    SyntaxERROR();
    
    ~SyntaxERROR();
    
    void push(ErrorTypes, PositionOfLexeme);
    void print();
};

#endif
