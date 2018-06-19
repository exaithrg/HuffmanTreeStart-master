#include "HuffmanTree.h"

int main(int argc, char *argv[])
{
	const char *opt;
	const char *str;
	//opt = argv[1];
	//str = argv[2];
	opt = "-e";
	str = "C:\\Users\\syhao\\Desktop\\Huff\\test.txt";
	if (argc > 0)
	{
		if (strcmp(opt, "-e") == 0)//比较opt与-e，相同则执行
			EnCode(str);
		else if (strcmp(opt, "-d") == 0)
			DeCode(str);
	}
	return 0;
}