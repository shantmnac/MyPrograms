#include "ErrorsCatchers.hpp"
#include "DictionaryLib.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Dictionary::Dictionary(){
    arrayOfElements = NULL;
    sizeOfArray = 0;
}

Dictionary::Dictionary(const Dictionary &temp){
    arrayOfElements = NULL;
    sizeOfArray = 0;
    
    for (unsigned long int i = 0; i < temp.sizeOfArray; i++){
        addElement(temp.arrayOfElements[i].i, temp.arrayOfElements[i].j, temp.arrayOfElements[i].content);
    }
}


Dictionary :: ~Dictionary(){
    delete []arrayOfElements;
}


Dictionary Dictionary::operator=(const Dictionary &temp){
    delete []arrayOfElements;
    sizeOfArray = 0;
    arrayOfElements = NULL;
    for (unsigned long int i = 0; i < temp.sizeOfArray; i++){
        addElement(temp.arrayOfElements[i].i, temp.arrayOfElements[i].j, temp.arrayOfElements[i].content);
    }
    return *this;
}


void Dictionary::addElement(unsigned long int i, unsigned long int j, const RationalNumber &temp){
    
    for (unsigned long int k = 0; k < sizeOfArray; k++){
        if ((arrayOfElements[k].i == i) && (arrayOfElements[k].j == j)){
            arrayOfElements[k].content = temp;
            return;
        }
    }
    
    element* tmp;
    tmp = new element[sizeOfArray + 1];
    if (tmp == NULL) MemoryAllocatingERROR(sizeOfArray + 1);
    
    for (unsigned long int k = 0; k < sizeOfArray; k++){
        tmp[k].i = arrayOfElements[k].i;
        tmp[k].j = arrayOfElements[k].j;
        tmp[k].content = arrayOfElements[k].content;
    }
    
    if (arrayOfElements != NULL) delete []arrayOfElements;
    arrayOfElements = tmp;
    sizeOfArray++;
    arrayOfElements[sizeOfArray - 1].i = i;
    arrayOfElements[sizeOfArray - 1].j = j;
    arrayOfElements[sizeOfArray - 1].content = temp;
}

void Dictionary::removeElement(unsigned long int i, unsigned long int j){
    unsigned long int k;
    
    if (arrayOfElements == NULL) return;
    
    for (k = 0; k < sizeOfArray; k++){
        if ((arrayOfElements[k].i == i) && (arrayOfElements[k].j == j)) break;
    }
    
    if ((sizeOfArray == 1) && (k == 0)){
        delete []arrayOfElements;
        arrayOfElements = NULL;
        sizeOfArray = 0;
    }
    
    Dictionary tmp;
    
    for (unsigned long int i = 0; i < sizeOfArray; i++){
        if (i != k)
            tmp.addElement(arrayOfElements[i].i, arrayOfElements[i].j, arrayOfElements[i].content);
    }
    
    *this = tmp;
}


RationalNumber Dictionary::get(unsigned long int i, unsigned long int j)const{
    for (unsigned long int k = 0; k < sizeOfArray; k++){
        if ((arrayOfElements[k].i == i) && (arrayOfElements[k].j == j))
            return arrayOfElements[k].content;
    }
    return RationalNumber();
}