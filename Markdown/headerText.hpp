#ifndef headerText_hpp
#define headerText_hpp

#include "abstractText.hpp"
#include <stdio.h>

class HeaderText:public AbstractText{
    char* text;
    int headerLevel;
public:
    HeaderText();
    
    ~HeaderText();
    
    unsigned long int countOfSymbols()const;
    unsigned long int countOfWords()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
    void prepare()const;
    unsigned int insertResh(unsigned int)const;
};

#endif
