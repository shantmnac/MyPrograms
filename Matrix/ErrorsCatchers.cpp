#include "ErrorsCatchers.hpp"
#include <stdio.h>

DenominatorIsZeroERROR::DenominatorIsZeroERROR(){};

DividerIsZeroERROR::DividerIsZeroERROR(){};

MemoryAllocatingERROR::MemoryAllocatingERROR(unsigned long int sizeTemp):size(sizeTemp){};

TypesConvertingERROR::TypesConvertingERROR(){};

MatrixSizeERROR::MatrixSizeERROR(){};

VectorSizeERROR::VectorSizeERROR(){};