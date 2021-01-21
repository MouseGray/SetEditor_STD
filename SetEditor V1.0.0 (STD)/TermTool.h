#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "TermResurses.h"
#include "DATerm.h"

using namespace std;

class TermTool
{
public:
	// ������� ���� �� ���������� ���������, ���������
	// ������ ���� ���������� (�� ������������ ������).
	// --------------------------------------------------------
	// ���������:
	// const string& expression: ��������� � ��������� �������.
	// --------------------------------------------------------
	// ������������ ��������:
	// term_ptr: ��������� �� ��������� ����.
	static term_ptr CreateTerm(const string& expression);

	// ����������� ���� � ���������� �����.
	// --------------------------------------------------------
	// ���������:
	// term_ptr pterm: �������� ���� (����� ���� �������).
	// --------------------------------------------------------
	// ������������ ��������:
	// void: ������.
	static void ToNormalForm(term_ptr pterm);

	// ���������� ��� ����� �������� �������� �������.
	// --------------------------------------------------------
	// ���������:
	// term_ptr* pterm_1: �������� ���� ���� (����� ���� �������).
	// term_ptr* pterm_2: �������� ���� ��� (����� ���� �������).
	// bool (*F)(const term_ptr&, const term_ptr&) - ������� ���������.
	// --------------------------------------------------------
	// ������������ ��������:
	// bool: ture - ���� ����� �����, ����� false.
	static bool Equal(const term_ptr& pterm_1, const term_ptr& pterm_2, bool (*F)(const term_ptr&, const term_ptr&));

	// ���������� ��� �����.
	// --------------------------------------------------------
	// ���������:
	// const term_ptr& pterm_1: ���� ����.
	// const term_ptr& pterm_2: ���� ���.
	// --------------------------------------------------------
	// ������������ ��������:
	// bool: ture - ���� ����� �����, ����� false.
	static bool Equal(const term_ptr& pterm_1, const term_ptr& pterm_2);

	// ������� ����� �����.
	// --------------------------------------------------------
	// ���������:
	// const term_ptr& pterm: �������� ����.
	// --------------------------------------------------------
	// ������������ ��������:
	// term_ptr: ��������� �� ��������� ����.
	static term_ptr Copy(const term_ptr& pterm);

	// ����������� �������� �����, ���������� �� �����. ���� 
	// ���������� �� �����, �� ���������� ������.
	// --------------------------------------------------------
	// ���������:
	// const term_ptr& pterm: �������� ����.
	// --------------------------------------------------------
	// ������������ ��������:
	// bool: true - �������� ���������, ����� false.
	static bool Calculate(const term_ptr& pterm, double* value);

	// ������� ���������� �������� �� ���� 1 � ���� 2.
	// --------------------------------------------------------
	// ���������:
	// term_ptr& pterm_1: ���� ����.
	// term_ptr& pterm_2: ���� ���.
	// --------------------------------------------------------
	// ������������ ��������:
	// void: ������.
	static void Collect(term_ptr& pterm_1, term_ptr& pterm_2);

	// �������� ��������� �� ������������.
	// --------------------------------------------------------
	// ���������:
	// const term_ptr& pterm: �������� ����.
	// double* coef: �����������.
	// term_ptr* resTerm: ���� ���������� �� ������������.
	// --------------------------------------------------------
	// ������������ ��������:
	// bool: ture - ������� ���������, ����� false.
	static bool ToCell(const term_ptr& pterm, double* coef, term_ptr* resTerm);


	static void PrintTerm(const term_ptr& pterm);
	static void _PrintTerm(ofstream& stream, const term_ptr& pterm);

	static bool IsBinaryMinus(const string& str, size_t pos);
	static int GetPriority(char operation);
};

