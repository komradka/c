#include <functional>

#pragma once

template<typename ... Args>
class Signal
{
private:
    std::function<void(Args ...)> connected_slot;

public:
    Signal() = default;

    void Connect(std::function<void(Args ...)> &&f)
    {
        connected_slot = std::move(f);
    }

    void Emit(Args&&... args)
    {
        connected_slot(std::forward<Args>(args)...);
    }


    void operator()(Args&& ... args)
    {
        connected_slot(std::forward<Args>(args)...);
    }
};