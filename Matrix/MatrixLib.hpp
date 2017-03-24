#ifndef MATRIXLIB_HPP
#define MATRIXLIB_HPP

#include "DictionaryLib.hpp"
#include "RationalNumberLib.hpp"

struct coordinatesOfMatrix{
    unsigned long int row, column;
    
    coordinatesOfMatrix(){
        row = column = 1;
    }
    coordinatesOfMatrix(unsigned long int rowTmp, unsigned long int columnTmp){
        row = rowTmp;
        column = columnTmp;
    }
};

struct coordinatesOfMatrixRow{
    unsigned long int row;
    
    coordinatesOfMatrixRow(){
        row = 1;
    }
    coordinatesOfMatrixRow(unsigned long int rowTmp){
        row = rowTmp;
    }
};

struct coordinatesOfMatrixColumn{
    unsigned long int column;
    
    coordinatesOfMatrixColumn(){
        column = 1;
    }
    coordinatesOfMatrixColumn(unsigned long int columnTmp){
        column = columnTmp;
    }
};

class Matrix{
protected:
    unsigned long int counterOfRows;
    unsigned long int counterOfColumns;
    Dictionary content;
public:
    Matrix();
    Matrix(unsigned long int, unsigned long int);
    Matrix(unsigned long int, unsigned long int, const RationalNumber&);
    Matrix(const Matrix&);
    Matrix(FILE*);
    
    ~Matrix();
    
    unsigned long int getNumberOfRows()const;
    unsigned long int getNumberOfColumns()const;
    
    void read(FILE*);
    void write(FILE*);
    void printMatrix(FILE*);
    void set(unsigned long int, unsigned long int, const RationalNumber&);
    void makeCanonical();
    
    
    Matrix operator+(const Matrix&)const;
    
    Matrix operator-(const Matrix&)const;
    Matrix operator-();
    
    Matrix operator*(const RationalNumber&)const;
    Matrix operator*(const Matrix&)const;
    friend Matrix operator*(const RationalNumber&, const Matrix&);
    
    Matrix operator=(const Matrix&);
    
    Matrix operator/(const RationalNumber&)const;
    
    Matrix operator~()const;
    
    RationalNumber operator[](const coordinatesOfMatrix&)const;
    Matrix operator[](const coordinatesOfMatrixRow&)const;
    Matrix operator[](const coordinatesOfMatrixColumn&)const;
};

#endif