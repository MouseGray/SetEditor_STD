#pragma once

INT_PTR CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TAWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OSWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK buttonWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK scrollWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK tipWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK sbuttonWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK numEditWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK stateWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK taskWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK resultWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK keypadWndProc(HWND, UINT, WPARAM, LPARAM);

ATOM mainWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;// MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = "main";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}

ATOM textAreaWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = TAWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr; //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "textArea";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}

ATOM opSelectorWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	wcex.lpfnWndProc = OSWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_GRAYTEXT + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "WNDSelector";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM buttonWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = buttonWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "nButton";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM sButtonWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = sbuttonWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "sButton";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM numEditWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = numEditWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "nEdit";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM scrollWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = scrollWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "nScroll";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM tipWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = tipWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "nTip";
	wcex.hIconSm = nullptr;

	return RegisterClassEx(&wcex);
}

ATOM stateWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(RGB(42, 41, 65));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = stateWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hBrush;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "WNDState";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}

ATOM taskWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(RGB(42, 41, 65));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = taskWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hBrush;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "WNDTask";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}

ATOM resultWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(RGB(42, 41, 65));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = resultWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hBrush;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "WNDResult";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}

ATOM keypadWinClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = keypadWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "WNDKeypad";
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	return RegisterClassEx(&wcex);
}