#pragma comment(lib, "winmm")
#pragma comment(lib, "comctl32.lib")
#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <wtypes.h>
#include <windowsx.h>
#include <winbase.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#include <psapi.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include "BasicFunc.h"
#define	TIMER_SEC		10013
#define	TIME_REFRESH	300
#define WIN_WIDE		800
#define	WIN_HEIGHT		600

#define ALIVE			0xF0
#define DEAD			0x00
#define POS(I,J)		((I)*WIN_WIDE+(J))
//using namespace std;


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
//窗口消息处理程序	（窗口句柄，消息类型，消息数据1，消息数据2）

int	iPixel[WIN_WIDE*WIN_HEIGHT];


int WINAPI WinMain (HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					PSTR		szCmdLine,
					int			iCmdShow)
{
	int	cxClient, cyClient;
	RECT	rectMain;
	POINT	pTemp;



	static		TCHAR szAppName[] = TEXT ("Beginning") ;
	HWND		hwnd ;
	MSG			msg ;
	WNDCLASS	wndclass ;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW ;

	wndclass.lpfnWndProc	= WndProc ;
	wndclass.cbClsExtra		= 0 ;
	wndclass.cbWndExtra		= 0 ;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName	= NULL ;
	wndclass.lpszClassName	= szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL,
			TEXT ("This program requires Windows NT!"),
			szAppName,
			MB_ICONERROR) ;
		return 0 ;
	}

	hwnd	= CreateWindow(	szAppName,					// window class name
		TEXT ("LifeGame"),				// window caption
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_CAPTION,		// window style
		0,				// initial x position
		0,				// initial y position
		WIN_WIDE,				// initial x size
		WIN_HEIGHT,				// initial y size
		NULL,						// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL) ;						// creation parameters
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	GetClientRect(hwnd, &rectMain);
	cxClient	= rectMain.right - rectMain.left;
	cyClient	= rectMain.bottom- rectMain.top;
	pTemp.x		= cxClient/3;
	pTemp.y		= cyClient/3;
	//	while (GetMessage (&msg, NULL, 0, 0))
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static	HDC		hDC, hMemDC;
	RECT			rectMain;
	static	HBITMAP	hBitmap, hOldBitmap;


	PAINTSTRUCT		ps ;
	POINT			pTemp;

	static	HINSTANCE	hInstance ;
	static	RECT	rect ;


	static	int		cxChar, cyChar;
	static	int		cxClient, cyClient;	//窗口宽度和高度
	static	BOOL	Pause;
	int	i, j;
	int	count;

	RECT	rectText;
	static	RECT	hrect;

	TCHAR	szStr[128];


	switch (message)
	{
	case WM_CREATE:
		SendMessage(hwnd,
			WM_SETICON,
			(WPARAM)TRUE,
			(LPARAM)LoadIcon(GetModuleHandle(NULL),
			NULL));
		//initialization
		SetTimer(hwnd, TIMER_SEC, TIME_REFRESH, NULL) ;
		GetClientRect (hwnd, &rect) ;	//
		cxChar	= LOWORD(GetDialogBaseUnits());
		cyChar	= HIWORD(GetDialogBaseUnits());
		for (i=0 ; i<WIN_HEIGHT ; i++)
		{
			for (j=0 ; j<WIN_WIDE ; j++)
			{
				iPixel[POS(i, j)]	= (rand()%10>rand()%3)?DEAD:ALIVE;
			}
		}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_SEC:
			if (FALSE == Pause)
			{
				hDC		= GetDC(hwnd);
				hMemDC	= CreateCompatibleDC(hDC);
				hBitmap	= CreateCompatibleBitmap(hDC, WIN_WIDE, WIN_HEIGHT);
				hOldBitmap	= (HBITMAP)SelectObject(hMemDC, hBitmap);
				
				for (i=1 ; i<WIN_HEIGHT-1 ; i++)
				{
					for (j=1 ; j<WIN_WIDE-1 ; j++)
					{
						count	=0;
						if (ALIVE == iPixel[POS(i-1, j-1)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i-1, j)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i-1, j+1)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i, j-1)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i, j+1)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i+1, j-1)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i+1, j)])
						{
							count++;
						}
						if (ALIVE == iPixel[POS(i+1, j+1)])
						{
							count++;
						}
						if (3 == count)
						{
							iPixel[POS(i, j)]	= ALIVE;
						}
						else if (2 != count)
						{
							iPixel[POS(i, j)]	= DEAD;
						}
					}
				}
				//////Draw start
				SetBitmapBits(hBitmap,WIN_WIDE*WIN_HEIGHT*4,iPixel) ;
				//////Draw over
				BitBlt(hDC,	0,	0,	cxClient,	cyClient,
					hMemDC,	0, 0, SRCCOPY);

				SelectObject(hMemDC, hOldBitmap);
				DeleteDC(hMemDC);
				ReleaseDC(hwnd, hDC);
				DeleteObject(hBitmap);
			}
			break;
		}
		return 0;
	case	WM_KEYDOWN:
		switch (wParam)
		{
		case	VK_LEFT:
			break;
		case	VK_RIGHT:
			break;
		case	'P':
			break;
		case	VK_ESCAPE:
			PostQuitMessage (0) ;
			break;
		}
		return 0;
	case   WM_COMMAND :
		return 0;
	case	WM_LBUTTONUP:
		return 0;
	case	WM_RBUTTONUP:
		return 0;
	case	WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		rect.right	= rect.left + cxClient;
		rect.bottom	= rect.top + cyClient;
		return 0 ;
	case   WM_PAINT:
		hDC = BeginPaint (hwnd, &ps) ;
		EndPaint (hwnd, &ps) ;
		return 0 ;
	case   WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}