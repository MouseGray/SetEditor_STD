#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>

#include "ERedoUndoComponent.h"

class ETextComponent : public ERedoUndoComponent
{
	using Callback = std::function<void(size_t, size_t, ERedoUndoComponent::Type)>;
	std::string m_buffer;

	std::vector<Callback> m_eraseListeners;
	std::vector<Callback> m_eraseLineListeners;
	std::vector<Callback> m_insertListeners;
	std::vector<Callback> m_insertLineListeners;
public:
	enum class CallbackType {
		erase,
		eraseLine,
		insert,
		insertLine
	};

	inline auto begin() { return m_buffer.begin(); }
	inline auto end() { return m_buffer.end(); }

	inline const std::string& data() { return m_buffer; }

	void addCallback(Callback callback, CallbackType type);

	void notify(std::vector<Callback>& listeners, size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void erase(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	inline void erase(size_t begin, ERedoUndoComponent::Type ruType) { erase(begin, begin + 1, ruType); }

	void insert(size_t pos, const std::string& text, ERedoUndoComponent::Type ruType);

	std::string toCorrectText(const std::string& text);

	static bool isCorrectChar(const char c) noexcept;
};