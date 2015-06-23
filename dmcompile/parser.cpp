#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
//#define PARSER_DEBUG

#ifndef PARSER_DEBUG
#include "semantics.h"
#endif

#ifdef PARSER_DEBUG
#define enter(x) printf("enter in  ");printf(x);printf("\n");
#else
#define enter(x)
#endif

#ifdef PARSER_DEBUG
#define back(x) printf("exit from  ");printf(x);printf("\n");
#else
#define back(x)
#endif

#ifdef PARSER_DEBUG
#define call_match(x) printf("matchtoken   ");printf(x);printf("\n");
#else
#define call_match(x)
#endif

#ifdef PARSER_DEBUG
#define Tree_trace(x) PrintSyntaxTree(x, 1);
#else
#define Tree_trace(x)
#endif

#ifdef PARSER_DEBUG
double Parameter = 0; //����T�Ĵ洢�ռ�
#else
double Parameter = 0; //����T�Ĵ洢�ռ�O
double Origin_x = 0, Origin_y = 0; //����ƽ�ƾ���
double Scale_x = 1, Scale_y = 1; //���������������
double Rot_angle = 0; //��ת�Ƕ�
#endif 

static Token token; //�Ǻ�                                                                                                                                                                                                                                                                                                                                                                                                                                             


//-------------------������������
static void FetchToken();
static void MatchToken(enum Token_Type AToken);
static void SyntaxError(int case_of);
static void ErrMsg(unsigned LineNo, char *descript, char *string);
static void PrintSyntaxTree(struct ExptrNode *root, int indeng);

//------------------���ս���ĵݹ��ӳ�������
static void Program();
static void Statement();
static void OriginStatement();
static void RotStatement();
static void ScaleStatement();
static void ForStatement();
static struct ExprNode *Expression();
static struct ExprNode *Term();
static struct ExprNode *Factor();
static struct ExprNode *Component();
static struct ExprNode *Atom();

//-----------------------�ⲿ�ӿ����﷨�����캯������
extern void Parser(char *srcFilePtr);
static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...);

//-----------------ͨ���˷��������ӿ�GetToken��ȡһ���Ǻ�
static void FetchToken()
{
	token = GetToken();
	if (token.type == ERRTOKEN)
		SyntaxError(1);
}

//----------------ƥ��Ǻ�
static void MatchToken(enum Token_Type The_Token)
{
	if (token.type != The_Token)
		SyntaxError(2);
	FetchToken();
}

//--------------�﷨������
static void SyntaxError(int case_of)
{
	switch (case_of)
	{
	case 1:
		ErrMsg(LineNo, " ����Ǻ� ", token.lexeme);
		break;
	case 2:
		ErrMsg(LineNo, "����Ԥ�ڼǺ�", token.lexeme);
		break;
	}
}

//--------------��ӡ������Ϣ
void ErrMsg(unsigned LineNo, char *descrip, char *string)
{
#ifdef PARSER_DEBUG
	printf("Line No %5d:%s %s\n", LineNo, descrip, string);
#else
	char msg[256];
	memset(msg, 0, 256);
	sprintf(msg, "Line No %5d:%s %s!", LineNo, descrip, string);
#endif

#ifdef _VC_COMPILER
	MessageBox(NULL, (LPCWSTR)msg, L"error!", MB_OK);
#endif

#ifdef _BC_COMPILER
	printf("%s\n", msg);
#endif

	CloseScanner();
	exit(1);
}

//--------------�����������ӡ���ʽ���﷨��
void PrintSyntaxTree(struct ExprNode *root, int indent)
{
	int temp;
	for (temp = 1; temp <= indent; temp++)
		printf("\t"); //����

	switch (root->OpCode)
	{
	case PLUS: 
		printf("%s\n", "+");
		break;
	case MINUS:
		printf("%s\n", "-");
		break;
	case MUL:
		printf("%s\n", "*");
		break;
	case DIV:
		printf("%s\n", "/");
		break;
	case POWER:
		printf("%s\n", "**");
		break;
	case FUNC:
		printf("%x\n", root->Content.CaseFunc.MathFuncPtr);
		break;
	case CONST_ID:
		printf("%f\n", root->Content.CaseConst);
		break;
	case T:
		printf("%s\n", "T");
		break;
	default:
		printf("Error Tree Node !\n");
		exit(0);		
	}

	if (root->OpCode == CONST_ID || root->OpCode == T) //Ҷ�ӽڵ㷵��
		return;
	if (root->OpCode == FUNC) //�ݹ��ӡһ��Ҷ�ӵĽڵ�
		PrintSyntaxTree(root->Content.CaseFunc.Child, indent + 1);
	else
	{
		PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
		PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
	}
}

//-------------��ͼ���Խ�������ڣ�����������ⲿ�ӿڣ�
void Parser(char *SrcFilePtr)
{
	enter("Parser");
	if (!InitScanner(SrcFilePtr)) //��ʼ���ʷ�������
	{
		printf("Open Source File Error!\n");
		return;
	}
	FetchToken(); //��ȡ��һ���Ǻ�
	Program(); //�ݹ��½�����
	CloseScanner(); //�رմ˷�������
	back("Parser");
	return;
}

//-----------Program�ĵݹ��ӳ���
static void Program()
{
	enter("Program");
	while (token.type != NONTOKEN)
	{
		Statement();
		MatchToken(SEMICO);
	}
	back("Program");
}

//-----------Statement�ĵݹ���ӳ���
static void Statement()
{
	enter("Statement");
	switch (token.type)
	{
	case ORIGIN:
		OriginStatement();
		break;
	case SCALE:
		ScaleStatement();
		break;
	case ROT:
		RotStatement();
		break;
	case FOR:
		ForStatement();
		break;
	default:
		SyntaxError(2);
	}
	back("Statement");
}

//-----------OriginStatement�ĵݹ��ӳ���
static void OriginStatement(void)
{
	struct ExprNode *tmp;

	enter("OriginStatement");
	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	tmp = Expression();
#ifndef PARSER_DEBUG
	Origin_x = GetExprValue(tmp); //��ȡ�������ƽ�ƾ���
	DelExprTree(tmp);
#endif
	MatchToken(COMMA);
	tmp = Expression();
#ifndef PARSER_DEBUG
	Origin_y = GetExprValue(tmp); //��ȡ�������ƽ�ƾ���
	DelExprTree(tmp);
#endif
	MatchToken(R_BRACKET);
	back("OriginStatement");
}

//-----------ScleStatement�ĵݹ��ӳ���
static void ScaleStatement(void)
{
	struct ExprNode *tmp;
	enter("ScaleStatement");
	MatchToken(SCALE);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	tmp = Expression();
#ifndef PARSER_DEBUG
	Scale_x = GetExprValue(tmp); //��ȡ������ı�������
	DelExprTree(tmp);
#endif
	MatchToken(COMMA);
	tmp = Expression();
#ifndef PARSER_DEBUG
	Scale_y = GetExprValue(tmp); //��ȡ������ı�������
	DelExprTree(tmp);
#endif
	MatchToken(R_BRACKET);
	back("ScaleStatement");
}

//-----------------RotStatement�ĵݹ�
static void RotStatement(void)
{
	struct ExprNode *tmp;

	enter("RotStatement");
	MatchToken(ROT);
	MatchToken(IS);
	tmp = Expression();

#ifndef PARSER_DEBUG
	Rot_angle = GetExprValue(tmp); //��ȡ��ת�Ƕ�
	DelExprTree(tmp);
#endif
	back("RotStatement");
}


//---------------ForStatement�ĵݹ��ӳ���
static void ForStatement(void)
{
#ifndef PARSER_DEBUG
	double Start, End, Step; //��ͼ��㡢�յ㡢����
#endif
	struct ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr; //�����ʽ�﷨�����ڵ�ָ��
	enter("ForStatement");
	MatchToken(FOR);
	call_match("FOR");
	MatchToken(T);
	call_match("T");
	MatchToken(FROM);
	call_match("FROM");
	start_ptr = Expression(); //���������������﷨��
#ifndef PARSER_DEBUG
	Start = GetExprValue(start_ptr); //������������ʽ��ֵ
	DelExprTree(start_ptr); //�ͷŲ�������﷨�������ռ�
#endif
	MatchToken(TO);
	call_match("TO");
	end_ptr = Expression(); //��������յ���ʽ�﷨��

#ifndef PARSER_DEBUG
	End = GetExprValue(end_ptr); //��������յ���ʽ��ֵ
	DelExprTree(end_ptr); //�ͷŲ����յ��﷨����ռ�ռ�
#endif
	MatchToken(STEP);
	call_match("STEP");
	step_ptr = Expression(); //��������������ʽ�﷨��

#ifndef PARSER_DEBUG
	Step = GetExprValue(step_ptr); //��������������ʽ��ֵ
	DelExprTree(step_ptr);
#endif

	MatchToken(DRAW);
	call_match("DRAW");
	MatchToken(L_BRACKET);
	call_match("(");
	x_ptr = Expression(); //�����������ʽ�﷨��
	MatchToken(COMMA);
	call_match(",");
	y_ptr = Expression(); //������������ʽ�﷨��
	MatchToken(R_BRACKET);
	call_match(")");
#ifndef PARSER_DEBUG
	DrawLoop(Start, End, Step, x_ptr, y_ptr); //����ͼ��
	DelExprTree(x_ptr); //�ͷź������﷨����ռ�ռ�
	DelExprTree(y_ptr); //�ͷ��������﷨����ռ�ռ�
#endif
	back("ForStatement");
}

//-------------Expression�ĵݹ��ӳ���
static struct ExprNode *Expression()
{
	struct ExprNode *left, *right; //�������ڵ��ָ��
	Token_Type token_tmp; //��ǰ�Ǻ�

	enter("Expression");
	left = Term(); //������������ҵõ����﷨��
	while (token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Term(); //�����Ҳ������ҵõ����﷨��
		left = MakeExprNode(token_tmp, left, right); //����������﷨�������Ϊ������ 
	}
	Tree_trace(left); //��ӡ���ʽ���﷨��
	back("Expression");
	return left; //�������ձ��ʽ���﷨��
}

//------------------Term�ĵݹ��ӳ���
static struct ExprNode * Term()
{
	struct ExprNode *left, *right;
	Token_Type token_tmp;

	left = Factor();
	while (token.type == MUL || token.type == DIV)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

//----------------------Factor�ĵݹ��ӳ���
static struct ExprNode *Factor()
{
	struct ExprNode *left, *right;

	if (token.type == PLUS) //ƥ��һԪ������
	{
		MatchToken(PLUS);
		right = Factor(); //���ʽ�˻�Ϊ�����Ҳ������ı��ʽ
	}
	else if (token.type == MINUS)
	{
		MatchToken(MINUS); //���ʽת��Ϊ��Ԫ������ı��ʽ
		right = Factor();
		left = new ExprNode;
		left->OpCode = CONST_ID;
		left->Content.CaseConst = 0.0;
		right = MakeExprNode(MINUS, left, right);
	}
	else
		right = Component(); //ƥ����ս��Component
	return right;
}


//--------------Component�ĵݹ��ӳ���
static struct ExprNode *Component()
{
	struct ExprNode *left, *right;
	left = Atom();
	if (token.type == POWER)
	{
		MatchToken(POWER);
		right = Component(); //�ݹ����Component��ʵ��POWER���ҽ��
		left = MakeExprNode(POWER, left, right);
	}
	return left;
}

//------------------Atom�ĵݹ��ӳ���
static struct ExprNode *Atom()
{
	struct Token  t = token;
	struct ExprNode *address = NULL, *tmp;

	switch (token.type)
	{
	case CONST_ID:
		MatchToken(CONST_ID);
		address = MakeExprNode(CONST_ID, t.value);
		break;
	case T:
		MatchToken(T);
		address = MakeExprNode(T);
		break;
	case FUNC:
		MatchToken(FUNC);
		MatchToken(L_BRACKET);
		tmp = Expression();
		address = MakeExprNode(FUNC, t.FuncPtr, tmp);
		MatchToken(R_BRACKET);
		break;
	case L_BRACKET:
		MatchToken(L_BRACKET);
		address = Expression();
		MatchToken(R_BRACKET);
		break;
	default:
		SyntaxError(2);
	}
	return address;
}

//-----------�����﷨����һ���ڵ�
static struct ExprNode *MakeExprNode(enum Token_Type opcode, ...)
{
	struct ExprNode *ExprPtr = new (struct ExprNode);
	ExprPtr->OpCode = opcode; //���ռǺŵ����
	va_list ArgPtr;
	va_start(ArgPtr, opcode);
	switch (opcode) //���ݼǺŵ�����첻ͬ�Ľڵ�
	{
	case CONST_ID: //�����ڵ�
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);
		break;
	case T: //�����ڵ�
		ExprPtr->Content.CaseParmPtr = &Parameter;
		break;
	case FUNC:
		ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
		ExprPtr->Content.CaseFunc.Child = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
		break;
	default: //��Ԫ�����
		ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
		ExprPtr->Content.CaseOperator.Right = (struct ExprNode*)va_arg(ArgPtr, struct ExprNode *);
		break;
	}
	va_end(ArgPtr);
	return ExprPtr;
}