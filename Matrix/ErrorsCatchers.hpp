#ifndef ERRORSCATCHERSLIB_HPP
#define ERRORSCATCHERSLIB_HPP

class DenominatorIsZeroERROR{
public:
    DenominatorIsZeroERROR();
};

class DividerIsZeroERROR{
public:
    DividerIsZeroERROR();
};

class MemoryAllocatingERROR{
public:
    unsigned long int size;
    MemoryAllocatingERROR(unsigned long int);
};

class TypesConvertingERROR{
public:
    TypesConvertingERROR();
};

class MatrixSizeERROR{
public:
    MatrixSizeERROR();
};

class VectorSizeERROR{
public:
    VectorSizeERROR();
};

#endif