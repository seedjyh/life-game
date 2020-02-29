#define	SPACE_EMPTY	0
#define SPACE_DEAD	1
#define	TIMER_SEC		1001
#define	TIME_REFRESH	10

#define TIMER_MOVE		1002
#define TIME_MOVE		800

#define GRID_WIDE_NUM	16
#define GRID_HEIGHT_NUM	24
#define GRID_SIZE		10

#define MAX_OBJ_SIZE	5

#define DIRECTION_UP	0
#define DIRECTION_LEFT	1
#define DIRECTION_DOWN	2
#define DIRECTION_RIGHT	3






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
using namespace std;

struct  POSITION{
	int	iLine;
	int	iColumn;
	POSITION	() {}
	POSITION	(int il, int ic)
	{
		this->iLine	= il;
		this->iColumn = ic;
	}
};

typedef  vector <POSITION>	AREA;

typedef struct{
	int	iSpace[GRID_HEIGHT_NUM][GRID_WIDE_NUM];
	int	iState;
}SPACE;