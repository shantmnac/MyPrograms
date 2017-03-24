#ifndef lexicalAnalysis_hpp
#define lexicalAnalysis_hpp

#include <stdio.h>

enum LexemesTypes{PERCENT, TOKEN, OPEN_BRACE, CLOSE_BRACE, PIPE, COLON, SEMICOLON, SOME_TEXT, STAR, FORWARD_SLASH};

struct PositionOfLexeme{
    unsigned int column, row;
    
    PositionOfLexeme();
};

struct Lexeme{
    LexemesTypes type;
    char *content;
    PositionOfLexeme position;
    
    Lexeme();
};

class Lexica{
public:
    Lexeme *lexeme;
    unsigned int counter;

    Lexica();
    
    ~Lexica();
    
    void analyzeFile(FILE*);
    unsigned int getCounter();
    Lexeme* getLexeme();
    void push(Lexeme);
    void print();
    void deleteBrackets(unsigned int);
};

#endif