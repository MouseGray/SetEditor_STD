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
	HWND hWnd = nullptr;							// ID окна, устанавливается автоматически, не изменять.
	HWND tipHWnd = nullptr;							// Окно, отображающие подсказку.

	bool type = BUTTON_TYPE_TEXT;					// Тип кнопки: отображает текст или картинку, не изменять.
	bool tipType = TIP_TYPE_HIDE;					// Тип отображения подсказки.

	string text;									// Текст кнопки.
	string tipText;									// Текст подсказки.

	HBITMAP bm = nullptr;							// Указатель на картинку.

	COLORREF tColor = STD_BUTTON_TEXT_COLOR;		// Цвет текста.
	COLORREF bColor = STD_BUTTON_BK_COLOR;			// Цвет фона.
	COLORREF fColor = STD_BUTTON_FOCUS_COLOR;		// Цвет фокуса.
	COLORREF pColor = STD_BUTTON_PRESS_COLOR;		// Цвет нажатия.
	COLORREF dColor = STD_BUTTON_DISABLE_COLOR;		// Цвет отключения.

	bool focused = false;							// Не изменять.
	bool pressed = false;							// Не изменять.
	bool tip = false;								// Не изменять.

	bool enable = true;								// Состояние кнопки: включена/отключена.
};

struct SBUTTON
{
	HWND hWnd = nullptr;							// ID окна, устанавливается автоматически, не изменять.
	HWND tipHWnd = nullptr;							// Окно, отображающие подсказку.

	bool type = BUTTON_TYPE_TEXT;					// Тип кнопки: отображает текст или картинку, не изменять.
	bool tipType = TIP_TYPE_HIDE;					// Тип отображения подсказки.

	string text;									// Текст кнопки.
	string tipText;									// Текст подсказки.

	HBITMAP bm = nullptr;							// Указатель на картинку.
	
	COLORREF tColor = STD_BUTTON_TEXT_COLOR;		// Цвет текста.
	COLORREF bColor = STD_BUTTON_BK_COLOR;			// Цвет фона.
	COLORREF fbColor = STD_SBUTTON_BK_COLOR;		// Цвет рамки.
	COLORREF ffColor = STD_SBUTTON_FOCUS_COLOR;		// Цвет рамки при фокуса.
	COLORREF fpColor = STD_SBUTTON_PRESS_COLOR;		// Цвет рамки при нажатии.

	bool focused = false;							// Не изменять.
	bool selected = false;							// Не изменять.
	bool tip = false;								// Не изменять.

	bool enable = true;								// Состояние кнопки: включена/отключена.
};

#endif