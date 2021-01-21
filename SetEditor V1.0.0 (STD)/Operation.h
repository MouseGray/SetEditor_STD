#pragma once
// ======================================== |Includes| ======================================== //

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

#include "vector_type.h"
#include "Header.h"
using namespace std;
// ======================================== |Defines| ======================================== //

#define OE_1 '='

#define OS_1 '~'
#define OS_2 '#'
#define OS_3 '$'
#define OS_4 '^'

#define OA_0 '`'
#define OA_1 '-'
#define OA_2 '+'
#define OA_3 '*'
#define OA_4 '/'

#define OB_1 '('
#define OB_2 ')'
#define OB_3 '['
#define OB_4 ']'
#define OB__ '|'

#define N_0 '0'
#define N_1 '1'
#define N_2 '2'
#define N_3 '3'
#define N_4 '4'
#define N_5 '5'
#define N_6 '6'
#define N_7 '7'
#define N_8 '8'
#define N_9 '9'
#define N__ '.'

#define NE '_'

#define NULLSET '!'
#define UNISET '@'

// Нет ошибок
#define ERR_NO_ERROR 0x0
// Неверная запись числа
#define ERR_INVALID_NUMER 0x1 
// Операция для чисел в выражении для множеств
#define ERR_NUMBER_OPERATION_IN_SET 0x2
// Операция для множеств в выражении для чисел
#define ERR_SET_OPERATION_IN_NUMBER 0x3
// Не найдена открывающая скобка
#define ERR_NOT_FOUND_OPEN_BRACKET 0x4
// Не найдена закрывающая скобка
#define ERR_NOT_FOUND_CLOSE_BRACKET 0x5
// Не найдена открывающая скобка количества множеств
#define ERR_NOT_FOUND_SET_OPEN_BRACKET 0x6
// Не найдена закрывающая скобка количества множеств
#define ERR_NOT_FOUND_SET_CLOSE_BRACKET 0x7
// Найдено неизвестное множество
#define ERR_INVALID_SET 0x8

// Недостаточно операндов у дополнения множеств
#define ERR_NOT_ENOUGH_OPERANDS_OS1 0x10
// Недостаточно операндов у пересечения множеств
#define ERR_NOT_ENOUGH_OPERANDS_OS2 0x11
// Недостаточно операндов у объединения множеств
#define ERR_NOT_ENOUGH_OPERANDS_OS3 0x12
// Недостаточно операндов у вычитания множеств
#define ERR_NOT_ENOUGH_OPERANDS_OS4 0x13
// Недостаточно операндов у унарного вычитания
#define ERR_NOT_ENOUGH_OPERANDS_OA0 0x14
// Недостаточно операндов у вычитания
#define ERR_NOT_ENOUGH_OPERANDS_OA1 0x15
// Недостаточно операндов у сложения
#define ERR_NOT_ENOUGH_OPERANDS_OA2 0x16
// Недостаточно операндов у умножения
#define ERR_NOT_ENOUGH_OPERANDS_OA3 0x17
// Недостаточно операндов у деления
#define ERR_NOT_ENOUGH_OPERANDS_OA4 0x18

// Неверный тип операнда у дополнения множеств
#define ERR_INVALID_OPERAND_TYPE_OS1 0x20
// Неверный тип операнда у пересечения множеств
#define ERR_INVALID_OPERAND_TYPE_OS2 0x21
// Неверный тип операнда у объединения множеств
#define ERR_INVALID_OPERAND_TYPE_OS3 0x22
// Неверный тип операнда у вычитания множеств
#define ERR_INVALID_OPERAND_TYPE_OS4 0x23
// Неверный тип операнда у унарного вычитания
#define ERR_INVALID_OPERAND_TYPE_OA0 0x24
// Неверный тип операнда у вычитания
#define ERR_INVALID_OPERAND_TYPE_OA1 0x25
// Неверный тип операнда у сложения
#define ERR_INVALID_OPERAND_TYPE_OA2 0x26
// Неверный тип операнда у умножения
#define ERR_INVALID_OPERAND_TYPE_OA3 0x27
// Неверный тип операнда у деления
#define ERR_INVALID_OPERAND_TYPE_OA4 0x28
// Неверный тип операнда у количества элементов множества
#define ERR_INVALID_OPERAND_TYPE_OB4 0x29

// Вычисление выражения не дало правильного результата
#define ERR_INVALID_EXPRESSION_RESULT 0x30

#define get_previous(a, b) (a < 0) ? 0 : b[a]
#define get_next(a, b) (a >= b.size()) ? 0 : b[a]

// ======================================== |Structs| ======================================== //

// ======================================== |Prototypes| ======================================== //

// Определяет приоритет операции
int GetPriority(char a);

string OperationDescription(int id);

string ErrorDescription(int i);

// Заполняет вектор уникальным числом
void VTFill(Vec* vec, size_t count, int n);

// Задает векторам значения
vector<Vec> CreateSets(vector<char> sets, size_t* system_size);
