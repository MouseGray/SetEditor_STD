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
	// Создает терм по текстовому выражению, выражение
	// должно быть корректным (не обрабатывает ошибки).
	// --------------------------------------------------------
	// Аргументы:
	// const string& expression: выражение в текстовом формате.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// term_ptr: указатель на созданный терм.
	static term_ptr CreateTerm(const string& expression);

	// Преобразует терм к нормальной форме.
	// --------------------------------------------------------
	// Аргументы:
	// term_ptr pterm: исходный терм (может быть изменен).
	// --------------------------------------------------------
	// Возвращаемое значение:
	// void: ничего.
	static void ToNormalForm(term_ptr pterm);

	// Сравнивает два терма применяя заданную функцию.
	// --------------------------------------------------------
	// Аргументы:
	// term_ptr* pterm_1: исходный терм один (может быть изменен).
	// term_ptr* pterm_2: исходный терм два (может быть изменен).
	// bool (*F)(const term_ptr&, const term_ptr&) - функция сравнения.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// bool: ture - если термы равны, иначе false.
	static bool Equal(const term_ptr& pterm_1, const term_ptr& pterm_2, bool (*F)(const term_ptr&, const term_ptr&));

	// Сравнивает два терма.
	// --------------------------------------------------------
	// Аргументы:
	// const term_ptr& pterm_1: терм один.
	// const term_ptr& pterm_2: терм два.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// bool: ture - если термы равны, иначе false.
	static bool Equal(const term_ptr& pterm_1, const term_ptr& pterm_2);

	// Создает копию терма.
	// --------------------------------------------------------
	// Аргументы:
	// const term_ptr& pterm: исходный терм.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// term_ptr: указатель на созданный терм.
	static term_ptr Copy(const term_ptr& pterm);

	// Высчитывает значение терма, состоящего из чисел. Если 
	// встретится не число, то возвращает ошибку.
	// --------------------------------------------------------
	// Аргументы:
	// const term_ptr& pterm: исходный терм.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// bool: true - значение вычислено, иначе false.
	static bool Calculate(const term_ptr& pterm, double* value);

	// Убирает одинаковые элементы из терм 1 и терм 2.
	// --------------------------------------------------------
	// Аргументы:
	// term_ptr& pterm_1: терм один.
	// term_ptr& pterm_2: терм два.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// void: ничего.
	static void Collect(term_ptr& pterm_1, term_ptr& pterm_2);

	// Отделяет выражение от коэффициента.
	// --------------------------------------------------------
	// Аргументы:
	// const term_ptr& pterm: исходный терм.
	// double* coef: коэффициент.
	// term_ptr* resTerm: терм отделенный от коэффициента.
	// --------------------------------------------------------
	// Возвращаемое значение:
	// bool: ture - удалось разделить, иначе false.
	static bool ToCell(const term_ptr& pterm, double* coef, term_ptr* resTerm);


	static void PrintTerm(const term_ptr& pterm);
	static void _PrintTerm(ofstream& stream, const term_ptr& pterm);

	static bool IsBinaryMinus(const string& str, size_t pos);
	static int GetPriority(char operation);
};

