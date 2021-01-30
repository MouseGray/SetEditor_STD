#pragma once
#include <vector>
#include <string>
#include <algorithm>

using screenX = int;
using screenY = int;

class IEMetricsSheet
{
public:
	virtual int charWidth(const char) = 0;
	virtual int pageWidth() = 0;
	virtual int lineHeight() = 0;
};

class EMetricsSheet
{
public:
	struct Rect {
		screenX left;
		screenY top;
		screenX right;
		screenY bottom;
	};
	struct Line {
		size_t front;
		size_t back;
		screenY height;
		screenX width;
	};
public:
	std::vector<screenX> m_positionData;
	std::vector<Line>  m_linesData;

	IEMetricsSheet* m_obj;
public:
	explicit EMetricsSheet(IEMetricsSheet* _obj) { m_obj = _obj; }

	int getLineIndex(size_t pos) {
		for (size_t i = 0; i < m_linesData.size(); i++) 
			if (m_linesData[i].front <= pos && m_linesData[i].back >= pos) return i;
		return -1;
	}

	std::vector<Rect> getAreas(size_t startPos, size_t endPos)
	{
		auto startLine = getLineIndex(startPos);
		auto endLine = getLineIndex(endPos);

		decltype(getAreas(0, 0)) result;

		if (startLine == endLine) {
			auto start = m_positionData[startPos];
			auto end = endPos < m_linesData[startLine].back ? m_positionData[endPos] : m_linesData[startLine].width;
			result.push_back({ start, m_linesData[startLine].height, end, m_linesData[startLine].height + m_obj->lineHeight() });
		}
		else {
			auto start = m_positionData[startPos];
			auto end = m_linesData[startLine].width;

			result.push_back({ start, m_linesData[startLine].height, end, m_linesData[startLine].height + m_obj->lineHeight() });

			for (auto i = startLine + 1; i < endLine; i++) {
				start = 0;
				end = m_linesData[i].width;
				result.push_back({ start, m_linesData[i].height, end, m_linesData[i].height + m_obj->lineHeight() });
			}

			start = 0;
			end = endPos < m_linesData[endLine].back ? m_positionData[endPos] : m_linesData[endLine].width;;
			result.push_back({ start, m_linesData[endLine].height, end, m_linesData[endLine].height + m_obj->lineHeight() });
		}
		return result;
	}

	void update(size_t off, const std::string& data) {
		if (data.empty()) {
			m_positionData.clear();
			m_linesData.clear();
			m_linesData.push_back({ 0, 0, 0, 0 });
			return;
		}

		screenX startChar = 0;
		screenX endChar = 0; 
		screenX length = 0;

		screenY height = 0;

		size_t startRow = 0;

		auto line = getLineIndex(off);
		startRow = m_linesData[line].front;
		height   = m_linesData[line].height;
		m_linesData.erase(m_linesData.begin() + line, m_linesData.end());

		if (!m_positionData.empty()) {
			startChar = endChar = m_positionData[off];
			m_positionData.erase(m_positionData.begin() + off, m_positionData.end());
		}

		size_t index;
		for (index = off; index < data.size(); index++)
		{
			length = m_obj->charWidth(data[index]);
			endChar += length;
			if (endChar > m_obj->pageWidth()) {
				m_linesData.push_back({ startRow, index, height, startChar });
				startChar = 0;
				endChar = length;
				height += m_obj->lineHeight();
				startRow = index;
			}
			if (data[index] == '\n') {
				m_linesData.push_back({ startRow, index + 1, height, startChar });
				endChar = 0;
				height += m_obj->lineHeight();
				startRow = index + 1;
			}
			m_positionData.push_back(startChar);
			startChar = endChar;
		}
		if (startRow == index) return;
		m_linesData.push_back({ startRow, index, height, startChar });
	}
};

