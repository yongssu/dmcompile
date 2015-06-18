#define _CRT_SECURE_NO_WARNINGS
#define PARSER_DEBUG
#include <stdio.h>
#include <stdlib.h>
/*#include "scanner.h"
int main()
{
	Token token;
	if(!InitScanner("./test.txt"))
	{
		printf("open source file error!\n");
	}
	printf("记号类别     字符串     常数值     函数指针\n");
	while (1)
	{
		token = GetToken();
		if (token.type != NONTOKEN)
			printf("%4d, %12s, %12f, %12x\n", token.type, token.lexeme, token.value, token.FuncPtr);
		else
			break;
	}
	printf("____________________________________________________________\n");
	CloseScanner();
	system("pause");
	return 0;
}*/

extern void Parser(char *SrcFilePtr);
int main()
{

	char *file = "./test.txt";
	Parser(file);
	system("pause");
	return 0;
}