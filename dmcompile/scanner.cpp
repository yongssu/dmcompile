#define _CRT_SECURE_NO_WARNINGS

#include "scanner.h"
#define TOKEN_LEN 100  //�Ǻ���󳤶�

unsigned int LineNo; //����Դ�ļ��к�
static FILE *InFile; //�����ļ���
static char TokenBuffer[TOKEN_LEN]; //�Ǻ��ַ�����

//----------------��ʼ���ʷ�������
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

//---------------�رմʷ�������
extern void CloseScanner(void)
{
	if (InFile != NULL)
		fclose(InFile);
}


//----------------������Դ�����ж���һ���ַ�
static char GetChar(void)
{
	int ch = getc(InFile);
	return toupper(ch);
}

//---------------��Ԥ�����ַ��˻ص�����Դ������
static void BackChar(char ch)
{
	if (ch != EOF)
		ungetc(ch, InFile);
}

//---------------�����ַ����ǺŻ�����
static void AddCharTokenString(char ch)
{
	int TokenLength = strlen(TokenBuffer);
	if (TokenLength + 1 >= sizeof(TokenBuffer))
		return;
	TokenBuffer[TokenLength] = ch;
	TokenBuffer[TokenLength + 1] = '\0';
}

//---------------��ռǺŻ�����
static void EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

//----------------�ж��������ַ����Ƿ��ڷ��ű���
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

//----------------��ȡһ���Ǻ�
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

	//�����ǿո�TAB���س����ļ��������ȣ����ȼ��뵽�Ǻŵ��ַ���������
	if (isalpha(ch)) //��ch��A-Za-z����һ���Ǻ������ؼ��֣�PI��E��
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
	else if (isdigit(ch)) //����һ�����֣���һ���ǳ���
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
