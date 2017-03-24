#ifndef functions_h
#define functions_h

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <iterator>

int compareVectors(vector<string>, vector<string>);
void convertFileToSTL(void);
int searchInVector(vector <string>, string);
set <string> first(vector <string>, set <string>);
set <string> follow(int, set <string>);
int compareSets(set <string>, set <string>);
void checkRecursiveDescent(void);
void deleteNotTerminals(void);
void deleteSymbols(void);

#endif