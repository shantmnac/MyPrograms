#ifndef codeText_hpp
#define codeText_hpp

#include "abstractText.hpp"
#include <stdio.h>

class CodeText:public AbstractText{
    char **text;
    unsigned int counter;
public:
    CodeText();
    
    ~CodeText();
    
    void push(const char*);
    unsigned long int countOfSymbols()const;
    unsigned long int countOfWords()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
};

#endif
