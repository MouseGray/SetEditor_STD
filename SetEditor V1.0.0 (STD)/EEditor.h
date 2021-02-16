#pragma once

#include "EMetricsSheet.h"
#include "EFace.h"

class EEditor : public EFace, public IEMetricsSheet
{
	void eraseSelectedText();
	void eraseChar();
	void eraseCharBefor();

	void insertText(const std::string& text);

	bool isErasableChar();
};

