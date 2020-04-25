#include <mutex>

class some_big_object
{};

void swap(some_big_object& lhs,some_big_object& rhs)
{}

class X
{
private:
    some_big_object some_detail;
    mutable std::mutex m;
public:
    X(some_big_object const& sd):some_detail(sd){}

    friend void swap(X& lhs, X& rhs)
    {
        if(&lhs==&rhs)
            return;
        std::lock(lhs.m,rhs.m); // [by kingwei]获取两个锁
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock); // [by kingwei]将依据得到的锁lhs.m交给 lock_guard 管理
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock); // [by kingwei]将依据得到的锁rhs.m交给 lock_guard 管理
        swap(lhs.some_detail, rhs.some_detail);
    }
};

int main()
{}
