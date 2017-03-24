#ifndef textViewer_hpp
#define textViewer_hpp

#include "abstractText.hpp"
#include <stdio.h>

class TextViewer{
    AbstractText **text;
    unsigned int countOfSymbols, countOfWords, countOfParagraphs;
public:
    TextViewer();
    TextViewer(FILE*);
    
    ~TextViewer();
    
    void operator=(const TextViewer&);
    
    void read(FILE*);
    void print(FILE*);
    void deleteObject();
};

#endif
