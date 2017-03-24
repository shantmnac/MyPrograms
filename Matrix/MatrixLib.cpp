#include "MatrixLib.hpp"
#include "ErrorsCatchers.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix::Matrix(){
    counterOfRows = counterOfColumns = 1;
    content = Dictionary();
}

Matrix::Matrix(unsigned long int i, unsigned long int j){
    if ((i <= 0) || (j <= 0)) throw MatrixSizeERROR();
    
    if (i != j){
        counterOfRows = counterOfColumns = 1;
        content = Dictionary();
        throw MatrixSizeERROR();
    }
    
    counterOfRows = i;
    counterOfColumns = j;
    content = Dictionary();
    
    for (unsigned long int k = 1; k <= counterOfRows; k++){
        content.addElement(k, k, RationalNumber(1));
    }
}

Matrix::Matrix(unsigned long int i, unsigned long int j, const RationalNumber &temp){
    if ((i <= 0) || (j <= 0)) throw MatrixSizeERROR();
    
    counterOfRows = i;
    counterOfColumns = j;
    content = Dictionary();
    
    if (temp == 0) return;
    for (unsigned long int k = 1; k <= counterOfRows; k++){
        for (unsigned long int l = 1; l <= counterOfColumns; l++){
            content.addElement(k, l, temp);
        }
    }
}

Matrix::Matrix(const Matrix &temp){
    counterOfRows = temp.counterOfRows;
    counterOfColumns = temp.counterOfColumns;
    content = Dictionary(temp.content);
}

Matrix::Matrix(FILE *input){
    unsigned long int tempInt, sizeOfTmpWord = 0;
    long int coordinatesOfRow, coordinatesOfColumn;
    char tmpChar = '!', *tmpWord = NULL;
    
    if (input == NULL){
        counterOfRows = counterOfColumns = 1;
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
        counterOfRows = counterOfColumns = 1;
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
    
    if (strcmp(tmpWord, "matrix") == 0){
        fscanf(input, "%zu", &tempInt);
        counterOfRows = tempInt;
        
        fscanf(input, "%zu", &tempInt);
        counterOfColumns = tempInt;
        
        content = Dictionary();
        
        fscanf(input, "%c", &tmpChar);
        while (tmpChar != EOF){
            if (tmpChar == '#'){
                while (tmpChar != '\n') fscanf(input, "%c", &tmpChar);
                if(fscanf(input, "%c", &tmpChar) == EOF)
                    tmpChar = EOF;
                continue;
            }
            coordinatesOfRow = -1;
            fscanf(input, "%zu", &coordinatesOfRow);
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
            if (coordinatesOfRow != -1) set(coordinatesOfRow, coordinatesOfColumn, RationalNumber(tmpWord));
            if (tmpChar == EOF) break;
        }
        free(tmpWord);
        
    }
    else{
        free(tmpWord);
        counterOfRows = counterOfColumns = 1;
        content = Dictionary();
    }
}


Matrix::~Matrix(){}


unsigned long int Matrix::getNumberOfRows()const{
    return counterOfRows;
}

unsigned long int Matrix::getNumberOfColumns()const{
    return counterOfColumns;
}


void Matrix::write(FILE *output){
    fprintf(output, "matrix\t%zu\t%zu\n\n", counterOfRows, counterOfColumns);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        fprintf(output, "%zu\t%zu\t", content.arrayOfElements[k].i, content.arrayOfElements[k].j);
        content.arrayOfElements[k].content.printIntoFile(output);
        fprintf(output, "\n");
    }
}

void Matrix::read(FILE *input){
    Matrix tmp(input);
    *this = tmp;
}

void Matrix::printMatrix(FILE *output){
    for (unsigned long int i = 1; i <= counterOfRows; i++){
        for (unsigned long int j = 1; j <= counterOfColumns; j++){
            if (content.get(i, j) != 0) content.get(i, j).printIntoFile(output);
            else content.get(i, j).printIntoFile(output);
            fprintf(output, " ");
        }
        
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void Matrix::set(unsigned long int i, unsigned long int j, const RationalNumber &temp){
    if ((i > counterOfRows) || (i <= 0) || (j > counterOfColumns) || (j <= 0))
        throw MatrixSizeERROR();
    
    if (temp == RationalNumber((char*)"0") && (content.get(i, j) == RationalNumber()))
        return;
    
    if (temp == RationalNumber((char*)"0")){
        content.removeElement(i, j);
        return;
    }
    content.addElement(i, j, temp);
}

void Matrix::makeCanonical(){
    for (unsigned long int i = 0; i < content.sizeOfArray; i++){
        content.arrayOfElements[i].content.makeCanonical();
    }
}


Matrix Matrix::operator+(const Matrix &temp)const{
    if ((counterOfRows != temp.counterOfRows) || (counterOfColumns != temp.counterOfColumns))
        throw MatrixSizeERROR();
    
    coordinatesOfMatrix coordinates;
    Matrix result(*this);
    for (unsigned long int k = 0; k < temp.content.sizeOfArray; k++){
        coordinates.row = temp.content.arrayOfElements[k].i;
        coordinates.column = temp.content.arrayOfElements[k].j;
        result.set(coordinates.row, coordinates.column, result[coordinates] + temp.content.arrayOfElements[k].content);
    }
    return result;
}


Matrix Matrix::operator-(const Matrix &temp)const{
    if ((counterOfRows != temp.counterOfRows) || (counterOfColumns != temp.counterOfColumns))
        throw MatrixSizeERROR();
    
    coordinatesOfMatrix coordinates;
    Matrix result(*this);
    for (unsigned long int k = 0; k < temp.content.sizeOfArray; k++){
        coordinates.row = temp.content.arrayOfElements[k].i;
        coordinates.column = temp.content.arrayOfElements[k].j;
        result.set(coordinates.row, coordinates.column, result[coordinates] - temp.content.arrayOfElements[k].content);
    }
    return result;
}

Matrix Matrix::operator-(){
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content *= -1;
    }
    
    return result;
}


Matrix Matrix::operator*(const RationalNumber &temp)const{
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content *= temp;
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &temp)const{
    if (counterOfColumns != temp.counterOfRows) throw MatrixSizeERROR();
    
    coordinatesOfMatrix coordinates1, coordinates2;
    RationalNumber tmp;
    Matrix result(counterOfRows, temp.counterOfColumns);
    
    for (unsigned long int i = 1; i <= counterOfRows; i++){
        coordinates1.row = i;
        for (unsigned long int j = 1; j <= temp.counterOfColumns; j++){
            tmp = 0;
            coordinates2.column = j;
            
            for (unsigned long int k = 1; k <= counterOfColumns; k++){
                coordinates1.column = k;
                coordinates2.row = k;
                tmp += (*this)[coordinates1] * temp[coordinates2];
            }
            
            result.set(i, j, tmp);
        }
    }
    return result;
}

Matrix operator*(const RationalNumber &op1, const Matrix &op2){
    return op2 * op1;
}


Matrix Matrix::operator=(const Matrix &temp){
    counterOfRows = temp.counterOfRows;
    counterOfColumns = temp.counterOfColumns;
    content = temp.content;
    return *this;
}


Matrix Matrix::operator/(const RationalNumber &temp)const{
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        result.content.arrayOfElements[k].content /= temp;
    }
    return result;
}


Matrix Matrix::operator~()const{
    unsigned long int tmp;
    Matrix result(*this);
    
    tmp = result.counterOfRows;
    result.counterOfRows = result.counterOfColumns;
    result.counterOfColumns = tmp;
    for (unsigned long int k = 0; k < content.sizeOfArray; k++){
        tmp = result.content.arrayOfElements[k].i;
        result.content.arrayOfElements[k].i = result.content.arrayOfElements[k].j;
        result.content.arrayOfElements[k].j = tmp;
    }
    return result;
}


RationalNumber Matrix::operator[](const coordinatesOfMatrix &temp)const{
    if ((temp.row > counterOfRows) || (temp.row <= 0) || (temp.column > counterOfColumns) || (temp.column <= 0))
        throw MatrixSizeERROR();
    return content.get(temp.row, temp.column);
}

Matrix Matrix::operator[](const coordinatesOfMatrixRow &temp)const{
    if ((temp.row <= 0) || (temp.row > counterOfRows))
        throw MatrixSizeERROR();
    coordinatesOfMatrix coordinates;
    coordinates.row = temp.row;
    Matrix result(1, counterOfColumns, RationalNumber());
    for (unsigned long int k = 1; k <= counterOfColumns; k++){
        coordinates.column = k;
        result.set(1, k, (*this)[coordinates]);
    }
    return result;
}

Matrix Matrix:: operator[](const coordinatesOfMatrixColumn &temp)const{
    if ((temp.column <= 0) || (temp.column > counterOfRows))
        throw MatrixSizeERROR();
    coordinatesOfMatrix coordinates;
    coordinates.column = temp.column;
    Matrix result(counterOfRows, 1, RationalNumber());
    for (unsigned long int k = 1; k <= counterOfRows; k++){
        coordinates.row = k;
        result.set(k, 1, (*this)[coordinates]);
    }
    return result;
}