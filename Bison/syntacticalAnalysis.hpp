#ifndef syntacticalAnalysis_hpp
#define syntacticalAnalysis_hpp

#include <stdio.h>

enum SyntaxisTypes{NOT_TERMINAL, TERMINAL, OR, BEGIN, END};

struct SyntaxisToken{
    SyntaxisTypes type;
    char *content;
    
    SyntaxisToken();
};

class Syntaxis{
private:
    SyntaxisToken *tokens;
    unsigned int counter;
public:
    Syntaxis();
    
    ~Syntaxis();
    
    void push(SyntaxisToken);
    unsigned int parsing();
    
    unsigned int analyzePercent(unsigned int);
    unsigned int analyzeToken(unsigned int);
    unsigned int analyzeOpenBrace(unsigned int);
    unsigned int analyzePipe(unsigned int);
    unsigned int analyzeColon(unsigned int);
    unsigned int analyzeSemicolon(unsigned int);
    unsigned int analyzeSomeText(unsigned int);
    unsigned int analyzeForwardSlash(unsigned int);
};

class Terminal{
public:
    char **arrayOfTerminals;
    unsigned int counter;
    
    Terminal();
    
    ~Terminal();
    
    unsigned int getCounter();
    void push(char*);
    bool checkTerminal(char*);
};

struct Alternative{
    char *name;
    SyntaxisTypes type;
    
    Alternative();
    
    ~Alternative();
};

class Grammatic{
public:
    Alternative inputPoint;
    Alternative **alternatives;
    unsigned int countOfAlternatives;
    unsigned int *countOfMembersInAlternative;

    Grammatic();
    
    ~Grammatic();
    
    void newAlternative();
    void pushToAlternatives(char*);
    void setCountOfMembersInAlternative(unsigned int);
};

#endif
