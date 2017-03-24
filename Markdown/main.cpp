#include "textViewer.hpp"
#include "otherFunctions.hpp"
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cmath>

extern char *optarg;
extern int optind, opterr, optopt;

char *fileName = NULL, *helpFileName = (char*)"help.txt", markedListSymbol = '*';
unsigned int listIndent = 2, screenWidth = 20, redLineSize = 1;
float programVersion = 1;
struct winsize terminal;

int main(int argc, char *argv[]){
    char result;
    
    //ioctl(0, TIOCGWINSZ, &terminal);
    //screenWidth = terminal.ws_col;
    
    if (argc == 1) {
        printf("Invalid count of parametrs!\n");
        return 0;
    }
    
    while ((result = getopt(argc, argv, "f:w::t::m::r::hv")) != -1){
        switch (result){
            case 'f': analizeArgument('f'); break;
            case 'w': analizeArgument('w'); break;
            case 't': analizeArgument('t'); break;
            case 'm': analizeArgument('m'); break;
            case 'r': analizeArgument('r'); break;
            case 'h': printHelp(); return 0;
            case 'v': printf("You're using the latest version of program (%.2f)\n",
                             programVersion); return 0;
            case '?': printf("invalid parameter or argument!\n"); return 0;
        }
            
    }
    
    if (screenWidth < 4) {printf("Screen size is too small!\n"); return 0;}
    
    FILE *in = fopen(fileName, "rt");
    FILE *out = fopen("out.txt", "wt");
    
    TextViewer tst(in);
    tst.print(out);
    
    fclose(in);
    fclose(out);
    
    if (fileName != NULL) free(fileName);
    
    return 0;
}
