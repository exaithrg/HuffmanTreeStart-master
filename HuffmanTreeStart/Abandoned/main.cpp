#include "HuffmanTree.h"

int main(void)
{
	char input[NAMESIZE], output[NAMESIZE];
	//printf("Input file:");
	//scanf("%s", input);
	printf("Output file:");
	scanf("%s", output);
	//EnCode(input);
	//system("pause");
	DeCode(output);
	system("pause");
	return 0;
}