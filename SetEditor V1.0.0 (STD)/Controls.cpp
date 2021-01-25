#include "Header.h"

#define BUTTON_TYPE_TEXT 0
#define BUTTON_TYPE_IMAGE 1

void getTextSize(string a, SIZE* size, HFONT hfont);

extern HWND WND_Tip;

struct BUTTON
{
	HWND hWnd = nullptr;

	HBITMAP bm = nullptr;

	string str;
	
	COLORREF tColor = RGB(255, 255, 255);
	COLORREF bColor = RGB(0, 0, 0);
	COLORREF fColor = RGB(20, 20, 20);
	COLORREF pColor = RGB(255, 255, 255);

	bool focused = false;
	bool pressed = false;
	
	bool type = BUTTON_TYPE_TEXT;

	bool enable = true;
};

struct SBUTTON
{
	HWND hWnd = nullptr;

	string str;
	string tip_text;
	bool tip = false;

	COLORREF tColor = RGB(0, 0, 0);

	COLORREF bColor = RGB(255, 255, 255);

	COLORREF fbColor = RGB(211, 211, 211);
	COLORREF ffColor = RGB(0, 211, 0);
	COLORREF fpColor = RGB(211, 0, 0);

	bool focused = false;
	bool selected = false;

	int trys = -1;
};

LRESULT CALLBACK buttonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hfont;
	static vector<BUTTON> buttons;
	
	switch (message)
	{
		case WM_CREATE:
		{
			CREATESTRUCTA* winstr = (CREATESTRUCTA*)lParam;
			BUTTON button;

			hfont = CreateFont(16, 8, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("SetEditorBDFontNew"));
			
			button.hWnd = hWnd;
			button.str = winstr->lpszName;
			buttons.push_back(button);
			break;
		}

		case WM_SETDATA:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					switch (wParam)
					{
						case PROP_BUTTON_TEXT:
							buttons[i].str = (char*)lParam;
							buttons[i].type = BUTTON_TYPE_TEXT;
							InvalidateRect(hWnd, 0, false);
							break;
						case PROP_BUTTON_IMAGE:
							buttons[i].bm = (HBITMAP)lParam;
							buttons[i].type = BUTTON_TYPE_IMAGE;
							InvalidateRect(hWnd, 0, false);
							break;
						case PROP_BUTTON_TCOLOR:
							buttons[i].tColor = (COLORREF)lParam;
							break;
						case PROP_BUTTON_BCOLOR:
							buttons[i].bColor = (COLORREF)lParam;
							break;
						case PROP_BUTTON_FCOLOR:
							buttons[i].fColor = (COLORREF)lParam;
							break;
						case PROP_BUTTON_PCOLOR:
							buttons[i].pColor = (COLORREF)lParam;
							break;
						case PROP_BUTTON_ENABLE:
							buttons[i].enable = lParam;
							break;
						default:
							break;
					}
				}
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (!buttons[i].focused)
					{
						buttons[i].focused = true;

						TRACKMOUSEEVENT event;
						event.cbSize = sizeof(TRACKMOUSEEVENT);
						event.dwFlags = TME_LEAVE;
						event.hwndTrack = hWnd;

						TrackMouseEvent(&event);

						InvalidateRect(hWnd, 0, false);
					}
					break;
				}
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					buttons[i].focused = false;
					buttons[i].pressed = false;
					InvalidateRect(hWnd, 0, false);
					break;
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (!buttons[i].enable) break;
					buttons[i].pressed = true; 
					InvalidateRect(hWnd, 0, false);
					break;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (!buttons[i].enable) break;
					buttons[i].pressed = false; 
					SendMessage(GetParent(hWnd), WM_BTNPRESS, (WPARAM)hWnd, 0);
					InvalidateRect(hWnd, 0, false);
					break;
				}
			}
			break;
		}
		case WM_PAINT:
		{
			HDC hdc;
			HDC hdcMem;

			RECT rect;

			BITMAP bm;
			PAINTSTRUCT ps;
			HBRUSH hbrush;

			GetClientRect(hWnd, &rect);
			hdc = BeginPaint(hWnd, &ps);
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (!buttons[i].type)
					{
						hbrush = CreateSolidBrush(RGB(0, 0, 0));
						SelectObject(hdc, hbrush);
						SelectObject(hdc, hfont);
						Rectangle(hdc, 0, 0, rect.right, rect.bottom);

						SetBkMode(hdc, TRANSPARENT);
						SetTextColor(hdc, buttons[i].tColor);

						DrawText(hdc, buttons[i].str.data(), buttons[i].str.size(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						DeleteObject(hbrush);
					}
					else
					{
						hdcMem = CreateCompatibleDC(hdc);

						SelectObject(hdcMem, buttons[i].bm);
						GetObject(buttons[i].bm, sizeof(bm), &bm);
											   
						//TransparentBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, 0);

						BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

						DeleteDC(hdcMem);						
					}

					if (buttons[i].pressed) hbrush = CreateSolidBrush(buttons[i].pColor);
					else if (buttons[i].focused) hbrush = CreateSolidBrush(buttons[i].fColor);
					else hbrush = CreateSolidBrush(buttons[i].bColor);

					SelectObject(hdc, hbrush);
					BitBlt(hdc, 0, 0, rect.right, rect.bottom, 0, 0, 0, PATINVERT);
					DeleteObject(hbrush);
				}
			}
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			DeleteObject(hfont);
			//PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK sbuttonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hfont;
	static vector<SBUTTON> buttons;

	switch (message)
	{
		case WM_CREATE:
		{
			CREATESTRUCTA* winstr = (CREATESTRUCTA*)lParam;
			SBUTTON button;

			hfont = CreateFont(16, 8, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("SetEditorBDFontNew"));

			button.hWnd = hWnd;
			button.str = winstr->lpszName;
			buttons.push_back(button);
			break;
		}
		case WM_SETDATA:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					switch (wParam)
					{
						case PROP_SBUTTON_TIP:
							buttons[i].tip_text = (char*)lParam;
							break;
						case PROP_SBUTTON_TEXT:
							buttons[i].str = (char*)lParam;
							break;
						case PROP_SBUTTON_TCOLOR:
							buttons[i].tColor = (COLORREF)lParam;
							break;
						case PROP_SBUTTON_BCOLOR:
							buttons[i].bColor = (COLORREF)lParam;
							break;
						case PROP_SBUTTON_FBCOLOR:
							buttons[i].fbColor = (COLORREF)lParam;
							break;
						case PROP_SBUTTON_FFCOLOR:
							buttons[i].ffColor = (COLORREF)lParam;
							break;
						case PROP_SBUTTON_FPCOLOR:
							buttons[i].fpColor = (COLORREF)lParam;
							break;
						case PROP_SBUTTON_STATE:
							buttons[i].selected = lParam;
							InvalidateRect(hWnd, 0, false);
							break;
						case PROP_SBUTTON_TRYS:
							buttons[i].trys = lParam;
							InvalidateRect(hWnd, 0, false);
							break;
						default:
							break;
					}
				}
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS mPoint;
			POINT point;
			mPoint = MAKEPOINTS(lParam);

			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (!buttons[i].focused)
					{
						buttons[i].focused = true;

						TRACKMOUSEEVENT event;
						event.cbSize = sizeof(TRACKMOUSEEVENT);
						event.dwFlags = TME_LEAVE;
						event.hwndTrack = hWnd;

						TrackMouseEvent(&event);
						buttons[i].tip = true;

						if (!buttons[i].tip_text.empty()) {

							point.x = mPoint.x;
							point.y = mPoint.y;
							ClientToScreen(hWnd, &point);

							TIP tip;
							tip.text = buttons[i].tip_text.data();
							tip.pos.x = point.x;
							tip.pos.y = point.y;

							SendMessage(WND_Tip, WM_SHOWTIP, SW_SHOW, (LPARAM)&tip);
						}

						InvalidateRect(hWnd, 0, true);
					}
					break;
				}
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					buttons[i].tip = false;
					if (!buttons[i].tip_text.empty()) SendMessage(WND_Tip, WM_SHOWTIP, SW_HIDE, 0);

					buttons[i].focused = false;
					InvalidateRect(hWnd, 0, true);
					break;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					buttons[i].selected = true;
					if (!buttons[i].tip_text.empty()) SendMessage(WND_Tip, WM_SHOWTIP, SW_HIDE, 0);
					SendMessage(GetParent(hWnd), WM_BTNPRESS, (WPARAM)hWnd, 0);
					InvalidateRect(hWnd, 0, true);
					break;
				}
			}
			break;
		}
		case WM_PAINT:
		{
			HDC hdc;

			RECT rect;

			PAINTSTRUCT ps;
			HBRUSH hbrush;

			HPEN hpen;

			GetClientRect(hWnd, &rect);

			hdc = BeginPaint(hWnd, &ps);

			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					if (buttons[i].selected) hpen = CreatePen(PS_SOLID, 2, buttons[i].fpColor);
					else if (buttons[i].focused) hpen = CreatePen(PS_SOLID, 2, buttons[i].ffColor);
					else hpen = CreatePen(PS_SOLID, 2, buttons[i].fbColor);

					hbrush = CreateSolidBrush(buttons[i].bColor);
					
					SelectObject(hdc, hpen);
					SelectObject(hdc, hbrush);
					SelectObject(hdc, hfont);

					Rectangle(hdc, 0, 0, rect.right, rect.bottom);

					SetBkMode(hdc, TRANSPARENT);
					SetTextColor(hdc, buttons[i].tColor);

					if(buttons[i].trys == -1) DrawText(hdc, buttons[i].str.data(), buttons[i].str.size(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
					{
						HBRUSH ghbrush, rhbrush;

						ghbrush = CreateSolidBrush(RGB(0, 211, 0));
						rhbrush = CreateSolidBrush(RGB(211, 0, 0));
						rect.left = 5;
						DrawText(hdc, buttons[i].str.data(), buttons[i].str.size(), &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						/*
						-------------------------------
						|              11             |
						-------------------------------
						|  7  | 3 |  7  | 3 |  7  | 5 |
						-------------------------------
						|              12             |
						-------------------------------
						*/
						RECT rect_1 = { rect.right - 32, 11, rect.right - 25, rect.bottom - 12 };
						RECT rect_2 = { rect.right - 22, 11, rect.right - 15, rect.bottom - 12 };
						RECT rect_3 = { rect.right - 12, 11, rect.right - 5, rect.bottom - 12 };
						switch (buttons[i].trys)
						{
							case 0:
								FillRect(hdc, &rect_1, ghbrush);
								FillRect(hdc, &rect_2, ghbrush);
								FillRect(hdc, &rect_3, ghbrush);
								break;
							case 1:
								FillRect(hdc, &rect_1, rhbrush);
								FillRect(hdc, &rect_2, ghbrush);
								FillRect(hdc, &rect_3, ghbrush);
								break;
							case 2:
								FillRect(hdc, &rect_1, rhbrush);
								FillRect(hdc, &rect_2, rhbrush);
								FillRect(hdc, &rect_3, ghbrush);
								break;
							case 3:
								FillRect(hdc, &rect_1, rhbrush);
								FillRect(hdc, &rect_2, rhbrush);
								FillRect(hdc, &rect_3, rhbrush);
								break;
							default:
								break;
						}
						DeleteObject(rhbrush);
						DeleteObject(ghbrush);
					}

					DeleteObject(hbrush);

					DeleteObject(hpen);
				}
			}

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].hWnd == hWnd)
				{
					buttons.erase(buttons.begin() + i);
					break;
				}
			}
			DeleteObject(hfont);
			//PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

#define scroll_size(v, f, b)		((v < f) ? ((v*b)/f) : b)
#define scroll_offset_to(r, f, b)	(f == 0) ? 0 : ((r*b)/f)
#define scroll_to_offset(r, f, b)	(b == 0) ? 0 : ((r*f)/b)

struct SCROLL
{
	HWND hWnd = nullptr;
	HWND hWndParent = nullptr;

	int plineSize = 1;
	double slineSize = 1;

	int oCurPos = 0;
	int nCurPos = 0;

	int size = 1;

	int lines = 0;

	int fixPos = 0;

	bool pressed = false;

	bool type = SCROLL_TYPE_VERTICAL;
};

LRESULT CALLBACK scrollWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static vector<SCROLL> scrolls;

	switch (message)
	{
		case WM_CREATE:
		{
			SCROLL scroll;
			scroll.hWnd = hWnd;
			scrolls.push_back(scroll);
			break;
		}
		case WM_SIZE:
		{
			POINTS size;
			size = MAKEPOINTS(lParam);

			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					scrolls[i].slineSize =
						(scrolls[i].type ? ((double)size.x / scrolls[i].lines) : ((double)size.y / scrolls[i].lines));

				}
			}
			break;
		}
		case WM_SETDATA:
		{
			RECT rect;

			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					switch (wParam)
					{
						case PROP_SCROLL_LINESIZE:
						{
							scrolls[i].plineSize = std::max(1, static_cast<int>(lParam));

							InvalidateRect(hWnd, 0, false);
							break;
						}
						case PROP_SCROLL_LINES:
						{
							GetClientRect(hWnd, &rect);

							scrolls[i].lines = std::max(1, static_cast<int>(lParam));

							scrolls[i].slineSize = 
								(scrolls[i].type ? ((double)rect.right / lParam) : ((double)rect.bottom / lParam));

							if (scrolls[i].nCurPos + scrolls[i].size > scrolls[i].lines)
							{
								scrolls[i].nCurPos = max(0, scrolls[i].lines - scrolls[i].size);
								if (!scrolls[i].type) SendMessage(scrolls[i].hWndParent, WM_VSCROLL, scrolls[i].nCurPos * scrolls[i].plineSize, 0);
								else SendMessage(scrolls[i].hWndParent, WM_HSCROLL, scrolls[i].nCurPos * scrolls[i].plineSize, 0);
							}

							InvalidateRect(hWnd, 0, false);
							break;
						}
						case PROP_SCROLL_PAGESIZE:
						{
							GetClientRect(hWnd, &rect);

							scrolls[i].size = lParam / scrolls[i].plineSize;

							scrolls[i].slineSize =
								(scrolls[i].type ? ((double)rect.right / scrolls[i].lines) : ((double)rect.bottom / scrolls[i].lines));

							InvalidateRect(hWnd, 0, false);
							break;
						}
						case PROP_SCROLL_TYPE:
						{
							scrolls[i].type = lParam;
							break;
						}
						case PROP_SCROLL_OFFSET:
						{
							if (lParam)
							{
								if (scrolls[i].nCurPos + lParam < 0) lParam = 0;
								else if (scrolls[i].nCurPos + scrolls[i].size + lParam > scrolls[i].lines) lParam = max(0, scrolls[i].lines - scrolls[i].size);
								else lParam = scrolls[i].nCurPos + lParam;

								if (lParam != scrolls[i].nCurPos)
								{
									scrolls[i].nCurPos = lParam;
									if (!scrolls[i].type) SendMessage(scrolls[i].hWndParent, WM_VSCROLL, scrolls[i].nCurPos * scrolls[i].plineSize, 0);
									else SendMessage(scrolls[i].hWndParent, WM_HSCROLL, scrolls[i].nCurPos * scrolls[i].plineSize, 0);
									InvalidateRect(hWnd, 0, false);
								}
							}
							break;
						}
						case PROP_SCROLL_PARENT:
						{
							scrolls[i].hWndParent = (HWND)lParam;
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			POINTS mPoint;

			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					mPoint = MAKEPOINTS(lParam);

					if (!scrolls[i].type)
					{
						if (mPoint.y >= scrolls[i].nCurPos * scrolls[i].slineSize && mPoint.y <= (scrolls[i].nCurPos + scrolls[i].size) * scrolls[i].slineSize)
						{
							scrolls[i].fixPos = mPoint.y;
							scrolls[i].pressed = true;
						}
					}
					else
					{
						if (mPoint.x >= scrolls[i].nCurPos * scrolls[i].slineSize && mPoint.x <= (scrolls[i].nCurPos + scrolls[i].size) * scrolls[i].slineSize)
						{
							scrolls[i].fixPos = mPoint.x;
							scrolls[i].pressed = true;
						}
					}

					SetCapture(hWnd);

					break;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					scrolls[i].oCurPos = scrolls[i].nCurPos;
					scrolls[i].pressed = false;
					ReleaseCapture();
					break;
				}
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS mPoint;

			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					if (wParam == MK_LBUTTON)
					{
						mPoint = MAKEPOINTS(lParam);

						if (scrolls[i].pressed)
						{
							int _scroll = 0;

							_scroll = ((scrolls[i].type ? mPoint.x : mPoint.y) - scrolls[i].fixPos) / scrolls[i].slineSize;

							if (_scroll)
							{
								if (scrolls[i].oCurPos + _scroll < 0) _scroll = 0;
								else if (scrolls[i].oCurPos + scrolls[i].size + _scroll > scrolls[i].lines) _scroll = max(0, scrolls[i].lines - scrolls[i].size);
								else _scroll = scrolls[i].oCurPos + _scroll;

								if (_scroll != scrolls[i].nCurPos)
								{
									scrolls[i].nCurPos = _scroll;
									if(scrolls[i].type)
										SendMessage(scrolls[i].hWndParent, WM_HSCROLL, scrolls[i].nCurPos * scrolls[i].plineSize, 0);
									else
										SendMessage(scrolls[i].hWndParent, WM_VSCROLL, scrolls[i].nCurPos* scrolls[i].plineSize, 0);
									InvalidateRect(hWnd, 0, false);
								}
							}							
						}
					}
					break;
				}
			}
			break;
		}
		case WM_PAINT:
		{
			// ------------------------------
			// Отрисовка окна
			// ------------------------------
			HDC hdc;
			RECT rect;
			PAINTSTRUCT ps;

			HBRUSH hbrush;

			hdc = BeginPaint(hWnd, &ps);
			hbrush = CreateSolidBrush(RGB(0, 0, 0));

			GetClientRect(hWnd, &rect);

			for (size_t i = 0; i < scrolls.size(); i++)
			{
				if (scrolls[i].hWnd == hWnd)
				{
					if (scrolls[i].type == SCROLL_TYPE_VERTICAL)
					{
						rect.top = scrolls[i].nCurPos * scrolls[i].slineSize;
						rect.bottom = (scrolls[i].nCurPos + scrolls[i].size) * scrolls[i].slineSize;
					}
					else
					{
						rect.left = scrolls[i].nCurPos * scrolls[i].slineSize;
						rect.right = (scrolls[i].nCurPos + scrolls[i].size) * scrolls[i].slineSize;
					}
					FillRect(hdc, &rect, hbrush);
					break;
				}
			}

			DeleteObject(hbrush);
			EndPaint(hWnd, &ps);
			// ------------------------------
			break;
		}
		case WM_DESTROY:
		{
			// ------------------------------
			// Закрытие окна
			// ------------------------------
			PostQuitMessage(0);
			// ------------------------------
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT CALLBACK tipWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hfont;
	static vector<TIP> tips;

	switch (message)
	{
		case WM_CREATE:
		{
			hfont = CreateFont(16, 8, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("SetEditorBDFontNew"));
			
			TIP tip;
			tip.hWnd = hWnd;
			tips.push_back(tip);
			break;
		}
		case WM_NCCALCSIZE:
		{
			if (wParam) return 0;
			else return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_NCHITTEST:
		{
			return 1;
		}
		case WM_SHOWTIP:
		{
			TIP* tip;
			SIZE size;

			if (wParam == 0) ShowWindow(hWnd, SW_HIDE);
			else
			{
				for (size_t i = 0; i < tips.size(); i++)
				{
					if (tips[i].hWnd == hWnd)
					{
						tip = (TIP*)lParam;
						tips[i].text = tip->text;
						tips[i].pos = tip->pos;
						tips[i].bkgColor = tip->bkgColor;
						tips[i].txtColor = tip->txtColor;

						getTextSize(tips[i].text, &size, hfont);
						SetWindowPos(hWnd, 0, tips[i].pos.x + 5, tips[i].pos.y + 5, size.cx + 10, size.cy + 10, SWP_SHOWWINDOW | SWP_NOZORDER);
						InvalidateRect(hWnd, 0, false);
						break;
					}
				}
			}
			break;
		}
		case WM_PAINT:
		{
			HDC hdc;
			HPEN hpen;
			HBRUSH hbrush;
			PAINTSTRUCT ps;
			RECT rect;
			hdc = BeginPaint(hWnd, &ps);

			for (size_t i = 0; i < tips.size(); i++)
			{
				if (tips[i].hWnd == hWnd)
				{
					hbrush = CreateSolidBrush(tips[i].bkgColor);
					hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					Rectangle(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - 1, ps.rcPaint.bottom - 1);
					SetTextColor(hdc, tips[i].txtColor);
					SetBkMode(hdc, TRANSPARENT);
					//SelectObject(hdc, hfont);
					rect = ps.rcPaint;
					rect.left = 5;
					rect.top = 5;
					DrawText(hdc, tips[i].text.data(), tips[i].text.size(), &rect, DT_LEFT);

					DeleteObject(hpen);
					DeleteObject(hbrush);
					break;
				}
			}
			EndPaint(hWnd, &ps);
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

void getTextSize(string a, SIZE* size, HFONT hfont)
{
	string str;
	int row = 0;
	int mlength = 0;

	for (int i = 0; i < (int)a.size(); i++)
	{
		if (a[i] == '\n')
		{
			if (str.size())
			{
				SIZE length;
				HDC hdc = GetDC(0);
				SelectObject(hdc, hfont);
				GetTextExtentPoint(hdc, str.data(), str.size(), &length);
				if (length.cx > mlength) mlength = length.cx;
				ReleaseDC(0, hdc);
				str.clear();
			}
			row++;
		}
		else str += a[i];
	}
	if (str.size())
	{
		SIZE length;
		HDC hdc = GetDC(0);
		SelectObject(hdc, hfont);
		GetTextExtentPoint(hdc, str.data(), str.size(), &length);
		if (length.cx > mlength) mlength = length.cx;
		ReleaseDC(0, hdc);
	}
	row++;
	size->cx = mlength;
	size->cy = row * 16;
}

#define MAX_NEDITLENGTH 4

struct NEDIT
{
	HWND hWnd = nullptr;
	string text;

	int pos_x = 0;

	int size_x = 0;
	int size_y = 0;

	bool selected = false;

	bool enabled = true;
};

LRESULT CALLBACK numEditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static vector<NEDIT> nedits;

	static HDC hdc;
	static RECT rect;
	static PAINTSTRUCT ps;

	switch (message)
	{
		case WM_CREATE:
		{
			NEDIT nedit;
			TEXTMETRICA text_metric;

			hdc = GetDC(hWnd);

			//SelectObject(hdc, );

			GetTextMetrics(hdc, &text_metric);

			ReleaseDC(hWnd, hdc);

			nedit.size_x = text_metric.tmAveCharWidth;
			nedit.size_y = text_metric.tmHeight;

			nedit.hWnd = hWnd;
			nedits.push_back(nedit);
			break;
		}
		case WM_ENABLE:
		{
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					if (!lParam)
					{
						return nedits[i].enabled;
					}
					else
					{
						return nedits[i].enabled = wParam;
					}
				}
			}
			return 0;
		}
		case WM_SETFOCUS:
		{
			// ------------------------------
			// Установить каретку
			// ------------------------------
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					if (!nedits[i].enabled)
					{
						SetFocus(0);
						break;
					}

					nedits[i].selected = true;

					CreateCaret(hWnd, (HBITMAP)1, 0, nedits[i].size_y);
					SetCaretPos(nedits[i].pos_x * nedits[i].size_x, 0);
					ShowCaret(hWnd);
				}
			}
			// ------------------------------
			break;
		}
		case WM_KILLFOCUS:
		{
			// ------------------------------
			// Удалить каретку
			// ------------------------------
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					nedits[i].selected = false;

					HideCaret(hWnd);
					DestroyCaret();

					InvalidateRect(hWnd, 0, true);
				}
			}
			// ------------------------------
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// ------------------------------
			// Установить фокус
			// ------------------------------
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					if (GetFocus() != hWnd) SetFocus(hWnd);
					// ------------------------------
					// Задать позицию каретки
					// ------------------------------
					POINTS mouse_point;

					mouse_point = MAKEPOINTS(lParam);

					nedits[i].pos_x = min(mouse_point.x / nedits[i].size_x, (int)nedits[i].text.size());

					SetCaretPos(nedits[i].pos_x * nedits[i].size_x, 0);

					InvalidateRect(hWnd, 0, true);
				}
			}
			break;
		}
		case WM_CHAR:
		{
			// ------------------------------
			// Ввод символов:
			// - Удаление
			// - Ввод допустимого символа
			// ------------------------------
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					switch (wParam)
					{
						case 0x08:
						{
							// ------------------------------
							// Удаление
							// ------------------------------
							if (nedits[i].pos_x > 0)
							{
								nedits[i].text.erase(nedits[i].text.begin() + nedits[i].pos_x - 1);

								nedits[i].pos_x--;

								InvalidateRect(hWnd, 0, true);
								SendMessage(GetParent(hWnd), WM_SETDATA, PROP_SELECTOR_DEPENDENT_LINE, MAKEWORD(atoi(nedits[i].text.data()), i));
							}
							// ------------------------------
							break;
						}
						default:
						{
							// ------------------------------
							// Ввод допустимого символа
							// ------------------------------
							char character = (TCHAR)wParam;

							if (character >= '0' && character <= '9')
							{
								if (nedits[i].text.size() < MAX_NEDITLENGTH)
								{
									nedits[i].text.insert(nedits[i].text.begin() + nedits[i].pos_x, character);

									nedits[i].pos_x++;

									InvalidateRect(hWnd, 0, true);
									SendMessage(GetParent(hWnd), WM_SETDATA, PROP_SELECTOR_DEPENDENT_LINE, MAKEWORD(i, atoi(nedits[i].text.data())));
								}
							}
							// ------------------------------
							break;
						}
					}
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			// ------------------------------
			// Перемещение каретки:
			// - Удаление 'del'
			// - Сдвиг вправо
			// - Сдвиг влево
			// ------------------------------
			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					switch (wParam)
					{
						case VK_DELETE:
						{
							if (nedits[i].pos_x < (int)nedits[i].text.size())
							{
								nedits[i].text.erase(nedits[i].text.begin() + nedits[i].pos_x);

								InvalidateRect(hWnd, 0, true);
								SendMessage(GetParent(hWnd), WM_SETDATA, PROP_SELECTOR_DEPENDENT_LINE, MAKEWORD(i, atoi(nedits[i].text.data())));
							}
							break;
						}
						case VK_LEFT:
						{
							if (nedits[i].pos_x > 0)
							{
								nedits[i].pos_x--;

								SetCaretPos(nedits[i].pos_x * nedits[i].size_x, 0);
							}
							break;
						}
						case VK_RIGHT:
						{
							if (nedits[i].pos_x < (int)nedits[i].text.size())
							{
								nedits[i].pos_x++;

								SetCaretPos(nedits[i].pos_x * nedits[i].size_x, 0);
							}
							break;
						}
					}
					break;
				}
			}
			// ------------------------------
			break;
		}

		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_SELECTOR_DEPENDENT_LINE:
				{
					for (size_t i = 0; i < nedits.size(); i++)
					{
						if (nedits[i].hWnd == hWnd)
						{
							if (lParam == -1) nedits[i].text.clear();
							else nedits[i].text = to_string(lParam);
							InvalidateRect(hWnd, 0, true);
						}
					}
					break;
				}
			}
			break;
		}

		case WM_PAINT:
		{
			// ------------------------------
			// Отрисовка окна:
			// - Вывод текста
			// ------------------------------
			HDC hdc;
			RECT rect;
			PAINTSTRUCT ps;

			for (size_t i = 0; i < nedits.size(); i++)
			{
				if (nedits[i].hWnd == hWnd)
				{
					HideCaret(hWnd);

					GetClientRect(hWnd, &rect);

					hdc = BeginPaint(hWnd, &ps);

					//SelectObject(hdc, hFont);

					SetBkMode(hdc, OPAQUE);

					if (nedits[i].enabled) SetBkColor(hdc, RGB(255, 255, 255));
					else SetBkColor(hdc, RGB(171, 171, 171));

					if (nedits[i].text.empty() && !nedits[i].selected) DrawText(hdc, "NONE", -1, &rect, DT_VCENTER);
					else  DrawText(hdc, nedits[i].text.data(), nedits[i].text.size(), &rect, DT_VCENTER);

					EndPaint(hWnd, &ps);

					SetCaretPos(nedits[i].pos_x * nedits[i].size_x, 0);

					ShowCaret(hWnd);
				}
			}
			// ------------------------------
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}