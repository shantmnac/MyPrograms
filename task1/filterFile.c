#include <stdio.h>
#define N 256
void filterFile(FILE *fi, int max){
	char word[N], temp;
	int currentLen = -1, i;
	do{
		temp = getc(fi);

		if (temp == EOF) {
			if ((currentLen+1) <= max){
				for (i = 0; i <= currentLen ; i++){
					putchar (word[i]);
				}
				putchar (EOF);
			}
			break;
		}
		else {		
			if (temp == ' '){
				if ((currentLen+1) <= max){
					for (i = 0; i <= currentLen ; i++){
						putchar (word[i]);
					}
					putchar (' ');
				}
				currentLen = -1;
				continue;
			}
			else {
				if (temp == '\n'){			
					if ((currentLen+1) <= max){
						for(i = 0; i <= currentLen ; i++){
							putchar (word[i]);
						}
						putchar ('\n');
					}
					putchar ('\n');
					currentLen = -1;
					continue;	
				}
				else{
					currentLen++;
					word[currentLen] = temp;
				}
			}	
		}
	} while (temp != EOF);
} 

int main(int argc, char *argv[]){
	int choise, maxWord;
	char fileName[N];
	FILE *file;
		if (argc == 1){
			fprintf (stderr, "Enter file name\n>");
			scanf ("%s", fileName);
			file=fopen(fileName, "r");
			if (file == NULL){
				fprintf (stderr, "This file does not exist!\n");
			}
			else {	
				fprintf (stderr, "Enter max word length\n>");
				scanf ("%i", &maxWord);
				filterFile(file, maxWord);		
			}
		}
		else {
			if (argc == 2){
				file=fopen(argv[1], "r");
				if (file == NULL){
					fprintf (stderr, "This file does not exist!\n");
				}
				else {
					fprintf (stderr, "Enter max word length\n>");
					scanf ("%i", &maxWord);
					filterFile(file, maxWord);
				}
			}
			else {
				fprintf (stderr, "Exceeded the number of parameters!");
			}
		};
	return 0;
}
