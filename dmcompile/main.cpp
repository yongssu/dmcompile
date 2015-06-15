#define _CRT_SECURE_NO_WARNINGS

#include "scanner.h"

int main()
{
	Token token;
	if (InitScanner("./test.txt"))
	{
		printf("open source file error!\n");
	}
	printf("�Ǻ����     �ַ���     ����ֵ     ����ָ��\n");
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
}

