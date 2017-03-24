#include "VectorLib.hpp"
#include "RationalNumberLib.hpp"
#include "ErrorsCatchers.hpp"
#include "MatrixLib.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

Vector::Vector(){
    counterOfColumns = 1;
    content = Dictionary();
}

Vector::Vector(unsigned long int i, const RationalNumber &temp){
    if (i <= 0) throw VectorSizeERROR();
    
    counterOfColumns = i;
    content = Dictionary();
    
    if (temp == 0) return;
    for (unsigned long int k = 1; k <= counterOfColumns; k++){
        content.addElement(1, k, temp);
    }
}

Vector::Vector(const Vector &temp){
    counterOfColumns = temp.counterOfColumns;
    content = Dictionary(temp.content);
}

Vector::Vector(FILE *input){
    unsigned long int tempInt, sizeOfTmpWord = 0;
    long int coordinatesOfColumn;
    char tmpChar = '!', *tmpWord = NULL;
    
    if (input == NULL){
        counterOfColumns = 1;
        content = Dictionary();
        return;
    }
    
    fscanf(input, "%c", &tmpChar);
    while ((tmpChar != EOF) && ((tmpChar == '\n') || (tmpChar == '#'))){
        if (tmpChar == '#'){
            while (tmpChar != '\n') fscanf(input, "%c", &tmpChar);
            fscanf(input, "%c", &tmpChar);
            continue;
        }
        
        if (tmpChar == '\n')
            fscanf(input, "%c", &tmpChar);
    }
    
    if (tmpChar == EOF){
        counterOfColumns = 1;
        content = Dictionary();
        return;
    }
    
    while (tmpChar != '\t'){
        tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
        sizeOfTmpWord++;
        tmpWord[sizeOfTmpWord - 1] = tmpChar;
        if(fscanf(input, "%c", &tmpChar) == EOF) tmpChar = EOF;
    }
    
    tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
    sizeOfTmpWord++;
    tmpWord[sizeOfTmpWord - 1] = '\0';
    
    if (strcmp(tmpWord, "vector") == 0){
        fscanf(input, "%zu", &tempInt);
        counterOfColumns = tempInt;
        content = Dictionary();
        
        fscanf(input, "%c", &tmpChar);
        while (tmpChar != EOF){
            if (tmpChar == '#'){
                while (tmpChar != '\n') fscanf(input, "%c", &tmpChar);
                if(fscanf(input, "%c", &tmpChar) == EOF) tmpChar = EOF;
                continue;
            }
            coordinatesOfColumn = -1;
            fscanf(input, "%zu", &coordinatesOfColumn);
            
            free(tmpWord);
            tmpWord = NULL;
            sizeOfTmpWord = 0;
            if(fscanf(input, "%c", &tmpChar) == EOF) tmpChar = EOF;
            while ((tmpChar != '\n') && (tmpChar!= EOF)){
                if (isdigit(tmpChar) || (tmpChar == '/')) {
                    tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
                    sizeOfTmpWord++;
                    tmpWord[sizeOfTmpWord - 1] = tmpChar;
                }
                if(fscanf(input, "%c", &tmpChar) == EOF) tmpChar = EOF;
            }
            tmpWord = (char*)realloc(tmpWord, sizeOfTmpWord + 1);
            sizeOfTmpWord++;
            tmpWord[sizeOfTmpWord - 1] = '\0';
            if (coordinatesOfColumn != -1) set(coordinatesOfColumn, RationalNumber(tmpWord));
            
            if (tmpChar == EOF) break;
        }
        free(tmpWord);
        
    }
    else{
        free(tmpWord);
        counterOfColumns = 1;
        content = Dictionary();
    }
}


Vector::~Vector(){}


unsigned long int Vector::getNumberOfColumns()const{
    return counterOfColumns;
}

void Vector::write(FILE *output){
    fprintf(output, "vector\t%zu\n\n", counterOfColumns);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        fprintf(output, "%zu\t", content.arrayOfElements[k].j);
        content.arrayOfElements[k].content.printIntoFile(output);
        fprintf(output, "\n");
    }
}

void Vector::read(FILE *input){
    Vector tmp(input);
    *this = tmp;
}

void Vector::printVector(FILE *output){
    for (unsigned long int j = 1; j <= counterOfColumns; j++){
        content.get(1, j).printIntoFile(output);
        fprintf(output, " ");
    }
    fprintf(output, "\n");
}

void Vector::set(unsigned long int j, const RationalNumber &temp){
    if ((j > counterOfColumns) || (j <= 0)) throw VectorSizeERROR();
    
    if ((temp == 0) && (content.get(1, j) == 0)) return;
    
    if (temp == 0){
        content.removeElement(1, j);
        return;
    }
    
    content.addElement(1, j, temp);
}

void Vector::makeCanonical(){
    for (unsigned long int i = 0; i < content.sizeOfArray; i++){
        content.arrayOfElements[i].content.makeCanonical();
    }
}


Vector Vector::operator+(const Vector &temp)const{
    if ((counterOfColumns != temp.counterOfColumns)) throw VectorSizeERROR();
    
    unsigned long int coordinates;
    Vector result(*this);
    
    for (unsigned long int k = 0; k < temp.content.sizeOfArray; k++){
        coordinates = temp.content.arrayOfElements[k].j;
        result.set(coordinates, result[coordinates] + temp.content.arrayOfElements[k].content);
    }
    return result;
}


Vector Vector::operator-(const Vector &temp)const{
    if ((counterOfColumns != temp.counterOfColumns)) throw VectorSizeERROR();
    
    unsigned long int coordinates;
    Vector result(*this);
    
    for (unsigned long int k = 0; k < temp.content.sizeOfArray; k++){
        coordinates = temp.content.arrayOfElements[k].j;
        result.set(coordinates, result[coordinates] - temp.content.arrayOfElements[k].content);
    }
    return result;
}

Vector Vector::operator-(){
    Vector result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content *= -1;
    }
    
    return result;
}


Vector Vector::operator*(const RationalNumber &temp)const{
    Vector result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content *= temp;
    }
    return result;
}

Vector Vector::operator*(const Matrix &temp)const{
    if (counterOfColumns != temp.getNumberOfRows()) throw MatrixSizeERROR();
    
    coordinatesOfMatrix coordinates;
    RationalNumber tmp;
    Vector result(counterOfColumns, RationalNumber());
    
    for (unsigned long int i = 1; i <= counterOfColumns; i++) {
        coordinates.column = i;
        tmp = 0;
        for (unsigned long int j = 1; j <= counterOfColumns; j++) {
            coordinates.row = j;
            tmp += (*this)[j] * temp[coordinates];
        }
        result.set(i, tmp);
    }
    
    return result;
}

Vector operator*(const RationalNumber &operand1, const Vector &operand2){
    return operand2 * operand1;
}


Vector Vector::operator=(const Vector &temp){
    counterOfColumns = temp.counterOfColumns;
    content = temp.content;
    return *this;
}


Vector Vector::operator/(const RationalNumber &temp)const{
    Vector result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content /= temp;
    }
    return result;
}


RationalNumber Vector::operator[](unsigned long int temp)const{
    if ((temp > counterOfColumns) || (temp <= 0)) throw VectorSizeERROR();
    return content.get(1, temp);
}