#define _CRT_SECURE_NO_WARNINGS

#include "scanner.h"
#define TOKEN_LEN 100  //记号最大长度

unsigned int LineNo; //跟踪源文件行号
static FILE *InFile; //输入文件流
static char TokenBuffer[TOKEN_LEN]; //记号字符缓冲

//----------------初始化词法分析器
extern int InitScanner(const char *FileName)
{
	LineNo = 1;
	InFile = fopen(FileName, "r");
	if (InFile != NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}

}

//---------------关闭词法分析器
extern void CloseScanner(void)
{
	if (InFile != NULL)
		fclose(InFile);
}


//----------------从输入源程序中读入一个字符
static char GetChar(void)
{
	int ch = getc(InFile);
	return toupper(ch);
}

//---------------把预读的字符退回到输入源程序中
static void BackChar(char ch)
{
	if (ch != EOF)
		ungetc(ch, InFile);
}

//---------------加入字符到记号缓冲区
static void AddCharTokenString(char ch)
{
	int TokenLength = strlen(TokenBuffer);
	if (TokenLength + 1 >= sizeof(TokenBuffer))
		return;
	TokenBuffer[TokenLength] = ch;
	TokenBuffer[TokenLength + 1] = '\0';
}

//---------------清空记号缓冲区
static void EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

//----------------判断所给的字符串是否在符号表中
static Token JudgeKeyToken(const char *IDString)
{
	int loop;
	for (loop = 0; loop < sizeof(TokenTab) / sizeof(TokenTab[0]); loop++)
	{
		if (strcmp(TokenTab[loop].lexeme, IDString) == 0)
			return TokenTab[loop];
	}
	Token errortoken;
	memset(&errortoken, 0, sizeof(Token));
	errortoken.type = ERRTOKEN;
	return errortoken;
}

//----------------获取一个记号
extern Token GetToken(void)
{
	Token token;
	int ch;
	memset(&token, 0, sizeof(token));
	EmptyTokenString();
	token.lexeme = TokenBuffer;
	while (1)
	{
		ch = GetChar();
		if (ch == EOF)
		{
			token.type = NONTOKEN;
			return token;
		}
		if (ch == '\n')
			LineNo++;
		if (!isspace(ch))
			break;
	} //end of while
	AddCharTokenString(ch);

	//若不是空格、TAB、回车，文件结束符等，则先加入到记号的字符缓冲区中
	if (isalpha(ch)) //若ch是A-Za-z，则一定是函数，关键字，PI，E等
	{
		while (1)
		{
			ch = GetChar();
			if (isalnum(ch))
				AddCharTokenString(ch);
			else
				break;
		}
		BackChar(ch);
		token = JudgeKeyToken(TokenBuffer);
		token.lexeme = TokenBuffer;
		return token;
	}
	else if (isdigit(ch)) //若是一个数字，则一定是常量
	{
		while (1)
		{
			ch = GetChar();
			if (isdigit(ch))
				AddCharTokenString(ch);
			else
				break;
		}
		if (ch == '.')
		{
			AddCharTokenString(ch);
			while (1)
			{
				ch = GetChar();
				if (isdigit(ch))
					AddCharTokenString(ch);
				else
					break;
			}
		}//end of if
		BackChar(ch);
		token.type = CONST_ID;
		token.value = atof(TokenBuffer);
		return token;
	}
	else
	{
		switch (ch)
		{
		case ';':
			token.type = SEMICO;
			break;
		case '(':
			token.type = L_BRACKET;
			break;
		case ')':
			token.type = R_BRACKET;
			break;
		case ',':
			token.type = COMMA;
			break;
		case '+':
			token.type = PLUS;
			break;
		case '-':
			ch = GetChar();
			if (ch == '-')
			{
				while (ch != '\n' && ch != EOF)
					ch = GetChar();
				BackChar(ch);
				return GetToken();
			}
			else
			{
				BackChar(ch);
				token.type = MINUS;
				break;
			}
		case '/':
			ch = GetChar();
			if (ch == '/')
			{
				while (ch != '\n' && ch != EOF)
					ch = GetChar();
				BackChar(ch);
				return GetToken();
			}
			else
			{
				BackChar(ch);
				token.type = DIV;
				break;
			}
		case '*':
			ch = GetChar();
			if (ch == '*')
			{
				token.type = POWER;
				break;
			}
			else
			{
				BackChar(ch);
				token.type = MUL;
				break;
			}
		default:
			token.type = ERRTOKEN;
			break;
		} //end of switch
	} //end token
	return token;
} // end of GetToken
