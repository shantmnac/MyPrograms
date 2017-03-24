#ifndef RATIONALNUMBERLIB_HPP
#define RATIONALNUMBERLIB_HPP

#include "BigIntLib.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class RationalNumber{
private:
    int sign;
    BigInt numerator;
    BigInt denominator;
public:
    RationalNumber();
    RationalNumber(long int);
    RationalNumber(long int, unsigned long int);
    RationalNumber(const RationalNumber&);
    RationalNumber(const char*);
    RationalNumber(const char*, const char*);
    
    ~RationalNumber();
    
    void print()const;
    void checkZero();
    void floor();
    void round();
    void makeCanonical();
    void printIntoFile(FILE*);
    
    explicit operator int()const;
    explicit operator long()const;
    explicit operator short()const;
    
    RationalNumber operator+(const RationalNumber&)const;
    RationalNumber operator+(long int)const;
    friend RationalNumber operator+(long int, const RationalNumber&);
    
    RationalNumber operator-(const RationalNumber&)const;
    RationalNumber operator-(long int)const;
    friend RationalNumber operator-(long int, const RationalNumber&);
    
    RationalNumber operator*(const RationalNumber&)const;
    RationalNumber operator*(long int)const;
    friend RationalNumber operator*(long int, const RationalNumber&);
    
    RationalNumber operator/(const RationalNumber&)const;
    RationalNumber operator/(long int)const;
    friend RationalNumber operator/(long int, const RationalNumber&);
    
    RationalNumber operator=(const RationalNumber&);
    RationalNumber operator=(long int);
    
    RationalNumber operator+=(const RationalNumber&);
    RationalNumber operator+=(long int);
    
    RationalNumber operator-=(const RationalNumber&);
    RationalNumber operator-=(long int);
    
    RationalNumber operator*=(const RationalNumber&);
    RationalNumber operator*=(long int);
    
    RationalNumber operator/=(const RationalNumber&);
    RationalNumber operator/=(long int);
    
    RationalNumber operator++()const;
    
    RationalNumber operator--()const;
    
    bool operator<(const RationalNumber&)const;
    bool operator<(long int)const;
    friend bool operator<(long int, const RationalNumber&);
    
    bool operator>(const RationalNumber&)const;
    bool operator>(long int)const;
    friend bool operator>(long int, const RationalNumber&);
    
    bool operator<=(const RationalNumber&)const;
    bool operator<=(long int)const;
    friend bool operator<=(long int, const RationalNumber&);
    
    bool operator>=(const RationalNumber&)const;
    bool operator>=(long int)const;
    friend bool operator>=(long int, const RationalNumber&);
    
    bool operator==(const RationalNumber&)const;
    bool operator==(long int)const;
    friend bool operator==(long int, const RationalNumber&);
    
    bool operator!=(const RationalNumber&)const;
    bool operator!=(long int)const;
    friend bool operator!=(long int, const RationalNumber&);
};

#endif