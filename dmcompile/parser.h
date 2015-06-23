#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"

typedef double (*FuncPtr)(double); 

//�﷨���ڵ�����
struct ExprNode
{
	enum Token_Type OpCode; //PLUS, MINUS, MUL, DIV, POWER, FUNC, CONST_ID��
	union
	{
		struct
		{
			ExprNode *Left;
			ExprNode *Right;
		} CaseOperator;
		struct
		{
			ExprNode *Child;
			FuncPtr MathFuncPtr;
		} CaseFunc;
		double CaseConst;
		double *CaseParmPtr;
	} Content;
};

//�﷨����������ӿ�
extern void Parser(char *SrcFilePtr, HDC h);
#endif
