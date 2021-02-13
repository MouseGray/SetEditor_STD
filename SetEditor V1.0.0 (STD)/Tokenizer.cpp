#include <regex>

#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string& data)
{
    enum class ExprType : char {
        Arrow,
        Equal,
        Expression
    };
    auto parts = std::vector<std::tuple<size_t, size_t, ExprType>>();

    auto start = size_t(0);
    for (auto index = size_t(0); index < data.size(); index++)
    {
        if (data[index] == '=') {
            if (index < data.size() - 1) {
                if (data[index + 1] == '>') {
                    if (index - start != 0)
                        parts.push_back(std::make_tuple(start, index, ExprType::Expression));
                    parts.push_back(std::make_tuple(index, index + 2, ExprType::Arrow));
                    start = index + 2;
                    index++;
                    continue;
                }
            }
            if (index - start != 0)
                parts.push_back(std::make_tuple(start, index, ExprType::Expression));
            parts.push_back(std::make_tuple(index, index + 1, ExprType::Equal));
            start = index + 1;
            continue;
        }
        if (data[index] == '\n') {
            if (index - start != 0)
                parts.push_back(std::make_tuple(start, index, ExprType::Expression));
            start = index + 1;
            continue;
        }
    }
    if (data.size() - start != 0) {
        parts.push_back(std::make_tuple(start, data.size(), ExprType::Expression));
    }

    auto isFirst = true;
    for (auto index = size_t(0); index < parts.size(); index++)
    {
        if (std::get<2>(parts[index]) == ExprType::Expression) {
            if (index == parts.size() - 1 || std::get<2>(parts[index + 1]) != ExprType::Equal) {
                if (isFirst) {
                    errors.push_back(std::make_tuple(std::get<0>(parts[index]), std::get<1>(parts[index]), Error_NeedlessExpression));
                    continue;
                }
                isFirst = true;
            }
            else {
                if (isFirst)
                    tokens.push_back({});
                isFirst = false;
            }
            tokens.back().segments.push_back(std::make_pair(std::get<0>(parts[index]), std::get<1>(parts[index])));
            continue;
        }

        if (std::get<2>(parts[index]) == ExprType::Equal) {
            if (index == 0 || std::get<2>(parts[index - 1]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index]), std::get<1>(parts[index]), Error_NotFoundLeftOperand));
                continue;
            }
            if (index == parts.size() - 1 || std::get<2>(parts[index + 1]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index]), std::get<1>(parts[index]), Error_NotFoundRightOperand));
                continue;
            }
            continue;
        }

        if (std::get<2>(parts[index]) == ExprType::Arrow) {
            if (index == 0 || std::get<2>(parts[index - 1]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index]), std::get<1>(parts[index]), Error_NotFoundLeftOperand));
                continue;
            }
            if (index == 1 || std::get<2>(parts[index - 2]) != ExprType::Equal) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index - 1]), std::get<1>(parts[index - 1]), Error_IncorrectLeftOperand));
                continue;
            }
            if (index == 2 || std::get<2>(parts[index - 3]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index - 2]), std::get<1>(parts[index - 1]), Error_IncorrectLeftOperand));
                continue;
            }

            if (index == parts.size() - 1 || std::get<2>(parts[index + 1]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index]), std::get<1>(parts[index]), Error_NotFoundRightOperand));
                continue;
            }
            if (index == parts.size() - 2 || std::get<2>(parts[index + 2]) != ExprType::Equal) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index + 1]), std::get<1>(parts[index + 1]), Error_IncorrectRightOperand));
                continue;
            }
            if (index == parts.size() - 3 || std::get<2>(parts[index + 3]) != ExprType::Expression) {
                errors.push_back(std::make_tuple(std::get<0>(parts[index + 1]), std::get<1>(parts[index + 2]), Error_IncorrectRightOperand));
                continue;
            }
            bridges.push_back({ std::make_pair(std::get<0>(parts[index - 3]), std::get<1>(parts[index - 1])), std::make_pair(std::get<0>(parts[index + 1]), std::get<1>(parts[index + 2])) });
            continue;
        }
    }

	current_line = 0;
	current_pos = 0;
}

bool Tokenizer::next()
{
	if (current_line == tokens.size()) return false;
	current_pos++;

	while (current_pos == tokens[current_line].segments.size()) {
		current_line++;
		current_pos = 1;
		if (current_line == tokens.size()) return false;
	}

	first = tokens[current_line].segments[current_pos - 1];
	second = tokens[current_line].segments[current_pos];
	return true;
}

std::vector<std::tuple<size_t, size_t, int>>& Tokenizer::getErrors()
{
	return errors;
}