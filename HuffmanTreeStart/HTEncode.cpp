#include"HuffmanTree.h"
/*
修改日志
1. 略 6.18
*/

//字符串拷贝，将c对应的内容拷贝到str对应的字符串空间里，直到遇到文件结束符为止
void StrCopy(char *str, const char *c)//c是一个指向字符常量的const指针
{
	char *s = str;
	while (*c != '\0')//c不是文件结束符
		*s++ = *c++;//s指向的是c指向的。先进行*s=*c，再分别自加
	*s = '\0';//补充上遗漏的文件结束符
	return;
}

//作用：将l和r两个变量置为权值最小的两个孤立点坐标
void WeightSelect(HuffmanTree T, int n, int &l, int &r)
//left，right
{
	HuffmanTree p = T + 1;//结点
	int a = 0, b = 0, i;
	for (i = 1; i <= n; ++i, ++p)//小修
		//注意是<=n，n=256，就是ASCII数组的n总量
	{
	//p不孤立那么p++
		if (!p->parent)//如果p是孤立的
		{
			if (!a)//如果a是0.语句只执行1次，就是给left赋初值
				//把赋初值语句放到循环内可以避免赋值时把l或r赋成不孤立的结点
				//但这一点在之前的定义中就能避免，有些多此一举
			{
				l = i;
				a = 1;
			}
			else if (!b)//否则的话如果b是0。同理就是给r赋初值
			{
				r = i;
				b = 1;
			}
			else if (p->weight < (T + l)->weight || p->weight < (T + r)->weight)
				//再如果，p指向的weight值比left指向的小，或
				//比right指向的小，那么：
			{
				if ((T + l)->weight <= (T + r)->weight)
					r = i;
				else
					l = i;
				//在前面else if的条件下，即在left或right的weight值至少有一个比p大时
				//如果left的weight值比right的小的话，就把right变小
				//right的weight小的话，就把left变小
				//总之，就是把指向weight值大的变小
			}
		}
	}//遍历所有结点
	return;
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, unsigned long *w, int n)
//使用时w是weight数组，n是256，即编码的字符种类
{
	int f, c, start, m1, m2, i, m = 2 * n - 1;
	//m=2n-1的含义：分配m+1个存储空间，前n个用来存储那些孤立节点
	//后面的用来存储为了构建霍夫曼树而增加的新结点。
	HuffmanTree p;//p是指向结点的指针
	HT = (HuffmanTree)malloc(sizeof(HTNode)*(m + 1));
	if (!HT)//存储分配失败
	{
		printf("存储分配失败！\n");
		exit(-1);
	}
	for (p = HT + 1, i = 1; i <= n; ++i, ++w, ++p)
		//这个for循环制造出了一堆孤立的仅记录权值信息的结点，是编码的基础
	{
		p->weight = *w;//不断访问weight数组看统计的每个字符的出现次数
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}

	for (i = n + 1; i <= m; ++i, ++p)
		//这个for循环把所有n+1 - m的结点全部置空
	{
		p->weight = 0;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}

	for (i = n + 1, p = HT + i; i <= m; ++i, ++p)//i从257开始
		//也就是说这棵树存储在一条横线上，前面n个是初始结点，再往后从n+1开始
		//就是建立树用的新增结点。
	{
		WeightSelect(HT, i - 1, m1, m2);//寻找到两个最小的孤立结点
		p->weight = (HT + m1)->weight + (HT + m2)->weight;
		//将新节点的weight值变为之前两个最小weight值之和
		p->lchild = m1;//构造树
		p->rchild = m2;//构造树
		(HT + m1)->parent = i;//两个树的parent都是i，也就是新增结点
		(HT + m2)->parent = i;
	}
	HC = (HuffmanCode)malloc(sizeof(char*)*(n + 1));
	//构建一个存储霍夫曼编码数组名的数组
	//以下操作用于给HC数组填充内容。
	if (!HC)
	{
		printf("存储分配失败！");
		exit(-1);
	}
	char *cd = (char*)malloc(sizeof(char)*n);
	//cd为指向char类型的指针。该语句的作用是给这个指针分配n个char类型的存贮空间
	//导致的结果是cd相当于一个有n个元素的char类型数组的数组名
	cd[n - 1] = '\0';//cd数组的最后一个元素置为文件结束符
	//这个操作可以保证拷贝操作的顺利进行.leak.
	for (i = 1; i <= n; ++i)
	{
		start = n - 1;//从数组最后一个开始
		for (f = HT[i].parent, c = i; f != 0; c = f, f = HT[f].parent)
			//将f置为i的双亲值，c从1开始，然后将被置为双亲值
			//只要有双亲就继续循环，f不断的往上追溯
		{
			if (HT[f].lchild == c)//霍夫曼编码。左为0，右为1
				//如果发现往上追的是左孩子，那么就定0
				cd[--start] = '0';
			else
				cd[--start] = '1';
		}
		HC[i] = (char*)malloc(sizeof(char)*(n - start));
		//注，HC[]数组中的所有元素都是指向char类型指针变量的指针，
		//换句话说，HC数组中保存了一堆数组名，这些数组名所代表的数组为char类型数组
		//而这些char类型数组存储的数据只有0和1两种，就是第i个元素所代表的霍夫曼编码
		StrCopy(HC[i], cd + start);
		//将编码好的霍夫曼编码拷贝到HC[i]中。
	}
	free(cd);
	return;
}

//输出霍夫曼编码
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

void EnCode(const char *s)
{
	FILE *pr, *pw;//read write
	int a, i = 0,  j, n = 256, m = 2 * n - 1,h;
	//把weight数组改为unsigned long类型,从而符合结点定义并获得更大的存储
	unsigned long weight[256] = { 0 };
	HuffmanTree HT;
	HuffmanCode HC, hc;

	char CodedName[NAMESIZE], c = ' ';//NAMESIZE 500 127
	//NAMESIZE指文件名长度，
	while (s[i] != '.')//就是文件拓展名前的 .
		//循环把s数组变为文件名
	{
		CodedName[i] = s[i];
		i++;
	}
	//出来的时候s[i]为'.'
	CodedName[i] = '.';
	CodedName[1 + i] = 'e';
	CodedName[2 + i] = 'n';
	CodedName[3 + i] = 'c';
	CodedName[4 + i] = '\0';//文件名最后也要有文件结束符

	pr = fopen(s, "rb");//二进制只读
	if (!pr)
	{
		printf("打开文件失败\n");
		exit(-1);
	}

	a = fgetc(pr);//读取一个字符且光标后移一个字节
	while (!feof(pr))//如果pr不是文件结束符
	{
		weight[a]++;//在ASCIIweight数组中的相应值的数目增1
		a = fgetc(pr);//continue...
	}
	fclose(pr);//关闭文件

	//以上内容是第一遍循环文件统计数据


	HuffmanCoding(HT, HC, weight, 256);//对记录的数据进行编码

	pw = fopen(CodedName, "wb");//打开只写二进制文件

	//先写入原拓展名，占4个字节。不足4个用空格补齐。
	j = 0;
	while (s[++i])//遇到/0会退出。刚进入循环时i已经增1了，退出时s[i]='\0'
	{
		fwrite(s + i, 1, 1, pw);
		//写入s+i指针指向的数据。这就是在写拓展名。
		//注意，未进入循环时s[i]是 . ，进入循环后i变为i+1，故s+i指向的是拓展名第一个字母
		//会把文件结束符写入。
		//写入1个字节，1个数据项，总共写入1*1=1个字节
		//pw是文件指针。
		j++;
	}
	//leak(need?)
	while (j++ < 4)//如果j比4小，那么j+1，然后进入循环
		fwrite(&c, 1, 1, pw);

	for (i = n + 1; i <= m; i++)
	{
		fwrite(&((HT + i)->lchild), 2, 1, pw);
		fwrite(&((HT + i)->rchild), 2, 1, pw);
	}
	pr = fopen(s, "rb");//只读打开原文件。
	a = fgetc(pr);
	i = 0;
	int b = 0;
	hc = HC;
	char *p = *(hc + a + 1);//找到对应的编码
	//此时p是一个数组名，数组存储的是编码
	//leak
	while (!feof(pr) || *p != '\0')
		//文件未结束||p对应的编码非0
	{
		for (i = 0; i < 8; i++)
		{
			if (*p == '\0')
			{
				a = fgetc(pr);
				//从文件中读取一个字符。
				if (a >= 0)
					p = hc[a + 1];
				//如果a是正常的ASCII值，那么就找到它对应的编码数组，该数组名用p表示
				else
				{
					h = 1;
					for (j = 0; j < 8 - i; j++)
						h = h * 2;
					b = b * h;
					break;
				}
			}
			h = 1;
			//删掉int定义
			for (j = 0; j < 7 - i; j++)
				h = h * 2;
			b = b + (*p - 48)*h;//ASCII中的48为0
			//如果读取的ASCII字符>=0，那么右边的b=0
			++p;
		}
		//内部for循环结束
		fwrite(&b, 1, 1, pw);
		b = 0;
	}//while循环结束
	printf("EnCoding finished!\n");
	fclose(pr);
	fclose(pw);
	return;
}