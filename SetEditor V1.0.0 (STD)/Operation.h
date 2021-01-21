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

// ��� ������
#define ERR_NO_ERROR 0x0
// �������� ������ �����
#define ERR_INVALID_NUMER 0x1 
// �������� ��� ����� � ��������� ��� ��������
#define ERR_NUMBER_OPERATION_IN_SET 0x2
// �������� ��� �������� � ��������� ��� �����
#define ERR_SET_OPERATION_IN_NUMBER 0x3
// �� ������� ����������� ������
#define ERR_NOT_FOUND_OPEN_BRACKET 0x4
// �� ������� ����������� ������
#define ERR_NOT_FOUND_CLOSE_BRACKET 0x5
// �� ������� ����������� ������ ���������� ��������
#define ERR_NOT_FOUND_SET_OPEN_BRACKET 0x6
// �� ������� ����������� ������ ���������� ��������
#define ERR_NOT_FOUND_SET_CLOSE_BRACKET 0x7
// ������� ����������� ���������
#define ERR_INVALID_SET 0x8

// ������������ ��������� � ���������� ��������
#define ERR_NOT_ENOUGH_OPERANDS_OS1 0x10
// ������������ ��������� � ����������� ��������
#define ERR_NOT_ENOUGH_OPERANDS_OS2 0x11
// ������������ ��������� � ����������� ��������
#define ERR_NOT_ENOUGH_OPERANDS_OS3 0x12
// ������������ ��������� � ��������� ��������
#define ERR_NOT_ENOUGH_OPERANDS_OS4 0x13
// ������������ ��������� � �������� ���������
#define ERR_NOT_ENOUGH_OPERANDS_OA0 0x14
// ������������ ��������� � ���������
#define ERR_NOT_ENOUGH_OPERANDS_OA1 0x15
// ������������ ��������� � ��������
#define ERR_NOT_ENOUGH_OPERANDS_OA2 0x16
// ������������ ��������� � ���������
#define ERR_NOT_ENOUGH_OPERANDS_OA3 0x17
// ������������ ��������� � �������
#define ERR_NOT_ENOUGH_OPERANDS_OA4 0x18

// �������� ��� �������� � ���������� ��������
#define ERR_INVALID_OPERAND_TYPE_OS1 0x20
// �������� ��� �������� � ����������� ��������
#define ERR_INVALID_OPERAND_TYPE_OS2 0x21
// �������� ��� �������� � ����������� ��������
#define ERR_INVALID_OPERAND_TYPE_OS3 0x22
// �������� ��� �������� � ��������� ��������
#define ERR_INVALID_OPERAND_TYPE_OS4 0x23
// �������� ��� �������� � �������� ���������
#define ERR_INVALID_OPERAND_TYPE_OA0 0x24
// �������� ��� �������� � ���������
#define ERR_INVALID_OPERAND_TYPE_OA1 0x25
// �������� ��� �������� � ��������
#define ERR_INVALID_OPERAND_TYPE_OA2 0x26
// �������� ��� �������� � ���������
#define ERR_INVALID_OPERAND_TYPE_OA3 0x27
// �������� ��� �������� � �������
#define ERR_INVALID_OPERAND_TYPE_OA4 0x28
// �������� ��� �������� � ���������� ��������� ���������
#define ERR_INVALID_OPERAND_TYPE_OB4 0x29

// ���������� ��������� �� ���� ����������� ����������
#define ERR_INVALID_EXPRESSION_RESULT 0x30

#define get_previous(a, b) (a < 0) ? 0 : b[a]
#define get_next(a, b) (a >= b.size()) ? 0 : b[a]

// ======================================== |Structs| ======================================== //

// ======================================== |Prototypes| ======================================== //

// ���������� ��������� ��������
int GetPriority(char a);

string OperationDescription(int id);

string ErrorDescription(int i);

// ��������� ������ ���������� ������
void VTFill(Vec* vec, size_t count, int n);

// ������ �������� ��������
vector<Vec> CreateSets(vector<char> sets, size_t* system_size);
