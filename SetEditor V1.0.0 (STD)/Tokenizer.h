#pragma once

#include <string>
#include <vector>

class Tokenizer final
{
	struct Token
	{
		/* start pos, end pos (= or endline) */
		std::vector<std::pair<int, int>> segments;
	};
	struct TokenBridge
	{
		std::pair<int, int> left;
		std::pair<int, int> right;
	};
public:
	explicit Tokenizer(const std::string& data);

	bool next();

	std::vector<std::tuple<size_t, size_t, int>>& getErrors();

	std::pair<int, int> first;
	std::pair<int, int> second;

	static constexpr int Error_NeedlessExpression = 0x001;
	static constexpr int Error_NotFoundLeftOperand = 0x002;
	static constexpr int Error_NotFoundRightOperand = 0x003;
	static constexpr int Error_IncorrectLeftOperand = 0x004;
	static constexpr int Error_IncorrectRightOperand = 0x005;
private:
	size_t current_line;
	size_t current_pos;

	std::pair<int, int> checkOperand(const std::string& data, size_t delimiter_pos, int stop_pos, int direction);

	/* start pos, end pos, error code */
	std::vector<std::tuple<size_t, size_t, int>> errors;

	/* left => right */
	std::vector<TokenBridge> bridges;
	/* newline or => */
	std::vector<Token> tokens;
};

