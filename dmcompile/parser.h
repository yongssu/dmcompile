#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"

typedef double (*FuncPtr)(double); 

//语法树节点类型
struct ExprNode
{
	enum Token_Type OpCode; //PLUS, MINUS, MUL, DIV, POWER, FUNC, CONST_ID等
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

//语法分析器对外接口
extern void Parser(char *SrcFilePtr, HDC h);
#endif
