#define byte win_byte
#include <Windows.h>
#undef byte
#include "resource.h"

INT_PTR CALLBACK LineNumProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
