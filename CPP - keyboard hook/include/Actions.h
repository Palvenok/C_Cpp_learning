#include <functional>
#include <vector>
#include <algorithm>

template<typename... Args>
class Action {
public:
    using Handler = std::function<void(Args...)>;

    Action& operator+=(Handler h) 
    {
        handlers.push_back(std::move(h));
        return *this;
    }

    Action& operator-=(const Handler& h) 
    {
        auto it = std::remove_if(handlers.begin(), handlers.end(),
            [&](const Handler& stored) 
        {
            // Проверяем, что тип совпадает
            if (stored.target_type() != h.target_type())
                return false;

            // Если это указатель на функцию
            if (auto f1 = stored.template target<void(*)(Args...)>()) {
                if (auto f2 = h.template target<void(*)(Args...)>())
                    return *f1 == *f2;
            }
            // Если это захваченная лямбда без состояния (stateless) — тоже можно сравнить
            // Для stateful сравнение практически невозможно
            return false;
        });
        handlers.erase(it, handlers.end());
        return *this;
    }

    void Invoke(Args... args) const 
    {
        for (auto& h : handlers) 
            if (h) h(args...);
    }

    void Clear() 
    {
        handlers.clear();
    }

private:
    std::vector<Handler> handlers;
};