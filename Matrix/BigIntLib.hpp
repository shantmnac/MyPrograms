#ifndef BIGINTLIB_HPP
#define BIGINTLIB_HPP

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class BigInt{
private:
    char* integer;
    int sign;
public:
    BigInt();
    BigInt(long int);
    BigInt(const char*);
    BigInt(const BigInt&);
    
    ~BigInt();
    
    void changeSign();
    void setSign(int);
    void print()const;
    BigInt get()const;
    char* getInt()const;
    int getSign()const;
    void deleteZero();
    void pushFront(int);
    void pushBack(int);
    int convertToInt();
    long int convertToLongInt();
    short convertToShort();
    void printIntoFile(FILE*);
    
    BigInt operator+(const BigInt&)const;
    BigInt operator+(long int)const;
    friend BigInt operator+(long int, const BigInt&);
    
    BigInt operator-(const BigInt&)const;
    BigInt operator-(long int)const;
    friend BigInt operator-(long int, const BigInt&);
    
    BigInt operator*(const BigInt&)const;
    BigInt operator*(long int)const;
    friend BigInt operator*(long int, const BigInt&);
    
    BigInt operator/(const BigInt&)const;
    BigInt operator/(long int)const;
    friend BigInt operator/(long int, const BigInt&);
    
    BigInt operator=(const BigInt&);
    BigInt operator=(long int);
    
    BigInt operator+=(const BigInt&);
    BigInt operator+=(long int);
    
    BigInt operator-=(const BigInt&);
    BigInt operator-=(long int);
    
    BigInt operator*=(const BigInt&);
    BigInt operator*=(long int);
    
    BigInt operator/=(const BigInt&);
    BigInt operator/=(long int);
    
    BigInt operator++()const;
    
    BigInt operator--()const;
    
    bool operator<(const BigInt&)const;
    bool operator<(long int)const;
    friend bool operator<(long int, const BigInt&);
    
    bool operator>(const BigInt&)const;
    bool operator>(long int)const;
    friend bool operator>(long int, const BigInt&);
    
    bool operator<=(const BigInt&)const;
    bool operator<=(long int)const;
    friend bool operator<=(long int, const BigInt&);
    
    bool operator>=(const BigInt&)const;
    bool operator>=(long int)const;
    friend bool operator>=(long int, const BigInt&);
    
    bool operator==(const BigInt&)const;
    bool operator==(long int)const;
    friend bool operator==(long int, const BigInt&);
    
    bool operator!=(const BigInt&)const;
    bool operator!=(long int)const;
    friend bool operator!=(long int, const BigInt&);
    
    BigInt operator%(const BigInt&)const;
    BigInt operator%(long int temp)const;
};

unsigned long int max(unsigned long int, unsigned long int);
unsigned long int min(unsigned long int, unsigned long int);
char convertIntToChar(int);
int convertCharToInt(char);
char* reverseChar(char*);
BigInt abs(const BigInt&);

#endif