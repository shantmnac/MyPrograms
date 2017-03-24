#ifndef regularText_hpp
#define regularText_hpp

#include "abstractText.hpp"
#include <stdio.h>

class RegularText:public AbstractText{
    char* text;
public:
    RegularText();
    
    ~RegularText();
    
    unsigned long int countOfSymbols()const;
    unsigned long int countOfWords()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
    char* prepare()const;
};

#endif
