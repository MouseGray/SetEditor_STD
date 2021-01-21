#include "CEdit.h"

void CEdit::W_Create(HWND hWnd)
{
	this->hWnd = hWnd;
}

void CEdit::W_Size(int w, int h)
{
	if (hBM) {
		DeleteObject(hBM);
		hBM = nullptr;
	}
	if (hDC) {
		DeleteDC(hDC);
		hDC = nullptr;
	}

	HDC tempHDC = GetDC(hWnd);
	hDC = CreateCompatibleDC(tempHDC);
	hBM = CreateCompatibleBitmap(tempHDC, w, h);
	SelectObject(hDC, hBM);
	ReleaseDC(hWnd, tempHDC);

	pageWidth_P = w;
	pageHeight_P = h;

	textWidth_P = pageWidth_P - textOffset_P;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE;
	si.nPage = 1;
	SetScrollInfo(hWnd, SB_VERT, &si, true);

	UpdateScroll();
	SetCaretPosition();
	Update();
}

void CEdit::W_SetFont(HFONT font)
{
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	SelectObject(hDC, font);
	charWidth_P = tm.tmAveCharWidth;
	charHeight_P = tm.tmHeight;
	charOffset_P = 11;
	lineHeight_P = charOffset_P + charHeight_P;

	this->font = font;
}

void CEdit::W_LButtonDown(int x, int y, bool shift)
{
	GetUFromP((int*)&caretPos_U, x, y);
	if (!shift) selectPos_U = caretPos_U;
	SetCaretPosition();
	Update();
	if (startSelectPos != endSelectPos) {
		RECT rect;
		GetClientRect(hWnd, &rect);
		POINT point = { rect.right >> 1, rect.bottom >> 1 };
		ClientToScreen(hWnd, &point);
		if (this->SegmentEditCallback != nullptr) SegmentEditCallback(&segments[GetSegmentPos(caretPos_U)], point.x, point.y);
	}
}

void CEdit::W_RButtonDown(int x, int y)
{
	HMENU Menu = CreatePopupMenu();
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 5, (LPCSTR)"Вставить");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 4, (LPCSTR)"Копировать");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 3, (LPCSTR)"Вырезать");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 2, (LPCSTR)"Установить номер строки");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 1, (LPCSTR)"Удалить номер строки");
	POINT point = { x, y };
	ClientToScreen(hWnd, &point);
	int result = TrackPopupMenu(Menu, TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON, point.x, point.y, 0, hWnd, nullptr);
	int line = 0;
	int res = 0;
	switch (result)
	{
		case 1: 
			line = GetLinePos(min(caretPos_U, selectPos_U));
			if (line == -1) break;
			codes[line] = 0;
			Update();
			break;
		case 2:
			line = GetLinePos(min(caretPos_U, selectPos_U));
			if (line == -1) break;
			res = DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, LineNumProc, NULL);
			if (res == -1) break;
			codes[line] = res;
			Update();
			break;
		case 3:
			this->Cut();
			break;
		case 4:
			this->Copy();
			break;
		case 5:
			this->Paste();
			break;
		default:
			break;
	}
}

void CEdit::W_MouseMove(int x, int y, bool shift)
{
	int tempCaretPos_U;
	bool fact = GetUFromP(&tempCaretPos_U, x, y);
	if (shift) {
		if (tempCaretPos_U != caretPos_U) {
			caretPos_U = tempCaretPos_U;

			SetCaretPosition();
			Update();
		}
	}
	else {
		if (fact && tempCaretPos_U < text.size())
		{
			if (text[tempCaretPos_U] == '=') {
				startSelectPos = GetLeftSelectU(tempCaretPos_U);
				endSelectPos = GetRightSelectU(tempCaretPos_U);
				POINT point = { x, y };
				ClientToScreen(hWnd, &point);
				ShowTipCallback(hWnd, point.x, point.y, GetError(codes[tempCaretPos_U]), &segments[GetSegmentPos(tempCaretPos_U)]);
				Update();
			}
			else {
				POINT point = { x, y };
				ClientToScreen(hWnd, &point);
				ShowTipCallback(hWnd, point.x, point.y, GetError(codes[tempCaretPos_U]), nullptr);
				if (startSelectPos != endSelectPos) {
					startSelectPos = endSelectPos = 0;
					Update();
				}
			}
		}
		else {
			ShowTipCallback(hWnd, 0, 0, 0, nullptr);
			startSelectPos = endSelectPos = 0;
			Update();
		}
	}
}

void CEdit::W_MouseWheel(int delta)
{
	pageOffset_U += delta;
	if (pageOffset_U < 0) pageOffset_U = 0;
	int curPos_U = 0;

	int curPosX_P = 0;
	int curPosY_U = 0;

	int curCharWidth_P = 0;

	for (curPos_U = 0; curPos_U < text.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			curPosX_P = 0;
			curPosY_U++;
		}

		curPosX_P += curCharWidth_P;
	}
	curPosY_U++;
	int scrollRange = max(0, curPosY_U - (pageHeight_P / lineHeight_P));

	if (pageOffset_U > scrollRange) pageOffset_U = scrollRange;

	SetScrollPos(hWnd, SB_VERT, pageOffset_U, true);

	SetCaretPosition(false);
	Update();
}

void CEdit::W_Backspace()
{
	if (EraseSelect()) {
		SetCaretPosition();
		Update();
		return;
	}

	if (ToLeft()) {
		selectPos_U = caretPos_U;
		Erase(caretPos_U, 1);

		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Delete()
{
	if (EraseSelect()) {
		SetCaretPosition();
		Update();
		return;
	}
	if (caretPos_U < text.size()) {
		Erase(caretPos_U, 1);

		Update();
	}
}

void CEdit::W_Char(char c)
{
	if (!IsCorrectChar(c)) return;
	EraseSelect();
	string character;
	character += toupper(c);
	Insert(caretPos_U, character, vector<int>(1, 0));
	ToRight();
	selectPos_U = caretPos_U;

	SetCaretPosition();
	Update();
}

void CEdit::W_Left(bool shift)
{
	if (caretPos_U != selectPos_U) {
		caretPos_U = selectPos_U = min(caretPos_U, selectPos_U);
		SetCaretPosition();
		Update();
		return;
	}
	if (ToLeft()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Right(bool shift)
{
	if (caretPos_U != selectPos_U) {
		caretPos_U = selectPos_U = max(caretPos_U, selectPos_U);
		SetCaretPosition();
		Update();
		return;
	}
	if (ToRight()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Up(bool shift)
{
	if (ToUp()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Down(bool shift)
{
	if (ToDown()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Home(bool shift)
{
	if (ToHome()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_End(bool shift)
{
	if (ToEnd()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Prior(bool shift)
{
	if (ToPrior()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Next(bool shift)
{
	if (ToNext()) {
		if (!shift) selectPos_U = caretPos_U;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Paint()
{
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rect);

	HideCaret(hWnd);
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
	ShowCaret(hWnd);

	EndPaint(hWnd, &ps);
}

void CEdit::W_Scroll(int act, int pos)
{
	switch (act)
	{
		case SB_THUMBTRACK:
			pageOffset_U = pos;
			SetScrollPos(hWnd, SB_VERT, pos, true);
			SetCaretPosition(false);
			Update();
			break;
		case SB_THUMBPOSITION:
			pageOffset_U = pos;
			SetScrollPos(hWnd, SB_VERT, pos, true);
			SetCaretPosition(false);
			Update();
			break;
		default:
			break;
	}
}

void CEdit::Erase(size_t off, size_t count)
{
	if (text[off] == '}') return;
	AddUndo();
	undo.back().push(CAction());
	int overline = GetRemoveTextOverline(text.substr(off, count));
	_Erase(off, count);
	for (; overline > 0; overline--) {
		_Insert(off, "}", vector<int>(1, 0));
	}
	UpdateMarkers();
	UpdateOverline();
	UpdateScroll();
}

void CEdit::_Erase(size_t off, size_t count)
{
	// Получить позицию сегмента off
	int startPos_S = GetSegmentPos(off);
	// Получить позицию сегмента off + count
	int endPos_S = GetSegmentPos(off + count);
	vector<Segment> tempSegments;
	for (size_t i = startPos_S; i < endPos_S; i++) tempSegments.push_back(segments[i]);

	if (!tempSegments.empty()) this->segments.erase(this->segments.begin() + startPos_S, this->segments.begin() + startPos_S + tempSegments.size());

	undo.back().top().CreateRemove(caretPos_U, selectPos_U, startPos_S, text.substr(off, count), vector<int>(codes.begin() + off, codes.begin() + off + count), tempSegments);
	this->text.erase(this->text.begin() + off, this->text.begin() + off + count);
	this->codes.erase(this->codes.begin() + off, this->codes.begin() + off + count);
}

int CEdit::Insert(size_t off, const string& text, const vector<int>& codes)
{
	AddUndo();
	undo.back().push(CAction());
	string paste = text;
	vector<int> pasteCode = codes;
	PrepareText(&paste, &pasteCode);
	_Insert(off, paste, pasteCode);
	UpdateMarkers();
	UpdateOverline();
	UpdateScroll();
	return paste.size();
}

void CEdit::_Insert(size_t off, const string& text, const vector<int>& codes)
{
	// Получить позицию сегмента off
	int startPos_S = GetSegmentPos(off);
	// Получить позицию сегмента off + count
	int count = 0;
	for (auto& c : text) {
		if (c == '=') count++;
	}

	this->segments.insert(this->segments.begin() + startPos_S, count, Segment());

	undo.back().top().CreateAdd(caretPos_U, selectPos_U, startPos_S, text.size(), count);
	this->text.insert(this->text.begin() + off, text.begin(), text.end());
	this->codes.insert(this->codes.begin() + off, codes.begin(), codes.end());
}

void CEdit::AddUndo()
{
	undo.push_back(stack<CAction>());
	if (undo.size() > MAX_UNDO) undo.pop_front();
}

bool CEdit::EraseSelect()
{
	if (caretPos_U == selectPos_U) return false;
	Erase(min(caretPos_U, selectPos_U), max(caretPos_U, selectPos_U) - min(caretPos_U, selectPos_U));
	caretPos_U = selectPos_U = min(caretPos_U, selectPos_U);
	return true;
}

bool CEdit::ToLeft()
{
	if (caretPos_U > 0) {
		caretPos_U--;
		return true;
	}
	return false;
}

bool CEdit::ToRight()
{
	if (caretPos_U < text.size()) {
		caretPos_U++;
		return true;
	}
	return false;
}

bool CEdit::ToUp()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P += 1;
	posY_P -= lineHeight_P - 1;
	if (posY_P > 0) {
		GetUFromP((int*)&caretPos_U, posX_P, posY_P);
		return true;
	}
	return false;
}

bool CEdit::ToDown()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P += 1;
	posY_P += lineHeight_P + 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != caretPos_U) {
		caretPos_U = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToHome()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P = 0;
	posY_P += 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != caretPos_U) {
		caretPos_U = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToEnd()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P = textWidth_P;
	posY_P += 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != caretPos_U) {
		caretPos_U = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToPrior()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P += 1;
	posY_P -= pageHeight_P;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != caretPos_U) {
		caretPos_U = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToNext()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	posX_P += 1;
	posY_P += pageHeight_P;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != caretPos_U) {
		caretPos_U = tempCaretPos;
		return true;
	}
	return false;
}

void CEdit::ToBack()
{
	if (undo.empty()) return;
	while (!undo.back().empty())
	{
		undo.back().top().Release(&text, &codes, &caretPos_U, &selectPos_U, &segments);
		undo.back().pop();
	}

	undo.pop_back();

	UpdateScroll();
	SetCaretPosition();
	Update();
}

void CEdit::Cut()
{
	Copy();
	EraseSelect();
	SetCaretPosition();
	Update();
}

void CEdit::Copy()
{
	bufferText = text.substr(min(caretPos_U, selectPos_U), max(caretPos_U, selectPos_U) - min(caretPos_U, selectPos_U));
}

void CEdit::Paste()
{
	EraseSelect();
	caretPos_U = selectPos_U = caretPos_U + Insert(caretPos_U, bufferText, vector<int>(bufferText.size(), 0));
	SetCaretPosition();
	Update();
}

void CEdit::SetOverline()
{
	if (caretPos_U == selectPos_U) return;
	string text;
	vector<int> codes;
	text.insert(text.begin(), this->text.begin() + min(caretPos_U, selectPos_U), this->text.begin() + max(caretPos_U, selectPos_U));
	text.insert(text.begin(), '{');
	text.insert(text.end(), '}');
	codes.insert(codes.begin(), this->codes.begin() + min(caretPos_U, selectPos_U), this->codes.begin() + max(caretPos_U, selectPos_U));
	codes.insert(codes.begin(), 0);
	codes.insert(codes.end(), 0);
	AddUndo();
	undo.back().push(CAction());
	_Erase(min(caretPos_U, selectPos_U), max(caretPos_U, selectPos_U) - min(caretPos_U, selectPos_U));
	_Insert(min(caretPos_U, selectPos_U), text, codes);
	caretPos_U = selectPos_U = min(caretPos_U, selectPos_U);
	UpdateOverline();
	Update();
}

void CEdit::Mark(int id)
{
	if (caretPos_U == selectPos_U) return;

	for (size_t i = min(caretPos_U, selectPos_U); i < max(caretPos_U, selectPos_U); i++) {
		if (text[i] == '=' || text[i] == '\r') continue;
		SetMarker(id, &codes[i]);
	}

	RemoveLeftMarker(id, min(caretPos_U, selectPos_U));
	RemoveRightMarker(id, max(caretPos_U, selectPos_U));

	caretPos_U = selectPos_U = min(caretPos_U, selectPos_U);
	SetCaretPosition();
	Update();
}

bool CEdit::Check()
{
	CheckSyntax cs(&text, &codes);
	return cs.MarkErrors();
}

void CEdit::SetCaretPosition(bool off)
{
	int posX_P, posY_P;
	GetPFromU(&posX_P, &posY_P, caretPos_U);
	if (off) {
		if (posY_P < 0) {
			int offset = (posY_P - lineHeight_P + 1) / lineHeight_P;
			pageOffset_U += offset;
			posY_P -= offset * lineHeight_P;
			SetScrollPos(hWnd, SB_VERT, pageOffset_U, true);
		}
		if (posY_P + lineHeight_P > pageHeight_P) {
			int offset = (posY_P + lineHeight_P - pageHeight_P + lineHeight_P - 1) / lineHeight_P;
			pageOffset_U += offset;
			posY_P -= offset * lineHeight_P;
			SetScrollPos(hWnd, SB_VERT, pageOffset_U, true);
		}
	}
	SetCaretPos(posX_P, posY_P + charOffset_P);
}

int CEdit::GetCaretHeight()
{
	return charHeight_P;
}

void CEdit::UpdateScroll()
{
	int curPos_U = 0;

	int curPosX_P = 0;
	int curPosY_U = 0;

	int curCharWidth_P = 0;

	for (curPos_U = 0; curPos_U < text.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			curPosX_P = 0;
			curPosY_U++;
		}

		curPosX_P += curCharWidth_P;
	}
	curPosY_U++;
	int scrollRange = max(0, curPosY_U - (pageHeight_P / lineHeight_P));

	SetScrollRange(hWnd, SB_VERT, 0, scrollRange, true);
	if (pageOffset_U > scrollRange) {
		pageOffset_U = scrollRange;
		SetScrollPos(hWnd, SB_VERT, pageOffset_U, true);
	}
}

void CEdit::UpdateOverline()
{
	int overline = 0;
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == '{') overline++;
		if (text[i] == '}') {
			if (!overline) {
				_Erase(i, 1);
				i--;
				continue;
			}
			overline--;
		}
		if (text[i] == '=' || text[i] == '\n') {
			for (; overline > 0; overline--) {
				_Insert(i, "}", vector<int>(1, 0));
				i++;
			}
		}
	}
	for (; overline > 0; overline--) {
		_Insert(text.size(), "}", vector<int>(1, 0));
	}
}

void CEdit::UpdateMarkers()
{
	char lmarker_1 = 0;
	char lmarker_2 = 0;
	char lmarker_3 = 0;
	char lmarker_4 = 0;
	char lmarker_5 = 0;

	char dmarker_1 = 0;
	char dmarker_2 = 0;
	char dmarker_3 = 0;
	char dmarker_4 = 0;
	char dmarker_5 = 0;
	for (size_t i = 0; i < codes.size(); i++)
	{
		if (text[i] == '=' || text[i] == '\r') {
			lmarker_1 = 0;
			lmarker_2 = 0;
			lmarker_3 = 0;
			lmarker_4 = 0;
			lmarker_5 = 0;

			dmarker_1 = 0;
			dmarker_2 = 0;
			dmarker_3 = 0;
			dmarker_4 = 0;
			dmarker_5 = 0;
			continue;
		}
		if (codes[i] & LMARKER_1) {
			if (lmarker_1 == 0) lmarker_1 = 1;
			else if (lmarker_1 != 1) codes[i] &= ~LMARKER_1;
		}
		else if (lmarker_1 == 1) lmarker_1 = 2;
		if (codes[i] & LMARKER_2) {
			if (lmarker_2 == 0) lmarker_2 = 1;
			else if (lmarker_2 != 1) codes[i] &= ~LMARKER_2;
		}
		else if (lmarker_2 == 1) lmarker_2 = 2;
		if (codes[i] & LMARKER_3) {
			if (lmarker_3 == 0) lmarker_3 = 1;
			else if (lmarker_3 != 1) codes[i] &= ~LMARKER_3;
		}
		else if (lmarker_3 == 1) lmarker_3 = 2;
		if (codes[i] & LMARKER_4) {
			if (lmarker_4 == 0) lmarker_4 = 1;
			else if (lmarker_4 != 1) codes[i] &= ~LMARKER_4;
		}
		else if (lmarker_4 == 1) lmarker_4 = 2;
		if (codes[i] & LMARKER_5) {
			if (lmarker_5 == 0) lmarker_5 = 1;
			else if (lmarker_5 != 1) codes[i] &= ~LMARKER_5;
		}
		else if (lmarker_5 == 1) lmarker_5 = 2;

		if (codes[i] & DMARKER_1) {
			if (dmarker_1 == 0) dmarker_1 = 1;
			else if (dmarker_1 != 1) codes[i] &= ~DMARKER_1;
		}
		else if (dmarker_1 == 1) dmarker_1 = 2;
		if (codes[i] & DMARKER_2) {
			if (dmarker_2 == 0) dmarker_2 = 1;
			else if (dmarker_2 != 1) codes[i] &= ~DMARKER_2;
		}
		else if (dmarker_2 == 1) dmarker_2 = 2;
		if (codes[i] & DMARKER_3) {
			if (dmarker_3 == 0) dmarker_3 = 1;
			else if (dmarker_3 != 1) codes[i] &= ~DMARKER_3;
		}
		else if (dmarker_3 == 1) dmarker_3 = 2;
		if (codes[i] & DMARKER_4) {
			if (dmarker_4 == 0) dmarker_4 = 1;
			else if (dmarker_4 != 1) codes[i] &= ~DMARKER_4;
		}
		else if (dmarker_4 == 1) dmarker_4 = 2;
		if (codes[i] & DMARKER_5) {
			if (dmarker_5 == 0) dmarker_5 = 1;
			else if (dmarker_5 != 1) codes[i] &= ~DMARKER_5;
		}
		else if (dmarker_5 == 1) dmarker_5 = 2;
	}
}

void CEdit::PrepareText(string* text, vector<int>* codes)
{
	int overline = 0;
	for (size_t i = 0; i < text->size(); i++) {
		if (text->at(i) == '{') overline++;
		if (text->at(i) == '}') {
			if (!overline) {
				text->erase(text->begin() + i);
				codes->erase(codes->begin() + i);
				i--;
				continue;
			}
			overline--;
		}
		if (text->at(i) == '=' || text->at(i) == '\n') {
			for (int j = 0; j < overline; j++) {
				text->insert(text->begin() + i, '}');
				codes->insert(codes->begin() + i, 0);
				i++;
			}
			overline = 0;
		}
	}
	for (int j = 0; j < overline; j++) {
		text->insert(text->end(), '}');
		codes->insert(codes->end(), 0);
	}
}

int CEdit::GetRemoveTextOverline(const string& text)
{
	int result = 0;
	int overline = 0;
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == '{') overline++;
		if (text[i] == '}') {
			if (!overline) {
				result++;
				continue;
			}
			overline--;
		}
	}
	return result;
}

int CEdit::GetTextOverline(int start, int end)
{
	int overline = 0;
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == '{') overline++;
		if (text[i] == '}') overline--;
	}
	return overline;
}

bool CEdit::IsCorrectChar(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'
		|| c == '.' || c == '!' || c == '@' || c == '#'
		|| c == '$' || c == '^' || c == '-' || c == '+'
		|| c == '*' || c == '/' || c == '(' || c == ')'
		|| c == '|'
		|| c >= '0' && c <= '9' || c == '\r' || c == '=') return true;
	return false;
}

int CEdit::GetLeftSelectU(int pos_U)
{
	if (!pos_U) return pos_U;

	while (true)
	{
		pos_U--;
		if (!pos_U) break;
		if (text[pos_U] == '=' || text[pos_U] == '\r') {
			pos_U++;
			break;
		}
	}
	return pos_U;
}

int CEdit::GetRightSelectU(int pos_U)
{
	if (pos_U == text.size()) return pos_U;

	while (true)
	{
		pos_U++;
		if (pos_U == text.size()) break;
		if (text[pos_U] == '=' || text[pos_U] == '\r') break;
	}
	return pos_U;
}

inline void CEdit::SetMarker(int id, int* code)
{
	if (id & 0x000000FF) *code = (*code & 0xFFFFFF00) | id;
	else *code = (*code & 0xFFFF00FF) | id;
}

void CEdit::RemoveLeftMarker(int id, int pos_U)
{
	if (!pos_U) return;

	while (true)
	{
		pos_U--;
		if (!pos_U) break;
		if (text[pos_U] == '=' || text[pos_U] == '\r') {
			break;
		}
		codes[pos_U] &= ~id;
	}
}

void CEdit::RemoveRightMarker(int id, int pos_U)
{
	if (pos_U == text.size()) return;

	while (true)
	{
		pos_U++;
		if (pos_U == text.size()) break;
		if (text[pos_U] == '=' || text[pos_U] == '\r') {
			break;
		}
		codes[pos_U] &= ~id;
	}
}

int CEdit::GetCharWidthU(char c)
{
	if (c == '{' || c == '}' || c == '\r') return 0;
	return 1;
}

bool CEdit::GetUFromP(int* pos_U, const int posX_P, const int posY_P)
{
	int fixPos_U = -1;
	int curPos_U = 0;

	int curPosX_P = 0;
	int curPosY_P = -pageOffset_U * lineHeight_P;

	int curCharWidth_P = 0;

	bool isLine = false;

	if (curPosY_P + lineHeight_P > posY_P) isLine = true;

	for (curPos_U = 0; curPos_U < text.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			if (isLine) break;
			fixPos_U = -1;
			curPosX_P = 0;
			curPosY_P += lineHeight_P;
			if (curPosY_P + lineHeight_P > posY_P) isLine = true;
		}

		curPosX_P += curCharWidth_P;
		if (fixPos_U == -1) {
			if (curPosX_P > posX_P) {
				fixPos_U = curPos_U;
				if (isLine) break;
			}
		}
	}

	if (isLine || fixPos_U == -1) *pos_U = curPos_U;
	else *pos_U = fixPos_U;
	if (isLine && fixPos_U != -1) return true;
	return false;
}

void CEdit::GetPFromU(int* posX_P, int* posY_P, const int pos_U)
{
	int curPos_U = 0;

	int curPosX_P = 0;
	int curPosY_P = -pageOffset_U * lineHeight_P;

	int curCharWidth_P = 0;

	bool isLine = false;

	for (curPos_U = 0; curPos_U < pos_U; curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			curPosX_P = 0;
			curPosY_P += lineHeight_P;
		}

		curPosX_P += curCharWidth_P;
	}

	*posX_P = curPosX_P;
	*posY_P = curPosY_P;
}

struct color
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	void Clear()
	{
		r = g = b = 255;
	}

	void Mix(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = (this->r + r) >> 1;
		this->g = (this->g + g) >> 1;
		this->b = (this->b + b) >> 1;
	}
};

void CEdit::Update()
{
	if (!hDC) return;

	SelectObject(hDC, font);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(0, 0, 0));

	HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
	SelectObject(hDC, hBrush);
	HBRUSH hWBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hRBrush = CreateSolidBrush(RGB(255, 0, 0));

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hDC, pen);

	RECT rect;
	GetClientRect(hWnd, &rect);

	RECT charRect = { 0, -pageOffset_U * lineHeight_P, 0, 0 };
	RECT subCharRect;
	int curCharWidth_P = 0;
	int fixHeight = 0;
	color curColor;
	
	int curSegment = 0;

	for (size_t curPos_U = 0; curPos_U < text.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (charRect.left + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			if (charRect.top >= 0) {
				charRect.right = rect.right;
				charRect.bottom = charRect.top + lineHeight_P;
				curColor.Clear();
				SetDCBrushColor(hDC, RGB(curColor.r, curColor.g, curColor.b));
				FillRect(hDC, &charRect, hBrush);
			}
			if (text[curPos_U] == '\r') {
				if (codes[curPos_U] != 0) {
					int h = (charRect.top + lineHeight_P - fixHeight - charHeight_P) >> 1;
					string num = to_string(codes[curPos_U]);
					num.insert(num.begin(), '<');
					num.insert(num.end(), '>');
					int w = (textOffset_P - num.size() * charWidth_P) >> 1;
					TextOut(hDC, textWidth_P + w, fixHeight + h, num.data(), num.size());
				}
				fixHeight = charRect.top + lineHeight_P;
			}
			charRect.left = 0;
			charRect.top += lineHeight_P;
		}

		if (charRect.top >= 0) {
			charRect.right = charRect.left + charWidth_P;
			charRect.bottom = charRect.top + lineHeight_P;
			curColor.Clear();
			if (curPos_U >= min(caretPos_U, selectPos_U) && curPos_U < max(caretPos_U, selectPos_U)) curColor.Mix(0, 255, 0);
			if (curPos_U >= startSelectPos && curPos_U < endSelectPos) curColor.Mix(150, 150, 150);
			if (codes[curPos_U] & LMARKER_1) curColor.Mix(255, 255, 0);
			if (codes[curPos_U] & LMARKER_2) curColor.Mix(255, 200, 0);
			if (codes[curPos_U] & LMARKER_3) curColor.Mix(255, 150, 255);
			if (codes[curPos_U] & LMARKER_4) curColor.Mix(255, 100, 255);
			if (codes[curPos_U] & LMARKER_5) curColor.Mix(255, 50, 0);

			if (codes[curPos_U] & DMARKER_1) curColor.Mix(0, 255, 255);
			if (codes[curPos_U] & DMARKER_2) curColor.Mix(0, 200, 255);
			if (codes[curPos_U] & DMARKER_3) curColor.Mix(0, 150, 255);
			if (codes[curPos_U] & DMARKER_4) curColor.Mix(0, 100, 255);
			if (codes[curPos_U] & DMARKER_5) curColor.Mix(0, 50, 255);
			SetDCBrushColor(hDC, RGB(curColor.r, curColor.g, curColor.b));

			subCharRect = charRect;
			subCharRect.top += charOffset_P;
			FillRect(hDC, &subCharRect, hBrush);

			subCharRect = charRect;
			subCharRect.bottom -= charHeight_P;
			FillRect(hDC, &subCharRect, hWBrush);

			if (text[curPos_U] == '=') {
				if (!segments[curSegment].operation) SetTextColor(hDC, RGB(200, 200, 200));
				else if (!segments[curSegment].error) SetTextColor(hDC, RGB(0, 0, 0));
				else if (segments[curSegment].error == ERR_NO_ERROR) SetTextColor(hDC, RGB(0, 255, 0));
				else SetTextColor(hDC, RGB(255, 0, 0));
				TextOut(hDC, charRect.left, charRect.top + charOffset_P, &text[curPos_U], 1);
				SetTextColor(hDC, RGB(0, 0, 0));
				curSegment++;
			}
			else if (curCharWidth_P) TextOut(hDC, charRect.left, charRect.top + charOffset_P, &text[curPos_U], 1);
		}
		charRect.left += curCharWidth_P;
	}

	curColor.Clear();
	SetDCBrushColor(hDC, RGB(curColor.r, curColor.g, curColor.b));

	charRect.right = rect.right;
	charRect.bottom = charRect.top + lineHeight_P;
	FillRect(hDC, &charRect, hBrush);
	charRect.left = 0;
	charRect.top += lineHeight_P;

	charRect.bottom = rect.bottom;
	charRect.right = rect.right;
	FillRect(hDC, &charRect, hBrush);

	charRect = { 0, -pageOffset_U * lineHeight_P, 0, 0 };

	for (size_t curPos_U = 0; curPos_U < text.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(text[curPos_U]) * charWidth_P;

		if (charRect.left + curCharWidth_P > textWidth_P || text[curPos_U] == '\r') {
			charRect.left = 0;
			charRect.top += lineHeight_P;
		}

		if (text[curPos_U] == '{') DrawOverline(curPos_U, charRect.left, charRect.top, curPos_U == caretPos_U);

		if ((GetError(codes[curPos_U]) != ERR_NO_ERROR) && (GetError(codes[curPos_U]) != ERR_NO_TESTED)) {
			SetDCBrushColor(hDC, RGB(250, 0, 0));
			MoveToEx(hDC, charRect.left, charRect.top + lineHeight_P, nullptr);
			LineTo(hDC, charRect.left + curCharWidth_P, charRect.top + lineHeight_P);
			SetDCBrushColor(hDC, RGB(0, 0, 0));
		}

		charRect.left += curCharWidth_P;
	}
	DeleteObject(pen);
	DeleteObject(hWBrush);
	DeleteObject(hRBrush);
	InvalidateRect(hWnd, nullptr, false);
}

void CEdit::DrawOverline(int pos_U, int posX_P, int posY_P, bool isSelect)
{
	int overline = 0;
	int startPos_P = posX_P;
	int endPos_P = posX_P;
	int curPosY_P = posY_P;
	int curCharWidth_P = 0;
	int overlineHeight_U = GetOverlineHeight(pos_U);

	HPEN pen = nullptr;
	if (isSelect) pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 210));
	else pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hDC, pen);

	for (size_t i = pos_U; i < text.size(); i++)
	{
		curCharWidth_P = GetCharWidthU(text[i]) * charWidth_P;

		if (endPos_P + curCharWidth_P > textWidth_P || text[i] == '\r') {
			MoveToEx(hDC, startPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U, nullptr);
			LineTo(hDC, endPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U);
			startPos_P = endPos_P = 0;
			curPosY_P += lineHeight_P;
		}

		if (text[i] == '{') {
			overline++;
		}
		if (text[i] == '}') {
			overline--;
			if (!overline) {
				MoveToEx(hDC, startPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U, nullptr);
				LineTo(hDC, endPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U);
				break;
			}
		}

		endPos_P += curCharWidth_P;
	}

	DeleteObject(pen);
}

int CEdit::GetOverlineHeight(int pos_U)
{
	int overline = 0;
	int maxOverline = 0;
	for (size_t i = pos_U; i < text.size(); i++)
	{
		if (text[i] == '{') {
			overline++;
			if (overline > maxOverline) maxOverline = overline;
		}
		if (text[i] == '}') {
			overline--;
			if (!overline) break;
		}
	}
	return maxOverline;
}

int CEdit::GetLinePos(int pos_U)
{
	for (size_t i = pos_U; i < text.size(); i++)
		if (text[i] == '\r') return i;
	return -1;
}

int CEdit::GetSegmentPos(int pos_U)
{
	int curPos_S = 0;
	for (size_t i = 0; i < pos_U; i++) 
		if (text[i] == '=') curPos_S++;
	return curPos_S;
}

void CEdit::Restart()
{
	text.clear();
	codes.clear();
	undo.clear();
	caretPos_U = selectPos_U = 0;
	startSelectPos = endSelectPos = 0;

	segments.clear();

	SetCaretPosition();
	UpdateScroll();
	Update();
}

string CEdit::Open()
{
	char szFile[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Editor of Sets\0*.eos\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)) return "";

	Restart();

	Load(ofn.lpstrFile);

	SetCaretPosition();
	UpdateScroll();
	Update();

	return string(ofn.lpstrFile);
}

string CEdit::SaveAs()
{
	char szFile[64];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Editor of Sets\0*.eos\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	GetSaveFileName(&ofn);

	string fileName = ofn.lpstrFile;
	if (fileName.size() < 5 || fileName.substr(fileName.size() - 4, 4) != ".eos") fileName += ".eos";
	return fileName;
}

void CEdit::Load(string fileName)
{
	ifstream in(fileName);
	int tempInt = 0;
	int tempInt_2 = 0;
	int tempInt_3 = 0;
	char tempChar = 0;
	// Текст
	in.read((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < tempInt; i++)
	{
		in.read(&tempChar, sizeof(tempChar));
		text.push_back(tempChar);
	}
	// Коды
	in.read((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < tempInt; i++)
	{
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		codes.push_back(tempInt_2);
	}
	// Сегменты
	in.read((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < tempInt; i++)
	{
		segments.push_back(Segment());
		// Операция
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		segments[i].operation = tempInt_2;
		// Ошибка
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		segments[i].error = tempInt_2;
		// Указатели
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		for (size_t j = 0; j < tempInt_2; j++)
		{
			in.read((char*)&tempInt_3, sizeof(tempInt_3));
			segments[i].pointers.push_back(tempInt_3);
		}
	}
	// Позиции каретки и выделения
	in.read((char*)&tempInt, sizeof(tempInt));
	caretPos_U = tempInt;
	in.read((char*)&tempInt, sizeof(tempInt));
	selectPos_U = tempInt;
	// Позиции выделения
	in.read((char*)&tempInt, sizeof(tempInt));
	startSelectPos = tempInt;
	in.read((char*)&tempInt, sizeof(tempInt));
	endSelectPos = tempInt;

	in.close();
}

void CEdit::Save(string fileName)
{
	ofstream out(fileName);
	int tempInt = 0;
	// Текст
	tempInt = text.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	out.write(text.data(), tempInt);
	// Коды
	tempInt = codes.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	out.write((char*)codes.data(), sizeof(tempInt) * tempInt);
	// Сегменты
	tempInt = segments.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < segments.size(); i++)
	{
		// Операция
		out.write((char*)&segments[i].operation, sizeof(segments[i].operation));
		// Ошибка
		out.write((char*)&segments[i].error, sizeof(segments[i].error));
		// Указатели
		tempInt = segments[i].pointers.size();
		out.write((char*)&tempInt, sizeof(tempInt));
		out.write((char*)segments[i].pointers.data(), sizeof(tempInt) * tempInt);
	}
	// Позиции каретки и выделения
	out.write((char*)&caretPos_U, sizeof(caretPos_U));
	out.write((char*)&selectPos_U, sizeof(selectPos_U));
	// Позиции выделения
	out.write((char*)&startSelectPos, sizeof(startSelectPos));
	out.write((char*)&endSelectPos, sizeof(endSelectPos));

	out.close();
}

