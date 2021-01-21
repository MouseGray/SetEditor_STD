#include "Operation.h"

int GetPriority(char a)
{
	switch (a)
	{
		case OS_1: return 0;
		case OS_2: return 1;
		case OS_3: return 2;
		case OS_4: return 1;

		case OA_1: return 3;
		case OA_2: return 5;
		case OA_3: return 4;
		case OA_4: return 4;

		case OB_1: return 6;
		case OB_2: return 5;
		case OB_3: return 6;
		case OB_4: return 5;
		case OB__: return -1;

		case OE_1: return 7;

		case NE: return 10;
	}
	return 20;
}

string OperationDescription(int id)
{
	/*switch (id) {
		case OP_COMPL_NULLSET: 
			return "Дополнение к пустому множеству.\n\
					~!=@";
		case OP_COMPL_UNISET: 
			return "Дополнение к универсальному множеству.\n\
					~@=!";
		case OP_COMPL_A:
			return "Преобразование по закону Де Моргана.\n\
					~(A#B)=~A$~B\n\
					~(A$B)=~A#~B";
		case OP_IN_PARANTHESES:			
			return "Дистрибутивность по пересечению.\n\
					A#(B$C)=(A#B)&(A#C)";
		case OP_UN_PARANTHESES:
			return "Дистрибутивность по объединению.\n\
					A$(B#C)=(A$B)#(A$C)";
		case OP_DOUBLE_COMPL:			
			return "Двойное дополнение.\n\
					~~A=A";
		case OP_IN_NULLSET:
			return "Пересечение с пустым множеством.\n\
					A#!=!";
		case OP_UN_NULLSET:
			return "Объединение с пустым множеством.\n\
					A$!=A";
		case OP_IN_UNISET:
			return "Пересечение с универсальным множеством.\n\
					A#!=!";
		case OP_UN_UNISET:
			return "Объединение с универсальным множеством.\n\
					A#!=!";
		case OP_IN_A_AND_A:
			return "Пересечение одинаковых множеств.\n\
					A#A=A";
		case OP_UN_A_AND_A:
			return "Объединение одинаковых множеств.\n\
					A$A=A";
		case OP_IN_A_COMPL_A:
			return "Пересечение множества и его дополнения.\n\
					A#~A=!";
		case OP_UN_A_COMPL_A:
			return "Объединение множества и его дополнения.\n\
					A$~A=@";
		case OP_SET_MINUS: 
			return "Разность множеств.\n\
					A^B=A#~B";
		case OP_INC_EXC_X2: 
			return "Формула вкл. искл. для 2 множеств.\n\
					|A$B|=|A|+|B|-|A#B|";
		case OP_INC_EXC_X3: 
			return "Формула вкл. искл. для 3 множеств.\n\
					|A$B$C|=|A|+|B|+|C|-|A#B|-|A#C|-|B#C|+|A#B#C|";
		case OP_DOUBLE_NEG: 
			return "Формула вкл. искл. для 3 множеств.\n\
					|A$B$C|=|A|+|B|+|C|-|A#B|-|A#C|-|B#C|+|A#B#C|";
		case OP_NEG_A: 
			return "Раскрытие скобок.\n\
					-(A+B)=-A-B";
		case OP_MP_PARANTHESES: 
			return "Дистрибутивность по умножению.\n\
					A*(B+C)=A*B+A*C";
		case MULTIPLY_BY_0:
			return "Умножение на 0.\n\
					A*0=0";
		case OP_ADD_A_NEG_A:
			return "Сложение противоположных выражений.\n\
					A+(-A)=0";
		case OP_COMP:
			return "Вычисление численного выражения.\n\
					n1+n2*n3/n4...=N";
		case OP_CHANGE_EQUAL:
			return "Перенос через равно.\n\
					A+B=0=>A=-B";
		case OP_SUBSTITUTION:
			return "Подстановка.\n\
					Укажите номер строки подставляемого\n\
					выражения в окне доп. строки.";
		case OP_CONCLUSION:
			return "Вывод.\n\
					Левое выражение является\n\
					следствием из правого.";
		case OP_GIVEN: 
			return "Дано.\n\
					Выражение из  условия задачи.";
		case OP_TOFIND: 
			return "Дано.\n\
					Выражение из  условия задачи.";
		case OP_DECISION:
			return "Подстановка чисел.\n\
					Подстановка числовых значений из\n\
					дано в окончательную формулу.";
		case OP_ANSWER:
			return "Ответ.";

		case OP_ADD_ZERO:
			return "Сложение с нулем.\n\
					A+0=A";
		case OP_CARDIALITY:
			return "Мощность пустого множества.\n\
					|!|=0";
	}*/
	return "";
}

void VTFill(Vec* vec, size_t count, int n)
{
	switch (n)
	{
		case -2:
			for (size_t i = 0; i < count; i++) (*vec)[i] = 1;
			break;
		case -1:
			for (size_t i = 0; i < count; i++) (*vec)[i] = 0;
			break;
		default:
			int var = 1 << n;
			for (size_t i = 0; i < count; i++) (*vec)[i] = (i / var) % 2;
			break;
	}
}

vector<Vec> CreateSets(vector<char> sets, size_t* system_size)
{
	*system_size = 1 << (sets.size() + 1);

	Vec NVT(vec_type::set, 0.0, *system_size);

	vector<Vec> result;

	for (size_t i = 0; i < sets.size(); i++)
	{
		NVT.code(sets[i]);
		VTFill(&NVT, *system_size, i);
		result.push_back(NVT);
	}

	NVT.code('X');
	VTFill(&NVT, *system_size, sets.size());
	result.push_back(NVT);

	NVT.code(NULLSET);
	VTFill(&NVT, *system_size, -1);
	result.push_back(NVT);

	NVT.code(UNISET);
	VTFill(&NVT, *system_size, -2);
	result.push_back(NVT);

	return result;
}


