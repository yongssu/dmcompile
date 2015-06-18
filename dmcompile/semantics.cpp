#include "semantics.h"


extern double Parameter; //����T�Ĵ洢�ռ�
extern double Origin_x, Origin_y; //��������ƽ�ƾ���
extern double Scale_x, Scale_y; //���������������
extern double Rot_angle; //��ת�Ƕ�

double GetExprValue(struct ExprNode *root); //���һ�����ʽ��ֵ
void DrawPixel(unsigned long x, unsigned long y); //����һ����
extern void DrawLoop(double Start, double End, double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr); //ͼ�λ���
void DelExprTree(struct ExprNode *root); //ɾ��һ���﷨��

static void Errmsg(char *string); //������
static void CalcCoord(struct ExprNode *Hor_Exp, struct ExprNode *Ver_Exp, double &Hor_x, double &Ver_y); //����������

//----------------������
void Errmsg(char *string)
{
	exit(1);
}

//---------------���㱻���Ƶ������
static void CalcCoord(struct ExprNode *Hor_Exp, struct ExprNode *Ver_Exp, double &Hor_x, double &Ver_y)
{
	double HorCord, VerCord, Hor_tmp;

	//������ʽ��ֵ�� �õ����ԭʼ����
	HorCord = GetExprValue(Hor_Exp);
	VerCord = GetExprValue(Ver_Exp);

	//���б����任
	HorCord *= Scale_x;
	HorCord *= Scale_y;

	//������ת�任
	Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);
	VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
	HorCord = Hor_tmp;

	//����ƽ�Ʊ任
	HorCord += Origin_x;
	VerCord += Origin_y;

	//���ر任��������
	Hor_x = HorCord;
	Ver_y = VerCord;
}

//ѭ�����Ƶ�����
void DrawLoop(double Start, double End, double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr)
{
	extern double Parameter;
	double x, y;
	for (Parameter = Start; Parameter <= End; Parameter += Step)
	{
		CalcCoord(HorPtr, VerPtr, x, y);
		DrawPixel((unsigned long)x, (unsigned long)y);
	}
}

double GetExprValue(struct ExprNode *root)
{
	if (root == NULL)
		return 0.0;
	switch (root->OpCode)
	{
	case PLUS:
		return GetExprValue(root->Content.CaseOperator.Left) + GetExprValue(root->Content.CaseOperator.Right);
	case MINUS:
		return GetExprValue(root->Content.CaseOperator.Left) - GetExprValue(root->Content.CaseOperator.Right);
	case MUL:
		return GetExprValue(root->Content.CaseOperator.Left) * GetExprValue(root->Content.CaseOperator.Right);
	case DIV:
		return GetExprValue(root->Content.CaseOperator.Left) / GetExprValue(root->Content.CaseOperator.Right);
	case POWER:
		return pow(GetExprValue(root->Content.CaseOperator.Left), GetExprValue(root->Content.CaseOperator.Right));
	case FUNC:
		return (*root->Content.CaseFunc.MathFuncPtr)(GetExprValue(root->Content.CaseFunc.Child));
	case CONST_ID:
		return root->Content.CaseConst;
	case T:
		return *(root->Content.CaseParmPtr);
	default:
		return 0.0;
	}
}

//------------------ɾ��һ���﷨��
void DelExprTree(struct ExprNode  *root)
{
	if (root == NULL)
		return;
	switch (root->OpCode)
	{
	case PLUS: //�������ӵ��ڲ��ڵ�
	case MINUS:
	case MUL:
	case DIV:
	case POWER:
		DelExprTree(root->Content.CaseOperator.Left);
		DelExprTree(root->Content.CaseOperator.Right);
		break;
	case FUNC: //һ�����ӵ��ڲ��ڵ�
		DelExprTree(root->Content.CaseFunc.Child);
		break;
	default:
		break;
	}
	delete(root); //ɾ���ڵ�
}

#ifdef _BC_COMPILER
int InGraphMode = 0;
int InitGraph(void)
{
	int gd = DETECT, gm;
	if(InGraphMode)
		return 0;
	registerbgidriver(EGAVFA_draver);
	initgraph(&gd, &gm, "");
	setcolor(-1);
	InGraphMode = 1;
	return 1;
}

void CloseGraph(void)
{
	if(!InGraphMode)
		return;
	getch();
	closegraph();
	InGraphMode = 0;
}
#endif

//-------------����һ����
void DrawPixel(unsigned long x, unsigned long y)
{
#ifdef _VC_COMPILER
	SetPixel(hDC, x, y, black);
#endif

#ifdef _BC_COMPILER
	putpixel(x, y, white);
#endif
}