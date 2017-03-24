#include "lexicalAnalysis.hpp"
#include "syntacticalAnalysis.hpp"
#include "errors.hpp"
#include <iostream>

Lexica tmp;
Syntaxis tmpSynt;
extern SyntaxERROR errorsArray;

int main(int argc, const char *argv[]) {
    
    FILE *in = fopen("graph_file_format.ypp", "r");
    
    tmp.analyzeFile(in);
    tmpSynt.parsing();
    errorsArray.print();
    printf("\n");

    fclose(in);
    return 0;
}
