#include "Header.h"
#include "WinClasses.h"
#include "resource.h"
// ==================== =========== ==================== //

Config MainConfig;
UserInfo MainUserInfo;

struct card
{
	string title;
	string body;
};
class Description
{
public:
	map<int, card> transformations;
	map<int, string> errors;
};

Description Descriptions;

HWND WND = nullptr;
HWND WND_Selector = nullptr;
HWND WND_Tip = nullptr;

HBITMAP BM_Menu_Turn = nullptr;
HBITMAP BM_Menu_Restore = nullptr;
HBITMAP BM_Menu_Close = nullptr;

HBITMAP BM_Panel_Setting = nullptr;
HBITMAP BM_Panel_State = nullptr;
HBITMAP BM_Panel_Check = nullptr;
HBITMAP BM_Panel_Keypad = nullptr;

HBITMAP BM_Panel_Light_Markers[LIGHT_MARKERS];
HBITMAP BM_Panel_Dark_Markers[DARK_MARKERS];

bool LoadImages();
bool LoadTransformDescription();
bool LoadErrorDescription();

void MainBridge(CEdit* edit);

string OperationText(int id);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ----------------------------------------
	// �������� ����������� ������
	// ----------------------------------------
	// ��������� ������
	if (!AddFontResource(FONT_NAME)) {
		string errorText;
		errorText += "������ 0x0001! �� ������� ��������� ����� ";
		errorText += FONT_NAME;
		errorText += ".";
		MessageBox(NULL, errorText.data(), ERR_CAPTION, MB_OK | MB_ICONERROR);
		return 2;
	}
	// ��������� ����������� 
	if (!LoadImages()) {
		string errorText;
		errorText += "������ 0x0002! �� ������� ��������� �����������.";
		MessageBox(NULL, errorText.data(), ERR_CAPTION, MB_OK | MB_ICONERROR);
		return 2;
	}
	// ��������� �������� ��������������
	if (!LoadTransformDescription()) {
		string errorText;
		errorText += "������ 0x0003! �� ������� ��������� �������� ��������������.";
		MessageBox(NULL, errorText.data(), ERR_CAPTION, MB_OK | MB_ICONERROR);
		return 2;
	}
	// ��������� �������� ������
	if (!LoadErrorDescription()) {
		string errorText;
		errorText += "������ 0x0003! �� ������� ��������� �������� ������.";
		MessageBox(NULL, errorText.data(), ERR_CAPTION, MB_OK | MB_ICONERROR);
		return 2;
	}
	// ----------------------------------------
	// ������������� ������� �������
	// ----------------------------------------
	if (!mainWinClass(hInstance)) return false;
	if (!stateWinClass(hInstance)) return false;
	if (!taskWinClass(hInstance)) return false;
	if (!resultWinClass(hInstance)) return false;
	if (!textAreaWinClass(hInstance)) return false;
	if (!opSelectorWinClass(hInstance)) return false;
	if (!buttonWinClass(hInstance)) return false;
	if (!scrollWinClass(hInstance)) return false;
	if (!tipWinClass(hInstance)) return false;
	if (!sButtonWinClass(hInstance)) return false;
	if (!keypadWinClass(hInstance)) return false;
	// ----------------------------------------
	// ������� ������
	// ----------------------------------------
	MainConfig.Font = CreateFont(16, 8, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("SetEditorBDFontNew"));
	
	WND = CreateWindowEx(WS_EX_COMPOSITED, "main", "SETS", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!WND) return false;
	WND_Tip = CreateWindowEx(WS_EX_TOPMOST, "nTip", "", WS_POPUP, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(WND, nCmdShow);
	UpdateWindow(WND);

	SetWindowPos(WND, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE);

	if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), WND, LoginProc, NULL)) DestroyWindow(WND);

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	RemoveFontResource("SetEditorBDFontNew.ttf");

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isMaximize = false;
	static bool isVisibleKeypad = false;

	// �������� ���������� ����������
	//				������ ����
	static	HWND	WND_Menu_File			=	nullptr;	// ������ ������ �������� ����
	static	HWND	WND_Menu_Help			=	nullptr;	// ������ ������ �������
	static	HWND	WND_Menu_Turn			=	nullptr;	// ������ '�������� ����'
	static	HWND	WND_Menu_Restore		=	nullptr;	// ������ '���������� ����'
	static	HWND	WND_Menu_Close			=	nullptr;	// ������ '������� ����������'
	//				������ �������� ����
	static	HWND	WND_Menu_File_New		=	nullptr;	// ������ �������� ������ ���������
	static	HWND	WND_Menu_File_Open		=	nullptr;	// ������ �������� ���������
	static	HWND	WND_Menu_File_Save		=	nullptr;	// ������ ���������� ��������� ���������
	static	HWND	WND_Menu_File_SaveAs	=	nullptr;	// ������ ����������� ������ ���������
	//				������ ������ ������������
	static	HWND	WND_Panel_Setting		=	nullptr;	// ������ ��������
	static	HWND	WND_Panel_State			=	nullptr;	// ������ ����������
	static	HWND	WND_Panel_Task			=	nullptr;	// ������ ������ �������
	static	HWND	WND_Panel_Check			=	nullptr;	// ������ ��������
	static	HWND	WND_Panel_Keypad		=	nullptr;	// ������ ������ ����������
	//				�������
	static	HWND	WND_Panel_Light_Markers[LIGHT_MARKERS];	// ������� �������
	static	HWND	WND_Panel_Dark_Markers[DARK_MARKERS];	// ������ �������
	//				����� �������
	static	HWND	WND_Text_Task			=	nullptr;	// ����� �������
	//				�������� ������
	static	HWND	WND_Editor				=	nullptr;	// �������� ������
	//				���������� ����
	static	HWND	WND_Setting				=	nullptr;	// ���� ��������
	static	HWND	WND_State				=	nullptr;	// ���� ����������
	static	HWND	WND_Task				=	nullptr;	// ���� ������ �������
	static	HWND	WND_Keypad				=	nullptr;	// ���� ����������

	static	HWND	WND_Result				=	nullptr;	// ���� ����������
	switch (message)
	{
		case WM_CREATE:
		{
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															������ ����
			// --------------------------------------------------------------------------------------------------------------------------------------------
			WND_Menu_File			= CreateWindow("nButton",	"����",		WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_Menu_Help			= CreateWindow("nButton",	"�������",	WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);

			WND_Menu_Turn			= CreateWindow("nButton",	"",			WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Menu_Turn, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Menu_Turn);

			WND_Menu_Restore		= CreateWindow("nButton",	"",			WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Menu_Restore, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Menu_Restore);

			WND_Menu_Close			= CreateWindow("nButton",	"",			WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Menu_Close, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Menu_Close);
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															������ �������� ����
			// --------------------------------------------------------------------------------------------------------------------------------------------
			WND_Menu_File_New		= CreateWindow("nButton",	"�������",			WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_Menu_File_Open		= CreateWindow("nButton",	"�������",			WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_Menu_File_Save		= CreateWindow("nButton",	"���������",		WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_Menu_File_SaveAs	= CreateWindow("nButton",	"��������� ���...", WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															������ ������ ������������
			// --------------------------------------------------------------------------------------------------------------------------------------------
			WND_Panel_Setting		= CreateWindow("nButton",	"",					WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Panel_Setting, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BM_Panel_Setting);

			WND_Panel_State			= CreateWindow("nButton",	"",					WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Panel_State, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Panel_State);

			WND_Panel_Task			= CreateWindow("nButton",	"-",				WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);

			WND_Panel_Check			= CreateWindow("nButton",	"",					WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Panel_Check, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Panel_Check);

			WND_Panel_Keypad		= CreateWindow("nButton",	"",					WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Panel_Keypad, WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Panel_Keypad);

			for (size_t i = 0; i < LIGHT_MARKERS; i++) {
				WND_Panel_Light_Markers[i] = CreateWindow("nButton", "", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
				SendMessage(WND_Panel_Light_Markers[i], WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Panel_Light_Markers[i]);
			}
			for (size_t i = 0; i < DARK_MARKERS; i++) {
				WND_Panel_Dark_Markers[i] = CreateWindow("nButton", "", WS_VISIBLE | WS_BORDER | WS_CHILD, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
				SendMessage(WND_Panel_Dark_Markers[i], WM_SETDATA, PROP_BUTTON_IMAGE, (LPARAM)BM_Panel_Dark_Markers[i]);
			}
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															����� �������
			// --------------------------------------------------------------------------------------------------------------------------------------------
			WND_Text_Task = CreateWindow("Static", "", WS_VISIBLE | WS_BORDER | WS_CHILD | WS_VSCROLL, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															��������
			// --------------------------------------------------------------------------------------------------------------------------------------------
			WND_Editor = CreateWindow("textArea", "", WS_VISIBLE | WS_BORDER | WS_CHILD | WS_VSCROLL, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Editor, WM_SETFONT, (WPARAM)MainConfig.Font, 0);  
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//															���������� ����
			// --------------------------------------------------------------------------------------------------------------------------------------------
			//WND_Setting = CreateWindow("WNDSetting", "", WS_VISIBLE | WS_BORDER | WS_CHILD | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_State		= CreateWindow("WNDState",	"", WS_BORDER, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_State, WM_SETDATA, PROP_STATE_USERINFO, (LPARAM)&MainUserInfo);
			WND_Task		= CreateWindow("WNDTask",	"", WS_BORDER, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			SendMessage(WND_Task, WM_SETDATA, PROP_TASK_UPDATE, (LPARAM)&MainUserInfo);
			WND_Keypad		= CreateWindow("WNDKeypad", "", WS_BORDER, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
			WND_Result		= CreateWindow("WNDResult", "", WS_BORDER, 0, 0, 0, 0, hWnd, nullptr, 0, nullptr);
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
			RECT rect;
			GetClientRect(hWnd, &rect);
			ScreenToClient(hWnd, &point);

			if (point.x < 4) return HTLEFT;
			if (point.x > (rect.right - rect.left) - 4) return HTRIGHT;
			if (point.y < 4) return HTTOP;
			if (point.y > (rect.bottom - rect.top) - 4) return HTBOTTOM;
			if (point.y < 30) return HTCAPTION;

			return 1;
		}
		case WM_SIZE:
		{
			SetWindowPos(WND_Menu_File, 0, 10, 7, 70, 16, SWP_NOZORDER);
			SetWindowPos(WND_Menu_Help, 0, 85, 7, 70, 16, SWP_NOZORDER);

			SetWindowPos(WND_Menu_Turn, 0, LOWORD(lParam) - 69, 5, 20, 20, SWP_NOZORDER);
			SetWindowPos(WND_Menu_Restore, 0, LOWORD(lParam) - 47, 5, 20, 20, SWP_NOZORDER);
			SetWindowPos(WND_Menu_Close, 0, LOWORD(lParam) - 25, 5, 20, 20, SWP_NOZORDER);

			SetWindowPos(WND_Menu_File_New, 0, 10, 30, 150, 25, SWP_NOZORDER);
			SetWindowPos(WND_Menu_File_Open, 0, 10, 55, 150, 25, SWP_NOZORDER);
			SetWindowPos(WND_Menu_File_Save, 0, 10, 80, 150, 25, SWP_NOZORDER);
			SetWindowPos(WND_Menu_File_SaveAs, 0, 10, 105, 150, 25, SWP_NOZORDER);

			SetWindowPos(WND_Panel_Setting, 0, 10, 34, 24, 22, SWP_NOZORDER);
			SetWindowPos(WND_Panel_State, 0, 36, 34, 24, 22, SWP_NOZORDER);
			SetWindowPos(WND_Panel_Task, 0, 62, 34, 74, 22, SWP_NOZORDER);
			SetWindowPos(WND_Panel_Check, 0, 138, 34, 24, 22, SWP_NOZORDER);
			SetWindowPos(WND_Panel_Keypad, 0, 164, 34, 24, 22, SWP_NOZORDER);

			for (size_t i = 0; i < LIGHT_MARKERS; i++) SetWindowPos(WND_Panel_Light_Markers[i], 0, 218 + i * 26, 34, 24, 22, SWP_NOZORDER);
			for (size_t i = 0; i < DARK_MARKERS; i++) SetWindowPos(WND_Panel_Dark_Markers[i], 0, 248 + LIGHT_MARKERS * 26 + i * 26, 34, 24, 22, SWP_NOZORDER);

			SetWindowPos(WND_Text_Task, 0, 10, 70, LOWORD(lParam) - 20, 120, SWP_SHOWWINDOW);

			SetWindowPos(WND_Editor, 0, 10, 195, LOWORD(lParam) - 20, HIWORD(lParam) - 210, SWP_NOZORDER);			
			break;
		}
		case WM_BTNPRESS:
		{
			if ((HWND)wParam == WND_Menu_File)
			{
				SetWindowPos(WND_Menu_File_New, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				SetWindowPos(WND_Menu_File_Open, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				SetWindowPos(WND_Menu_File_Save, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				SetWindowPos(WND_Menu_File_SaveAs, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				//SetCapture(hWnd);
				break;
			}
			if ((HWND)wParam == WND_Menu_File_New)
			{
				MainConfig.FileName.clear();
				MainConfig.Init = false;
				CEdit* edit = (CEdit*)SendMessage(WND_Editor, WM_GETDATA, 0, 0);
				edit->Restart();
				InvalidateRect(hWnd, nullptr, true);
				break;
			}
			if ((HWND)wParam == WND_Menu_File_Open)
			{
				CEdit* edit = (CEdit*)SendMessage(WND_Editor, WM_GETDATA, 0, 0);
				string fn = edit->Open();
				if (!fn.empty()) {
					MainConfig.FileName = fn;
					MainConfig.Init = true;
					InvalidateRect(hWnd, nullptr, true);
				}
				break;
			}
			if ((HWND)wParam == WND_Menu_File_Save)
			{
				CEdit* edit = (CEdit*)SendMessage(WND_Editor, WM_GETDATA, 0, 0);
				if (MainConfig.Init) edit->Save(MainConfig.FileName);
				else {
					edit->Save(MainConfig.FileName = edit->SaveAs());
					MainConfig.Init = true;
				}
				InvalidateRect(hWnd, nullptr, true);
				break;
			}
			if ((HWND)wParam == WND_Menu_File_SaveAs)
			{
				CEdit* edit = (CEdit*)SendMessage(WND_Editor, WM_GETDATA, 0, 0);
				edit->Save(MainConfig.FileName = edit->SaveAs());
				MainConfig.Init = true;
				InvalidateRect(hWnd, nullptr, true);
				break;
			}
			if ((HWND)wParam == WND_Menu_Turn)
			{
				ShowWindow(hWnd, SW_MINIMIZE);
				break;
			}
			if ((HWND)wParam == WND_Menu_Restore)
			{
				if (!isMaximize) ShowWindow(hWnd, SW_MAXIMIZE);
				else ShowWindow(hWnd, SW_SHOWNORMAL);
				isMaximize = !isMaximize;
				break;
			}
			if ((HWND)wParam == WND_Menu_Close)
			{
				DestroyWindow(hWnd);
				break;
			}
			if ((HWND)wParam == WND_Panel_State)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				POINT point = { rect.right >> 1, rect.bottom >> 1 };
				ClientToScreen(hWnd, &point);
				SetWindowPos(WND_State, 0, point.x - 75, point.y - 100, 150, 200, SWP_SHOWWINDOW | SWP_NOZORDER);
				break;
			}
			if ((HWND)wParam == WND_Panel_Task)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				POINT point = { rect.right >> 1, rect.bottom >> 1 };
				ClientToScreen(hWnd, &point);
				SendMessage(WND_Task, WM_UPDATE, 0, 0);
				SetWindowPos(WND_Task, 0, point.x - 75, point.y - 100, 150, MainUserInfo.CurrentTasks.size() * 33 + 77, SWP_SHOWWINDOW | SWP_NOZORDER);
				break;
			}
			if ((HWND)wParam == WND_Panel_Check)
			{
				if (!SendMessage(WND_Editor, WM_CHECK, 0, 0)) {
					CEdit* edit = (CEdit*)SendMessage(WND_Editor, WM_GETDATA, 0, 0);
					MainBridge(edit);
				}
				break;
			}
			if ((HWND)wParam == WND_Panel_Keypad)
			{
				if (isVisibleKeypad) ShowWindow(WND_Keypad, SW_HIDE);
				else {
					RECT rect;
					GetClientRect(hWnd, &rect);
					POINT point = { rect.right >> 1, rect.bottom >> 1 };
					ClientToScreen(hWnd, &point);
					SetWindowPos(WND_Keypad, 0, point.x - 145, point.y - 80, 290, 160, SWP_SHOWWINDOW | SWP_NOZORDER);
				}
				isVisibleKeypad = !isVisibleKeypad;
				break;
			}
			if ((HWND)wParam == WND_Panel_Light_Markers[0])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, LMARKER_1);
				break;
			}
			if ((HWND)wParam == WND_Panel_Light_Markers[1])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, LMARKER_2);
				break;
			}
			if ((HWND)wParam == WND_Panel_Light_Markers[2])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, LMARKER_3);
				break;
			}
			if ((HWND)wParam == WND_Panel_Light_Markers[3])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, LMARKER_4);
				break;
			}
			if ((HWND)wParam == WND_Panel_Light_Markers[4])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, LMARKER_5);
				break;
			}
			if ((HWND)wParam == WND_Panel_Dark_Markers[0])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, DMARKER_1);
				break;
			}
			if ((HWND)wParam == WND_Panel_Dark_Markers[1])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, DMARKER_2);
				break;
			}
			if ((HWND)wParam == WND_Panel_Dark_Markers[2])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, DMARKER_3);
				break;
			}
			if ((HWND)wParam == WND_Panel_Dark_Markers[3])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, DMARKER_4);
				break;
			}
			if ((HWND)wParam == WND_Panel_Dark_Markers[4])
			{
				SendMessage(WND_Editor, WM_SETMARKER, 0, DMARKER_5);
				break;
			}
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			//ReleaseCapture();
			ShowWindow(WND_Menu_File_New, SW_HIDE);
			ShowWindow(WND_Menu_File_Open, SW_HIDE);
			ShowWindow(WND_Menu_File_Save, SW_HIDE);
			ShowWindow(WND_Menu_File_SaveAs, SW_HIDE);
			break;
		}
		case WM_UPDATETASK:
		{
			string buttonText;
			if (!MainUserInfo.CurrentTasks.empty())
			{
				mysql_LoadTaskInfo(MainUserInfo.CurrentTask, &MainUserInfo);
				buttonText = "������� " + to_string(MainUserInfo.GetCurrentTask()->ID);
				SendMessage(WND_Panel_Task, WM_SETDATA, PROP_BUTTON_TEXT, (LPARAM)buttonText.data());
				SendMessage(WND_Text_Task, WM_SETTEXT, 0, (LPARAM)MainUserInfo.GetCurrentTask()->TaskText.data());
			}
			else
			{
				buttonText = "������� ���";
				SendMessage(WND_Panel_Task, WM_SETDATA, PROP_BUTTON_TEXT, (LPARAM)buttonText.data());
				SendMessage(WND_Panel_Task, WM_SETDATA, PROP_BUTTON_ENABLE, false);
				SendMessage(WND_Text_Task, WM_SETTEXT, 0, (LPARAM)"");
			}
			SendMessage(WND_Keypad, WM_SETDATA, PROP_USERINFO, (LPARAM)&MainUserInfo);
			break;
		}
		case WM_PAINT:
		{
			// ----------------------------------------
			// ������ ���������
			// ----------------------------------------
			RECT rect;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			// ----------------------------------------
			// ��������� ���������
			// ----------------------------------------
			string captionText = APP_CAPTION;
			if (!MainConfig.FileName.empty()) captionText += " | " + MainConfig.FileName;
			// ----------------------------------------
			// ��������� ���� ���������
			// ----------------------------------------
			SetDCBrushColor(hdc, RGB(0, 0, 0));
			Rectangle(hdc, 0, 0, rect.right, 60);
			// ----------------------------------------
			// ��������� �������� �����
			// ----------------------------------------
			SetDCBrushColor(hdc, RGB(255, 255, 255));
			Rectangle(hdc, 0, 60, rect.right, rect.bottom);
			// ----------------------------------------
			// ��������� ������ ���������
			// ----------------------------------------
			rect.left = 165;
			rect.bottom = 30;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));
			DrawText(hdc, captionText.data(), captionText.size(), &rect, DT_VCENTER | DT_SINGLELINE);
			// ----------------------------------------
			// ����� ���������
			// ----------------------------------------
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			// ----------------------------------------
			// �������� ����������
			// ----------------------------------------
			PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SegmentEditCallback(Segment* segment, int x, int y)
{
	SendMessage(WND_Selector, WM_SETDATA, PROP_SELECTOR_LINE, (LPARAM)segment);
	SetWindowPos(WND_Selector, HWND_TOP, x - 222, y - 150, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	EnableWindow(WND, false);
}

void ShowTipCallback(HWND hWnd, int x, int y, int error, Segment* segment)
{
	string text;
	if (segment != nullptr) {
		TIP tip;
		tip.text += Descriptions.transformations.find(segment->operation)->second.title;
		tip.text += '\n';
		tip.text += Descriptions.errors.find(segment->error)->second;
		tip.text += '\n';
		tip.text += "������: ";
		for (size_t i = 0; i < segment->pointers.size(); i++) {
			tip.text += to_string(segment->pointers[i]);
			tip.text += ' ';
		}
		if (error != 0) {
			tip.text += '\n';
			tip.text += Descriptions.errors.find(error)->second;
		}
		tip.pos.x = x;
		tip.pos.y = y;
		SendMessage(WND_Tip, WM_SHOWTIP, SW_SHOW, (LPARAM)&tip);
		if(GetFocus() != hWnd) SetFocus(hWnd);
		return;
	}
	if (error != 0) {
		TIP tip;
		tip.text += Descriptions.errors.find(error)->second;
		tip.pos.x = x;
		tip.pos.y = y;
		SendMessage(WND_Tip, WM_SHOWTIP, SW_SHOW, (LPARAM)&tip);
		if (GetFocus() != hWnd) SetFocus(hWnd);
		return;
	}
	SendMessage(WND_Tip, WM_SHOWTIP, SW_HIDE, 0);
}

LRESULT CALLBACK TAWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CEdit edit;

	//static HWND WND_Selector = nullptr;

	switch (message)
	{
		case WM_CREATE: {
			edit.W_Create(hWnd);
			edit.SegmentEditCallback = *SegmentEditCallback;
			edit.ShowTipCallback = *ShowTipCallback;

			WND_Selector = CreateWindow("WNDSelector", "", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 510, 320, 0, nullptr, 0, nullptr);
			break;
		}
		case  WM_GETDATA:
		{
			return LRESULT(&edit);
		}
		case WM_SIZE:
		{
			edit.W_Size(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		case WM_SETFONT:
		{
			edit.W_SetFont((HFONT)wParam);
			break;
		}
		case WM_SETMARKER:
		{
			edit.Mark(lParam);
			break;
		}
		case WM_SETFOCUS:
		{
			// ----------------------------------------
			// ���������� �������
			// ----------------------------------------
			CreateCaret(hWnd, (HBITMAP)1, 0, edit.GetCaretHeight());
			ShowCaret(hWnd);
			// ----------------------------------------
			break;
		}
		case WM_KILLFOCUS:
		{
			// ----------------------------------------
			// ������� �������
			// ----------------------------------------
			HideCaret(hWnd);
			DestroyCaret();
			// ----------------------------------------
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// ----------------------------------------
			// ��������� ���������
			// ----------------------------------------
			bool hasChanged = false;
			// ----------------------------------------
			// ���������� �����
			// ----------------------------------------
			if (GetFocus() != hWnd) SetFocus(hWnd);
			// ----------------------------------------
			// ������ ������� �������
			// ----------------------------------------
			POINTS mPoint = MAKEPOINTS(lParam);
			edit.W_LButtonDown(mPoint.x, mPoint.y, GetKeyState(VK_SHIFT) & 0x8000);
			// ----------------------------------------
			// ������ ���������
			// ----------------------------------------
			SetCapture(hWnd);
			// ----------------------------------------
			break;
		}
		case WM_RBUTTONDOWN:
		{
			POINTS mPoint = MAKEPOINTS(lParam);
			edit.W_RButtonDown(mPoint.x, mPoint.y);
		}
		case WM_LBUTTONUP:
		{
			ReleaseCapture();
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS mPoint = MAKEPOINTS(lParam);
			edit.W_MouseMove(mPoint.x, mPoint.y, wParam & MK_LBUTTON);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			// ----------------------------------------
			// ������������ ������ ����
			// ----------------------------------------
			POINTS mPoint = MAKEPOINTS(wParam);
			edit.W_MouseWheel(-mPoint.y / 40);
			// ----------------------------------------
			break;
		}
		case WM_CHAR:
		{
			// ----------------------------------------
			// ���� ��������:
			// - ��������
			// - ������� �������
			// - ���� ����������� �������
			// ----------------------------------------
			switch (wParam)
			{
				case 0x08:
				{
					edit.W_Backspace();
					break;
				}
				default:
				{
					// ----------------------------------------
					// ���� ����������� �������
					// ----------------------------------------
					char character = (TCHAR)wParam;
					edit.W_Char(character);
					// ----------------------------------------
					break;
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_DELETE:
				{
					edit.W_Delete();
					break;
				}
				case VK_LEFT:
				{
					edit.W_Left(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_RIGHT:
				{
					edit.W_Right(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_UP:
				{
					edit.W_Up(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_DOWN:
					edit.W_Down(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				case VK_HOME:
				{
					edit.W_Home(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_END:
				{
					edit.W_End(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_PRIOR:
				{
					edit.W_Prior(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case VK_NEXT:
				{
					edit.W_Next(GetKeyState(VK_SHIFT) & 0x8000);
					break;
				}
				case 0x5A:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000)
					{
						edit.ToBack();
						break;
					}
				}
				case 0x43:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000)
					{
						edit.Copy();
					}
					break;
				}
				case 0x56:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000)
					{
						edit.Paste();
					}
					break;
				}
				case 0x49:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000)
					{
						edit.SetOverline();
					}
					break;
				}
				case 0x51:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000) edit.Mark(1);
					break;
				}
				case 0x57:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000) edit.Mark(2);
					break;
				}
				case 0x45:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000) edit.Mark(0x100);
					break;
				}
				case 0x52:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000) edit.Mark(0x200);
					break;
				}
				case 0x53:
				{
					if (GetKeyState(VK_CONTROL) & 0x8000) {
						edit.Check();
					}
					break;
				}
			}
			// ----------------------------------------
			break;
		}
		case WM_CHECK:
		{
			return edit.Check();
		}
		case WM_VSCROLL:
			edit.W_Scroll(LOWORD(wParam), HIWORD(wParam));
			break;
		case WM_ERASEBKGND: return 1;
		case WM_PAINT: {
			edit.W_Paint();
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

void SetListBox(HWND hWnd, int type, int subtype, Segment* segment = nullptr)
{
	for (int i = SendMessage(hWnd, LB_GETCOUNT, 0, 0); i >= 0; i--) {
		SendMessage(hWnd, LB_DELETESTRING, 0, 0);
	}
	switch (type)
	{
		case 0:
			switch (subtype)
			{
				case -1:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"���������� � ������� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"���������� � �������������� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����� �� �������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"���������������� �� �����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"���������������� �� �����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� � ������ ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� � ������ ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� � ������������� ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� � ������������� ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������� ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� ���������� ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� ���������� ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� ��������� � ��� ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����������� ��������� � ��� ����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ���������� � ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������� ���. ����. ��� 2 ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������� ���. ����. ��� 3 ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ������� ���������");
					break;
				case 1: case 5: case 12:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� ��������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������ �����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					break;
				case 19:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������ �����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					break;
				default:
					break;
			}
			break;
		case 1:
			switch (subtype)
			{
				case -1:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� ������ �� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� ������ �� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ��������������� �����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ���������� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� �������� ���������� � ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"���������� �������� ������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� �� 0");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� �� 1");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� �� -1");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"��������� �� 1");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� � 0");
					break;
				case 3: case 7:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������ �����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������");
					break;
				default:
					break;
			}
			break;
		case 2:
			switch (subtype)
			{
				case -1:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�������� ��������� ���������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"������� ����� �����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����������");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����");
					break;
				case 2: case 3:
					if (segment) {
						for (size_t i = 0; i < segment->pointers.size(); i++) {
							SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)to_string(segment->pointers[i]).data());
						}
					}
					break;
				default:
					break;
			}
			break;
		case 3:
			switch (subtype)
			{
				case -1:
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����");
					SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)"�����");
					break;
			}
			break;
	}
}

LRESULT CALLBACK OSWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND SelectorLevel_1 = nullptr;
	static HWND SelectorLevel_2 = nullptr;
	static HWND SelectorLevel_3 = nullptr;

	static HWND TransformDescription = nullptr;

	static HWND NumberField = nullptr;
	static HWND AddButton = nullptr;
	static HWND RemoveButton = nullptr;

	static HWND CancelButton = nullptr;
	static HWND OKButton = nullptr;

	static Segment* segment = nullptr;
	static int operation = 0;
	static char numBuffer[20];
	switch (message)
	{
		case WM_CREATE:
		{
			SelectorLevel_1 = CreateWindow("LISTBOX", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL, 10, 10, 150, 150, hWnd, nullptr, nullptr, nullptr);
			SendMessage(SelectorLevel_1, LB_ADDSTRING, 0, (LPARAM)"���������");
			SendMessage(SelectorLevel_1, LB_ADDSTRING, 0, (LPARAM)"�����");
			SendMessage(SelectorLevel_1, LB_ADDSTRING, 0, (LPARAM)"������");
			SendMessage(SelectorLevel_1, LB_ADDSTRING, 0, (LPARAM)"���������");

			SelectorLevel_2 = CreateWindow("LISTBOX", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL, 170, 10, 330, 150, hWnd, nullptr, nullptr, nullptr);

			SelectorLevel_3 = CreateWindow("LISTBOX", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL, 320, 170, 180, 115, hWnd, nullptr, nullptr, nullptr);

			TransformDescription = CreateWindow("STATIC", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 170, 300, 120, hWnd, nullptr, nullptr, nullptr);

			NumberField = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER, 320, 290, 100, 20, hWnd, nullptr, nullptr, nullptr);
			AddButton = CreateWindow("BUTTON", "ADD", WS_BORDER | WS_CHILD | WS_VISIBLE, 420, 290, 40, 20, hWnd, nullptr, nullptr, nullptr);
			RemoveButton = CreateWindow("BUTTON", "DEL", WS_BORDER | WS_CHILD | WS_VISIBLE, 460, 290, 40, 20, hWnd, nullptr, nullptr, nullptr);

			CancelButton = CreateWindow("BUTTON", "�������", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 290, 150, 20, hWnd, nullptr, nullptr, nullptr);
			OKButton = CreateWindow("BUTTON", "�������", WS_BORDER | WS_CHILD | WS_VISIBLE, 160, 290, 150, 20, hWnd, nullptr, nullptr, nullptr);
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
		case WM_SIZE:
		{
			break;
		}
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == LBN_DBLCLK) {
				if ((HWND)lParam == SelectorLevel_1) {
					SetListBox(SelectorLevel_2, SendMessage(SelectorLevel_1, LB_GETCURSEL, 0, 0), -1);
					operation = SendMessage(SelectorLevel_1, LB_GETCURSEL, 0, 0) + 1;
				}
				if ((HWND)lParam == SelectorLevel_2) {
					SetListBox(SelectorLevel_3, SendMessage(SelectorLevel_1, LB_GETCURSEL, 0, 0), SendMessage(SelectorLevel_2, LB_GETCURSEL, 0, 0), segment);
					if (SendMessage(SelectorLevel_3, LB_GETCOUNT, 0, 0) > 0) SendMessage(SelectorLevel_3, LB_SETCURSEL, 0, 0);
					operation = (SendMessage(SelectorLevel_1, LB_GETCURSEL, 0, 0) + 1) | ((SendMessage(SelectorLevel_2, LB_GETCURSEL, 0, 0) + 1) << 4) | (1 << 12);
				}
				if ((HWND)lParam == SelectorLevel_3) {
					if (operation != OP_SUBSTITUTION)
						operation = (SendMessage(SelectorLevel_1, LB_GETCURSEL, 0, 0) + 1) | ((SendMessage(SelectorLevel_2, LB_GETCURSEL, 0, 0) + 1) << 4) | ((SendMessage(SelectorLevel_3, LB_GETCURSEL, 0, 0) + 1) << 12);
				}
				if (operation == OP_SUBSTITUTION) {
					EnableWindow(AddButton, true);
					EnableWindow(RemoveButton, true);
				}
				else {
					EnableWindow(AddButton, false);
					EnableWindow(RemoveButton, false);
				}
				SendMessage(TransformDescription, WM_SETTEXT, 0, (LPARAM)Descriptions.transformations.find(operation)->second.body.c_str());
			}
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((HWND)lParam == AddButton) {
					if (SendMessage(NumberField, WM_GETTEXTLENGTH, 0, 0) > 0) {
						SendMessage(NumberField, WM_GETTEXT, 20, (LPARAM)numBuffer);
						segment->pointers.push_back(atoi(numBuffer));
						SendMessage(SelectorLevel_3, LB_ADDSTRING, 0, (LPARAM)numBuffer);
					}
				}
				if ((HWND)lParam == RemoveButton) {
					int p = SendMessage(SelectorLevel_3, LB_GETCURSEL, 0, 0);
					if (p != -1) {
						segment->pointers.erase(segment->pointers.begin() + p);
						SendMessage(SelectorLevel_3, LB_DELETESTRING, p, 0);
					}
				}
				if ((HWND)lParam == CancelButton) {
					ShowWindow(hWnd, SW_HIDE);
					EnableWindow(WND, true);
					SetActiveWindow(WND);
				}
				if ((HWND)lParam == OKButton) {
					if (operation & 0x0FF0) segment->operation = operation;
					else segment->operation = 0;
					ShowWindow(hWnd, SW_HIDE);
					EnableWindow(WND, true);
					SetActiveWindow(WND);
				}
			}
			break;
		}
		case WM_SETDATA:
		{
			switch (wParam)
			{
				case PROP_SELECTOR_LINE:
					segment = (Segment*)lParam;
					int type = segment->operation & 0xF;
					if (type != 0) {
						SendMessage(SelectorLevel_1, LB_SETCURSEL, type - 1, 0);
						SetListBox(SelectorLevel_2, type - 1, -1);
					}
					int subtype = (segment->operation >> 4) & 0xF;
					if (subtype != 0) {
						SendMessage(SelectorLevel_2, LB_SETCURSEL, subtype - 1, 0);
						SetListBox(SelectorLevel_3, type - 1, subtype - 1, segment);
					}
					int extratype = (segment->operation >> 12) & 0xF;
					if (extratype != 0) {
						SendMessage(SelectorLevel_3, LB_SETCURSEL, extratype - 1, 0);
					}
					operation = segment->operation;
					if (operation == OP_SUBSTITUTION) {
						EnableWindow(AddButton, true);
						EnableWindow(RemoveButton, true);
					}
					else {
						EnableWindow(AddButton, false);
						EnableWindow(RemoveButton, false);
					}
					SendMessage(TransformDescription, WM_SETTEXT, 0, (LPARAM)Descriptions.transformations.find(operation)->second.body.c_str());
					break;
			}
			break;
		}
		//case WM_ERASEBKGND: return true;
		case WM_PAINT:
		{
			// ----------------------------------------
			// ������ ���������
			// ----------------------------------------
			RECT rect;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			// ----------------------------------------
			// ����� ���������
			// ----------------------------------------
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

INT_PTR CALLBACK LoginProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
				string login, password;
				char buffer[46];
				SendMessage(GetDlgItem(hDlg, IDC_EDIT1), WM_GETTEXT, 46, (LPARAM)buffer);
				login = buffer;
				SendMessage(GetDlgItem(hDlg, IDC_EDIT2), WM_GETTEXT, 46, (LPARAM)buffer);
				password = buffer;

				int error = mysql_LoadUserInfo(login, password, &MainUserInfo);

				switch (error)
				{
					case ERR_SQL_NO_ERROR:
						break;
					case ERR_SQL_QUERY_NOT_FOUND:
						SendMessage(GetDlgItem(hDlg, IDC_STATIC1), WM_SETTEXT, 0, (LPARAM)"�������� ����� ��� ������!");
						return 0;
					case ERR_SQL_NO_CONNECTION:
						SendMessage(GetDlgItem(hDlg, IDC_STATIC1), WM_SETTEXT, 0, (LPARAM)"��� ���������� � ��������!");
						return 0;
					case ERR_SQL_DATABASE_NOT_AVAILABLE: case ERR_SQL_QUERY_INVALID_VALUE:
						MessageBox(hDlg, "���� ������ ����������. ����������\n� ��������������.", ERR_CAPTION, MB_ICONERROR | MB_APPLMODAL);
						EndDialog(hDlg, 0);
						return 0;
				}
				MainUserInfo.CurrentTask = 0;

				SendMessage(GetParent(hDlg), WM_UPDATETASK, 0, 0);
				EndDialog(hDlg, LOWORD(wParam));
			}
			if (LOWORD(wParam) == IDCLOSE) EndDialog(hDlg, 0);
			break;
		}
	}
	return (INT_PTR)false;
}

bool LoadImages()
{
	if ((BM_Menu_Turn = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MINIMAZE))) == NULL) return false;
	if ((BM_Menu_Restore = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MAXIMAZE))) == NULL) return false;
	if ((BM_Menu_Close = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CLOSE))) == NULL) return false;

	if ((BM_Panel_Setting = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SETTING))) == NULL) return false;
	if ((BM_Panel_State = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_STATE))) == NULL) return false;
	if ((BM_Panel_Check = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CHECK))) == NULL) return false;
	if ((BM_Panel_Keypad = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_KEYPAD))) == NULL) return false;

	for (size_t i = 0; i < LIGHT_MARKERS; i++) 
		if ((BM_Panel_Light_Markers[i] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LMARKER_1 + i))) == NULL) return false;

	for (size_t i = 0; i < DARK_MARKERS; i++)
		if ((BM_Panel_Dark_Markers[i] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DMARKER_1 + i))) == NULL) return false;
	return true;
}

bool LoadTransformDescription()
{
	card Card;
	Card.title = "�� �������";
	Card.body = "�� �������\n";
	Descriptions.transformations.insert({ 0, Card });

	Card.title = "���������";
	Card.body = "���������\n�������� ���� �����������";
	Descriptions.transformations.insert({ 0x0001, Card });

	Card.title = "���������� ������� ���������";
	Card.body = "���������� ������� ���������\n";
	Descriptions.transformations.insert({ OP_COMPL_NULLSET, Card });

	Card.title = "���������� � �������������� ���������";
	Card.body = "���������� � �������������� ���������\n";
	Descriptions.transformations.insert({ OP_COMPL_UNISET_R1, Card });
	Card.title = "���������� � �������������� ���������.��������� ��������";
	Card.body = "���������� � �������������� ���������.��������� ��������\n";
	Descriptions.transformations.insert({ OP_COMPL_UNISET_R2, Card });
	Card.title = "���������� � �������������� ���������.�������";
	Card.body = "���������� � �������������� ���������.�������\n";
	Descriptions.transformations.insert({ OP_COMPL_UNISET_R3, Card });
	Card.title = "���������� � �������������� ���������.������ �����";
	Card.body = "���������� � �������������� ���������.������ �����\n";
	Descriptions.transformations.insert({ OP_COMPL_UNISET_R4, Card });
	Card.title = "���������� � �������������� ���������.�������";
	Card.body = "���������� � �������������� ���������.�������\n";
	Descriptions.transformations.insert({ OP_COMPL_UNISET_R5, Card });

	Card.title = "����� �� �������";
	Card.body = "����� �� �������\n";
	Descriptions.transformations.insert({ OP_COMPL_A, Card });

	Card.title = "���������������� �� �����������";
	Card.body = "���������������� �� �����������\n";
	Descriptions.transformations.insert({ OP_IN_PARANTHESES, Card });

	Card.title = "���������������� �� �����������";
	Card.body = "���������������� �� �����������\n";
	Descriptions.transformations.insert({ OP_UN_PARANTHESES, Card });

	Card.title = "����������� � ������ ����������";
	Card.body = "����������� � ������ ����������\n";
	Descriptions.transformations.insert({ OP_IN_NULLSET_R1, Card });
	Card.title = "����������� � ������ ����������.��������� ��������";
	Card.body = "����������� � ������ ����������.��������� ��������\n";
	Descriptions.transformations.insert({ OP_IN_NULLSET_R2, Card });
	Card.title = "����������� � ������ ����������.�������";
	Card.body = "����������� � ������ ����������.�������\n";
	Descriptions.transformations.insert({ OP_IN_NULLSET_R3, Card });
	Card.title = "����������� � ������ ����������.������ �����";
	Card.body = "����������� � ������ ����������.������ �����\n";
	Descriptions.transformations.insert({ OP_IN_NULLSET_R4, Card });
	Card.title = "����������� � ������ ����������.�������";
	Card.body = "����������� � ������ ����������.�������\n";
	Descriptions.transformations.insert({ OP_IN_NULLSET_R5, Card });

	Card.title = "����������� � ������ ����������";
	Card.body = "����������� � ������ ����������\n";
	Descriptions.transformations.insert({ OP_UN_NULLSET, Card });

	Card.title = "����������� � ������������� ����������";
	Card.body = "����������� � ������������� ����������\n";
	Descriptions.transformations.insert({ OP_IN_UNISET, Card });

	Card.title = "����������� � ������������� ����������";
	Card.body = "����������� � ������������� ����������\n";
	Descriptions.transformations.insert({ OP_UN_UNISET, Card });

	Card.title = "������� ����������";
	Card.body = "������� ����������\n";
	Descriptions.transformations.insert({ OP_DOUBLE_COMPL, Card });

	Card.title = "����������� ���������� ��������";
	Card.body = "����������� ���������� ��������\n";
	Descriptions.transformations.insert({ OP_IN_A_AND_A, Card });

	Card.title = "����������� ���������� ��������";
	Card.body = "����������� ���������� ��������\n";
	Descriptions.transformations.insert({ OP_UN_A_AND_A, Card });

	Card.title = "����������� ��������� � ��� ����������";
	Card.body = "����������� ��������� � ��� ����������\n";
	Descriptions.transformations.insert({ OP_IN_A_COMPL_A_R1, Card });
	Card.title = "����������� ��������� � ��� ����������.��������� ��������";
	Card.body = "����������� ��������� � ��� ����������.��������� ��������\n";
	Descriptions.transformations.insert({ OP_IN_A_COMPL_A_R2, Card });
	Card.title = "����������� ��������� � ��� ����������.�������";
	Card.body = "����������� ��������� � ��� ����������.�������\n";
	Descriptions.transformations.insert({ OP_IN_A_COMPL_A_R3, Card });
	Card.title = "����������� ��������� � ��� ����������.������ �����";
	Card.body = "����������� ��������� � ��� ����������.������ �����\n";
	Descriptions.transformations.insert({ OP_IN_A_COMPL_A_R4, Card });
	Card.title = "����������� ��������� � ��� ����������.�������";
	Card.body = "����������� ��������� � ��� ����������.�������\n";
	Descriptions.transformations.insert({ OP_IN_A_COMPL_A_R5, Card });
	
	Card.title = "����������� ��������� � ��� ����������";
	Card.body = "����������� ��������� � ��� ����������\n";
	Descriptions.transformations.insert({ OP_UN_A_COMPL_A, Card });

	Card.title = "�������� ���������� � ���������";
	Card.body = "�������� ���������� � ���������\n";
	Descriptions.transformations.insert({ OP_UNISET_MINUS_A, Card });

	Card.title = "�������� ��������";
	Card.body = "�������� ��������\n";
	Descriptions.transformations.insert({ OP_SET_MINUS, Card });

	Card.title = "������� ���. ����. ��� 2 ��������";
	Card.body = "������� ���. ����. ��� 2 ��������\n";
	Descriptions.transformations.insert({ OP_INC_EXC_X2, Card });

	Card.title = "������� ���. ����. ��� 3 ��������";
	Card.body = "������� ���. ����. ��� 3 ��������\n";
	Descriptions.transformations.insert({ OP_INC_EXC_X3, Card });

	Card.title = "�������� ������� ���������";
	Card.body = "�������� ������� ���������\n";
	Descriptions.transformations.insert({ OP_CARDIALITY_R1, Card });
	Card.title = "�������� ������� ���������.�������";
	Card.body = "�������� ������� ���������.�������\n";
	Descriptions.transformations.insert({ OP_CARDIALITY_R3, Card });
	Card.title = "�������� ������� ���������.������ �����";
	Card.body = "�������� ������� ���������.������ �����\n";
	Descriptions.transformations.insert({ OP_CARDIALITY_R4, Card });
	Card.title = "�������� ������� ���������.�������";
	Card.body = "�������� ������� ���������.�������\n";
	Descriptions.transformations.insert({ OP_CARDIALITY_R5, Card });
	
	Card.title = "�����";
	Card.body = "�����\n�������� ���� �������";
	Descriptions.transformations.insert({ 0x0002, Card });

	Card.title = "��������� ������ �� ���������";
	Card.body = "��������� ������ �� ���������\n";
	Descriptions.transformations.insert({ OP_NEG_A, Card });

	Card.title = "��������� ������ �� ���������";
	Card.body = "��������� ������ �� ���������\n";
	Descriptions.transformations.insert({ OP_MP_PARANTHESES, Card });

	Card.title = "������� ���������";
	Card.body = "������� ���������\n";
	Descriptions.transformations.insert({ OP_DOUBLE_NEG, Card });

	Card.title = "�������� ��������������� �����";
	Card.body = "�������� ��������������� �����\n";
	Descriptions.transformations.insert({ OP_ADD_A_NEG_A_R1, Card });
	Card.title = "�������� ��������������� �����.�������";
	Card.body = "�������� ��������������� �����.�������\n";
	Descriptions.transformations.insert({ OP_ADD_A_NEG_A_R3, Card });
	Card.title = "�������� ��������������� �����.������ �����";
	Card.body = "�������� ��������������� �����.������ �����\n";
	Descriptions.transformations.insert({ OP_ADD_A_NEG_A_R4, Card });
	Card.title = "�������� ��������������� �����.�������";
	Card.body = "�������� ��������������� �����.�������\n";
	Descriptions.transformations.insert({ OP_ADD_A_NEG_A_R5, Card });

	Card.title = "�������� ���������� ���������";
	Card.body = "�������� ���������� ���������\n";
	Descriptions.transformations.insert({ OP_CARD_COMPL_A, Card });

	Card.title = "�������� �������� ���������� � ���������";
	Card.body = "�������� �������� ���������� � ���������\n";
	Descriptions.transformations.insert({ OP_CARD_UNISET_MINUS, Card });

	Card.title = "���������� �������� ������";
	Card.body = "���������� �������� ������\n";
	Descriptions.transformations.insert({ OP_INDENTICAL_A, Card });

	Card.title = "��������� �� 0";
	Card.body = "��������� �� 0\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_0_R1, Card });
	Card.title = "��������� �� 0.�������";
	Card.body = "��������� �� 0.�������\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_0_R3, Card });
	Card.title = "��������� �� 0.������ �����";
	Card.body = "��������� �� 0.������ �����\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_0_R4, Card });
	Card.title = "��������� �� 0.�������";
	Card.body = "��������� �� 0.�������\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_0_R5, Card });

	Card.title = "��������� �� 1";
	Card.body = "��������� �� 1\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_1, Card });

	Card.title = "��������� �� -1";
	Card.body = "��������� �� -1\n";
	Descriptions.transformations.insert({ MULTIPLY_BY_MINUS, Card });

	Card.title = "��������� �� 1";
	Card.body = "��������� �� 1\n";
	Descriptions.transformations.insert({ DIVIDE_BY_1, Card });

	Card.title = "�������� � 0";
	Card.body = "�������� � 0\n";
	Descriptions.transformations.insert({ OP_ADD_ZERO, Card });

	Card.title = "�������� ��������� ���������";
	Card.body = "�������� ��������� ���������\n";
	Descriptions.transformations.insert({ OP_COMP, Card });

	Card.title = "������";
	Card.body = "������\n�������������� ��������";
	Descriptions.transformations.insert({ 0x0003, Card });

	Card.title = "������������";
	Card.body = "������������\n";
	Descriptions.transformations.insert({ OP_TRANSPOSITION, Card });

	Card.title = "������� ����� �����";
	Card.body = "������� ����� �����\n";
	Descriptions.transformations.insert({ OP_CHANGE_EQUAL, Card });

	Card.title = "�����������";
	Card.body = "�����������\n";
	Descriptions.transformations.insert({ OP_SUBSTITUTION, Card });

	Card.title = "�����";
	Card.body = "�����\n";
	Descriptions.transformations.insert({ OP_CONCLUSION, Card });


	Card.title = "���������";
	Card.body = "���������\n�������� ��� ���������������� �������";
	Descriptions.transformations.insert({ 0x0004, Card });

	Card.title = "����";
	Card.body = "����\n";
	Descriptions.transformations.insert({ OP_GIVEN, Card });

	Card.title = "�����";
	Card.body = "�����\n";
	Descriptions.transformations.insert({ OP_TOFIND, Card });

	Card.title = "�����";
	Card.body = "�����\n";
	Descriptions.transformations.insert({ OP_ANSWER, Card });

	return true;
}

bool LoadErrorDescription()
{
	Descriptions.errors.insert({ ERR_NO_ERROR, "������ ���!" });
	Descriptions.errors.insert({ ERR_NO_TESTED, "�� ���������!" });
	Descriptions.errors.insert({ ERR_SET_IN_NUMER_AREA, "��������� � ������� �����!" });
	Descriptions.errors.insert({ ERR_NUMER_IN_SET_AREA, "����� � ������� ��������!" });
	Descriptions.errors.insert({ ERR_INCORRECT_BRACKET_POS, "������������ ����������� ������!" });
	Descriptions.errors.insert({ ERR_INCORRECT_NUMBER, "������������ ������ �����!" });
	Descriptions.errors.insert({ ERR_INCORRECT_SET, "������������ ������ ���������!" });
	Descriptions.errors.insert({ ERR_INVALID_LEFT_OPERAND, "������������ ����� �������!" });
	Descriptions.errors.insert({ ERR_INVALID_RIGHT_OPERAND, "������������ ������ �������!" });

	Descriptions.errors.insert({ ERR_EXPRESSION_NOT_EQUALITY, "��������� �� �������� ����������!" });
	Descriptions.errors.insert({ ERR_EXPRESSIONS_NOT_EQUAL, "��������� �� �����!" });
	Descriptions.errors.insert({ ERR_OPERATION_ERROR, "�������������� ������� �����������!" });
	Descriptions.errors.insert({ ERR_FOUNT_INVALID_SET, "����������� ���������!" });

	return true;
}

int FindNextTask()
{
	int c_max = 0;
	int p_max = 0;
	for (size_t i = 0; i < MainUserInfo.CurrentTasks.size(); i++)
		if (MainUserInfo.CurrentTasks[i].ID > c_max) c_max = MainUserInfo.CurrentTasks[i].ID;

	for (size_t i = 0; i < MainUserInfo.CompletedTasks.size(); i++)
		if (MainUserInfo.CompletedTasks[i] > p_max) p_max = MainUserInfo.CompletedTasks[i];

	return max(c_max, p_max) + 1;
}

bool ComplNullset(const term_ptr& pTerm_1, const term_ptr& pTerm_2)
{
	// ������: ������������� ��������
	// �����: ���������� � ������� ���������
	// ----------------------------------------
	// �������� �� ����
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return false;
	// ----------------------------------------
	// ���������� ������
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(UNISET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(UNISET, term_type::symbol)) return false;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// ��������� ����� �������
	// ----------------------------------------
	if (!left->is('{', term_type::operation, 1)) return false;
	if (!left->get(0)->is(NULLSET, term_type::symbol)) return false;
	return true;
}

void MainBridge(CEdit* edit)
{
	size_t system_size;
	int error = ERR_NO_TESTED;

	vector<Vec> sets = CreateSets(MainUserInfo.GetCurrentTask()->SetNames, &system_size);

	CTable table;
	table.CreateTable(edit->m_buffer, edit->codes, edit->segments);
	table.SetConnections();

	vector<string> tempPartsOfText;
	vector<vector<int>> tempPartsOfCodes;

	vector<string> partsOfText;
	vector<vector<int>> partsOfCodes;

	term_ptr left_term, right_term;

	//SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_TYPE, TYPE_WAIT);
	//ShowWindow(WND_Result, SW_SHOW);

	for (size_t i = 0; i < table.Lines.size(); i++)
	{
		for (size_t j = 0; j < table.Lines[i].segments.size(); j++)
		{
			table.Lines[i].segments[j].error = ERR_NO_TESTED;

			left_term = TermCoder(table.Lines[i].segments[j].left, &error);
			right_term = TermCoder(table.Lines[i].segments[j].right, &error);

			/*if (IsSerialConversion(table.Lines[i].segments[j].operation)) {
				// ����� �������� ����� ����� �� ����������� ��������
				Vec left_vec = TermCalculate(left_term, &sets, system_size, &error);
				// ������� �������� ������ �����
				error = ERR_NO_ERROR;
				Vec right_vec = TermCalculate(right_term, &sets, system_size, &error);
				if (error != ERR_NO_ERROR) {
					table.Lines[i].segments[j].error = error;
					continue;
				}
				// ���������� ������ � ����� �����
				if (left_vec != right_vec) {
					table.Lines[i].segments[j].error = ERR_EXPRESSIONS_NOT_EQUAL;
					continue;
				}
			}*/

			if (Distributor(&table, i, j)) {
				table.Lines[i].segments[j].error = ERR_NO_ERROR;
				continue;
			}
			else {
				table.Lines[i].segments[j].error = ERR_OPERATION_ERROR;
				continue;
			}
		}
	}

	size_t e = 0;
	for (size_t l = 0; l < table.Lines.size(); l++)
		for (size_t s = 0; s < table.Lines[l].segments.size(); s++, e++) 
			edit->segments[e].error = table.Lines[l].segments[s].error;

	// ������
	int errors = 0;
	int warnings = 0;
	bool hasANSWER = false;
	// ���������� ������
	/*for (size_t i = 0; i < MainStorage.error.size(); i++)
	{
		if (MainStorage.operation[i] == OP_ANSWER) hasANSWER = true;
		if (MainStorage.error[i] == ERR_OPERATION_ERROR) warnings++;
		else if (MainStorage.error[i] > ERR_NO_ERRORS) errors++;
	}*/
	//SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_TASK, MainUserInfo.GetCurrentTask().ID);
	//if (errors != 0 || warnings != 0 || hasANSWER == false)
	//{
	//	if (MainUserInfo.GetCurrentTask()->Attempts < 3)
	//	{
	//		SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_TYPE, TYPE_ERROR);
	//		MainUserInfo.GetCurrentTask()->Attempts++;
	//	}
	//	else
	//	{
	//		MainUserInfo.GetCurrentTask()->Attempts = 0;
	//		//MainUserInfo->current.push_back(FindNextTask());
	//		//pUI->trys.push_back(0);
	//		SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_TYPE, TYPE_ERROR_AND_NEW);

	//	}
	//	SendMessage(WND_Task, WM_UPDATE, 0, 0);

	//	SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_ERROR_AMOUNT, errors);
	//	SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_WARNING_AMOUNT, warnings);
	//	//SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_NEWTASK, pUI->current.back());
	//	SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_ANSWER, hasANSWER);
	//	InvalidateRect(WND_Result, 0, true);
	//}
	//else
	//{
	//	SendMessage(WND_Result, WM_SETDATA, PROP_RESULT_TYPE, TYPE_PERFORMED);
	//	InvalidateRect(WND_Result, 0, true);

	//	/*pUI->perform.push_back(pUI->current[pUI->curr]);
	//	pUI->current.erase(pUI->current.begin() + pUI->curr);
	//	pUI->trys.erase(pUI->trys.begin() + pUI->curr);
	//	if (pUI->current.size() == 0) pUI->curr = -1;
	//	else pUI->curr = 0;
	//	SendMessage(GetParent(WND_Result), WM_SETDATA, PROP_TASK_UPDATE, 0);
	//	SendMessage(WND_Task, WM_UPDATE, 0, 0);*/
	//}
	//mysql_SaveUserInfo(pUI);
}

void SplitIntoSegments(const string& text, vector<string>* partsOfText)
{
	int segment = 0;
	partsOfText->clear();
	partsOfText->push_back(string());

	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i] == '=') {

			segment++;
			partsOfText->push_back(string());
			continue;
		}
		partsOfText->at(segment) += text[i];
	}
}

void SplitIntoSegments(const string& text, const vector<int>& codes, vector<string>* partsOfText, vector<vector<int>>* partsOfCodes)
{
	int segment = 0;
	partsOfText->push_back(string());
	partsOfCodes->push_back(vector<int>());

	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i] == '=') {

			segment++;
			partsOfText->push_back(string());
			partsOfCodes->push_back(vector<int>());
			continue;
		}
		partsOfText->at(segment) += text[i];
		partsOfCodes->at(segment).push_back(codes[i]);
	}
}

string OperationText(int id)
{
	switch (id)
	{
		case OP_COMPL_NULLSET:
			return "{!}";
		case OP_COMPL_UNISET_R1:
			return "{@}";
		case OP_COMPL_UNISET_R2:
			return "|{@}|";
		case OP_COMPL_UNISET_R3:
			return "+|{@}|";
		case OP_COMPL_UNISET_R4:
			return "-|{@}|";
		case OP_COMPL_UNISET_R5:
			return "+-|{@}|";
		case OP_COMPL_A:
			return "{A#B}";
		case OP_IN_PARANTHESES:
			return "#(..)";
		case OP_UN_PARANTHESES:
			return "$(..)";
		case OP_IN_NULLSET_R1:
			return "#!";
		case OP_IN_NULLSET_R2:
			return "|#!|";
		case OP_IN_NULLSET_R3:
			return "+|#!|";
		case OP_IN_NULLSET_R4:
			return "-|#!|";
		case OP_IN_NULLSET_R5:
			return "+-|#!|";
		case OP_UN_NULLSET:
			return "$!";
		case OP_IN_UNISET:
			return "#@";
		case OP_UN_UNISET:
			return "$@";
		case OP_DOUBLE_COMPL:
			return "{{A}}";
		case OP_IN_A_AND_A:
			return "A#A";
		case OP_UN_A_AND_A:
			return "A$A";
		case OP_IN_A_COMPL_A_R1:
			return "A#{A}";
		case OP_IN_A_COMPL_A_R2:
			return "|A#{A}|";
		case OP_IN_A_COMPL_A_R3:
			return "+|A#{A}|";
		case OP_IN_A_COMPL_A_R4:
			return "-|A#{A}|";
		case OP_IN_A_COMPL_A_R5:
			return "+-|A#{A}|";
		case OP_UN_A_COMPL_A:
			return "A${A}";
		case OP_UNISET_MINUS_A:
			return "@^A";
		case OP_SET_MINUS:
			return "A^B";
		case OP_INC_EXC_X2:
			return "|$|x2";
		case OP_INC_EXC_X3:
			return "|$|x3";
		case OP_CARDIALITY_R1:
			return "|!|";
		case OP_CARDIALITY_R3:
			return "+|!|";
		case OP_CARDIALITY_R4:
			return "-|!|";
		case OP_CARDIALITY_R5:
			return "+-|!|";

		case OP_NEG_A:
			return "-(A+B)";
		case OP_MP_PARANTHESES:
			return "*(..)";
		case OP_DOUBLE_NEG:
			return "--A";
		case OP_ADD_A_NEG_A_R1:
			return "A-A";
		case OP_ADD_A_NEG_A_R3:
			return "+(A-A)";
		case OP_ADD_A_NEG_A_R4:
			return "-(A-A)";
		case OP_ADD_A_NEG_A_R5:
			return "+-(A-A)";
		case OP_CARD_COMPL_A:
			return "|{A}|";
		case OP_CARD_UNISET_MINUS:
			return "|@^A|";
		case OP_INDENTICAL_A:
			return "A+..+A";
		case MULTIPLY_BY_0_R1:
			return "*0";
		case MULTIPLY_BY_0_R3:
			return "+*0";
		case MULTIPLY_BY_0_R4:
			return "-*0";
		case MULTIPLY_BY_0_R5:
			return "+-*0";
		case OP_ADD_ZERO:
			return "+0";

		case OP_COMP:
			return "Comp";
		case OP_CHANGE_EQUAL:
			return "A\241-A";
		case OP_SUBSTITUTION:
			return "=\242=`";
		case OP_CONCLUSION:
			return "A\243A`";

		case OP_GIVEN:
			return "Given";
		case OP_TOFIND:
			return "ToFind";
		case OP_ANSWER:
			return "Answer";
	}
	return "?";
}

bool IsSerialConversion(int operationID)
{
	switch (operationID)
	{
		case OP_GIVEN: case OP_TOFIND: case OP_ANSWER: return false;
	}
	return true;
}

