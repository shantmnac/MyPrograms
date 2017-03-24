#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define NAMELEN 256

void sortStrings();
void writeStrings(char**, FILE*);
void graph(FILE*, int);
int cmpStrings(char*, char*);
char **readString();

int counterForStrings = 0;
int nFlag = 1;

struct charRate{
    char ch;
    double rate;
};

int main(int argc, char *argv[]){
    int screenWidth, i;
    char fileName1[NAMELEN], fileName2[NAMELEN], **arrayOfStrings;
    FILE *input, *output;
    if ((argc > 1)){
        if (argc == 4){
            
            input = fopen(argv[1], "rb");
            if (input == NULL) {
                printf("Input file does not exist!\n");
                fclose(input);
                exit(1);
            }
            
            output = fopen(argv[2], "wb");
            if (output == NULL) {
                printf("Output file does not exist!\n");
                fclose(input);
                fclose(output);
                exit(1);
            }
            
            screenWidth = atoi(argv[3]);
            if (screenWidth <= 0){
                printf("The width should be positive!\n");
                fclose(input);
                fclose(output);
                exit(1);
            }
        }
        else{
            printf("Wrong number of parameters!\n");
            return 0;
        }
    }
    else {
        printf("Enter input file name\n>");
        scanf("%s", fileName1);
        input = fopen(fileName1, "rb");
        if (input == NULL) {
            printf("This file does not exist!\n");
            fclose(input);
            exit(1);
        }
        
        printf("Enter output file name\n>");
        scanf("%s", fileName2);
        output = fopen(fileName2, "wb");
        if (output == NULL) {
            printf("This file does not exist!\n");
            fclose(input);
            fclose(output);
            exit(1);
        }
        
        printf("Enter screen width\n>");
        scanf("%i", &screenWidth);
        if (screenWidth <= 0){
            printf("The width should be positive!\n");
            fclose(input);
            fclose(output);
            exit(1);
        }
    }
    
    arrayOfStrings = readString(input);
    if (counterForStrings == 0) {
        printf("Input file is empty!\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    sortStrings(arrayOfStrings);
    writeStrings(arrayOfStrings, output);
    graph(input, screenWidth);
    
    for (i=0; i<counterForStrings; i++) {
        free(arrayOfStrings[i]);
    }
    free(arrayOfStrings);
    fclose(input);
    fclose(output);
    return 0;
}

void graph(FILE *inputFile, int width){
    char temp;
    int arrCounter = 0, i, j, simFlag = 0, max = 0, sumChar = 0;
    struct charRate *charArray = NULL;
    fseek(inputFile, 0L, 0);
    do{
        fread(&temp, sizeof(char), 1, inputFile);
        if (feof(inputFile)) {
            break;
        }
        for (i=0; i<arrCounter; i++){
            if (temp == charArray[i].ch) {
                simFlag = 1;
                charArray[i].rate++;
                break;
            }
        }
        if (!simFlag) {
            arrCounter++;
            charArray = (struct charRate*)realloc(charArray, arrCounter * sizeof(struct charRate));
            charArray[arrCounter-1].ch = temp;
            charArray[arrCounter-1].rate = 1;
        }
        simFlag = 0;
    }while (!feof(inputFile));
    
    for (i=0; i<arrCounter; i++) {
        if (charArray[i].rate > max){
            max = charArray[i].rate;
        }
    }
    
    printf("\n");
    
    for (i=0; i<arrCounter; i++) {
        sumChar += charArray[i].rate;
        if (((int)charArray[i].ch <= 32) || ((int)charArray[i].ch == 127)) {
            printf("#%i\t\t|%i\t|", (int)charArray[i].ch, (int)charArray[i].rate);
        }
        else{
            printf("%c\t\t|%i\t|", charArray[i].ch, (int)charArray[i].rate);
        }
        
        for (j = 1; j<=(charArray[i].rate/max*width); j++) {
            
            printf("*");
        }
        printf("\n");
    }
    printf("Total of %i characters\n", sumChar);
    
    
    free(charArray);
}

char **readString(FILE *inputFile){
    int counterForChars = 0, charFlag = 0;
    char temp = '!', enter = '\n', *arrayOfChars = NULL, **arrayOfStrings = NULL;
    do{
        while(temp != '\n'){
            fread(&temp, sizeof(char), 1, inputFile);
            counterForChars++;
            arrayOfChars = (char*)realloc(arrayOfChars, counterForChars * sizeof(char));
            if (arrayOfChars == NULL) {
                printf("Not enough memory");
                exit (1);
            }
            if (feof(inputFile)) {
                break;
            }
            charFlag = 1;
            arrayOfChars[counterForChars-1] = temp;
        }
        if (charFlag) {
            if (feof(inputFile)) {
                nFlag = 0;
                if (arrayOfChars == NULL) {
                    printf("Not enough memory");
                    exit (1);
                }
                arrayOfChars[counterForChars-1] = enter;
                
            }
            else{
                charFlag = 0;
                counterForChars = 0;
                temp = '!';
            }
            counterForStrings++;
            arrayOfStrings = (char**)realloc(arrayOfStrings, counterForStrings * sizeof(char*));
            if (arrayOfStrings == NULL) {
                printf("Not enough memory");
                exit (1);
            }
            arrayOfStrings[counterForStrings-1] = arrayOfChars;
            arrayOfChars = NULL;
        }
    }while (!(feof(inputFile)));
    
    free(arrayOfChars);
    
    return arrayOfStrings;
}


void sortStrings(char **arrayOfStrings){
    int sortCounter = 0;
    char *tempString;
    while((sortCounter + 1) != counterForStrings){
        if (cmpStrings(arrayOfStrings[sortCounter], arrayOfStrings[sortCounter+1])) {
            sortCounter++;
        }
        else{
            tempString = arrayOfStrings[sortCounter];
            arrayOfStrings[sortCounter] = arrayOfStrings[sortCounter+1];
            arrayOfStrings[sortCounter+1] = tempString;
            sortCounter = 0;
        }
    }
};

int cmpStrings(char *firstString, char *secondString){  //1 - арг1<арг2, 0 - арг1>арг2
    int i = 0;
    while ((firstString[i] != '\n') && (secondString[i] != '\n')) {
        if (firstString[i] == secondString[i]) {
            i++;
        }
        else {
            if (firstString[i] < secondString[i]){
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    if (firstString[i] == '\n') {
        return 1;
    }
    else {
        return 0;
    }
}


void writeStrings(char **stringArray, FILE *outputFile){
    int i=0, j;
    char enter = '\n';
    if (nFlag)
        for (j=0; j<=(counterForStrings-1); j++){
            while (stringArray[j][i] != '\n') {
                fwrite(&(stringArray[j][i]), sizeof(char), 1, outputFile);
                i++;
            }
            fwrite(&enter, sizeof(char), 1, outputFile);
            i = 0;
        }
    else {
        for (j=0; j<(counterForStrings-1); j++){
            while (stringArray[j][i] != '\n') {
                fwrite(&(stringArray[j][i]), sizeof(char), 1, outputFile);
                i++;
            }
            fwrite(&enter, sizeof(char), 1, outputFile);
            i = 0;
        }
        j = (counterForStrings - 1);
        i = 0;
        while (stringArray[j][i] != '\n') {
            fwrite(&(stringArray[j][i]), sizeof(char), 1, outputFile);
            i++;
        }
    }
}
