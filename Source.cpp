#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

void Update(HWND hWnd)
{
	Sleep(5);
	InvalidateRect(hWnd, 0, 0);
	UpdateWindow(hWnd);
}

VOID InsertionSortStep(HWND hWnd, int* pList, SIZE_T size)
{
	for (SIZE_T i = 1; i < size; ++i)
	{
		const int tmp = pList[i];
		if (pList[i - 1] > tmp)
		{
			SIZE_T j = i;
			do {
				pList[j] = pList[j - 1];
				Update(hWnd);
				--j;
			} while (j > 0 && pList[j - 1] > tmp);
			pList[j] = tmp;
			Update(hWnd);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static LPINT lpData;
	static SIZE_T nMaxSize = 32;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("ソート"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		lpData = (LPINT)GlobalAlloc(0, sizeof(INT) * nMaxSize);
		for (SIZE_T i = 0; i < nMaxSize; ++i)
		{
			lpData[i] = (int)i + 1;
		}
		break;
	case WM_SIZE:
		MoveWindow(hButton, 10, 10, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EnableWindow(hButton, FALSE);
			for (SIZE_T i = 0; i < nMaxSize; ++i)
			{
				const int temp = lpData[i];
				SIZE_T index = rand() % nMaxSize;
				lpData[i] = lpData[index];
				lpData[index] = temp;
			}
			Update(hWnd);
			InsertionSortStep(hWnd, lpData, nMaxSize);
			InvalidateRect(hWnd, 0, 1);
			EnableWindow(hButton, TRUE);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		for (SIZE_T i = 0; i < nMaxSize; ++i)
		{
			RECT rect1 = { 10,50 + 20 * (int)i,lpData[i] * 10 + 10, 50 + 20 * (int)i + 10 };
			RECT rect2 = { lpData[i] * 10 + 10,50 + 20 * (int)i,(int)nMaxSize * 10 + 10, 50 + 20 * (int)i + 10 };
			FillRect(hdc, &rect1, (HBRUSH)GetStockObject(BLACK_BRUSH));
			FillRect(hdc, &rect2, (HBRUSH)GetStockObject(WHITE_BRUSH));
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		GlobalFree(lpData);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("ソートアルゴリズムのアニメーション表示"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
