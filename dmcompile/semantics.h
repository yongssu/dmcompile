#ifndef SEMANTICS_H_
#define SEMANTICS_H_

#define _VC_COMPILER
#ifdef _VC_COMPILER
#include <Windows.h>
#include <wingdi.h>
extern HDC hDC;
#endif

#ifdef _BC_COMPILER
#include <graphics.h>
#inclue <conio.h>
#endif

#include "parser.h"

#ifdef _VC_COMPILER
#define red RGB(255, 0, 0) //红色
#define black RGB(0, 0, 0) //黑色
#endif

#ifdef _BC_COMPILER
#define while 255 //白色
#endif

//外部函数声明
extern void DrawPixel(unsigned long x, unsigned long y); //绘制一个点
extern double GetExprValue(struct ExprNode *root); //获得表达式的值
extern void DrawLoop(double Start, double End, double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr); //图形绘制
extern void DelExprTree(struct ExprNode *root); //删除一颗树

#ifdef _BC_COMPILER
extern int InGraphMode;
extern int InitGraph (void);
extern void CloseGraph(void);
#endif

#endif