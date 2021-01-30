#include "CEdit.h"

CEdit::CEdit() : m_metrics(this)
{
	selectBrush = CreateSolidBrush(RGB(0, 255, 0));

	m_metrics.update(0, m_buffer);

	HDC tempHDC = GetDC(hWnd);
	hDC = CreateCompatibleDC(tempHDC);
	hBM = CreateCompatibleBitmap(tempHDC, 2000, 1000);
	SelectObject(hDC, hBM);
	ReleaseDC(hWnd, tempHDC);
}

void CEdit::W_Create(HWND hWnd)
{
	this->hWnd = hWnd;
}

void CEdit::W_Size(int w, int h)
{
	/*if (hBM) {
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
	ReleaseDC(hWnd, tempHDC);*/

	pageWidth_P = w;
	pageHeight_P = h;

	textWidth_P = pageWidth_P - textOffset_P;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE;
	si.nPage = 1;
	SetScrollInfo(hWnd, SB_VERT, &si, true);

	m_metrics.update(0, m_buffer);

	UpdateScroll();
	SetCaretPosition();
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

	m_metrics.update(0, m_buffer);
}

void CEdit::W_LButtonDown(int x, int y, bool shift)
{
	GetUFromP((int*)&m_cursor, x, y);
	if (!shift) m_sCursor = m_cursor;
	SetCaretPosition();

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
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 5, (LPCSTR)"��������");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 4, (LPCSTR)"����������");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 3, (LPCSTR)"��������");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 2, (LPCSTR)"���������� ����� ������");
	InsertMenu(Menu, 0, MF_BYPOSITION | MF_STRING, 1, (LPCSTR)"������� ����� ������");
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
	selected = select(tempCaretPos_U);
	if (shift) {
		if (tempCaretPos_U != m_cursor) {
			m_cursor = tempCaretPos_U;

			SetCaretPosition();
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
			}
			else {
				POINT point = { x, y };
				ClientToScreen(hWnd, &point);
				ShowTipCallback(hWnd, point.x, point.y, 0, nullptr);
				if (startSelectPos != endSelectPos) {
					startSelectPos = endSelectPos = 0;
				}
			}
		}
		else {
			ShowTipCallback(hWnd, 0, 0, 0, nullptr);
			startSelectPos = endSelectPos = 0;
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
	if (isSelected()) erase<>();
	else erase<>(-1);
	SetCaretPosition();
	m_metrics.update(0, m_buffer);
}

void CEdit::W_Delete()
{
	if (isSelected()) erase<>();
	else erase<>(1);
	SetCaretPosition();
	m_metrics.update(0, m_buffer);
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
	m_metrics.update(0, m_buffer);
}

void CEdit::W_Left(bool shift)
{
	if (m_cursor != m_sCursor) {
		m_cursor = m_sCursor = min(m_cursor, m_sCursor);
		SetCaretPosition();
		return;
	}
	if (ToLeft()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Right(bool shift)
{
	if (m_cursor != m_sCursor) {
		m_cursor = m_sCursor = max(m_cursor, m_sCursor);
		SetCaretPosition();
		return;
	}
	if (ToRight()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Up(bool shift)
{
	if (ToUp()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Down(bool shift)
{
	if (ToDown()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Home(bool shift)
{
	if (ToHome()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_End(bool shift)
{
	if (ToEnd()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Prior(bool shift)
{
	if (ToPrior()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
	}
}

void CEdit::W_Next(bool shift)
{
	if (ToNext()) {
		if (!shift) m_sCursor = m_cursor;
		SetCaretPosition();
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
			break;
		case SB_THUMBPOSITION:
			pageOffset_U = pos;
			SetScrollPos(hWnd, SB_VERT, pos, true);
			SetCaretPosition(false);
			break;
		default:
			break;
	}
}

void CEdit::Erase(size_t off, size_t count)
{
	if (m_buffer[off] == '}') return;
	AddUndo();
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
	// �������� ������� �������� off
	int startPos_S = GetSegmentPos(off);
	// �������� ������� �������� off + count
	int endPos_S = GetSegmentPos(off + count);
	vector<Segment> tempSegments;
	for (size_t i = startPos_S; i < endPos_S; i++) tempSegments.push_back(segments[i]);

	if (!tempSegments.empty()) this->segments.erase(this->segments.begin() + startPos_S, this->segments.begin() + startPos_S + tempSegments.size());

	//undo.back().top().CreateRemove(m_cursor, m_sCursor, startPos_S, m_buffer.substr(off, count), vector<int>(codes.begin() + off, codes.begin() + off + count), tempSegments);
	erase(count);
	m_metrics.update(0, m_buffer);
	//this->m_buffer.erase(this->m_buffer.begin() + off, this->m_buffer.begin() + off + count);
	//this->codes.erase(this->codes.begin() + off, this->codes.begin() + off + count);
}

int CEdit::Insert(size_t off, const string& text, const vector<int>& codes)
{
	AddUndo();
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
	// �������� ������� �������� off
	int startPos_S = GetSegmentPos(off);
	// �������� ������� �������� off + count
	int count = 0;
	for (auto& c : text) {
		if (c == '=') count++;
	}

	this->segments.insert(this->segments.begin() + startPos_S, count, Segment());

	//undo.back().top().CreateAdd(m_cursor, m_sCursor, startPos_S, text.size(), count);
	insert(text);
	m_metrics.update(0, m_buffer);
	//this->m_buffer.insert(this->m_buffer.begin() + off, text.begin(), text.end());
	//this->codes.insert(this->codes.begin() + off, codes.begin(), codes.end());
}

void CEdit::AddUndo()
{

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
#undef min
void CEdit::Copy()
{
	auto p = OpenClipboard(hWnd);
	EmptyClipboard();
	auto size = std::abs(m_sCursor - m_cursor);
	auto start = std::min(m_cursor, m_sCursor);
	auto hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (size + 1) * sizeof(TCHAR));

	LPSTR lptstrCopy = (LPSTR)GlobalLock(hglbCopy);
	memcpy(lptstrCopy, &m_buffer[start], size * sizeof(TCHAR));
	lptstrCopy[size] = '\0';
	auto y = GlobalUnlock(hglbCopy);

	auto er = GetLastError();

	SetClipboardData(CF_TEXT, hglbCopy);

	CloseClipboard();
	//bufferText = m_buffer.substr(min(m_cursor, m_sCursor), max(m_cursor, m_sCursor) - min(m_cursor, m_sCursor));
}

void CEdit::Paste()
{
	auto p = OpenClipboard(hWnd);

	auto hglb = GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		auto lptstr = GlobalLock(hglb);
		if (lptstr != NULL)
		{
			auto str = std::string(static_cast<TCHAR*>(hglb));
			insert<>(str);
			GlobalUnlock(hglb);
		}
	}
	CloseClipboard();


	/*EraseSelect();
	m_cursor = m_sCursor = m_cursor + Insert(m_cursor, bufferText, vector<int>(bufferText.size(), 0));
	SetCaretPosition();*/
	Update();
}

void CEdit::SetOverline()
{
	setOverline<>();
	//if (m_cursor == m_sCursor) return;
	//string text;
	//vector<int> codes;
	//text.insert(text.begin(), this->m_buffer.begin() + min(m_cursor, m_sCursor), this->m_buffer.begin() + max(m_cursor, m_sCursor));
	//text.insert(text.begin(), '{');
	//text.insert(text.end(), '}');
	//AddUndo();
	//undo.back().push(CAction());
	//_Erase(min(m_cursor, m_sCursor), max(m_cursor, m_sCursor) - min(m_cursor, m_sCursor));
	//_Insert(min(m_cursor, m_sCursor), text, codes);
	//m_cursor = m_sCursor = min(m_cursor, m_sCursor);
	//UpdateOverline();
	m_metrics.update(0, m_buffer);
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
#undef min
bool CEdit::GetUFromP(int* pos_U, const int posX_P, const int posY_P)
{
	auto row = std::min(posY_P/lineHeight_P, static_cast<int>(m_metrics.m_linesData.size()) - 1);

	for (*pos_U = m_metrics.m_linesData[row].front; *pos_U < m_metrics.m_linesData[row].back; (*pos_U)++)
	{
		if (m_metrics.m_positionData[*pos_U] > posX_P)
		{
			(*pos_U)--;
			break;
		}
	}

	return true;
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
#undef max
void CEdit::drawSelect()
{
	if (!isSelected()) return;
	auto startPos = std::min(m_cursor, m_sCursor);
	auto endPos   = std::max(m_cursor, m_sCursor);
	auto areas = m_metrics.getAreas(startPos, endPos);
	std::for_each(areas.begin(), areas.end(), 
		[this](EMetricsSheet::Rect& rect) { 
			RECT r = { rect.right, rect.top, rect.left, rect.bottom }; 
			FillRect(hDC, &r, selectBrush); 
		});
}

void CEdit::drawConnections()
{
	for (auto a : m_connections)
	{
		auto border_1 = select(a.first);

		auto areas = m_metrics.getAreas(border_1.front(), border_1.back());
		std::for_each(areas.begin(), areas.end(),
			[this](EMetricsSheet::Rect& rect) {
			RECT r = { rect.right, rect.top, rect.left, rect.bottom };
			FillRect(hDC, &r, selectBrush);
		});

		if (a.second == -1) continue;
		auto border_2 = select(a.second);

		areas = m_metrics.getAreas(border_2.front(), border_2.back());
		std::for_each(areas.begin(), areas.end(),
			[this](EMetricsSheet::Rect& rect) {
			RECT r = { rect.right, rect.top, rect.left, rect.bottom };
			FillRect(hDC, &r, selectBrush);
		});
	}
}

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

	FillRect(hDC, &rect, hBrush);

	RECT subCharRect;
	int curCharWidth_P = 0;
	int fixHeight = 0;
	
	int curSegment = 0;

	clock_t s = clock();
	drawSelect();
	drawConnections();
	int curRow = 0;
	for (size_t curPos_U = 0; curPos_U < m_buffer.size(); curPos_U++)
	{
		if (curPos_U >= m_metrics.m_linesData[curRow].back) curRow++;
		if (m_buffer[curPos_U] == '{') DrawOverline(curPos_U, m_metrics.m_positionData[curPos_U], m_metrics.m_linesData[curRow].height, curPos_U == m_cursor);
		if (m_buffer[curPos_U] == '\n') continue;
		if (m_buffer[curPos_U] == '{' || m_buffer[curPos_U] == '}') continue;
		TextOut(hDC, m_metrics.m_positionData[curPos_U], m_metrics.m_linesData[curRow].height + charOffset_P, &m_buffer[curPos_U], 1);
	}

	DeleteObject(pen);
	DeleteObject(hWBrush);
	DeleteObject(hRBrush);
	InvalidateRect(hWnd, nullptr, false);

	clock_t e = clock();

	clock_t res = e - s;
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
	m_cursor = m_sCursor = 0;
	startSelectPos = endSelectPos = 0;

	segments.clear();

	m_metrics.update(0, m_buffer);

	SetCaretPosition();
	UpdateScroll();
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
	// �����
	in.read((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < tempInt; i++)
	{
		in.read(&tempChar, sizeof(tempChar));
		m_buffer.push_back(tempChar);
	}
	// ��������
	in.read((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < tempInt; i++)
	{
		segments.push_back(Segment());
		// ��������
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		segments[i].operation = tempInt_2;
		// ������
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		segments[i].error = tempInt_2;
		// ���������
		in.read((char*)&tempInt_2, sizeof(tempInt_2));
		for (size_t j = 0; j < tempInt_2; j++)
		{
			in.read((char*)&tempInt_3, sizeof(tempInt_3));
			segments[i].pointers.push_back(tempInt_3);
		}
	}
	// ������� ������� � ���������
	in.read((char*)&tempInt, sizeof(tempInt));
	m_cursor = tempInt;
	in.read((char*)&tempInt, sizeof(tempInt));
	m_sCursor = tempInt;
	// ������� ���������
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
	// �����
	tempInt = m_buffer.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	out.write(m_buffer.data(), tempInt);
	// ��������
	tempInt = segments.size();
	out.write((char*)&tempInt, sizeof(tempInt));
	for (size_t i = 0; i < segments.size(); i++)
	{
		// ��������
		out.write((char*)&segments[i].operation, sizeof(segments[i].operation));
		// ������
		out.write((char*)&segments[i].error, sizeof(segments[i].error));
		// ���������
		tempInt = segments[i].pointers.size();
		out.write((char*)&tempInt, sizeof(tempInt));
		out.write((char*)segments[i].pointers.data(), sizeof(tempInt) * tempInt);
	}
	// ������� ������� � ���������
	out.write((char*)&m_cursor, sizeof(m_cursor));
	out.write((char*)&m_sCursor, sizeof(m_sCursor));
	// ������� ���������
	out.write((char*)&startSelectPos, sizeof(startSelectPos));
	out.write((char*)&endSelectPos, sizeof(endSelectPos));

	out.close();
}