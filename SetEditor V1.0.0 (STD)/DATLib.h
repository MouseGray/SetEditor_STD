#include <string>
#include <vector>
#include <stack>
#include <map>

#include "DATerm.h"
#include "vector_type.h"

class TermTool final
{
public:
	static std::pair<int, int> check(const std::string& data, const std::pair<int, int>& location);

	static Term* createTerm(const std::string& str);

	static bool isEqual(const Term& left, const Term& right);

	static bool isContain(const Term& term, const Term& sub_term);

	static int priority(const action value);

	static Term* copy(const Term* term);

	static void remove(Term* term);

	static void removeChildren(Term* term);

	static void collapse(Term* dest, Term* src);

	static void implement(Term* term, std::pair<Term*, std::vector<int>> part);
};

class TermCalculator final
{
public:
	TermCalculator(const std::string sets);

	Vec calculate(const Term& term);
private:
	std::map<char, Vec> m_vecs;
	int m_system_size;
};