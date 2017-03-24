#ifndef DICTIONARYLIB_HPP
#define DICTIONARYLIB_HPP

#include "RationalNumberLib.hpp"
#include <cstdio>

struct element{
    unsigned long int i, j;
    RationalNumber content;
};

class Dictionary{
public:
    element* arrayOfElements;
    unsigned long int sizeOfArray;
    
    Dictionary();
    Dictionary(const Dictionary&);
    
    ~Dictionary();
    
    Dictionary operator=(const Dictionary&);
    
    void addElement(unsigned long int, unsigned long int, const RationalNumber&);
    void removeElement(unsigned long int, unsigned long int);
    RationalNumber get(unsigned long int, unsigned long int) const;
};

#endif