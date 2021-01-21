#include "Header.h"

LRESULT CALLBACK stateWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UserInfo* pUI;
	static HWND buttonCancel;

	switch (message)
	{
		case WM_CREATE:
		{
			buttonCancel = CreateWindow("nButton", "Закрыть", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			break;
		}

		case WM_NCCALCSIZE:
		{
			if (wParam) return 0;
			else return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_NCHITTEST:
		{
			POINT point;

			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			ScreenToClient(hWnd, &point);

			if (point.y < 30) return HTCAPTION;

			return 1;
		}

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			SetWindowPos(buttonCancel, 0, rect.right / 2 - 35, rect.bottom - 30, 70, 20, SWP_NOZORDER);
			break;
		}

		case WM_BTNPRESS:
		{
			if ((HWND)wParam == buttonCancel)
			{
				ShowWindow(hWnd, SW_HIDE);
				EnableWindow(GetParent(hWnd), true);
				SetActiveWindow(GetParent(hWnd));
			}
			return 1;
		}

		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_STATE_USERINFO:
				{
					pUI = (UserInfo*)lParam;
					return 1;
				}
				default: return 0;
			}
			break;
		}

		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;

			RECT rect;

			string text;

			GetClientRect(hWnd, &rect);
			rect.left += 15; rect.right -= 15;
			rect.top += 40; rect.bottom -= 40;

			hdc = BeginPaint(hWnd, &ps);

			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, RGB(0, 0, 0));
			Rectangle(hdc, 0, 0, rect.right + 15, 30);

			text += "Имя: " + pUI->UserName;
			text += "\nЗадания:";
			for (int i = 0; i < (int)pUI->CurrentTasks.size(); i++)
			{
				text += " " + to_string(pUI->CurrentTasks[i].ID);
			}
			text += "\nВыполнены:";
			for (int i = 0; i < (int)pUI->CompletedTasks.size(); i++)
			{
				text += " " + to_string(pUI->CompletedTasks[i]);
			}
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));
			DrawText(hdc, text.data(), text.size(), &rect, DT_LEFT);

			text = "Статистика";
			rect = { 0, 0, rect.right + 15, 30 };
			DrawText(hdc, text.data(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}