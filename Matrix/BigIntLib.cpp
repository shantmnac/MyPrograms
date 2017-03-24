#include "BigIntLib.hpp"
#include "ErrorsCatchers.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>

unsigned long int max(unsigned long int op1, unsigned long int op2){
    if (op1 >= op2) return op1;
    else return op2;
}

unsigned long int min(unsigned long int op1, unsigned long int op2){
    if (op1 <= op2) return op1;
    else return op2;
}

char convertIntToChar(int integer){return integer + '0';}

int convertCharToInt(char character){return character - '0';}

char* reverseChar(char* tempChar){
    unsigned long int length = strlen(tempChar), temp = length;
    char* tempBigInt = (char*)malloc((length + 1) * sizeof(char));
    
    if (tempBigInt == NULL) {
        throw MemoryAllocatingERROR((length + 1) * sizeof(char));
    }
    tempBigInt[length] = '\0';
    
    for (unsigned long int i = 0; i < temp; i++) {
        tempBigInt[i] = tempChar[length - 1];
        length--;
    }
    
    return tempBigInt;
}

BigInt abs(const BigInt& temp){
    
    if (temp > BigInt("0")) return temp;
    else {
        BigInt retInt = temp;
        retInt.changeSign();
        return retInt;
    }
}


BigInt::BigInt(){
    sign = 1; //1 = +. 0 = -;
    integer = (char*)malloc(2 * sizeof(char));
    if (integer == NULL) {
        throw MemoryAllocatingERROR(2 * sizeof(char));
    }
    integer[0] = '0';
    integer[1] = '\0';
}

BigInt::BigInt(long int temp){
    if (!temp) {
        sign = 1;
        integer = (char*)malloc(2 * sizeof(char));
        if (integer == NULL) {
            throw MemoryAllocatingERROR(2 * sizeof(char));
        }
        integer[0] = '0';
        integer[1] = '\0';
        return;
    }
    
    long int tempInt = temp;
    int ost = 0;
    int i = 0;
    
    if (temp > 0) sign = 1;
    else {
        sign = 0;
        tempInt = -tempInt;
    }
    
    integer = NULL;
    while (tempInt > 0) {
        ost = tempInt % 10;
        tempInt /= 10;
        integer = (char*)realloc(integer, (i + 1) * sizeof(char));
        if (integer == NULL) {
            throw MemoryAllocatingERROR((i + 1) * sizeof(char));
        }
        integer[i] = convertIntToChar(ost);
        i++;
    }
    integer = (char*)realloc(integer, (i + 1) * sizeof(char));
    if (integer == NULL) {
        throw MemoryAllocatingERROR((i + 1) * sizeof(char));
    }
    integer[i] = '\0';
}

BigInt::BigInt(const char* temp){
    char* tempInt;
    if (temp[0] == '-') {
        if (!convertCharToInt(temp[1])) {
            sign = 1; //1 = +. 0 = -;
            integer = (char*)malloc(2 * sizeof(char));
            if (integer == NULL) {
                throw MemoryAllocatingERROR(2 * sizeof(char));
            }
            integer[0] = '0';
            integer[1] = '\0';
            return;
        }
        sign = 0;
        integer = (char*)malloc(strlen(temp) * sizeof(char));
        if (integer == NULL) {
            throw MemoryAllocatingERROR((int)strlen(temp) * sizeof(char));
        }
        int j = 0;
        for (int i = 1; i < (int)strlen(temp); i++) {
            integer[j] = temp[i];
            j++;
        }
        integer[j] = '\0';
        tempInt = reverseChar(integer);
        free(integer);
        integer = tempInt;
    }
    else{
        if (!convertCharToInt(temp[0])) BigInt();
        sign = 1;
        integer = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        if (integer == NULL) {
            throw MemoryAllocatingERROR(((int)strlen(temp) + 1) * sizeof(char));
        }
        int j = 0;
        for (int i = 0; i < (int)strlen(temp); i++) {
            integer[j] = temp[i];
            j++;
        }
        integer[j] = '\0';
        tempInt = reverseChar(integer);
        free(integer);
        integer = tempInt;
    }
}

BigInt::BigInt(const BigInt& temp){
    sign = temp.sign;
    integer = (char*)malloc((strlen(temp.integer) + 1) * sizeof(char));
    if (integer == NULL) {
        throw MemoryAllocatingERROR(((int)strlen(temp.integer) + 1) * sizeof(char));
    }
    strcpy(integer, temp.integer);
}


BigInt::~BigInt(){
    free(integer);
    integer = NULL;
}


void BigInt::changeSign(){
    if (sign) sign = 0;
    else sign = 1;
};

void BigInt::setSign(int signToChange){
    sign = signToChange;
}

void BigInt::print()const{
    char* temp = reverseChar(integer);
    if (sign) printf("%s", temp);
    else printf("-%s", temp);
    free(temp);
};

BigInt BigInt::get()const{
    return *this;
};

char* BigInt::getInt()const{
    return integer;
};

int BigInt::getSign()const{
    return sign;
};

void BigInt::deleteZero(){
    unsigned long int zeroPosition = 1, i = 0;
    bool toDeleteFlag = 0;
    char* tempInteger = NULL;
    
    while (integer[i] != '\0') {
        if (integer[i] != '0') toDeleteFlag = 0;
        else {
            if (!toDeleteFlag) {
                zeroPosition = i;
            }
            toDeleteFlag = 1;
        }
        i++;
    }
    
    if (toDeleteFlag) {
        if (!zeroPosition) {
            integer = (char*)realloc(integer, 2 * sizeof(char));
            if (integer == NULL) {
                throw MemoryAllocatingERROR(2 * sizeof(char));
            }
            integer[0] = '0';
            integer[1] = '\0';
            return;
        }
        
        tempInteger = integer;
        integer = (char*)malloc((zeroPosition + 1) * sizeof(char));
        if (integer == NULL) {
            throw MemoryAllocatingERROR((zeroPosition + 1) * sizeof(char));
        }
        for (i = 0; i < zeroPosition; i++) {
            integer[i] = tempInteger[i];
        }
        integer[zeroPosition] = '\0';
        
        free(tempInteger);
    }
}

void BigInt::pushFront(int x){
    char *tmp;
    unsigned long int size = strlen(integer);
    
    if (integer == NULL) return;
    
    if ((size == 1) && (integer[0] == '0')){
        integer[0] = convertIntToChar(x);
        return;
    }
    
    tmp = (char*)realloc(integer, (size + 2) * sizeof(char));
    if (tmp == NULL) throw MemoryAllocatingERROR(size + 2);
    integer = tmp;
    
    for (unsigned long int i = 0; i <= size; i++){
        integer[size + 1 - i] = integer[size - i];
    }
    
    integer[0] = convertIntToChar(x);
}

void BigInt::pushBack(int x){
    char *tmp;
    unsigned long int size = strlen(integer);
    
    if (integer == NULL) return;
    
    tmp = (char*)realloc(integer, (size + 2) * sizeof(char));
    if (tmp == NULL) throw MemoryAllocatingERROR(size + 2);
    integer = tmp;
    integer[size] = convertIntToChar(x);
    integer[size + 1] = '\0';
}

int BigInt::convertToInt(){
    BigInt max((unsigned int)-1);
    int flag = 1, result = 0;
    unsigned long int size = strlen(integer);
    
    if (sign == 0) flag = -1;
    
    if (*this * flag > max)
        throw TypesConvertingERROR();
    
    for (unsigned long int i = 0; i < size; i++){
        if (i == size - 1)
            result += (integer[size - 1 - i] - '0');
        else{
            result += (integer[size - 1 - i] - '0');
            result *= 10;
        }
    }
    
    result *= flag;
    
    return result;
}

long int BigInt::convertToLongInt(){
    BigInt max((unsigned long int)-1);
    int result = 0;
    unsigned long int size = strlen(integer);
    
    if (sign == 1){
        if (*this > max)
            throw TypesConvertingERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (integer[size - 1 - i] - '0');
            else{
                result += (integer[size - 1 - i] - '0');
                result *= 10;
            }
        }
        
    }
    
    if (sign == 0){
        if ((*this) * (-1) > max)
            throw TypesConvertingERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (integer[size - 1 - i] - '0');
            else{
                result += (integer[size - 1 - i] - '0');
                result *= 10;
            }
        }
        result *= -1;
    }
    return result;
}

short BigInt::convertToShort(){
    BigInt max((unsigned short)-1);
    int result = 0;
    unsigned long int size = strlen(integer);
    
    if (sign == 1){
        if (*this > max)
            throw TypesConvertingERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (integer[size - 1 - i] - '0');
            else{
                result += (integer[size - 1 - i] - '0');
                result *= 10;
            }
        }
        
    }
    
    if (sign == 0){
        if ((*this) * (-1) > max)
            throw TypesConvertingERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (integer[size - 1 - i] - '0');
            else{
                result += (integer[size - 1 - i] - '0');
                result *= 10;
            }
        }
        result *= -1;
    }
    return result;
}

void BigInt::printIntoFile(FILE *output){
    unsigned long int size = strlen(integer);
    
    if(sign == 0) fprintf(output, "-");
    
    for (unsigned long int i = 0; i < size; i++){
        fprintf(output, "%c", integer[size - 1 - i]);
    }
}


BigInt BigInt::operator+(const BigInt& temp)const{
    BigInt tempInt;
    int tempSum, tempPerenos = 0;
    unsigned long int i, lengthOp1 = strlen(integer), lengthOp2 = strlen(temp.integer);
    unsigned long int minLenght = min(lengthOp1, lengthOp2), maxLenght = max(lengthOp1, lengthOp2);
    
    if (*this == 0) {
        return temp;
    }
    
    if (temp == 0) {
        return *this;
    }
    
    if (sign == temp.sign) {
        if (sign) tempInt.sign = 1;
        else tempInt.sign = 0;
        
        free(tempInt.integer);
        tempInt.integer = NULL;
        
        for (i = 0; i < minLenght; i++) {
            tempSum = tempPerenos + convertCharToInt(temp.integer[i]) + convertCharToInt(integer[i]);
            tempPerenos = tempSum/10;
            tempSum %= 10;
            tempInt.integer = (char*)realloc(tempInt.integer, (i + 1) * sizeof(char));
            if (tempInt.integer == NULL) {
                throw MemoryAllocatingERROR((i + 1) * sizeof(char));
            }
            tempInt.integer[i] = convertIntToChar(tempSum);
            tempSum = 0;
        }
        for (i = minLenght; i < maxLenght; i++) {
            if (lengthOp1 > lengthOp2) {
                tempSum = tempPerenos + convertCharToInt(integer[i]);
            }
            else {
                tempSum = tempPerenos + convertCharToInt(temp.integer[i]);
            }
            tempPerenos = tempSum/10;
            tempSum %= 10;
            tempInt.integer = (char*)realloc(tempInt.integer, (i + 1) * sizeof(char));
            if (tempInt.integer == NULL) {
                throw MemoryAllocatingERROR((i + 1) * sizeof(char));
            }
            tempInt.integer[i] = convertIntToChar(tempSum);
            tempSum = 0;
        }
        if (tempPerenos) {
            tempInt.integer = (char*)realloc(tempInt.integer, (maxLenght + 2) * sizeof(char));
            if (tempInt.integer == NULL) {
                throw MemoryAllocatingERROR((maxLenght + 2) * sizeof(char));
            }
            tempInt.integer[maxLenght] = convertIntToChar(tempPerenos);
            tempInt.integer[maxLenght + 1] = '\0';
            return tempInt;
        }
        
        tempInt.integer = (char*)realloc(tempInt.integer, (maxLenght + 1) * sizeof(char));
        if (tempInt.integer == NULL) {
            throw MemoryAllocatingERROR((maxLenght + 1) * sizeof(char));
        }
        tempInt.integer[maxLenght] = '\0';
        
        return tempInt;
    }
    
    switch (sign) {
        case 1: return tempInt = *this - abs(temp);
            
        case 0: return tempInt = temp - abs(*this);
            
        default: return BigInt();
    }
}

BigInt BigInt::operator+(long int temp)const{
    return *this + BigInt(temp);
}

BigInt operator+(long int tempOp1, const BigInt& tempOp2){
    return BigInt(tempOp1) + tempOp2;
}


BigInt BigInt::operator-(const BigInt& temp)const{
    BigInt tempInt;
    char *tempFirstOp = NULL, *tempSecondOp = NULL;
    int tempZaim = 0;
    unsigned long int minLenght = min(strlen(integer), strlen(temp.integer)), maxLenght = max(strlen(integer), strlen(temp.integer));
    
    if (*this == 0) {
        tempInt = temp;
        tempInt.changeSign();
        return tempInt;
    }
    
    if (temp == 0) {
        return *this;
    }
    
    if (!sign && temp.sign) {
        tempInt = abs(*this) + abs(temp);
        tempInt.sign = 0;
        return tempInt;
    }
    else{
        if (sign && !temp.sign) {
            return tempInt = *this + abs(temp);
        }
        else{
            
            if (abs(*this) >= abs(temp)) {
                tempFirstOp = integer;
                tempSecondOp = temp.integer;
                if (!sign) tempInt.changeSign();
            }
            else{
                tempFirstOp = temp.integer;
                tempSecondOp = integer;
                if (sign) tempInt.changeSign();
            }
            for (unsigned long int i = 0; i < minLenght; i++) {
                if (tempFirstOp[i] >= tempSecondOp[i]) {
                    tempInt.integer = (char*)realloc(tempInt.integer, (i + 1) * sizeof(char));
                    if (tempInt.integer == NULL) {
                        throw MemoryAllocatingERROR((i + 1) * sizeof(char));
                    }
                    tempInt.integer[i] = convertIntToChar(tempFirstOp[i] - tempSecondOp[i]);
                }
                else{
                    unsigned long int j = i + 1;
                    while (j < maxLenght) {
                        if (convertCharToInt(tempFirstOp[j]) >= 1) {
                            tempFirstOp[j] = tempFirstOp[j] - 1;
                            tempZaim = 1;
                            break;
                        }
                        else{j++;}
                    }
                    j--;
                    while (j != i) {
                        tempFirstOp[j] = '9';
                        j--;
                    }
                    tempInt.integer = (char*)realloc(tempInt.integer, (j + 1) * sizeof(char));
                    if (integer == NULL) {
                        throw MemoryAllocatingERROR((j + 1) * sizeof(char));
                    }
                    tempInt.integer[j] = convertIntToChar(convertCharToInt(tempFirstOp[i]) + 10 - convertCharToInt(tempSecondOp[i]));
                }
            }
            
            for (unsigned long int i = minLenght; i < maxLenght; i++) {
                tempInt.integer = (char*)realloc(tempInt.integer, (i + 1) * sizeof(char));
                if (tempInt.integer == NULL) {
                    throw MemoryAllocatingERROR((i + 1) * sizeof(char));
                }
                tempInt.integer[i] = tempFirstOp[i];
            }
            
            tempInt.integer = (char*)realloc(tempInt.integer, (maxLenght + 1) * sizeof(char));
            if (tempInt.integer == NULL) {
                throw MemoryAllocatingERROR((maxLenght + 1) * sizeof(char));
            }
            tempInt.integer[maxLenght] = '\0';
            
            tempInt.deleteZero();
            return tempInt;
        }
    }
}

BigInt BigInt::operator-(long int temp)const{
    return *this - BigInt(temp);
}

BigInt operator-(long int tempOp1, const BigInt& tempOp2){
    return BigInt(tempOp1) - tempOp2;
}


BigInt BigInt::operator*(const BigInt& temp)const{
    BigInt tempInt;
    unsigned long int lengthOp1 = strlen(integer), lengthOp2 = strlen(temp.integer);
    int carry = 0;
    char *tempSum = NULL, *toDelete;
    
    if ((temp == 0) || (*this == 0)) return BigInt();
    
    if (temp == 1) return *this;
    
    tempSum = (char*)malloc((lengthOp1 + lengthOp2 + 1) * sizeof(char));
    for (unsigned long int j = 0; j < lengthOp1 + lengthOp2; j++) {
        tempSum[j] = '0';
    }
    tempSum[lengthOp1 + lengthOp2] = '\0';
    
    for (unsigned long int i = 0; i < lengthOp2; i++) {
        if (temp.integer[i] == '0') continue;
        
        for (unsigned long int j = 0; j < lengthOp1; j++) {
            carry += convertCharToInt(integer[j]) * convertCharToInt(temp.integer[i]) + convertCharToInt(tempSum[i + j]);
            tempSum[i + j] = convertIntToChar(carry % 10);
            carry /= 10;
        }
        if (carry) tempSum[lengthOp1 + i] = convertIntToChar(carry);
        carry = 0;
        
    }
    toDelete = tempSum;
    tempSum = reverseChar(tempSum);
    tempInt = tempSum;
    free(toDelete);
    free(tempSum);
    if (*this == 1) return temp;
    
    if (sign) {
        if (!temp.sign) tempInt.sign = 0;
    }
    else{
        if (temp.sign) tempInt.sign = 0;
    }
    
    tempInt.deleteZero();
    return tempInt;
}

BigInt BigInt::operator*(long int temp)const{
    return *this * BigInt(temp);
}

BigInt operator*(long int tempOp1, const BigInt& tempOp2){
    return BigInt(tempOp1) * tempOp2;
}


BigInt BigInt::operator/(const BigInt& secondOperand)const{
    BigInt result, tmp, current, sOp = secondOperand;
    unsigned long int lengthOp1 = strlen(integer);
    bool flag = false;
    
    if (*this == 0) return result;
    
    if (secondOperand == 0) throw DividerIsZeroERROR();
    
    if (secondOperand == 1) return *this;
    
    if (secondOperand == -1) {
        result = *this;
        result.changeSign();
        return result;
    }
    
    if (*this == secondOperand) return result = 1;
    
    if (abs(*this) < abs(secondOperand)) return result;
    
    sOp.setSign(1);
    
    for(unsigned long int i = 0; i < lengthOp1;){
        tmp.pushFront(convertCharToInt(integer[lengthOp1 - 1 - i]));
        i++;
        while (tmp < sOp){
            result.pushFront(0);
            if (i >= lengthOp1){
                flag = true;
                break;
            }
            tmp.pushFront(convertCharToInt(integer[lengthOp1 - 1 - i]));
            i++;
        }
        if (flag) break;
        
        int l = 0, r = 10, m, x = 0;
        
        while (l <= r){
            m = (l+r) >> 1;
            current = sOp * m;
            if (tmp >= current){
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        
        result.pushFront(x);
        current = sOp * x;
        tmp = tmp - current;
    }
    
    result.deleteZero();
    
    if (sign) {
        switch (secondOperand.sign) {
            case 0: {
                result.setSign(0);
                break;
            }
                
            default: break;
        }
    }
    else{
        switch (secondOperand.sign) {
            case 1: {
                result.setSign(0);
                break;
            }
                
            default: break;
        }
    }
    
    return result;
}

BigInt BigInt::operator/(long int temp)const{
    return *this / BigInt(temp);
}

BigInt operator/(long int tempOp1, const BigInt& tempOp2){
    return BigInt(tempOp1) / tempOp2;
}


BigInt BigInt::operator=(const BigInt& temp){
    if (&temp == this) return *this;
    
    sign = temp.sign;
    free(integer);
    integer = NULL;
    integer = (char*)malloc((strlen(temp.integer) + 1) * sizeof(char));
    if (integer == NULL) {
        throw MemoryAllocatingERROR(((int)strlen(temp.integer) + 1) * sizeof(char));
    }
    strcpy(integer, temp.integer);
    
    return *this;
}

BigInt BigInt::operator=(long int temp){
    return (*this = BigInt(temp));
}


BigInt BigInt::operator+=(const BigInt& temp){
    return *this = *this + temp;
}

BigInt BigInt::operator+=(long int temp){
    return *this = *this + BigInt(temp);
}


BigInt BigInt::operator-=(const BigInt& temp){
    return *this = *this - temp;
}

BigInt BigInt::operator-=(long int temp){
    return *this = *this - BigInt(temp);
}


BigInt BigInt::operator*=(const BigInt& temp){
    return *this = *this * temp;
}

BigInt BigInt::operator*=(long int temp){
    return *this = *this * BigInt(temp);
}


BigInt BigInt::operator/=(const BigInt& temp){
    return *this = *this / temp;
}

BigInt BigInt::operator/=(long int temp){
    return *this = *this / BigInt(temp);
}


BigInt BigInt::operator++()const{
    return BigInt(*this + 1);
}


BigInt BigInt::operator--()const{
    return BigInt(*this - 1);
}


bool BigInt::operator<(const BigInt& temp)const{
    if (sign > temp.sign) return false;
    else {
        if (sign < temp.sign) return true;
        else{
            if (strlen(integer) > strlen(temp.integer)) return false;
            else{
                if (strlen(integer) < strlen(temp.integer)) return true;
                else{
                    for (int i = ((int)strlen(integer) - 1); i >= 0; i--) {
                        if (integer[i] > temp.integer[i]) return false;
                        else{
                            if (integer[i] < temp.integer[i]) return true;
                        }
                    }
                    return false;
                }
            }
        }
    }
}

bool BigInt::operator<(long int temp)const{
    return (*this < BigInt(temp));
}

bool operator<(const int op1, const BigInt& op2){
    return (BigInt(op1) < op2);
}


bool BigInt::operator>(const BigInt& temp)const{
    if (sign > temp.sign) return true;
    else {
        if (sign < temp.sign) return false;
        else{
            if (strlen(integer) > strlen(temp.integer)) return true;
            else{
                if (strlen(integer) < strlen(temp.integer)) return false;
                else{
                    for (int i = ((int)strlen(integer) - 1); i >= 0; i--) {
                        if (integer[i] > temp.integer[i]) return true;
                        else{
                            if (integer[i] < temp.integer[i]) return false;
                        }
                    }
                    return false;
                }
            }
        }
    }
}

bool BigInt::operator>(long int temp)const{
    return (*this > BigInt(temp));
}

bool operator>(long int op1, const BigInt& op2){
    return (BigInt(op1) > op2);
}


bool BigInt::operator<=(const BigInt& temp)const{
    if ((*this < temp) || (*this == temp)) return true;
    else return false;
}

bool BigInt::operator<=(long int temp)const{
    return (*this <= BigInt(temp));
}

bool operator<=(long int op1, const BigInt& op2){
    return (BigInt(op1) <= op2);
}


bool BigInt::operator>=(const BigInt& temp)const{
    if ((*this > temp) || (*this == temp)) return true;
    else return false;
}

bool BigInt::operator>=(long int temp)const{
    return (*this >= BigInt(temp));
}

bool operator>=(long int op1, const BigInt& op2){
    return (BigInt(op1) >= op2);
}


bool BigInt::operator==(const BigInt& temp)const{
    if (sign == temp.sign){
        if (strlen(integer) == strlen(temp.integer)) {
            for (int i = ((int)strlen(integer) - 1); i >= 0; i--) {
                if (integer[i] != temp.integer[i]) return false;
            }
            return true;
        }
        else return false;
    }
    else return false;
}

bool BigInt::operator==(long int temp)const{
    return (*this == BigInt(temp));
}

bool operator==(long int op1, const BigInt& op2){
    return (BigInt(op1) == op2);
}


bool BigInt::operator!=(const BigInt& temp)const{
    if (*this == temp) return false;
    else return true;
}

bool BigInt::operator!=(long int temp)const{
    return (*this != BigInt(temp));
}

bool operator!=(long int op1, const BigInt& op2){
    return (BigInt(op1) != op2);
}


BigInt BigInt::operator%(const BigInt &temp)const{
    return *this - (*this/temp) * temp;
}

BigInt BigInt::operator%(long int temp)const{
    return *this - (*this/temp) * temp;
}