#pragma once

#include <Windows.h>

#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <fstream>

#include "CheckSyntax.h"
#include "LineNum.h"
#include "resource.h"
using namespace std;

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

#define		MAX_UNDO				20

#define		ACTION_DEFAULT			0
#define		ACTION_ADD				1
#define		ACTION_REMOVE			2

#define		GetMarkerGroup(code)	(code & 0xC0)
#define		GetError(code)	(code & 0xFFFF0000)

struct Segment
{
	int error = 0;
	int operation = 0;
	vector<int> pointers;
};

class CAction
{
public:
	void CreateAdd(size_t caretPos, size_t selectPos, size_t segmentPos, size_t count, size_t segmentLength)
	{
		this->actionID = ACTION_ADD;
		this->caretPos = caretPos;
		this->selectPos = selectPos;
		this->segmentPos = segmentPos;
		this->count = count;
		this->segmentLength = segmentLength;
	}
	void CreateRemove(size_t caretPos, size_t selectPos, size_t segmentPos, const string& text, const vector<int>& codes, const vector<Segment>& segments)
	{
		this->actionID = ACTION_REMOVE;
		this->caretPos = caretPos;
		this->selectPos = selectPos;
		this->segmentPos = segmentPos;
		this->text = text;
		this->codes = codes;
		this->segments = segments;
	}
	void Release(string* text, vector<int>* codes, size_t* caretPos, size_t* selectPos, vector<Segment>* segments)
	{
		switch (actionID)
		{
			case ACTION_ADD:
				ReleaseAdd(text, codes, segments);
				break;
			case ACTION_REMOVE:
				ReleaseRemove(text, codes, segments);
				break;
			default:

				break;
		}
		*caretPos = this->caretPos;
		*selectPos = this->selectPos;
	}
private:
	void ReleaseAdd(string* text, vector<int>* codes, vector<Segment>* segments)
	{
		text->erase(text->begin() + caretPos, text->begin() + caretPos + count);
		codes->erase(codes->begin() + caretPos, codes->begin() + caretPos + count);
		if (segmentLength) segments->erase(segments->begin() + segmentPos, segments->begin() + segmentPos + segmentLength);
	}
	void ReleaseRemove(string* text, vector<int>* codes, vector<Segment>* segments)
	{
		text->insert(text->begin() + min(caretPos, selectPos), this->text.begin(), this->text.end());
		codes->insert(codes->begin() + min(caretPos, selectPos), this->codes.begin(), this->codes.end());
		if(!this->segments.empty()) segments->insert(segments->begin() + segmentPos, this->segments.begin(), this->segments.end());
	}

	__int8 actionID = ACTION_DEFAULT;

	size_t caretPos = 0;
	size_t selectPos = 0;

	size_t count = 0;
	string text;
	vector<int> codes;

	size_t segmentPos = 0;
	size_t segmentLength = 0;
	vector<Segment> segments;
};

class CEdit
{
public:
	// Команды окна
	void W_Create(HWND hWnd);
	void W_Size(int w, int h);
	void W_SetFont(HFONT font);
	void W_LButtonDown(int x, int y, bool shift);
	void W_RButtonDown(int x, int y);
	void W_MouseMove(int x, int y, bool shift);
	void W_MouseWheel(int delta);
	void W_Backspace();
	void W_Delete();
	void W_Char(char c);
	void W_Left(bool shift);
	void W_Right(bool shift);
	void W_Up(bool shift);
	void W_Down(bool shift);
	void W_Home(bool shift);
	void W_End(bool shift);
	void W_Prior(bool shift);
	void W_Next(bool shift);
	void W_Paint();
	void W_Scroll(int act, int pos);
	// Команды буфера
	void Erase(size_t off, size_t count);
	void _Erase(size_t off, size_t count);
	int Insert(size_t off, const string& text, const vector<int>& codes);
	void _Insert(size_t off, const string& text, const vector<int>& codes);
	void AddUndo();
	bool EraseSelect();
	// Команды управления
	bool ToLeft();
	bool ToRight();
	bool ToUp();
	bool ToDown();
	bool ToHome();
	bool ToEnd();
	bool ToPrior();
	bool ToNext();

	void ToBack();
	void Cut();
	void Copy();
	void Paste();
	void SetOverline();
	void Mark(int id);
	bool Check();

	void SetCaretPosition(bool off = true);
	int GetCaretHeight();
	void UpdateScroll();
	void UpdateOverline();
	void UpdateMarkers();

	void PrepareText(string* text, vector<int>* codes);
	int GetRemoveTextOverline(const string& text);
	int GetTextOverline(int start, int end);
	bool IsCorrectChar(char c);

	int GetLeftSelectU(int pos_U);
	int GetRightSelectU(int pos_U);

	inline void SetMarker(int id, int* code);

	void RemoveLeftMarker(int id, int pos_U);
	void RemoveRightMarker(int id, int pos_U);
	// Команды преобразования
	int GetCharWidthU(char c);
	bool GetUFromP(int* pos_U, const int posX_P, const int posY_P);
	void GetPFromU(int* posX_P, int* posY_P, const int pos_U);
	// Команды отрисовки
	void Update();
	void DrawOverline(int pos_U, int posX_P, int posY_P, bool isSelect);
	int GetOverlineHeight(int pos_U);

	int GetLinePos(int pos_U);
	int GetSegmentPos(int pos_U);

	void Restart();
	string Open();
	string SaveAs();
	void Load(string fileName);
	void Save(string fileName);

	string text;
	vector<int> codes;
	vector<Segment> segments;
	void (*SegmentEditCallback)(Segment*, int, int) = nullptr;
	void (*ShowTipCallback)(HWND, int, int, int, Segment*) = nullptr;
private:
	// Поля буфера
	deque<stack<CAction>> undo;

	string bufferText;
	// Поля управления
	size_t caretPos_U = 0;
	size_t selectPos_U = 0;

	int startSelectPos = 0;
	int endSelectPos = 0;
	// Поля размера
	int charWidth_P = 0;
	int charHeight_P = 0;

	int charOffset_P = 0;
	int lineHeight_P = 1;

	int pageOffset_U = 0;

	int textOffset_P = 50;

	int textWidth_P = 0;

	int pageWidth_P = 0;
	int pageHeight_P = 0;
	// Поля окна
	HWND hWnd = nullptr;

	HDC hDC = nullptr;
	HBITMAP hBM = nullptr;

	HFONT font = nullptr;
};