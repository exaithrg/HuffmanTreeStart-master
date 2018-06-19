#include"HuffmanTree.h"

int main(void)
{
	char input[20] = "";
	char output[20] = "";
	scanf("%s", input);
	scanf("%s", output);
	HuffmanTree tree;
	FILE *in = fopen(input, "r");
	FILE *out = fopen(output, "wb");
	if (in == NULL)
	{
		printf("File open process retured with a failure!\n");
		system("pause");
		exit(-1);
	}
	FileStatistic(in);
	tree = HuffmanTreeInit();
	EnCode(tree, in, out);
	system("pause");
	return 0;
}