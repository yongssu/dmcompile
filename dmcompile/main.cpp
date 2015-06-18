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
	printf("�Ǻ����     �ַ���     ����ֵ     ����ָ��\n");
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

HDC hDC; //���ھ��
char SrcFilePath[MAX_CHARAS]; //���ڴ��Դ�����ļ�·��
static wchar_t Name[] = L"Compiler"; //������

//----------------��ʼ�����ں�������
static bool PrepareWindow(HINSTANCE, HINSTANCE, int);

//----------------���Դ�����ļ��Ƿ�Ϸ���������
static bool CheckSrcFile(LPSTR);

//------------������Ϣ����������
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//------------window����������
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//����Դ�ļ�·��
	strcpy(SrcFilePath, "./test.txt");
	
	//��ʼ������
	if (PrepareWindow(hInstance, hPrevInstance, 1) != true)
	{
		MessageBox(NULL, L"���ڳ�ʼ��ʧ�ܣ�", L"����", MB_OK);
		return 1;
	}
	
	//���Ҫ������Դ�����ļ�
	if (!CheckSrcFile(SrcFilePath))
		return 1;

	//------------------------------
	// ���û�ͼ���Խ�����
	Parser(SrcFilePath);
	//--------------------------------

	//����window��Ϣѭ��
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//----------------��ʼ�����ں���ʵ��
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
		MessageBox(NULL, L"����ע��ʧ��", L"����", MB_OK);
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
		MessageBox(NULL, L"��������ʧ��", L"����", MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetCursor(LoadCursor(hInst, IDC_ARROW));

	hDC = GetDC(hWnd);
	return true;
}

//-----------���Դ�����ļ��Ƿ�Ϸ�����ʵ��
bool CheckSrcFile(LPSTR lpszCmdParam)
{
	FILE *file = NULL;

	if (strlen(lpszCmdParam) == 0)
	{
		MessageBox(NULL, L"δָ��Դ�����ļ���", L"����", MB_OK);
		return false;
	}

	if ((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		MessageBox(NULL, L"��Դ�����ļ�����", L"����", MB_OK);
		MessageBox(NULL, (LPCWSTR)SrcFilePath, L"�ļ���", MB_OK);
		return false;
	}
	else
		fclose(file);
	return true;
}

//--------------------���ڴ�����ʵ��
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
