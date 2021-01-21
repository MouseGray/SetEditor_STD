#include "Header.h"

LRESULT CALLBACK resultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UserInfo* pUI;
	static HWND buttonCancel;
	static int type;
	static int errors = 0;
	static int warnings = 0;
	static int newtask = 0;
	static int answer = 0;
	static int task = 0;
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
			}
			return 1;
		}

		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_RESULT_USERINFO:
				{
					pUI = (UserInfo*)lParam;
					return 1;
				}
				case PROP_RESULT_TYPE:
				{
					type = lParam;
					return 1;
				}
				case PROP_RESULT_ERROR_AMOUNT:
				{
					errors = lParam;
					return 1;
				}
				case PROP_RESULT_WARNING_AMOUNT:
				{
					warnings = lParam;
					return 1;
				}
				case PROP_RESULT_NEWTASK:
				{
					newtask = lParam;
					return 1;
				}
				case PROP_RESULT_ANSWER:
				{
					answer = lParam;
					return 1;
				}
				case PROP_RESULT_TASK:
				{
					task = lParam;
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

			switch (type)
			{
				case TYPE_WAIT:
				{
					text += "Проверка...";
					break;
				}
				case TYPE_ERROR:
				{
					text += "Имя: " + pUI->UserName;
					text += "\nЗадание: " + to_string(task);
					text += "\n\nОбнаружено:";
					text += "\n>Ошибки: " + to_string(errors);
					text += "\n>Предупреждения: " + to_string(warnings);
					if (!answer) text += "\n>Неполное решение!";
					text += "\n\nОсталось попыток: " + to_string(3 - pUI->GetCurrentTask()->Attempts);
					break;
				}
				case TYPE_ERROR_AND_NEW:
				{
					text += "Имя: " + pUI->UserName;
					text += "\nЗадание: " + to_string(task);
					text += "\n\nОбнаружено:";
					text += "\n>Ошибки: " + to_string(errors);
					text += "\n>Предупреждения: " + to_string(warnings);
					if (!answer) text += "\n>Неполное решение!";
					text += "\n\nДобавлено задание " + to_string(newtask) + "!";
					break;
				}
				case TYPE_PERFORMED:
				{
					text += "Имя: " + pUI->UserName;
					text += "\nЗадание: " + to_string(task);
					text += "\n\nЗадание выполнено!";
					break;
				}
			}

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));
			DrawText(hdc, text.data(), text.size(), &rect, DT_LEFT);

			text = "Результат";
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