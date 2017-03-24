#include <stdio.h>
#define MAXWORD 265

void reflectString(void){
	char words[MAXWORD], temp;
	int i = -1, j, bufOverload = 0;
	temp = getchar();
	while ((temp != EOF) && (bufOverload <= 256)) {
		while (temp != '\n'){
			i++;
			words[i] = temp;
			bufOverload++;
			temp = getchar();
			if (temp == EOF){
				for (j=i; j>=0; j--){
					putchar(words[j]);
				}
				return;
			}
		}
		for (j=i; j>=0; j--){
			putchar(words[j]);
		}
		putchar('\n');
		temp = getchar();
		i = -1;
	}
}

int main(void){
	reflectString();
	putchar ('\n');
	return 0;
}
