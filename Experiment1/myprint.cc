#include "myprint.h"

void my_print(char *string) {
	printf("The string is %s\n", string);
}

void my_print2(char *string) {
	char *string2;
	int size, i;
	size = strlen(string);
	string2 = (char *) malloc (size + 1);
	for (i = 0; i < size; i++)
        string2[size - i - 1] = string[i];
	printf("The string printed backward is %s\n", string2);
}
