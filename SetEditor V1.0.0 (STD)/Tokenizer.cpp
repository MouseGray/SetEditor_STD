#include <regex>

#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string& data)
{
	auto token_start = size_t(0);
	/* start | end | is '=>' */
	auto token_metric = std::vector<std::tuple<size_t, size_t, bool>>();

	for (auto index = size_t(0); index < data.size(); index++)
	{
		if (index == data.size() - 1) {
			if (index - token_start != 0)
				token_metric.push_back(std::make_tuple(token_start, index + 1, false));
		}
		if (data[index] == '=') {
			if (data[index + 1] != '>') continue;
			if (index - token_start != 0)
				token_metric.push_back(std::make_tuple(token_start, index, true));
			token_start = index + 2;
		}
		if (data[index] == '\n') {
			if (index - token_start != 0)
				token_metric.push_back(std::make_tuple(token_start, index, false));
			token_start = index + 1;
		}
	}

	for (auto& a : token_metric)
	{
		auto first_segment = true;
		auto segment_start = std::get<0>(a);
		for (auto index = std::get<0>(a); index < std::get<1>(a); index++)
		{
			if (data[index] == '=') {
				if (index - segment_start == 0) {
					errors.push_back(std::make_tuple(index, index + 1, Error_NotFoundLeftOperand));
					continue;
				}
				if (first_segment) tokens.push_back(Token());
				tokens.back().segments.push_back(std::make_pair(segment_start, index));
				segment_start = index + 1;
				first_segment = false;
				continue;
			}
		}
		if (std::get<1>(a) - segment_start == 0) {
			/* not the first guaranteed non-empty token */
			errors.push_back(std::make_tuple(segment_start - 1, segment_start, Error_NotFoundRightOperand));
			continue;
		}
		if (first_segment) {
			tokens.push_back(Token());
			errors.push_back(std::make_tuple(segment_start, std::get<1>(a), Error_NeedlessExpression));
		}
		tokens.back().segments.push_back(std::make_pair(segment_start, std::get<1>(a)));
	}

	auto delimiter_pos = size_t(0);
	while (true) {
		delimiter_pos = data.find("=>", delimiter_pos + 2);
		if (delimiter_pos == std::string::npos) break;
		/* left */
		auto left = checkOperand(data, delimiter_pos, -1, -1);
		if (left.first == -1) continue;
		auto right = checkOperand(data, delimiter_pos, data.size(), 1);
		if (right.first == -1) continue;
		bridges.push_back({ std::make_pair(left.second, left.first), right });
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

std::pair<int, int> Tokenizer::checkOperand(const std::string& data, size_t delimiter_pos, int stop_pos, int direction)
{
	auto end_pos = static_cast<int>(direction > 0 ? delimiter_pos + 2 : delimiter_pos - 1);
	auto start_pos = end_pos;
	/* skip newlines */
	while (true) {
		if (end_pos == stop_pos) {
			/* [error] empty operand */
			errors.push_back(std::make_tuple(delimiter_pos, delimiter_pos + 2, direction > 0 ? Error_NotFoundRightOperand : Error_NotFoundLeftOperand));
			return std::make_pair(-1, -1);
		}
		if (data[end_pos] != '\n') break;
		end_pos += direction;
	}
	start_pos = end_pos - (direction > 0 ? 0 : direction);
	/* right part */
	auto is_first = true;
	while (true) {
		if (end_pos == stop_pos) {
			/* [error] not found left part */
			errors.push_back(std::make_tuple(std::min(end_pos, start_pos), std::max(end_pos, start_pos), direction > 0 ? Error_IncorrectRightOperand : Error_IncorrectLeftOperand));
			return std::make_pair(-1, -1);
		}
		if (data[end_pos] == '\n' || data[end_pos] == '>') {
			/* [error] not found left part */
			errors.push_back(std::make_tuple(std::min(end_pos, start_pos), std::max(end_pos, start_pos), direction > 0 ? Error_IncorrectRightOperand : Error_IncorrectLeftOperand));
			return std::make_pair(-1, -1);
		}
		if (data[end_pos] == '=') {
			if (is_first) {
				errors.push_back(std::make_tuple(std::min(end_pos, start_pos), std::max(end_pos, start_pos), direction > 0 ? Error_IncorrectRightOperand : Error_IncorrectLeftOperand));
				return std::make_pair(-1, -1);
			}
			break;
		}
		end_pos += direction;
		is_first = false;
	}
	end_pos += direction;

	is_first = true;
	while (true) {
		if (end_pos == stop_pos || data[end_pos] == '\n' || data[end_pos] == '>' || data[end_pos] == '=') {
			if (is_first) {
				errors.push_back(std::make_tuple(std::min(end_pos, start_pos), std::max(end_pos, start_pos), direction > 0 ? Error_IncorrectRightOperand : Error_IncorrectLeftOperand));
				return std::make_pair(-1, -1);
			}
			return std::make_pair(start_pos, direction > 0 ? end_pos : end_pos + 1);
		}
		end_pos += direction;
		is_first = false;
	}

	return std::make_pair(-1, -1);
}

