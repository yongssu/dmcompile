#include "semantics.h"


extern double Parameter; //参数T的存储空间
extern double Origin_x, Origin_y; //横纵坐标平移距离
extern double Scale_x, Scale_y; //横纵坐标比例因子
extern double Rot_angle; //旋转角度

double GetExprValue(struct ExprNode *root); //获得一个表达式的值
void DrawPixel(unsigned long x, unsigned long y); //绘制一个点
extern void DrawLoop(double Start, double End, double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr); //图形绘制
void DelExprTree(struct ExprNode *root); //删除一颗语法树

static void Errmsg(char *string); //出错处理
static void CalcCoord(struct ExprNode *Hor_Exp, struct ExprNode *Ver_Exp, double &Hor_x, double &Ver_y); //计算点的坐标

//----------------出错处理
void Errmsg(char *string)
{
	exit(1);
}

//---------------计算被绘制点的坐标
static void CalcCoord(struct ExprNode *Hor_Exp, struct ExprNode *Ver_Exp, double &Hor_x, double &Ver_y)
{
	double HorCord, VerCord, Hor_tmp;

	//计算表达式的值， 得到点的原始坐标
	HorCord = GetExprValue(Hor_Exp);
	VerCord = GetExprValue(Ver_Exp);

	//进行比例变换
	HorCord *= Scale_x;
	HorCord *= Scale_y;

	//进行旋转变换
	Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);
	VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
	HorCord = Hor_tmp;

	//进行平移变换
	HorCord += Origin_x;
	VerCord += Origin_y;

	//返回变换后点的坐标
	Hor_x = HorCord;
	Ver_y = VerCord;
}

//循环绘制点坐标
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

//------------------删除一颗语法树
void DelExprTree(struct ExprNode  *root)
{
	if (root == NULL)
		return;
	switch (root->OpCode)
	{
	case PLUS: //两个孩子的内部节点
	case MINUS:
	case MUL:
	case DIV:
	case POWER:
		DelExprTree(root->Content.CaseOperator.Left);
		DelExprTree(root->Content.CaseOperator.Right);
		break;
	case FUNC: //一个孩子的内部节点
		DelExprTree(root->Content.CaseFunc.Child);
		break;
	default:
		break;
	}
	delete(root); //删除节点
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

//-------------绘制一个点
void DrawPixel(unsigned long x, unsigned long y)
{
#ifdef _VC_COMPILER
	SetPixel(hDC, x, y, black);
#endif

#ifdef _BC_COMPILER
	putpixel(x, y, white);
#endif
}