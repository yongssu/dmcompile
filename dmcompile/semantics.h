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
#define red RGB(255, 0, 0) //��ɫ
#define black RGB(0, 0, 0) //��ɫ
#endif

#ifdef _BC_COMPILER
#define while 255 //��ɫ
#endif

//�ⲿ��������
extern void DrawPixel(unsigned long x, unsigned long y); //����һ����
extern double GetExprValue(struct ExprNode *root); //��ñ��ʽ��ֵ
extern void DrawLoop(double Start, double End, double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr); //ͼ�λ���
extern void DelExprTree(struct ExprNode *root); //ɾ��һ����

#ifdef _BC_COMPILER
extern int InGraphMode;
extern int InitGraph (void);
extern void CloseGraph(void);
#endif

#endif