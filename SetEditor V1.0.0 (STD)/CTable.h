#pragma once
#include <string>
#include <vector>
#include "CEdit.h"

using namespace std;

#define GetOperation(code)	code & 0x0000FFFF

class CSegment
{
public:
	bool IsEqual(const CSegment& segment)
	{
		return left._Equal(segment.left) || left._Equal(segment.right) || right._Equal(segment.left) || right._Equal(segment.right);
	}

	int operation = 0;
	int error = 0;
	string left;
	vector<int> left_code;
	string right;
	vector<int> right_code;

	vector<int> pointers;
};

class CLine
{
public:
	int connection = 0;
	int number = 0;
	vector<CSegment> segments;
};

class CTable
{
public:
	void CreateTable(const string& text, const vector<int>& codes, const vector<Segment>& segments)
	{
		string tempSegmentPart;
		vector<int> tempSegmentPartCodes;
		bool isFirstSegmentPart = true;
		int curSegment = 0;
		Lines.push_back(CLine());
		for (int ch = 0; ch < text.size(); ch++)
		{
			if (text[ch] == '=') {
				if (!isFirstSegmentPart) {
					Lines.back().segments.back().right = tempSegmentPart;
					Lines.back().segments.back().right_code = tempSegmentPartCodes;
				}
				Lines.back().segments.push_back(CSegment());
				Lines.back().segments.back().left = tempSegmentPart;
				Lines.back().segments.back().left_code = tempSegmentPartCodes;
				Lines.back().segments.back().operation = segments[curSegment].operation;
				Lines.back().segments.back().pointers = segments[curSegment].pointers;
				curSegment++;
				tempSegmentPart.clear();
				tempSegmentPartCodes.clear();
				isFirstSegmentPart = false;
				continue;
			}
			if (text[ch] == '\r') {
				if (!isFirstSegmentPart) {
					Lines.back().segments.back().right = tempSegmentPart;
					Lines.back().segments.back().right_code = tempSegmentPartCodes;
				}
				Lines.back().number = codes[ch];
				Lines.push_back(CLine());
				tempSegmentPart.clear();
				tempSegmentPartCodes.clear();
				isFirstSegmentPart = true;
				continue;
			}
			tempSegmentPart += text[ch];
			tempSegmentPartCodes.push_back(codes[ch]);
		}

		if (!isFirstSegmentPart) {
			Lines.back().segments.back().right = tempSegmentPart;
			Lines.back().segments.back().right_code = tempSegmentPartCodes;
		}
	}

	void SetConnections()
	{
		// Пока есть 0
		bool hasNull = true;
		bool hasMinus = true;
		int curConnection = 0;
		while (hasNull)
		{
			hasNull = false;
			curConnection++;
			for (size_t i = 0; i < Lines.size(); i++)
			{
				if (Lines[i].connection == 0) {
					Lines[i].connection = -curConnection;
					hasMinus = true;
					while (hasMinus)
					{
						hasMinus = false;
						for (size_t j = 0; j < Lines.size(); j++)
						{
							if (Lines[j].connection < 0) {
								Lines[j].connection *= -1;
								for (size_t k = 0; k < Lines.size(); k++)
								{
									if (Lines[k].segments.empty()) continue;
									if (Lines[k].segments.front().left == Lines[j].segments.back().right) {
										Lines[k].connection = -curConnection;
									}
								}
								hasMinus = true;
							}
						}
					}
					hasNull = true;
				}
			}
		}
	}

	vector<CLine> Lines;
};

