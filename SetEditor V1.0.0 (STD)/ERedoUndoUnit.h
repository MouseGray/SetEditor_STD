#pragma once
#include <functional>

class ERedoUndoUnit
{
	std::function<void()> m_function;
public:
    template<typename Obj, typename F, typename ...Args>
	ERedoUndoUnit(Obj& obj, F f, Args&&... args) {
        m_function = [&obj, f, args...]()
        {
            (obj.*f)(args...);
        };
    }
    void release() {
        m_function();
    }
};

