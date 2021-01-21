#include "Header.h"

#define STD_BUTTON 25

LRESULT CALLBACK keypadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND WND_keypad_std_buttons[STD_BUTTON];
	static HWND* WND_keypad_user_buttons = nullptr;
	static HWND parent;
	static size_t bsize = 0;
	static vector<char> names;
	static UserInfo* pUI;
	switch (message)
	{
		case WM_CREATE:
		{
			WND_keypad_std_buttons[0] = CreateWindow("nButton", "~", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[1] = CreateWindow("nButton", "#", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[2] = CreateWindow("nButton", "$", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[3] = CreateWindow("nButton", "^", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[4] = CreateWindow("nButton", "=", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[5] = CreateWindow("nButton", "-", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[6] = CreateWindow("nButton", "+", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[7] = CreateWindow("nButton", "*", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[8] = CreateWindow("nButton", "/", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[9] = CreateWindow("nButton", "|", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[10] = CreateWindow("nButton", "(", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[11] = CreateWindow("nButton", ")", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[12] = CreateWindow("nButton", "!", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[13] = CreateWindow("nButton", "@", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[14] = CreateWindow("nButton", "7", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[15] = CreateWindow("nButton", "8", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[16] = CreateWindow("nButton", "9", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[17] = CreateWindow("nButton", "4", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[18] = CreateWindow("nButton", "5", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[19] = CreateWindow("nButton", "6", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[20] = CreateWindow("nButton", "1", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[21] = CreateWindow("nButton", "2", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[22] = CreateWindow("nButton", "3", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[23] = CreateWindow("nButton", "0", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_keypad_std_buttons[24] = CreateWindow("nButton", ".", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			break;
		}

		case WM_SIZE:
		{
			SetWindowPos(WND_keypad_std_buttons[0], 0, 20, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[1], 0, 50, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[2], 0, 80, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[3], 0, 110, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[4], 0, 140, 20, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[5], 0, 20, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[6], 0, 50, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[7], 0, 80, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[8], 0, 110, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[9], 0, 140, 50, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[10], 0, 20, 80, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[11], 0, 50, 80, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[12], 0, 80, 80, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[13], 0, 110, 80, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[14], 0, 180, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[15], 0, 210, 20, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[16], 0, 240, 20, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[17], 0, 180, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[18], 0, 210, 50, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[19], 0, 240, 50, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[20], 0, 180, 80, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[21], 0, 210, 80, 28, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[22], 0, 240, 80, 28, 28, SWP_NOZORDER);

			SetWindowPos(WND_keypad_std_buttons[23], 0, 180, 110, 58, 28, SWP_NOZORDER);
			SetWindowPos(WND_keypad_std_buttons[24], 0, 240, 110, 28, 28, SWP_NOZORDER);

			for (size_t i = 0; i < min(bsize, 5); i++)
				SetWindowPos(WND_keypad_user_buttons[i], 0, 20 + 30*i, 110, 28, 28, SWP_NOZORDER);
			break;
		}
		case WM_NCCALCSIZE:
		{
			if (wParam) return 0;
			else return DefWindowProc(hWnd, message, wParam, lParam);
		}
		case WM_NCHITTEST:
		{
			return HTCAPTION;
		}
		case WM_BTNPRESS:
		{
			if ((HWND)wParam == WND_keypad_std_buttons[0]) 
				SendMessage(parent, WM_CHAR, '~', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[1])
				SendMessage(parent, WM_CHAR, '#', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[2])
				SendMessage(parent, WM_CHAR, '$', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[3])
				SendMessage(parent, WM_CHAR, '^', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[4])
				SendMessage(parent, WM_CHAR, '=', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[5])
				SendMessage(parent, WM_CHAR, '-', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[6])
				SendMessage(parent, WM_CHAR, '+', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[7])
				SendMessage(parent, WM_CHAR, '*', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[8])
				SendMessage(parent, WM_CHAR, '/', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[9])
				SendMessage(parent, WM_CHAR, '|', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[10])
				SendMessage(parent, WM_CHAR, '(', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[11])
				SendMessage(parent, WM_CHAR, ')', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[12])
				SendMessage(parent, WM_CHAR, '!', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[13])
				SendMessage(parent, WM_CHAR, '@', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[14])
				SendMessage(parent, WM_CHAR, '7', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[15])
				SendMessage(parent, WM_CHAR, '8', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[16])
				SendMessage(parent, WM_CHAR, '9', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[17])
				SendMessage(parent, WM_CHAR, '4', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[18])
				SendMessage(parent, WM_CHAR, '5', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[19])
				SendMessage(parent, WM_CHAR, '6', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[20])
				SendMessage(parent, WM_CHAR, '1', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[21])
				SendMessage(parent, WM_CHAR, '2', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[22])
				SendMessage(parent, WM_CHAR, '3', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[23])
				SendMessage(parent, WM_CHAR, '0', NULL);
			else if ((HWND)wParam == WND_keypad_std_buttons[24])
				SendMessage(parent, WM_CHAR, '.', NULL);

			for (size_t i = 0; i < bsize; i++)
				if ((HWND)wParam == WND_keypad_user_buttons[i]) 
					SendMessage(parent, WM_CHAR, names[i], NULL);

			break;
		}

		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_USERINFO:
				{
					string text;
					pUI = (UserInfo*)lParam;
					for (size_t i = 0; i < bsize; i++) 
						DestroyWindow(WND_keypad_user_buttons[i]);
					delete[] WND_keypad_user_buttons;
					WND_keypad_user_buttons = nullptr;
					WND_keypad_user_buttons = new HWND[pUI->GetCurrentTask()->SetNames.size()];
					bsize = pUI->GetCurrentTask()->SetNames.size();
					names = pUI->GetCurrentTask()->SetNames;
					for (size_t i = 0; i < bsize; i++) {
						text = pUI->GetCurrentTask()->SetNames[i];
						WND_keypad_user_buttons[i] = CreateWindow("nButton", text.data(), WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
					}
					for (size_t i = 0; i < min(bsize, 5); i++)
						SetWindowPos(WND_keypad_user_buttons[i], 0, 20 + 30 * i, 110, 28, 28, SWP_NOZORDER);
					break;
				}
				case PROP_KEYPAD_PARENT:
				{
					parent = (HWND)lParam;
					return 1;
				}
				default: return 0;
			}
			break;
		}

		case WM_DESTROY:
		{
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}