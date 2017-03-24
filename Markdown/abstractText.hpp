#ifndef abstractText_hpp
#define abstractText_hpp

#include <stdio.h>

class AbstractText{
public:
    virtual ~AbstractText();
    
    virtual unsigned long int countOfSymbols()const = 0;
    virtual unsigned long int countOfWords()const = 0;
    virtual void print(FILE*)const = 0;
    virtual void read(FILE*) = 0;
    virtual void prints(FILE*)const = 0;
};

#endif