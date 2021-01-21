#include "LineNum.h"

INT_PTR CALLBACK LineNumProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			return (INT_PTR)true;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK)
			{
				char buffer[3];
				SendMessage(GetDlgItem(hDlg, IDC_NUMEDIT), WM_GETTEXT, 3, (LPARAM)buffer);
				EndDialog(hDlg, atoi(buffer));
			}
			if (LOWORD(wParam) == IDCANCEL) EndDialog(hDlg, 0);
			break;
		}
	}
	return (INT_PTR)false;
}
