#include "Header.h"

LRESULT CALLBACK taskWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UserInfo* pUI;

	static HWND* taskButtons = nullptr;

	static HWND buttonCancel;

	static int selected = 0;

	static int bsize = 0;

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
				return 1;
			}
			for (int i = 0; i < (int)pUI->CurrentTasks.size(); i++)
			{
				if (taskButtons[i] == (HWND)wParam)
				{
					if (selected != i)
					{
						SendMessage(taskButtons[selected], WM_SETDATA, PROP_SBUTTON_STATE, 0);

						selected = i;

						pUI->CurrentTask = i;

						SendMessage(GetParent(hWnd), WM_UPDATETASK, PROP_SBUTTON_STATE, 0);
					}
				}
			}
			break;
		}

		case WM_UPDATE:
		{
			// TODO: !!! Удаление из массива кнопок в контролах
			for (int i = 0; i < bsize; i++)
			{
				DestroyWindow(taskButtons[i]);
			}
			delete[] taskButtons;
			taskButtons = nullptr;
			taskButtons = new HWND[pUI->CurrentTasks.size()];
			bsize = pUI->CurrentTasks.size();

			string text;
			RECT rect;

			GetClientRect(hWnd, &rect);

			for (int i = 0; i < bsize; i++)
			{
				text = "Задание " + to_string(pUI->CurrentTasks[i].ID);
				taskButtons[i] = CreateWindow("sButton", text.data(), WS_VISIBLE | WS_CHILD, 10, 40 + i * 33, rect.right - 20, 30, hWnd, nullptr, nullptr, nullptr);

				SendMessage(taskButtons[i], WM_SETDATA, PROP_SBUTTON_TRYS, pUI->CurrentTasks[i].Attempts);
			}
			selected = pUI->CurrentTask;
			SendMessage(taskButtons[selected], WM_SETDATA, PROP_SBUTTON_STATE, 1);
			break;
		}

		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_STATE_USERINFO:
				{
					pUI = (UserInfo*)lParam;
					for (int i = 0; i < bsize; i++)
					{
						DestroyWindow(taskButtons[i]);
					}
					delete[] taskButtons;
					taskButtons = nullptr;
					taskButtons = new HWND[pUI->CurrentTasks.size()];
					bsize = pUI->CurrentTasks.size();
					string text;
					RECT rect;

					GetClientRect(hWnd, &rect);

					for (size_t i = 0; i < pUI->CurrentTasks.size(); i++)
					{
						text = "Задание " + to_string(pUI->CurrentTasks[i].ID);
						taskButtons[i] = CreateWindow("sButton", text.data(), WS_VISIBLE | WS_CHILD, 10, 40 + i * 33, rect.right - 20, 30, hWnd, nullptr, nullptr, nullptr);
						
						SendMessage(taskButtons[i], WM_SETDATA, PROP_SBUTTON_TRYS, pUI->CurrentTasks[i].Attempts);
					}
					selected = pUI->CurrentTask;
					SendMessage(taskButtons[selected], WM_SETDATA, PROP_SBUTTON_STATE, 1);

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

			hdc = BeginPaint(hWnd, &ps);

			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, RGB(0, 0, 0));
			Rectangle(hdc, 0, 0, rect.right, 30);

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));

			text = "Задания";
			rect.bottom = 30;
			DrawText(hdc, text.data(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			delete[] taskButtons;
			taskButtons = nullptr;
			PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}