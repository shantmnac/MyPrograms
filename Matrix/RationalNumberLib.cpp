#include "RationalNumberLib.hpp"
#include "ErrorsCatchers.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>


RationalNumber::RationalNumber(){
    sign = 1; //1 = +. 0 = -;
}

RationalNumber::RationalNumber(long int integer){
    if (!integer) {RationalNumber(); return;}
    
    numerator = integer;
    denominator = 1;
    sign = numerator.getSign();
}

RationalNumber::RationalNumber(long int numeratorTmp, unsigned long int denominatorTmp){
    if (!denominatorTmp) {
        if (!numeratorTmp) {RationalNumber(); return;}
        else throw DenominatorIsZeroERROR();
    }
    else{
        if (!numeratorTmp) {RationalNumber(); return;}
    }
    
    numerator = numeratorTmp;
    denominator = denominatorTmp;
    sign = numerator.getSign();
}

RationalNumber::RationalNumber(const RationalNumber& temp){
    sign = temp.sign;
    
    numerator = temp.numerator;
    denominator = temp.denominator;
}

RationalNumber::RationalNumber(const char* temp){
    char* tmpWord = NULL;
    unsigned long int i = 0, sizeOfTmpWord = 0;
    
    while ((temp[i] != '/') && (temp[i] != '\0')) {
        tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
        tmpWord[sizeOfTmpWord] = temp[i];
        sizeOfTmpWord++;
        i++;
    }
    
    if (temp[i] == '\0') {
        numerator = temp;
        denominator = 1;
        sign = numerator.getSign();
        free(tmpWord);
    }
    else{
        tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
        tmpWord[sizeOfTmpWord] = '\0';
        sizeOfTmpWord = 0;
        i++;
        numerator = tmpWord;
        sign = numerator.getSign();
        free(tmpWord);
        i++;
        while (temp[i] != '\0') {
            tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
            tmpWord[sizeOfTmpWord] = temp[i];
            sizeOfTmpWord++;
            i++;
        }
        tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
        tmpWord[sizeOfTmpWord] = '\0';
        denominator = tmpWord;
        free(tmpWord);
    }
    
    return;
}

RationalNumber::RationalNumber(const char* tempCharNumerator, const char* tempCharDenominator){
    if (!convertCharToInt(tempCharDenominator[0])) {
        if (tempCharNumerator[0] == '-') {
            if (!convertCharToInt(tempCharNumerator[1])) {RationalNumber(); return;}
            else throw DenominatorIsZeroERROR();
        }
        else{
            if (!convertCharToInt(tempCharNumerator[0])) {RationalNumber(); return;}
            else throw DenominatorIsZeroERROR();
        }
    }
    else{
        if (tempCharNumerator[0] == '-') {
            if (!convertCharToInt(tempCharNumerator[1])) {RationalNumber(); return;}
        }
        else{
            if (!convertCharToInt(tempCharNumerator[0])) {RationalNumber(); return;}
        }
    }
    
    numerator = tempCharNumerator;
    denominator = tempCharDenominator;
    sign = numerator.getSign();
}


RationalNumber::~RationalNumber(){}


void RationalNumber::print()const{
    if ((denominator == 1) || (numerator == 0)) {
        numerator.print();
        return;
    }
    else{
        numerator.print();
        printf("/");
        denominator.print();
        return;
    }
    
}

void RationalNumber::checkZero(){
    if (numerator == 0) denominator = 0;
    sign = 1;
}

void RationalNumber::floor(){
    if(*this == RationalNumber()) return;
    
    numerator = numerator / denominator;
    if (numerator == 0) denominator = 0;
    else denominator = 1;
}

void RationalNumber::round(){
    if(*this == RationalNumber()) return;
    
    RationalNumber tmp;
    tmp.numerator = numerator % denominator;
    tmp.denominator = denominator;
    
    if (tmp >= RationalNumber(1, 2)) numerator += tmp.denominator - tmp.numerator;
    else numerator -= tmp.numerator;
    
    if (numerator == 0) denominator = 0;
}

void RationalNumber::makeCanonical(){
    bool sign = 1;
    BigInt tmp, numeratorTmp(numerator), denominatorTmp(denominator);
    
    if (numerator < 0){
        numerator.setSign(1);
        sign = 0;
    }
    
    if (*this == RationalNumber()) return;
    while ((numeratorTmp != 0) && (denominatorTmp != 0)){
        if (numeratorTmp > denominatorTmp) numeratorTmp = numeratorTmp % denominatorTmp;
        else denominatorTmp = denominatorTmp % numeratorTmp;
    }
    tmp = numeratorTmp + denominatorTmp;
    numerator /= tmp;
    denominator /= tmp;
    numerator.setSign(sign);
}

void RationalNumber::printIntoFile(FILE *output){
    if ((numerator == 0) || (denominator == 1)){
        numerator.printIntoFile(output);
        return;
    }
    
    numerator.printIntoFile(output);
    fprintf(output, "/");
    denominator.printIntoFile(output);
}


RationalNumber::operator int()const{
    if(*this == RationalNumber()) return 0;
    return (numerator / denominator).convertToInt();
}

RationalNumber::operator long()const{
    if(*this == RationalNumber()) return 0;
    return (numerator / denominator).convertToLongInt();
}

RationalNumber::operator short() const{
    if(*this == RationalNumber()) return 0;
    return (numerator / denominator).convertToShort();
}


RationalNumber RationalNumber::operator+(const RationalNumber& temp)const{
    RationalNumber tempNum;
    
    tempNum.denominator = denominator * temp.denominator;
    tempNum.numerator = numerator * temp.denominator + temp.numerator * denominator;
    tempNum.sign = tempNum.numerator.getSign();
    
    tempNum.checkZero();
    
    return tempNum;
}

RationalNumber RationalNumber::operator+(long int temp)const{
    return *this + RationalNumber(temp);
}

RationalNumber operator+(long int tempOp1, const RationalNumber& tempOp2){
    return RationalNumber(tempOp1) + tempOp2;
}


RationalNumber RationalNumber::operator-(const RationalNumber& temp)const{
    RationalNumber tempNum;
    
    tempNum.denominator = denominator * temp.denominator;
    tempNum.numerator = numerator * temp.denominator - temp.numerator * denominator;
    tempNum.sign = tempNum.numerator.getSign();
    
    tempNum.checkZero();
    
    return tempNum;
}

RationalNumber RationalNumber::operator-(long int temp)const{
    return *this - RationalNumber(temp);
}

RationalNumber operator-(long int tempOp1, const RationalNumber& tempOp2){
    return RationalNumber(tempOp1) - tempOp2;
}


RationalNumber RationalNumber::operator*(const RationalNumber& temp)const{
    RationalNumber tempNum;
    
    tempNum.denominator = denominator * temp.denominator;
    tempNum.numerator = numerator * temp.numerator;
    tempNum.sign = tempNum.numerator.getSign();
    
    tempNum.checkZero();
    
    return tempNum;
}

RationalNumber RationalNumber::operator*(long int temp)const{
    return *this * RationalNumber(temp);
}

RationalNumber operator*(long int tempOp1, const RationalNumber& tempOp2){
    return RationalNumber(tempOp1) * tempOp2;
}


RationalNumber RationalNumber::operator/(const RationalNumber& temp)const{
    RationalNumber tempNum;
    
    if (temp == (int)0) {
        throw DividerIsZeroERROR();
    }
    
    tempNum.denominator = denominator * temp.numerator;
    tempNum.numerator = numerator * temp.denominator;
    tempNum.sign = tempNum.numerator.getSign();
    
    tempNum.checkZero();
    
    return tempNum;
}

RationalNumber RationalNumber::operator/(long int temp)const{
    return *this / RationalNumber(temp);
}

RationalNumber operator/(long int tempOp1, const RationalNumber& tempOp2){
    return RationalNumber(tempOp1) / tempOp2;
}


RationalNumber RationalNumber::operator=(const RationalNumber& temp){
    numerator = temp.numerator;
    denominator = temp.denominator;
    sign = temp.sign;
    
    return *this;
}

RationalNumber RationalNumber::operator=(long int temp){
    return (*this = RationalNumber(temp));
}


RationalNumber RationalNumber::operator+=(const RationalNumber& temp){
    return *this = *this + temp;
}

RationalNumber RationalNumber::operator+=(long int temp){
    return *this = *this + RationalNumber(temp);
}


RationalNumber RationalNumber::operator-=(const RationalNumber& temp){
    return *this = *this - temp;
}

RationalNumber RationalNumber::operator-=(long int temp){
    return *this = *this - RationalNumber(temp);
}


RationalNumber RationalNumber::operator*=(const RationalNumber& temp){
    return *this = *this * temp;
}

RationalNumber RationalNumber::operator*=(long int temp){
    return *this = *this * RationalNumber(temp);
}


RationalNumber RationalNumber::operator/=(const RationalNumber& temp){
    return *this = *this / temp;
}

RationalNumber RationalNumber::operator/=(long int temp){
    return *this = *this / RationalNumber(temp);
}


RationalNumber RationalNumber::operator++()const{
    return RationalNumber(*this + 1);
}


RationalNumber RationalNumber::operator--()const{
    return RationalNumber(*this - 1);
}


bool RationalNumber::operator<(const RationalNumber& temp)const{
    BigInt tempOp1, tempOp2;
    
    tempOp1 = numerator * temp.denominator;
    tempOp2 = temp.numerator * denominator;
    
    return tempOp1 < tempOp2;
    
}

bool RationalNumber::operator<(long int temp)const{
    return (*this < RationalNumber(temp));
}

bool operator<(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) < op2);
}


bool RationalNumber::operator>(const RationalNumber& temp)const{
    BigInt tempOp1, tempOp2;
    
    tempOp1 = numerator * temp.denominator;
    tempOp2 = temp.numerator * denominator;
    
    return tempOp1 > tempOp2;
}

bool RationalNumber::operator>(long int temp)const{
    return (*this > RationalNumber(temp));
}

bool operator>(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) > op2);
}


bool RationalNumber::operator<=(const RationalNumber& temp)const{
    if ((*this < temp) || (*this == temp)) return true;
    else return false;
}

bool RationalNumber::operator<=(long int temp)const{
    return (*this <= RationalNumber(temp));
}

bool operator<=(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) <= op2);
}


bool RationalNumber::operator>=(const RationalNumber& temp)const{
    if ((*this > temp) || (*this == temp)) return true;
    else return false;
}

bool RationalNumber::operator>=(long int temp)const{
    return (*this >= RationalNumber(temp));
}

bool operator>=(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) >= op2);
}


bool RationalNumber::operator==(const RationalNumber& temp)const{
    BigInt tempOp1, tempOp2;
    
    if (temp.numerator == 0) {
        if (numerator == 0) return true;
        else return false;
    }
    
    tempOp1 = numerator * temp.denominator;
    tempOp2 = temp.numerator * denominator;
    
    return tempOp1 == tempOp2;
}

bool RationalNumber::operator==(long int temp)const{
    return (*this == RationalNumber(temp));
}

bool operator==(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) == op2);
}


bool RationalNumber::operator!=(const RationalNumber& temp)const{
    if (*this == temp) return false;
    else return true;
}

bool RationalNumber::operator!=(long int temp)const{
    return (*this != RationalNumber(temp));
}

bool operator!=(long int op1, const RationalNumber& op2){
    return (RationalNumber(op1) != op2);
}