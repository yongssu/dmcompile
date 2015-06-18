#define _CRT_SECURE_NO_WARNINGS
//#define PARSER_DEBUG
//#include <stdio.h>
//#include <stdlib.h>
/*#include "scanner.h"
int main()
{
	Token token;
	if(!InitScanner("./test.txt"))
	{
		printf("open source file error!\n");
	}
	printf("记号类别     字符串     常数值     函数指针\n");
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
}*/
/*
extern void Parser(char *SrcFilePtr);
int main()
{

	char *file = "./test.txt";
	Parser(file);
	system("pause");
	return 0;
}*/

#define _VC_COMPILER
#ifdef _VC_COMPILER

#include "semantics.h"
#define MAX_CHARAS 200

HDC hDC; //窗口句柄
char SrcFilePath[MAX_CHARAS]; //用于存放源程序文件路径
static wchar_t Name[] = L"Compiler"; //窗口名

//----------------初始化窗口函数声明
static bool PrepareWindow(HINSTANCE, HINSTANCE, int);

//----------------检查源程序文件是否合法函数声明
static bool CheckSrcFile(LPSTR);

//------------窗口消息处理函数声明
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//------------window程序主函数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//保存源文件路径
	strcpy(SrcFilePath, "./test.txt");
	
	//初始化窗口
	if (PrepareWindow(hInstance, hPrevInstance, 1) != true)
	{
		MessageBox(NULL, L"窗口初始化失败！", L"错误", MB_OK);
		return 1;
	}
	
	//检查要分析的源程序文件
	if (!CheckSrcFile(SrcFilePath))
		return 1;

	//------------------------------
	// 调用绘图语言解释器
	Parser(SrcFilePath);
	//--------------------------------

	//进入window消息循环
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//----------------初始化窗口函数实现
bool PrepareWindow(HINSTANCE hInst, HINSTANCE hPreInstance, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS w;

	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInst;
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	w.lpszClassName = (LPCWSTR)Name;
	if (!RegisterClass(&w))
	{
		MessageBox(NULL, L"窗口注册失败", L"错误", MB_OK);
		return 0;
	}

	hWnd = CreateWindow((LPCWSTR)Name, 
		(LPCWSTR)Name, 
		WS_OVERLAPPEDWINDOW, 
		10, 
		10, 
		740, 
		490, 
		NULL, 
		NULL, 
		hInst, 
		NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, L"创建窗口失败", L"错误", MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetCursor(LoadCursor(hInst, IDC_ARROW));

	hDC = GetDC(hWnd);
	return true;
}

//-----------检查源程序文件是否合法函数实现
bool CheckSrcFile(LPSTR lpszCmdParam)
{
	FILE *file = NULL;

	if (strlen(lpszCmdParam) == 0)
	{
		MessageBox(NULL, L"未指定源程序文件！", L"错误", MB_OK);
		return false;
	}

	if ((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		MessageBox(NULL, L"打开源程序文件出错", L"错误", MB_OK);
		MessageBox(NULL, (LPCWSTR)SrcFilePath, L"文件名", MB_OK);
		return false;
	}
	else
		fclose(file);
	return true;
}

//--------------------窗口处理函数实现
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT pt;
	switch (Message)
	{
	case WM_DESTROY:
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &pt);
		//Parser(SrcFilePath);
		EndPaint(hWnd, &pt);
		break;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

#endif

#ifdef _BC_COMPILER
#include "semantics.h"
int main(int argc, char **argc)
{
	int Result;
	if (!InitGraph())
		return -1;
	if (argc != 2)
		return -1;
	Parser(argc[1]);
	CloseGraph();
	return 0;
}
#endif
