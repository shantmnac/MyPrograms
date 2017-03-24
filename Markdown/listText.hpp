#ifndef listText_hpp
#define listText_hpp

#include "abstractText.hpp"
#include <stdio.h>

struct ListElement{
    char* text;
    unsigned int indent;
    bool listType; // 1-нумерованный, 0-маркированный
};

class ListText:public AbstractText{
    ListElement *list;
    unsigned int counter;
public:
    ListText();
    
    ~ListText();
    
    void push(char*, unsigned int, bool);
    unsigned long int countOfSymbols()const;
    unsigned long int countOfWords()const;
    void print(FILE*) const;
    void read(FILE*);
    void prints(FILE*)const;
    void prepare(ListElement)const;
    unsigned int insertIndent(unsigned int, unsigned int)const;
    char* giveNum()const;
    char convertIntToChar(int)const;
};

#endif
