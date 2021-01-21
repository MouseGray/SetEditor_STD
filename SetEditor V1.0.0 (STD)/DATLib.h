// ======================================== |Includes| ======================================== //

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <iostream>
#include "DATerm.h"
#include "Operation.h"

using namespace std;
// ======================================== |Defines| ======================================== //


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

//#define dat_return(a, b, r) {_Remove_(a); _Remove_(b); return r;}
//#define dat_return_x3(a, b, c, r) {_Remove_(a); _Remove_(b); _Remove_(c); return r;}
//#define dat_return_x4(a, b, c, d, r) {_Remove_(a); _Remove_(b); _Remove_(c); _Remove_(d); return r;}
//#define dat_return_x5(a, b, c, d, e, r) {_Remove_(a); _Remove_(b); _Remove_(c); _Remove_(d); _Remove_(e); return r;}

// ======================================== |Structs| ======================================== //


// ======================================== |Prototypes| ======================================== //

// Преобразует строковое выражение в DAT
term_ptr TermCoder(const string& str, int* error);

// Сравнивает DAT
bool Equal(term_ptr pTerm_1, term_ptr pTerm_2);

// Удаляет DAT ветку полностью
term_ptr _Copy_(term_ptr pTerm);

// Вычисляет глубину ветки
size_t GetDeep(term_ptr pTerm);

// Вычисляет наибольшую глубину у ветки
int GetMaxDeep(term_ptr pTerm);

term_ptr TrimTop(term_ptr pTerm);