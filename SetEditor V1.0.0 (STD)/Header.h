#pragma once
#pragma comment(lib, "Msimg32.lib")

#include <Windows.h>
#include <CommCtrl.h>
//#include <windowsx.h>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
//#include <mysql.h>
using namespace std;
#include "DATLib.h"
#include "TermTool.h"

#include "Config.h"
#include "UserInfo.h"
#include "CEdit.h"
#include "CTable.h"

#define FONT_NAME "SetEditorBDFontNew.ttf"

#define ERR_CAPTION "Ошибка"

#define APP_CAPTION "SetEditor V3.1.0"

#define GetMarkersCode(i) (i & 0x0000FFFF)
#define GetErrorsCode(i) (i & 0xFFFF0000)

#define		ERR_SQL_NO_ERROR					0x0000

#define		ERR_SQL_NO_CONNECTION				0x0001
#define		ERR_SQL_DATABASE_NOT_AVAILABLE		0x0002

#define		ERR_SQL_QUERY_NOT_FOUND				0x0003
#define		ERR_SQL_QUERY_INVALID_VALUE			0x0004

#define		LIGHT_MARKERS		5
#define		DARK_MARKERS		5

#define CFG_LINE_HEIGHT 25

typedef unsigned char byte_t;
typedef unsigned char err;

struct RANGE
{
	int start = 0;
	int end = 0;
};

// ==================== Declaration ==================== //

#define WM_GETDATA 0x0400 // Получает основную структуру окна
/*
	wParam - не используется
	lParam - не используется
	return: указатель на структуру
*/

#define WM_BTNPRESS 0x0410 // Нажата кнопка 
/*
	wParam - дескриптор кнопки
	lParam - дополнительное значение
	return: 0
*/

#define WM_BTNSTATE 0x0402 // Статус кнопки
/*
	wParam - статус кнопки
	lParam - 0, чтобы получить статус; 1, чтобы установить статус
	return: статус
*/

//#define WM_ENABLE 0x0403 // Состояние элемента
/*
	wParam - состояние элемента
	lParam - 0, чтобы получить состояние; 1, чтобы установить состояние
	return: состояние
*/

#define WM_SETDATA 0x0405 // Устанавливает значение свойства в основную структуру окна
/*
	wParam - свойство
	lParam - значение
	return: 0 - ошибка; 1 - успешно
*/

#define WM_UPDATETASK	0x0409

#define WM_WIN_TYPE 0x0406

#define WM_UPDATE 0x0407

#define WM_REMOVE 0x0408

#define PROP_USERINFO 100

#define PROP_SELECTOR_LINE 1
#define PROP_SELECTOR_SELECTED 2
#define PROP_SELECTOR_DEPENDENT_LINE 3

#define PROP_SCROLL_LINESIZE 1 // Размер полного текста
#define PROP_SCROLL_LINES 2 // Размер видимого текста
#define PROP_SCROLL_TYPE 3 // Тип скрола (горизонатльный/вертикальный)
#define PROP_SCROLL_PARENT 4 // Зависимое окно
#define PROP_SCROLL_OFFSET 5
#define PROP_SCROLL_PAGESIZE 6

#define PROP_TIP_TEXT 1

#define PROP_STATE_USERINFO 1 // Указатель на UserInfo

#define PROP_TASK_UPDATE 1 // Команда обносить задание

#define PROP_BUTTON_TEXT 1
#define PROP_BUTTON_IMAGE 2
#define PROP_BUTTON_TCOLOR 3
#define PROP_BUTTON_BCOLOR 4
#define PROP_BUTTON_FCOLOR 5
#define PROP_BUTTON_PCOLOR 6
#define PROP_BUTTON_ENABLE 7

#define PROP_SBUTTON_TEXT 1
#define PROP_SBUTTON_TCOLOR 2
#define PROP_SBUTTON_BCOLOR 3
#define PROP_SBUTTON_FBCOLOR 4
#define PROP_SBUTTON_FFCOLOR 5
#define PROP_SBUTTON_FPCOLOR 6
#define PROP_SBUTTON_STATE 7
#define PROP_SBUTTON_TRYS 8
#define PROP_SBUTTON_TIP 9

#define SCROLL_TYPE_VERTICAL 0 // Вертикальный тип скролла
#define SCROLL_TYPE_HORIZONTAL 1 // Горизонтальный тип скролла

#define PROP_RESULT_USERINFO 0
#define PROP_RESULT_TYPE 1
#define PROP_RESULT_ERROR_AMOUNT 2
#define PROP_RESULT_WARNING_AMOUNT 3
#define PROP_RESULT_ANSWER 4
#define PROP_RESULT_NEWTASK 5
#define PROP_RESULT_TASK 6

#define PROP_KEYPAD_PARENT 1

#define TYPE_WAIT 0
#define TYPE_ERROR 1
#define TYPE_ERROR_AND_NEW 2
#define TYPE_PERFORMED 3

#define WM_SETTYPE WM_USER + 1
#define WM_SETLINE WM_USER + 2
#define WM_SETSIZE WM_USER + 3
#define WM_PRESSED WM_USER + 4
#define WM_ADDROW WM_USER + 5
#define WM_SHOWTIP WM_USER + 6

#define WM_SCROLLV WM_USER + 7
#define WM_SCROLLH WM_USER + 8

#define WM_SELECT WM_USER + 11

#define WM_TAUPDATE WM_USER + 12

#define ERR_SYNTAX_NOTEQUATION 1
#define ERR_ARITHMETIC_NOTEQUAL 2

#define MARKER_SOURCE 0x1
#define MARKER_RESULT 0x2

// ========== DataBase ========== //

int mysql_LoadUserInfo(string login, string pass, UserInfo* ui);
int mysql_SaveUserInfo(UserInfo* ui) noexcept(false);
int mysql_LoadTaskInfo(int id, UserInfo* ui) noexcept(false);

// ========== Processor ========== //
int GetPriority(char a);

// ======================================== |Prototypes| ======================================== //


// Заполняет набор векторов уникальными значениями
//vector<Vec> CreateSets(vector<char> sets, size_t* system_size);
// Преобразует строковое выражение в выражение из векторов
//vector<Vec> Coder(string str, vector<Vec>* named_vectors, size_t system_size) noexcept(false);
// Вычисляет значение векторного выражения
//Vec Calculate(vector<Vec> str, size_t system_size) noexcept(false);
// Проверяет сообветствие операции
int Distributor(CTable* table, int lineID, int segmentID);

#define OP_COMPL_NULLSET 		0x1011		// > Дополнение пустого множества
#define	OP_COMPL_UNISET_R1		0x1021		// > Дополнение универсального множества
#define	OP_COMPL_UNISET_R2			0x2021		
#define	OP_COMPL_UNISET_R3			0x3021		
#define	OP_COMPL_UNISET_R4			0x4021		
#define	OP_COMPL_UNISET_R5			0x5021			
#define	OP_COMPL_A				0x1031		// > Дополнение
#define	OP_IN_PARANTHESES 		0x1041		// > Раскрытие скобок пересечения
#define	OP_UN_PARANTHESES 		0x1051		// > Раскрытие скобок объединения
#define	OP_IN_NULLSET_R1		0x1061		// > Пересечение с пустым множеством
#define	OP_IN_NULLSET_R2			0x2061		
#define	OP_IN_NULLSET_R3			0x3061		
#define	OP_IN_NULLSET_R4			0x4061		
#define	OP_IN_NULLSET_R5			0x5061		
#define	OP_UN_NULLSET			0x1071		// > Объединение с пустым множеством
#define	OP_IN_UNISET			0x1081		// > Пересечение с универсальным множеством
#define	OP_UN_UNISET			0x1091		// > Объединение с универсальным множеством
#define	OP_DOUBLE_COMPL 		0x10A1		// > Двойное дополнение
#define	OP_IN_A_AND_A			0x10B1		// > Пересечение одинаковых множеств
#define	OP_UN_A_AND_A			0x10C1		// > Объединение одинаковых множеств
#define	OP_IN_A_COMPL_A_R1 		0x10D1		// > Пересечение множества и его дополнения
#define	OP_IN_A_COMPL_A_R2 			0x20D1	
#define	OP_IN_A_COMPL_A_R3 			0x30D1	
#define	OP_IN_A_COMPL_A_R4 			0x40D1	
#define	OP_IN_A_COMPL_A_R5 			0x50D1	
#define	OP_UN_A_COMPL_A 		0x10E1		// > Объединение множества и его дополнения
#define	OP_UNISET_MINUS_A		0x10F1		// > Разность универсума и множества
#define	OP_SET_MINUS			0x1101		// > Разность множеств
#define	OP_INC_EXC_X2			0x1111		// > Формула включения / исключения x2
#define	OP_INC_EXC_X3			0x1121		// > Формула включения / исключения x3
#define OP_CARDIALITY_R1		0x1131		// > Мощность пустого множества
#define OP_CARDIALITY_R3			0x2131	
#define OP_CARDIALITY_R4			0x3131		
#define OP_CARDIALITY_R5			0x4131

#define	OP_NEG_A				0x1012		// > Отрицание
#define	OP_MP_PARANTHESES 		0x1022		// > Раскрытие скобок умножения
#define	OP_DOUBLE_NEG			0x1032		// > Двойное отрицание
#define	OP_ADD_A_NEG_A_R1		0x1042		// > Сумма количесва элементов и его отрицания
#define	OP_ADD_A_NEG_A_R3			0x2042		
#define	OP_ADD_A_NEG_A_R4			0x3042		
#define	OP_ADD_A_NEG_A_R5			0x4042		
#define OP_CARD_COMPL_A			0x1052		// > Мощность дополнения множества
#define OP_CARD_UNISET_MINUS	0x1062		// > Мощность разности универсума и множества
#define OP_INDENTICAL_A			0x1072		// > Приведение подобных членов
#define	MULTIPLY_BY_0_R1			0x1082		// > Умножение на 0
#define	MULTIPLY_BY_0_R3			0x2082		
#define	MULTIPLY_BY_0_R4			0x3082		
#define	MULTIPLY_BY_0_R5			0x4082	
#define	MULTIPLY_BY_1			0x1092	
#define	MULTIPLY_BY_MINUS		0x10A2	
#define	DIVIDE_BY_1				0x10B2	
#define OP_ADD_ZERO				0x10C2		// > Сложение с 0

#define	OP_COMP 				0x1013		// > Выполнение операций с числами
#define OP_TRANSPOSITION		0x1023
#define	OP_CHANGE_EQUAL			0x1033		// > Перенос через равно
#define	OP_SUBSTITUTION 		0x1043		// > Подстановка
#define	OP_CONCLUSION			0x1053		// > Вывод

#define	OP_GIVEN 				0x1014		// > Дано
#define	OP_TOFIND				0x1024		// > Найти
#define	OP_ANSWER				0x1034		// > Ответ



// ======================================== |ERRORS| ======================================== //
#define	ERR_EXPRESSION_NOT_EQUALITY		0x110000
#define	ERR_EXPRESSIONS_NOT_EQUAL		0x120000
#define	ERR_OPERATION_ERROR				0x130000
#define	ERR_FOUNT_INVALID_SET           0x140000

#define ERR_NO_ERROR					0x0A0000
#define ERR_NO_TESTED					0x000000

#define ERR_SET_IN_NUMER_AREA			0x060000
#define ERR_NUMER_IN_SET_AREA			0x070000

#define ERR_NOT_FOUND_OPEN_BARKET		0x030000
#define ERR_NOT_FOUND_CLOSE_BARKET		0x040000
#define ERR_INCORRECT_BRACKET_POS		0x050000

#define ERR_INCORRECT_NUMBER			0x080000
#define ERR_INCORRECT_SET				0x090000

#define ERR_INVALID_LEFT_OPERAND		0x010000
#define ERR_INVALID_RIGHT_OPERAND		0x020000

bool IsSerialConversion(int operationID);

int CheckExpression(const string& expression);

void SplitIntoSegments(const string& text, vector<string>* partsOfText);
void SplitIntoSegments(const string& text, const vector<int>& codes, vector<string>* partsOfText, vector<vector<int>>* partsOfCodes);

void NormalForm(term_ptr pTerm);

Vec TermCalculate(term_ptr pTerm, vector<Vec>* nVecs, int system_size, int* error);

struct TIP
{
	HWND hWnd = nullptr;
	string text;
	COLORREF bkgColor = RGB(255, 255, 255);
	COLORREF txtColor = RGB(0, 0, 0);
	POINT pos;
};

#define		LMARKER_1				0x0001
#define		LMARKER_2				0x0002
#define		LMARKER_3				0x0004
#define		LMARKER_4				0x0008
#define		LMARKER_5				0x0010

#define		DMARKER_1				0x0100
#define		DMARKER_2				0x0200
#define		DMARKER_3				0x0400
#define		DMARKER_4				0x0800
#define		DMARKER_5				0x1000

#define		WM_SETMARKER			0x0410

#define		WM_CHECK				0x0411