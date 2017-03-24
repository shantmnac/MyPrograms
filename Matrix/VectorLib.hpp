#ifndef VECTORLIB_HPP
#define VECTORLIB_HPP

#include "RationalNumberLib.hpp"
#include "DictionaryLib.hpp"
#include "MatrixLib.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Vector{
private:
    unsigned long int counterOfColumns;
    Dictionary content;
public:
    Vector();
    Vector(unsigned long int, const RationalNumber&);
    Vector(const Vector&);
    Vector(FILE*);
    
    ~Vector();
    
    unsigned long int getNumberOfColumns()const;
    void read(FILE*);
    void write(FILE*);
    void printVector(FILE*);
    void set(unsigned long int, const RationalNumber&);
    void makeCanonical();
    
    
    Vector operator+(const Vector&)const;
    
    Vector operator-(const Vector&)const;
    Vector operator-();
    
    Vector operator*(const RationalNumber&)const;
    Vector operator*(const Matrix&)const;
    friend Vector operator*(const RationalNumber&, const Vector&);
    
    Vector operator=(const Vector&);
    
    Vector operator/(const RationalNumber&)const;
    
    RationalNumber operator[](unsigned long int)const;
};

#endif