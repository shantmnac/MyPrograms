#include "textViewer.hpp"
#include "regularText.hpp"
#include "headerText.hpp"
#include "listText.hpp"
#include "codeText.hpp"
#include "errors.hpp"
#include <iostream>

TextViewer::TextViewer(){
    countOfParagraphs = 0;
    countOfSymbols = 0;
    countOfWords = 0;
    text = NULL;
}

TextViewer::TextViewer(FILE *input){
    char tmpChar = '\0', *tmpWord = NULL;
    bool tmpBool = true;
    unsigned int i = 0;
    
    if (input == NULL) throw NoSuchFileERROR();
    
    countOfParagraphs = 0;
    countOfSymbols = 0;
    countOfWords = 0;
    text = NULL;
    
    do{
        tmpChar = getc(input);
        if (tmpChar == EOF) {
            for (unsigned int q = 0; q < countOfParagraphs; q++) {
                countOfWords += text[q] -> countOfWords();
                countOfSymbols += text[q] -> countOfSymbols();
            }
            return;
        }
        if (tmpChar == '\n') continue;
        try {
            text = (AbstractText**)realloc(text, (countOfParagraphs + 1) * sizeof(AbstractText*));
            if (text == NULL) throw MemoryAllocatingERROR((countOfParagraphs + 1) * sizeof(AbstractText*));
            
            
            switch (tmpChar) {
                case '#':{
                    ungetc(tmpChar, input);
                    text[countOfParagraphs] = new HeaderText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case '*':{
                    ungetc(tmpChar, input);
                    text[countOfParagraphs] = new ListText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case '\t':{
                    text[countOfParagraphs] = new CodeText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case ' ':{
                    for (unsigned int q = 0; q < 3; q++) {
                        tmpChar = getc(input);
                        if (tmpChar != ' ') {
                            ungetc(tmpChar, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            text[countOfParagraphs] = new RegularText;
                            text[countOfParagraphs] -> read(input);
                            tmpBool = false;
                            break;
                        }
                        else tmpBool = true;
                    }
                    if (tmpBool) {
                        text[countOfParagraphs] = new CodeText;
                        text[countOfParagraphs] -> read(input);
                    }
                    break;
                }
                    
                default:{
                    i = 0;
                    tmpBool = false;
                    ungetc(tmpChar, input);
                    do{
                        if (isdigit(tmpChar)) {
                            tmpChar = getc(input);
                            tmpWord = (char*)realloc(tmpWord, i + 1);
                            tmpWord[i] = tmpChar;
                            i++;
                        }
                        else {
                            if (tmpChar != '.') {
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(tmpWord[i - q - 1], input);
                                }
                                if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                text[countOfParagraphs] = new RegularText;
                                text[countOfParagraphs] -> read(input);
                                tmpBool = true;
                            }
                            else{
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(tmpWord[i - q - 1], input);
                                }
                                if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                text[countOfParagraphs] = new ListText;
                                text[countOfParagraphs] -> read(input);
                                tmpBool = true;
                            }
                        }
                    }while(!tmpBool);
                    break;
                }
            }
            countOfParagraphs++;
        }
        catch (EmptyParagraphERROR) {
            delete text[countOfParagraphs];
            countOfParagraphs--;
            text = (AbstractText**)realloc(text, (countOfParagraphs) * sizeof(AbstractText*));
            if (text == NULL) throw MemoryAllocatingERROR((countOfParagraphs) * sizeof(AbstractText*));
        }
        catch(TextProcessingERROR){}
    }while(1);
}

TextViewer::~TextViewer(){
    if(text != NULL) {
        for (unsigned int q = 0; q < countOfParagraphs; q++) delete text[q];
        free(text);
    }
}

void TextViewer::read(FILE *input){
    char tmpChar = '\0', *tmpWord = NULL;
    bool tmpBool = true;
    unsigned int i = 0;
    
    if (input == NULL) throw NoSuchFileERROR();
    
    countOfParagraphs = 0;
    countOfSymbols = 0;
    countOfWords = 0;
    text = NULL;
    
    do{
        tmpChar = getc(input);
        if (tmpChar == EOF) {
            for (unsigned int q = 0; q < countOfParagraphs; q++) {
                countOfWords += text[q] -> countOfWords();
                countOfSymbols += text[q] -> countOfSymbols();
            }
            return;
        }
        if (tmpChar == '\n') continue;
        try {
            text = (AbstractText**)realloc(text, (countOfParagraphs + 1) * sizeof(AbstractText*));
            if (text == NULL) throw MemoryAllocatingERROR((countOfParagraphs + 1) * sizeof(AbstractText*));
            
            
            switch (tmpChar) {
                case '#':{
                    ungetc(tmpChar, input);
                    text[countOfParagraphs] = new HeaderText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case '*':{
                    ungetc(tmpChar, input);
                    text[countOfParagraphs] = new ListText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case '\t':{
                    text[countOfParagraphs] = new CodeText;
                    text[countOfParagraphs] -> read(input);
                    break;
                }
                    
                case ' ':{
                    for (unsigned int q = 0; q < 3; q++) {
                        tmpChar = getc(input);
                        if (tmpChar != ' ') {
                            ungetc(tmpChar, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            text[countOfParagraphs] = new RegularText;
                            text[countOfParagraphs] -> read(input);
                            tmpBool = false;
                            break;
                        }
                        else tmpBool = true;
                    }
                    if (tmpBool) {
                        text[countOfParagraphs] = new CodeText;
                        text[countOfParagraphs] -> read(input);
                    }
                    break;
                }
                    
                default:{
                    i = 0;
                    tmpBool = false;
                    ungetc(tmpChar, input);
                    do{
                        if (isdigit(tmpChar)) {
                            tmpChar = getc(input);
                            tmpWord = (char*)realloc(tmpWord, i + 1);
                            tmpWord[i] = tmpChar;
                            i++;
                        }
                        else {
                            if (tmpChar != '.') {
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(tmpWord[i - q - 1], input);
                                }
                                if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                text[countOfParagraphs] = new RegularText;
                                text[countOfParagraphs] -> read(input);
                                tmpBool = true;
                            }
                            else{
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(tmpWord[i - q - 1], input);
                                }
                                if (tmpWord != NULL) {free(tmpWord); tmpWord = NULL;}
                                text[countOfParagraphs] = new ListText;
                                text[countOfParagraphs] -> read(input);
                                tmpBool = true;
                            }
                        }
                    }while(!tmpBool);
                    break;
                }
            }
            countOfParagraphs++;
        }
        catch (EmptyParagraphERROR) {
            delete text[countOfParagraphs];
            countOfParagraphs--;
            text = (AbstractText**)realloc(text, (countOfParagraphs) * sizeof(AbstractText*));
            if (text == NULL) throw MemoryAllocatingERROR((countOfParagraphs) * sizeof(AbstractText*));
        }
        catch(TextProcessingERROR){}
    }while(1);
}

void TextViewer::print(FILE *output){
    for (unsigned int q = 0; q < countOfParagraphs; q++) text[q] -> print(output);
}