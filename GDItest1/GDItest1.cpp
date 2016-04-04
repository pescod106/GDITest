// GDItest1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GDItest1.h"
#include <math.h>
#define MAX_LOADSTRING 100
#define MAX_COUNT 10
#define LINECOUNT 100
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID LineMouseDown(DRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LineMouseUp(DRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LineMouseMove(DRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);

VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);

VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GDITEST1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDITEST1));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDITEST1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GDITEST1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//

VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetCapture(hWnd);
	pDs->bMouseDown = TRUE;
	pDs->ptStart.x = LOWORD(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
}

VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	pDs->bMouseDown = false;
	HDC hdc = GetDC(hWnd);

	SetROP2(hdc, R2_NOT);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);

	pDs->ptEnd.x = LOWORD(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	HPEN hPenOld = (HPEN)SelectObject(hdc, pDs->hPen);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
	SelectObject(hdc, hPenOld);
	ReleaseDC(hWnd, hdc);

	/*if (iLineIndex<LINECOUNT - 1)
	{
		iLineIndex++;
		arrLine[iLineIndex].ptStart.x = ptStart.x;
		arrLine[iLineIndex].ptStart.y = ptStart.y;
		arrLine[iLineIndex].ptEnd.x = ptEnd.x;
		arrLine[iLineIndex].ptEnd.y = ptEnd.y;
	}*/
}

VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);

		SetROP2(hdc, R2_NOT);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptEnd.x = LOWORD(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetCapture(hWnd);
	pDs->bMouseDown = TRUE;
	pDs->ptStart.x = LOWORD(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
}

VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	pDs->bMouseDown = false;
	HDC hdc = GetDC(hWnd);

	SetROP2(hdc, R2_NOT);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	pDs->ptEnd.x = LOWORD(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	HPEN hPenOld = (HPEN)SelectObject(hdc, pDs->hPen);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	SelectObject(hdc, hPenOld);
	ReleaseDC(hWnd, hdc);
}

VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);

		SetROP2(hdc, R2_NOT);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptEnd.x = LOWORD(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetCapture(hWnd);
	pDs->bMouseDown = TRUE;
	pDs->ptStart.x = LOWORD(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
}

VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	pDs->bMouseDown = false;
	HDC hdc = GetDC(hWnd);

	SetROP2(hdc, R2_NOT);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	pDs->ptEnd.x = LOWORD(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	HPEN hPenOld = (HPEN)SelectObject(hdc, pDs->hPen);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	SelectObject(hdc, hPenOld);
	ReleaseDC(hWnd, hdc);
}

VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);

		SetROP2(hdc, R2_NOT);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptEnd.x = LOWORD(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

int s[100];
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PDRAWFUN pMouseDown, pMouseUp, pMouseMove;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	RECT rt;
	static POINT ptStart, ptEnd;
	static UINT nHeight;
	static bool bMouseDown = false;

	static LINE arrLine[LINECOUNT];
	static int iLineIndex = 1;

	static DRAWSTRU ds;

	HMENU hMenu = GetMenu(hWnd);
	switch (message)
	{
	case WM_CREATE:
		pMouseDown = LineMouseDown;
		pMouseUp = LineMouseUp;
		pMouseMove = LineMouseMove;
		ds.hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		break;
	case WM_LBUTTONDOWN:
		pMouseDown(&ds, hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		pMouseUp(&ds, hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		pMouseMove(&ds, hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_COLOR_RED:
			DeleteObject(ds.hPen);
			ds.hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

			break;
		case IDM_COLOR_GREEN:
			DeleteObject(ds.hPen);
			ds.hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			break;
		case IDM_COLOR_BLUE:
			DeleteObject(ds.hPen);
			ds.hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			break;
		case IDM_DRAW_LINE:
			pMouseDown = LineMouseDown;
			pMouseUp = LineMouseUp;
			pMouseMove = LineMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case IDM_DRAW_RECT:
			pMouseDown = RectMouseDown;
			pMouseUp = RectMouseUp;
			pMouseMove = RectMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case IDM_DRAW_ELLIPSE:
			pMouseDown = EllipseMouseDown;
			pMouseUp = EllipseMouseUp;
			pMouseMove = EllipseMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_CHECKED);
			break;
		case IDM_DRAW_LINE1:
			hdc = GetDC(hWnd);
			MoveToEx(hdc, 100, 100, NULL);
			LineTo(hdc, 600, 100);
			ReleaseDC(hWnd, hdc); 
			break;
		case IDM_DRAW_TITLE:
			hdc = GetWindowDC(hWnd);
			MoveToEx(hdc, 100, 100, NULL);
			LineTo(hdc, 600, 100);
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_DRAW_DESKTOP:
			hdc = GetWindowDC(NULL);
			MoveToEx(hdc, 100, 100, NULL);
			LineTo(hdc, 600, 100);
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		/*GetClientRect(hWnd, &rt);
		
		MoveToEx(hdc,0, rt.bottom / 2, NULL);
		for (i = 0; i < MAX_COUNT; i++)
		{
			LineTo(hdc, rt.right*i / MAX_COUNT, s[i]);
		}
		MoveToEx(hdc, 100, 200, NULL);
		LineTo(hdc,600,200);*/



		/*HPEN hPen, hPenOld;
		HBRUSH hBr, hBrOld;

		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));//����һ����ɫ����
		hPenOld = (HPEN)SelectObject(hdc, hPen);//ѡ�뻭��
		hBr = CreateSolidBrush(RGB(0, 0, 255));//������ɫ��ˢ
		hBrOld = (HBRUSH)SelectObject(hdc, hBr);

		//��(100,50)��(600,50)��һ���߶�
		MoveToEx(hdc, 100, 50, NULL);
		LineTo(hdc, 600, 50);

		Rectangle(hdc, 100, 100, 300, 200);//��һ����ɫ���ߣ���ɫ���ľ���
		Ellipse(hdc, 400, 100, 600, 200);//��һ����ɫ���ߣ���ɫ������Բ

		SelectObject(hdc, GetStockObject(NULL_PEN));;//ѡ��һ���յĻ���
		Rectangle(hdc, 100, 230, 300, 330);//��һ��û�б߿���ɫ�ľ���
		Ellipse(hdc, 400, 230, 600, 330);//��һ��û�б߿���ɫ����Բ

		SelectObject(hdc, hPen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, 100, 400, 300, 500);
		Ellipse(hdc, 400, 400, 600, 500);

		SelectObject(hdc, hPenOld);
		DeleteObject(hPen);
		DeleteObject(hBr);*/
		for (int i = 0; i <= iLineIndex; i++)
		{
			MoveToEx(hdc, arrLine[i].ptStart.x, arrLine[i].ptStart.y, NULL);
			LineTo(hdc, arrLine[i].ptEnd.x, arrLine[i].ptEnd.y);
		}
		

		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		//GetClientRect(hWnd, &rt);
		nHeight = HIWORD(lParam);

		for (i = 0; i < MAX_COUNT; i++)
		{
			s[i] = nHeight / 2 * (1 - sin(i * 2 * 3.14 / MAX_COUNT));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
