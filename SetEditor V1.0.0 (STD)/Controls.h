#pragma once
#ifndef _CONTROLS_
#define _CONTROLS_
// ======================================== |Includes| ======================================== //
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;
// ======================================== |Defines| ======================================== //

#define		STD_BUTTON_TEXT_COLOR					RGB(255, 255, 255)
#define		STD_BUTTON_BK_COLOR						RGB(0, 0, 0)
#define		STD_BUTTON_FOCUS_COLOR					RGB(20, 20, 20)
#define		STD_BUTTON_PRESS_COLOR					RGB(255, 255, 255)
#define		STD_BUTTON_DISABLE_COLOR				RGB(211, 211, 211)

#define		STD_SBUTTON_BK_COLOR					RGB(211, 211, 211)
#define		STD_SBUTTON_FOCUS_COLOR					RGB(0, 211, 0)
#define		STD_SBUTTON_PRESS_COLOR					RGB(211, 0, 0)

#define		BUTTON_TYPE_TEXT						false
#define		BUTTON_TYPE_IMAGE						true

#define		TIP_TYPE_SHOW							true
#define		TIP_TYPE_HIDE							false

// ======================================== |Structures| ======================================== //
struct BUTTON
{
	HWND hWnd = nullptr;							// ID ����, ��������������� �������������, �� ��������.
	HWND tipHWnd = nullptr;							// ����, ������������ ���������.

	bool type = BUTTON_TYPE_TEXT;					// ��� ������: ���������� ����� ��� ��������, �� ��������.
	bool tipType = TIP_TYPE_HIDE;					// ��� ����������� ���������.

	string text;									// ����� ������.
	string tipText;									// ����� ���������.

	HBITMAP bm = nullptr;							// ��������� �� ��������.

	COLORREF tColor = STD_BUTTON_TEXT_COLOR;		// ���� ������.
	COLORREF bColor = STD_BUTTON_BK_COLOR;			// ���� ����.
	COLORREF fColor = STD_BUTTON_FOCUS_COLOR;		// ���� ������.
	COLORREF pColor = STD_BUTTON_PRESS_COLOR;		// ���� �������.
	COLORREF dColor = STD_BUTTON_DISABLE_COLOR;		// ���� ����������.

	bool focused = false;							// �� ��������.
	bool pressed = false;							// �� ��������.
	bool tip = false;								// �� ��������.

	bool enable = true;								// ��������� ������: ��������/���������.
};

struct SBUTTON
{
	HWND hWnd = nullptr;							// ID ����, ��������������� �������������, �� ��������.
	HWND tipHWnd = nullptr;							// ����, ������������ ���������.

	bool type = BUTTON_TYPE_TEXT;					// ��� ������: ���������� ����� ��� ��������, �� ��������.
	bool tipType = TIP_TYPE_HIDE;					// ��� ����������� ���������.

	string text;									// ����� ������.
	string tipText;									// ����� ���������.

	HBITMAP bm = nullptr;							// ��������� �� ��������.
	
	COLORREF tColor = STD_BUTTON_TEXT_COLOR;		// ���� ������.
	COLORREF bColor = STD_BUTTON_BK_COLOR;			// ���� ����.
	COLORREF fbColor = STD_SBUTTON_BK_COLOR;		// ���� �����.
	COLORREF ffColor = STD_SBUTTON_FOCUS_COLOR;		// ���� ����� ��� ������.
	COLORREF fpColor = STD_SBUTTON_PRESS_COLOR;		// ���� ����� ��� �������.

	bool focused = false;							// �� ��������.
	bool selected = false;							// �� ��������.
	bool tip = false;								// �� ��������.

	bool enable = true;								// ��������� ������: ��������/���������.
};

#endif