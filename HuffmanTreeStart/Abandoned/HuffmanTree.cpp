#include"HuffmanTree.h"

void StrCopy(char *str, const char *c)
{
	char *s = str;
	while (*c != '\0')
		*s++ = *c++;
	*s = '\0';
	return;
}

void WeightSelect(HuffmanTree T, int n, int &l, int &r)
{
	HuffmanTree p = T + 1;
	int a = 0, b = 0;
	for (int i = 1; i <= n; ++i, ++p) 
	{
		if (!p->parent)
		{
			if (!a)
			{
				l = i;
				a = 1;
			}
			else if (!b)
			{
				r = i;
				b = 1;
			}
			else if (p->weight < (T + l)->weight || p->weight < (T + r)->weight)
			{
				if ((T + l)->weight <= (T + r)->weight)
					r = i;
				else
					l = i;
			}
		}
	}
	return;
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n)
{
	int f, c, start, m1, m2, i, m = 2 * n - 1;
	HuffmanTree p;
	HT = (HuffmanTree)malloc(sizeof(HTNode)*(m + 1));
	if (!HT)
	{
		printf("Out of memory!\n");
		exit(-1);
	}
	for (p = HT + 1, i = 1; i <= n; ++i, ++w, ++p)
	{
		p->weight = *w;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	for (i = n + 1; i <= m; ++i, ++p)
	{
		p->weight = 0;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	for (i = n + 1, p = HT + i; i <= m; ++i, ++p) 
	{
		WeightSelect(HT, i - 1, m1, m2);
		p->weight = (HT + m1)->weight + (HT + m2)->weight;
		p->lchild = m1;
		p->rchild = m2;
		(HT + m1)->parent = i;
		(HT + m2)->parent = i;
	}
	HC = (HuffmanCode)malloc(sizeof(char*)*(n + 1));
	if (!HC) 
	{
		printf("Out of memory!");
		exit(-1);
	}
	char *cd = (char*)malloc(sizeof(char)*n);
	cd[n - 1] = '\0';
	for (i = 1; i <= n; ++i) 
	{
		start = n - 1;
		for (f = HT[i].parent, c = i; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c)
				cd[--start] = '0';
			else
				cd[--start] = '1';
		}
		HC[i] = (char*)malloc(sizeof(char)*(n - start));
		StrCopy(HC[i], cd + start);
	}
	free(cd);
	return;
}

void HuffmanCodeOutput(HuffmanCode C, int* s, int n)
{
	char *p;
	for (int i = 1; i <= n; ++i)
	{
		printf("Weight:%3d，Code:", s[i - 1]);
		p = C[i];
		while (*p != '\0')
			printf("%c", *p++);
		printf("\n");
	}
	return;
}

void ReSync(HuffmanTree &T, char *s, int *x, int n, long long int &m)
{
	HuffmanTree p = T + 2 * n - 1;
	m = 0;
	while (*s != '#')
	{
		if (*s == '0' && p->lchild)
			p = T + p->lchild;
		else if (*s == '1' && p->rchild)
			p = T + p->rchild;
		if (!p->lchild && !p->rchild)
		{
			if (*s == '0')
				*x = (T + p->parent)->lchild;
			else
				*x = (T + p->parent)->rchild;
			x++;
			p = T + 2 * n - 1;
			m++;
		}
		s++;
	}
	return;
}

void EnCode(const char *s)
{
	FILE *pr, *pw;
	int a, i = 0, weight[256] = { 0 }, j, n = 256, m = 2 * n - 1;
	HuffmanTree HT; 
	HuffmanCode HC, hc;
	char CodedName[NAMESIZE], c = ' ';
	while (s[i] != '.')
	{
		CodedName[i] = s[i];
		i++;
	}
	CodedName[i] = '.';
	CodedName[1 + i] = 'e';
	CodedName[2 + i] = 'n';
	CodedName[3 + i] = 'c';
	CodedName[4 + i] = '\0';
	pr = fopen(s, "rb");
	if (!pr)
	{
		printf("Failed to open the file!\n");
		exit(-1);
	}
	a = fgetc(pr);
	while (!feof(pr))
	{
		weight[a]++;
		a = fgetc(pr);
	}
	fclose(pr);
	HuffmanCoding(HT, HC, weight, 256);
	pw = fopen(CodedName, "wb");
	//先写入原拓展名 ，占4个字节。 不足4个用空格补齐;。
	j = 0;
	while (s[++i])
	{
		fwrite(s + i, 1, 1, pw);
		j++;
	}
	while (j++ < 4)
		fwrite(&c, 1, 1, pw);
	for (i = n + 1; i <= m; i++)
	{
		fwrite(&((HT + i)->lchild), 2, 1, pw);
		fwrite(&((HT + i)->rchild), 2, 1, pw);
	}
	//FILE *prw = fopen(s, "ab");
	//fseek(prw, 0, SEEK_END);
	//fwrite("a", 1, 1, prw);
	//fclose(prw);
	pr = fopen(s, "rb");
	a = fgetc(pr);
	i = 0;
	int b = 0;
	hc = HC;
	char *p = *(hc + a + 1);
	while (!feof(pr) || *p != '\0')
	{
		for (i = 0; i < 8; i++)
		{
			if (*p == '\0')
			{
				a = fgetc(pr);
				if (a >= 0)
					p = hc[a + 1];
				else
				{
					int h = 1;
					for (j = 0; j < 8 - i; j++)
						h = h * 2;
					b = b * h;
					break;
				}
			}
			int h = 1;
			for (j = 0; j < 7 - i; j++)
				h = h * 2;
			b = b + (*p - 48)*h;
			++p;
		}
		fwrite(&b, 1, 1, pw);
		b = 0;
	}
	printf("EnCoding finished!\n");
	fclose(pr);
	fclose(pw);
	return;
}

void DeCode(const char *s)
{
	printf("Starting to decode!\n");
	FILE *pr;
	static int n = 256;
	int a, i = 0, j, m = 2 * n - 1;
	static HuffmanTree HT;
	static char DeCodeName[NAMESIZE];
	i = 0;
	while (s[i] != '.')
	{
		DeCodeName[i] = s[i];
		i++;
	}
	DeCodeName[i] = '.';
	i++;
	pr = fopen(s, "rb");
	if (!pr)
	{
		printf("Failed to access the file!\n");
		exit(-1);
	}
	j = 0;
	a = fgetc(pr);
	while (j < 4)
	{
		DeCodeName[i] = a;
		if (j < 3)
			a = fgetc(pr);
		j++;
		i++;
	}
	DeCodeName[i] = '\0';
	HT = (HuffmanTree)malloc(sizeof(HTNode)*(m + 1));
	HT[0].weight = 0;
	HT[0].parent = 0;
	HT[0].lchild = 0;
	HT[0].rchild = 0;
	for (i = 1; i <= n; i++)
	{
		HT[i].weight = 0; 
		HT[i].lchild = 0;
		HT[i].rchild = 0;
	}
	for (i = n + 1; i <= m; i++)
	{
		HT[i].weight = 0;
		fread(&a, 2, 1, pr);
		HT[i].lchild = a;
		HT[a].parent = i;
		fread(&a, 2, 1, pr);
		HT[i].rchild = a;
		HT[a].parent = i;
	}
	HT[m].parent = 0;
	long long int gm = 0;
	char aa[400000], b[8];
	int x[50000], d;
	a = fgetc(pr);
	while (!feof(pr))
	{
		for (i = 0; i < 8; i++)
		{
			if (a)
			{
				b[7 - i] = 48 + a % 2;
				a = a / 2;
			}
			else
				b[7 - i] = '0';
		}
		for (i = 0; i < 8; i++)
			aa[8 * j + i] = b[i];
		j++;
		d = a;
		a = fgetc(pr);
	}
	for (i = 0; i < 8; i++)
	{
		if (a)
		{
			b[7 - i] = 48 + a % 2;
			a = a / 2;
		}
		else
			break;
	}
	for (int k = 0; k < i; k++)
		aa[8 * j - 8 + k] = b[8 - i + k];
	aa[8 * j - 8 + i] = '0';
	aa[8 * j - 8 + i + 1] = '#';
	ReSync(HT, aa, x, n, gm);
	FILE *fw;
	fw = fopen(DeCodeName, "wb");
	for (i = 0; i < gm; i++)
	{
		x[i] = x[i] - 1;
		fwrite(x + i, 1, 1, fw);
	}
	fclose(fw);
	fclose(pr);
	printf("Decode finished!\n");
	return;
}