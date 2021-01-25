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
	GetUFromP((int*)&m_cursor, x, y);
	if (!shift) m_sCursor = m_cursor;
	SetCaretPosition();
	Update();
	if (startSelectPos != endSelectPos) {
		RECT rect;
		GetClientRect(hWnd, &rect);
		POINT point = { rect.right >> 1, rect.bottom >> 1 };
		ClientToScreen(hWnd, &point);
		if (this->SegmentEditCallback != nullptr) SegmentEditCallback(&segments[GetSegmentPos(m_cursor)], point.x, point.y);
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
			line = GetLinePos(min(m_cursor, m_sCursor));
			if (line == -1) break;
			Update();
			break;
		case 2:
			line = GetLinePos(min(m_cursor, m_sCursor));
			if (line == -1) break;
			res = DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, LineNumProc, NULL);
			if (res == -1) break;
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
		if (tempCaretPos_U != m_cursor) {
			m_cursor = tempCaretPos_U;

			SetCaretPosition();
			Update();
		}
	}
	else {
		if (fact && tempCaretPos_U < m_buffer.size())
		{
			if (m_buffer[tempCaretPos_U] == '=') {
				startSelectPos = GetLeftSelectU(tempCaretPos_U);
				endSelectPos = GetRightSelectU(tempCaretPos_U);
				POINT point = { x, y };
				ClientToScreen(hWnd, &point);
				ShowTipCallback(hWnd, point.x, point.y, 0, &segments[GetSegmentPos(tempCaretPos_U)]);
				Update();
			}
			else {
				POINT point = { x, y };
				ClientToScreen(hWnd, &point);
				ShowTipCallback(hWnd, point.x, point.y, 0, nullptr);
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

	for (curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
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
	if (isSelected()) erase();
	else erase(-1);
	SetCaretPosition();
	Update();
}

void CEdit::W_Delete()
{
	if (isSelected()) erase();
	else erase(1);
	SetCaretPosition();
	Update();
}

void CEdit::W_Char(char c)
{
	if (!IsCorrectChar(c)) return;
	EraseSelect();
	if (c == '\r') c = '\n';
	string character;
	character += toupper(c);
	Insert(m_cursor, character, vector<int>(1, 0));
	m_sCursor = m_cursor;

	SetCaretPosition();
	Update();
}

void CEdit::W_Left(bool shift)
{
	if (m_cursor != m_sCursor) {
		m_cursor = m_sCursor = min(m_cursor, m_sCursor);
		SetCaretPosition();
		Update();
		return;
	}
	if (ToLeft()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Right(bool shift)
{
	if (m_cursor != m_sCursor) {
		m_cursor = m_sCursor = max(m_cursor, m_sCursor);
		SetCaretPosition();
		Update();
		return;
	}
	if (ToRight()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Up(bool shift)
{
	if (ToUp()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Down(bool shift)
{
	if (ToDown()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Home(bool shift)
{
	if (ToHome()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_End(bool shift)
{
	if (ToEnd()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Prior(bool shift)
{
	if (ToPrior()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
		Update();
	}
}

void CEdit::W_Next(bool shift)
{
	if (ToNext()) {
		if (!shift) m_sCursor = m_cursor;
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
	if (m_buffer[off] == '}') return;
	AddUndo();
	undo.back().push(CAction());
	int overline = GetRemoveTextOverline(m_buffer.substr(off, count));
	_Erase(off, count);
	for (; overline > 0; overline--) {
		_Insert(off, "}", vector<int>(1, 0));
	}
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

	//undo.back().top().CreateRemove(m_cursor, m_sCursor, startPos_S, m_buffer.substr(off, count), vector<int>(codes.begin() + off, codes.begin() + off + count), tempSegments);
	erase(count);
	
	//this->m_buffer.erase(this->m_buffer.begin() + off, this->m_buffer.begin() + off + count);
	//this->codes.erase(this->codes.begin() + off, this->codes.begin() + off + count);
}

int CEdit::Insert(size_t off, const string& text, const vector<int>& codes)
{
	AddUndo();
	undo.back().push(CAction());
	string paste = text;
	vector<int> pasteCode = codes;
	PrepareText(&paste, &pasteCode);
	_Insert(off, paste, pasteCode);
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

	//undo.back().top().CreateAdd(m_cursor, m_sCursor, startPos_S, text.size(), count);
	insert(text);
	//this->m_buffer.insert(this->m_buffer.begin() + off, text.begin(), text.end());
	//this->codes.insert(this->codes.begin() + off, codes.begin(), codes.end());
}

void CEdit::AddUndo()
{
	undo.push_back(stack<CAction>());
	if (undo.size() > MAX_UNDO) undo.pop_front();
}

bool CEdit::EraseSelect()
{
	if (m_cursor == m_sCursor) return false;
	Erase(min(m_cursor, m_sCursor), max(m_cursor, m_sCursor) - min(m_cursor, m_sCursor));
	m_cursor = m_sCursor = min(m_cursor, m_sCursor);
	return true;
}

bool CEdit::ToLeft()
{
	if (m_cursor > 0) {
		m_cursor--;
		return true;
	}
	return false;
}

bool CEdit::ToRight()
{
	if (m_cursor < m_buffer.size()) {
		m_cursor++;
		return true;
	}
	return false;
}

bool CEdit::ToUp()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P += 1;
	posY_P -= lineHeight_P - 1;
	if (posY_P > 0) {
		GetUFromP((int*)&m_cursor, posX_P, posY_P);
		return true;
	}
	return false;
}

bool CEdit::ToDown()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P += 1;
	posY_P += lineHeight_P + 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != m_cursor) {
		m_cursor = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToHome()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P = 0;
	posY_P += 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != m_cursor) {
		m_cursor = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToEnd()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P = textWidth_P;
	posY_P += 1;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != m_cursor) {
		m_cursor = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToPrior()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P += 1;
	posY_P -= pageHeight_P;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != m_cursor) {
		m_cursor = tempCaretPos;
		return true;
	}
	return false;
}

bool CEdit::ToNext()
{
	int posX_P = 0;
	int posY_P = 0;
	GetPFromU(&posX_P, &posY_P, m_cursor);
	posX_P += 1;
	posY_P += pageHeight_P;
	int tempCaretPos = 0;
	GetUFromP(&tempCaretPos, posX_P, posY_P);
	if (tempCaretPos != m_cursor) {
		m_cursor = tempCaretPos;
		return true;
	}
	return false;
}

void CEdit::ToBack()
{
	EFace::undo();
	// if (undo.empty()) return;
	/*while (!undo.back().empty())
	{
		undo.back().top().Release(&m_buffer, &codes, &m_cursor, &m_sCursor, &segments);
		undo.back().pop();
	}*/

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
	bufferText = m_buffer.substr(min(m_cursor, m_sCursor), max(m_cursor, m_sCursor) - min(m_cursor, m_sCursor));
}

void CEdit::Paste()
{
	EraseSelect();
	m_cursor = m_sCursor = m_cursor + Insert(m_cursor, bufferText, vector<int>(bufferText.size(), 0));
	SetCaretPosition();
	Update();
}

void CEdit::SetOverline()
{
	if (m_cursor == m_sCursor) return;
	string text;
	vector<int> codes;
	text.insert(text.begin(), this->m_buffer.begin() + min(m_cursor, m_sCursor), this->m_buffer.begin() + max(m_cursor, m_sCursor));
	text.insert(text.begin(), '{');
	text.insert(text.end(), '}');
	AddUndo();
	undo.back().push(CAction());
	_Erase(min(m_cursor, m_sCursor), max(m_cursor, m_sCursor) - min(m_cursor, m_sCursor));
	_Insert(min(m_cursor, m_sCursor), text, codes);
	m_cursor = m_sCursor = min(m_cursor, m_sCursor);
	UpdateOverline();
	Update();
}

void CEdit::Mark(int id)
{
	if (m_cursor == m_sCursor) return;

	m_cursor = m_sCursor = min(m_cursor, m_sCursor);
	SetCaretPosition();
	Update();
}

bool CEdit::Check()
{
	//CheckSyntax cs(&m_buffer, &codes);
	return true;//cs.MarkErrors();
}

void CEdit::SetCaretPosition(bool off)
{
	int posX_P, posY_P;
	GetPFromU(&posX_P, &posY_P, m_cursor);
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

	for (curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
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
	for (size_t i = 0; i < m_buffer.size(); i++) {
		if (m_buffer[i] == '{') overline++;
		if (m_buffer[i] == '}') {
			if (!overline) {
				_Erase(i, 1);
				i--;
				continue;
			}
			overline--;
		}
		if (m_buffer[i] == '=' || m_buffer[i] == '\n') {
			for (; overline > 0; overline--) {
				_Insert(i, "}", vector<int>(1, 0));
				i++;
			}
		}
	}
	for (; overline > 0; overline--) {
		_Insert(m_buffer.size(), "}", vector<int>(1, 0));
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
	for (size_t i = 0; i < m_buffer.size(); i++) {
		if (m_buffer[i] == '{') overline++;
		if (m_buffer[i] == '}') overline--;
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
		if (m_buffer[pos_U] == '=' || m_buffer[pos_U] == '\n') {
			pos_U++;
			break;
		}
	}
	return pos_U;
}

int CEdit::GetRightSelectU(int pos_U)
{
	if (pos_U == m_buffer.size()) return pos_U;

	while (true)
	{
		pos_U++;
		if (pos_U == m_buffer.size()) break;
		if (m_buffer[pos_U] == '=' || m_buffer[pos_U] == '\n') break;
	}
	return pos_U;
}

inline void CEdit::SetMarker(int id, int* code)
{
	if (id & 0x000000FF) *code = (*code & 0xFFFFFF00) | id;
	else *code = (*code & 0xFFFF00FF) | id;
}

int CEdit::GetCharWidthU(char c)
{
	if (c == '{' || c == '}' || c == '\n') return 0;
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

	for (curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
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
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (curPosX_P + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
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

	for (size_t curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (charRect.left + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
			if (charRect.top >= 0) {
				charRect.right = rect.right;
				charRect.bottom = charRect.top + lineHeight_P;
				curColor.Clear();
				SetDCBrushColor(hDC, RGB(curColor.r, curColor.g, curColor.b));
				FillRect(hDC, &charRect, hBrush);
			}
			if (m_buffer[curPos_U] == '\n') {
				/*if (codes[curPos_U] != 0) {
					int h = (charRect.top + lineHeight_P - fixHeight - charHeight_P) >> 1;
					string num = to_string(codes[curPos_U]);
					num.insert(num.begin(), '<');
					num.insert(num.end(), '>');
					int w = (textOffset_P - num.size() * charWidth_P) >> 1;
					TextOut(hDC, textWidth_P + w, fixHeight + h, num.data(), num.size());
				}*/
				fixHeight = charRect.top + lineHeight_P;
			}
			charRect.left = 0;
			charRect.top += lineHeight_P;
		}

		if (charRect.top >= 0) {
			charRect.right = charRect.left + charWidth_P;
			charRect.bottom = charRect.top + lineHeight_P;
			curColor.Clear();
			if (curPos_U >= min(m_cursor, m_sCursor) && curPos_U < max(m_cursor, m_sCursor)) curColor.Mix(0, 255, 0);
			if (curPos_U >= startSelectPos && curPos_U < endSelectPos) curColor.Mix(150, 150, 150);
			SetDCBrushColor(hDC, RGB(curColor.r, curColor.g, curColor.b));

			subCharRect = charRect;
			subCharRect.top += charOffset_P;
			FillRect(hDC, &subCharRect, hBrush);

			subCharRect = charRect;
			subCharRect.bottom -= charHeight_P;
			FillRect(hDC, &subCharRect, hWBrush);

			if (m_buffer[curPos_U] == '=') {
				if (!segments[curSegment].operation) SetTextColor(hDC, RGB(200, 200, 200));
				else if (!segments[curSegment].error) SetTextColor(hDC, RGB(0, 0, 0));
				else if (segments[curSegment].error == ERR_NO_ERROR) SetTextColor(hDC, RGB(0, 255, 0));
				else SetTextColor(hDC, RGB(255, 0, 0));
				TextOut(hDC, charRect.left, charRect.top + charOffset_P, &m_buffer[curPos_U], 1);
				SetTextColor(hDC, RGB(0, 0, 0));
				curSegment++;
			}
			else if (curCharWidth_P) TextOut(hDC, charRect.left, charRect.top + charOffset_P, &m_buffer[curPos_U], 1);
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

	for (size_t curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[curPos_U]) * charWidth_P;

		if (charRect.left + curCharWidth_P > textWidth_P || m_buffer[curPos_U] == '\n') {
			charRect.left = 0;
			charRect.top += lineHeight_P;
		}

		if (m_buffer[curPos_U] == '{') DrawOverline(curPos_U, charRect.left, charRect.top, curPos_U == m_cursor);

		/*if ((GetError(codes[curPos_U]) != ERR_NO_ERROR) && (GetError(codes[curPos_U]) != ERR_NO_TESTED)) {
			SetDCBrushColor(hDC, RGB(250, 0, 0));
			MoveToEx(hDC, charRect.left, charRect.top + lineHeight_P, nullptr);
			LineTo(hDC, charRect.left + curCharWidth_P, charRect.top + lineHeight_P);
			SetDCBrushColor(hDC, RGB(0, 0, 0));
		}*/

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

	for (size_t i = pos_U; i < m_buffer.size(); i++)
	{
		curCharWidth_P = GetCharWidthU(m_buffer[i]) * charWidth_P;

		if (endPos_P + curCharWidth_P > textWidth_P || m_buffer[i] == '\n') {
			MoveToEx(hDC, startPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U, nullptr);
			LineTo(hDC, endPos_P, curPosY_P + charOffset_P - 2 * overlineHeight_U);
			startPos_P = endPos_P = 0;
			curPosY_P += lineHeight_P;
		}

		if (m_buffer[i] == '{') {
			overline++;
		}
		if (m_buffer[i] == '}') {
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
	for (size_t i = pos_U; i < m_buffer.size(); i++)
	{
		if (m_buffer[i] == '{') {
			overline++;
			if (overline > maxOverline) maxOverline = overline;
		}
		if (m_buffer[i] == '}') {
			overline--;
			if (!overline) break;
		}
	}
	return maxOverline;
}

int CEdit::GetLinePos(int pos_U)
{
	for (size_t i = pos_U; i < m_buffer.size(); i++)
		if (m_buffer[i] == '\n') return i;
	return -1;
}

int CEdit::GetSegmentPos(int pos_U)
{
	int curPos_S = 0;
	for (size_t i = 0; i < pos_U; i++) 
		if (m_buffer[i] == '=') curPos_S++;
	return curPos_S;
}

void CEdit::Restart()
{
	m_buffer.clear();
	undo.clear();
	m_cursor = m_sCursor = 0;
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
		m_buffer.push_back(tempChar);
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
	m_cursor = tempInt;
	in.read((char*)&tempInt, sizeof(tempInt));
	m_sCursor = tempInt;
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
	tempInt = m_buffer.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	out.write(m_buffer.data(), tempInt);
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
	out.write((char*)&m_cursor, sizeof(m_cursor));
	out.write((char*)&m_sCursor, sizeof(m_sCursor));
	// Позиции выделения
	out.write((char*)&startSelectPos, sizeof(startSelectPos));
	out.write((char*)&endSelectPos, sizeof(endSelectPos));

	out.close();
}

