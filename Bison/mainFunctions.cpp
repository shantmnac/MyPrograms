#include "syntacticalAnalysis.hpp"
#include "lexicalAnalysis.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <iterator>

using namespace std;

extern Terminal terminals;
extern Grammatic *grammatic;
extern unsigned int counterOfGrammatic;

vector <vector <vector <string> > > grammar;
vector <string> inputPoints;
set <string> terminalsSTL;
set <string> notTerminalsSTL;
set <vector <string> > checkedInFirst;
int epsilon = 0;

int compareVectors(vector<string> firstVector, vector<string> secondVector){
    if (firstVector.size() != secondVector.size()) return 1;
    for (unsigned int i = 0; i < (int)firstVector.size(); i++) if (firstVector[i] != secondVector[i]) return 1;
    return 0;
}

void convertFileToSTL(void){
    
    terminalsSTL.clear();
    for (unsigned int i = 0; i < terminals.counter; i++) terminalsSTL.insert(terminals.arrayOfTerminals[i]);
    
    string EPS("epsilon");
    terminalsSTL.insert(EPS);
    
    inputPoints.resize(counterOfGrammatic);
    for (unsigned int i = 0; i < counterOfGrammatic; i++){
        string temp(grammatic[i].inputPoint.name);
        inputPoints[i] = temp;
    }
    
    grammar.resize(counterOfGrammatic);
    for (unsigned int i = 0; i < counterOfGrammatic; i++){
        grammar[i].resize(grammatic[i].countOfAlternatives);
        for (unsigned int j = 0; j < grammatic[i].countOfAlternatives; j++){
            grammar[i][j].resize(grammatic[i].countOfMembersInAlternative[j]);
            for (unsigned int k = 0; k < grammatic[i].countOfMembersInAlternative[j]; k++){
                string temp(grammatic[i].alternatives[j][k].name);
                grammar[i][j][k] = temp;
            }
        }
    }
    
    
    for (unsigned int i = 0; i < (int)grammar.size(); i++){
        for (unsigned int j = 0; j < (int)grammar[i].size() - 1; j++){
            for (unsigned int k = j+1; k < (int)grammar[i].size(); k++)
                if (compareVectors(grammar[i][j],grammar[i][k]) == 0)
                    grammar[i].erase(grammar[i].begin()+k);
        }
    }
    
    notTerminalsSTL.clear();
    for (unsigned int i = 0; i < (int)inputPoints.size(); i++){
        for (unsigned int j = 0; j < (int)grammar[i].size(); j++){
            for (unsigned int k = 0; k < (int)grammar[i][j].size(); k++){
                if (grammar[i][j][k][0] != '\''){
                    if (terminalsSTL.find(grammar[i][j][k]) == terminalsSTL.end())
                        notTerminalsSTL.insert(grammar[i][j][k]);
                }
            }
        }
    }
    
    return;
}

int searchInVector(vector <string> vector, string strToFind){
    unsigned int i = 0;
    while (i != (unsigned int)vector.size()){
        if (vector[i] == strToFind) return i;
        i++;
    }
    return i;
}

set <string> first(vector <string> alternative, set <string> firstRec){
    if (checkedInFirst.find(alternative) != checkedInFirst.end()){
        epsilon = 0;
        return firstRec;
    }
    
    if (notTerminalsSTL.find(alternative[0]) == notTerminalsSTL.end()){
        firstRec.insert(alternative[0]);
        string EPS("epsilon");
        if (alternative[0] != EPS) epsilon = 0;
        else epsilon = 1;
        return firstRec;
    }
    
    checkedInFirst.insert(alternative);
    
    string EPS("epsilon");
    if (searchInVector(alternative, EPS) != (unsigned int)alternative.size()){
        firstRec.insert(EPS);
        epsilon = 1;
    }
    
    else (epsilon = 0);
    
    int tmpEpsilon = epsilon;
    epsilon = 1;
    unsigned int i = 0;
    while ((epsilon != 0) && (i < (unsigned int)alternative.size())){
        unsigned int w = searchInVector(inputPoints, alternative[i]);
        if (w == (int(inputPoints.size()))){
            firstRec.insert(EPS);
            return firstRec;
        }
        
        for (unsigned int q = 0; q < (unsigned int)grammar[w].size(); q++)
            firstRec = first(grammar[w][q], firstRec);
        i++;
    }
    
    epsilon = tmpEpsilon;
    return firstRec;
}

set <string> follow(int numberOfInputPoint, set <string> followRec){
    int flag = 0;
    
    for (unsigned int i = 0; i < (unsigned int)grammar.size(); i++)
        for (unsigned int j = 0; j < (unsigned int)grammar[i].size(); j++)
            for (unsigned int k = 0; k < (unsigned int)grammar[i][j].size(); k++)
                if (inputPoints[numberOfInputPoint] == grammar[i][j][k])
                    flag = 1;
    if (flag == 0){
        string temp("$");
        followRec.insert(temp);
        return followRec;
    }
    
    for (unsigned int i = 0; i < (unsigned int)grammar.size(); i++)
        for (int j = 0; j < (int)grammar[i].size(); j++){
            int N = searchInVector(grammar[i][j],inputPoints[numberOfInputPoint]);
            if (N < (int)grammar[i][j].size() - 1){
                set < string > fst;
                fst.clear();
                vector < string > alt;
                alt.resize(grammar[i][j].size() - 1 - N);
                for (int k = 0; k < (int)alt.size(); k++)
                    alt[k] = grammar[i][j][N+1+k];
                fst = first(alt,fst);
                string EPS("epsilon");
                fst.erase(EPS);
                followRec.insert(fst.begin(),fst.end());
                return followRec;
            }
        }
    
    for (unsigned int i = 0; i < (unsigned int)grammar.size(); i++)
        for (unsigned int j = 0; j < (unsigned int)grammar[i].size(); j++){
            int N = searchInVector(grammar[i][j],inputPoints[numberOfInputPoint]);
            if (N < (int)grammar[i][j].size() - 1){
                set <string> firstSet;
                firstSet.clear();
                vector <string> alternative;
                alternative.resize(grammar[i][j].size() - 1 - N);
                for (unsigned int k = 0; k < (unsigned int)alternative.size(); k++)
                    alternative[k] = grammar[i][j][N+ 1+ k];
                checkedInFirst.clear();
                firstSet = first(alternative, firstSet);
                
                string EPS("epsilon");
                if (firstSet.find(EPS) != firstSet.end()){
                    set <string> tmpFollow;
                    tmpFollow.clear();
                    tmpFollow = follow(i, tmpFollow);
                    followRec.insert(tmpFollow.begin(), tmpFollow.end());
                }
                return followRec;
            }
        }
    
    for (int i = 0; i < (int)grammar.size(); i++)
        for (int j = 0; j < (int)grammar[i].size(); j++)	{
            int N = searchInVector(grammar[i][j],inputPoints[numberOfInputPoint]);
            if (N == (int)grammar[i][j].size() - 1){
                set <string> tmpFollow;
                tmpFollow.clear();
                tmpFollow = follow(i, tmpFollow);
                followRec.insert(tmpFollow.begin(), tmpFollow.end());
                return followRec;
            }
        }
    
    return followRec;
}

int compareSets(set <string> firstSet, set <string> secondSet){
    set <string> thirdSet;
    thirdSet.clear();
    thirdSet.insert(firstSet.begin(), firstSet.end());
    thirdSet.insert(secondSet.begin(), secondSet.end());
    
    if ((thirdSet.size() == firstSet.size()) && (thirdSet.size() == secondSet.size())) return 2; //равны
    if ((int)thirdSet.size() != (int)firstSet.size() + (int)secondSet.size()) return 1; //пересечение
    return 0; //не равны
}

void checkRecursiveDescent(void){
    for (unsigned int i = 0; i < (int)grammar.size(); i++)
        for (unsigned int j = 0; j < (int)grammar[i].size() - 1; j++)
            for (unsigned int k = j+1; k < (int)grammar[i].size(); k++){
                set <string> first1;
                set < string > first2;
                first1.clear();
                first2.clear();
                
                checkedInFirst.clear();
                first1 = first(grammar[i][j], first1);
                checkedInFirst.clear();
                first2 = first(grammar[i][k], first2);
                
                if (compareSets(first2, first1) != 0){
                    fprintf(stderr, "Рекурсивный спуск не применим\n");
                    return;
                }
            }
    
    for (unsigned int i = 0; i < (int)grammar.size(); i++)
        for (unsigned int j = 0; j < (int)grammar[i].size() - 1; j++)
            for (unsigned int k = j+1; k < (int)grammar[i].size(); k++){
                set <string> firstTmp;
                set < string > followTmp;
                string EPS("epsilon");
                
                firstTmp.clear();
                followTmp.clear();
                checkedInFirst.clear();
                firstTmp = first(grammar[i][j], firstTmp);
                if (firstTmp.find(EPS) != firstTmp.end()){
                    checkedInFirst.clear();
                    firstTmp = first(grammar[i][k], firstTmp);
                    checkedInFirst.clear();
                    followTmp = follow(i, followTmp);
                    if (compareSets(firstTmp, followTmp) == 1){
                        fprintf(stderr, "Рекурсивный спуск не применим\n");
                        return;
                    }
                }
                
                firstTmp.clear();
                followTmp.clear();
                checkedInFirst.clear();
                firstTmp = first(grammar[i][k], firstTmp);
                if (firstTmp.find(EPS) != firstTmp.end()){
                    checkedInFirst.clear();
                    firstTmp = first(grammar[i][j], firstTmp);
                    checkedInFirst.clear();
                    followTmp = follow(i, followTmp);
                    if (compareSets(firstTmp, followTmp) != 0){
                        fprintf(stderr,"Рекурсивный спуск не применим\n");
                        return;
                    }
                }
            }
    
    fprintf(stderr, "Рекурсивный спуск применим!\n");
    
    string epsilon("epsilon");
    terminalsSTL.erase(epsilon);
    
    return;
}

void deleteNotTerminals(void){
    set <string> set1, set2;
    set1.clear();
    set2.clear();
    
    do{
        set1.clear();
        set1.insert(set2.begin(), set2.end());
        
        for (unsigned int i = 0; i < (int)inputPoints.size(); i++)
            for (int j = 0; j < (int)grammar[i].size(); j++)
            {
                int flag = 1;
                for (int k = 0; k < (int)grammar[i][j].size(); k++)
                {
                    string currentWord = grammar[i][j][k];
                    if (notTerminalsSTL.find(currentWord) != notTerminalsSTL.end())
                        if (set1.find(currentWord) == set1.end()) flag = 0;
                }
                if (flag == 1)  set2.insert(inputPoints[i]);
            }
    } while (compareSets(set1, set2) != 2);
    
    int i = 0;
    
    while (i < (int)inputPoints.size()){
        if (set2.find(inputPoints[i]) == set2.end())
        {
            inputPoints.erase(inputPoints.begin()+i);
            grammar.erase(grammar.begin() + i);
        }
        else i++;
    }
    
    int j;
    for (i = 0; i < (int)grammar.size(); i++){
        j = 0;
        while (j < (int)grammar[i].size()){
            int flag = 1;
            for (int k = 0; k < (int)grammar[i][j].size(); k++){
                string currentWord = grammar[i][j][k];
                if (notTerminalsSTL.find(currentWord) != notTerminalsSTL.end())
                    if (set1.find(currentWord) == set1.end()) flag = 0;
            }
            if (flag == 0) grammar[i].erase(grammar[i].begin()+j);
            else j++;
        }
    }
    return;
}

void deleteSymbols(void){
    set <string> set1, set2;
    set1.clear();
    set2.clear();
    if (inputPoints.size() == 0) return;
    set2.insert(inputPoints[0]);
    
    do{
        set1.clear();
        set1.insert(set2.begin(), set2.end());
        
        for (int i = 0; i < (int)inputPoints.size(); i++)
            if (set1.find(inputPoints[i]) != set1.end())
                for (int j = 0; j < (int)grammar[i].size(); j++)
                    for (int k = 0; k < (int)grammar[i][j].size(); k++)
                        set2.insert(grammar[i][j][k]);
        
    } while (compareSets(set2, set1) != 2);
    
    int i = 0;
    
    while (i < (int)inputPoints.size()){
        if (set2.find(inputPoints[i]) == set2.end()){
            inputPoints.erase(inputPoints.begin() + i);
            grammar.erase(grammar.begin() + i);
        }
        else i++;
    }
    
    return;
}