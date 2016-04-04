#pragma once

#include "resource.h"

typedef struct tagLine
{
	POINT ptStart;
	POINT ptEnd;
}LINE;

typedef struct tagDrawStru
{
	POINT ptStart;
	POINT ptEnd;
	BOOL bMouseDown;
	HPEN hPen;
} DRAWSTRU,*PDRAWSTRU;

typedef VOID(*PDRAWFUN)(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam);
