# mark

## 管理线程

1. 启动新线程
2. 等待线程与分离线程
3. 线程唯一标识符

异常安全问题 exception safety

std::thread

t.joinable();

t.join();

t.detach();

std::ref();

传递一个成员函数指针作为线程函数，并提供一个合适的对象指针作为第一个参数，也可以为成员函数提供参数，std::thread的第三个参数就是成员函数的第一个参数，提供的参数可以移动但是不能拷贝。原变量是一个临时变量时，自动进行移动操作，当原对象是一个命名变量时，需要使用std::move进行显示移动。

std::thread实例的可移动不可复制性。不可复制性保证了在同一个时间点，一个std::thread实例只能关联一个执行线程，可移动性是的程序员可以决定哪个实例拥有实际执行线程的所有权。

c++标准库中有很多资源占有类型，std::ifstream，std::unique_ptr，还有std::thread都是可移动，但不可拷贝。

需要在线程对象被析构前，显示的等待线程完成，或者分离他。进行赋值也需要满足这些条件（不能通过赋一个新值给std::thread对象的方式丢弃一个线程，否则std::terminate会被调用）

当所有权可以在函数内部传递，就允许std::thread实例可作为参数进行传递

std::thread::hardware_concurrency() 能同时并发在一个程序中的线程数量

std::thread::id

t.get_id();

std::this_thread::get_id()

## 线程间共享数据

1. 共享数据带来的问题
2. 使用互斥量保护数据
3. 数据保护的替代方案

数据竞争，一种特殊的条件竞争：并发的去修改一个独立对象，数据竞争是（可怕的）未定义行为的起因。

std::mutex

std::lock_guard

c++ 标准库为互斥量提供了一个RAII语法的模板类std::lock_guard，其会在构造的时候提供已锁的互斥量，并在析构的时候进行解锁，从而保证了一个已锁的互斥量总是会被正确的解锁

std::lock

std::adopt_lock: std::lock_guard 对象可获取锁之外，还将锁交由 std::lock_guard 对象管理，而不需要 std::lock_guard 对象再去构建新的锁

std::defer_lock

避免死锁:

1. 避免嵌套锁
2. 避免在持有锁时，调用用户提供的代码
3. 使用固定顺序获取锁
4. 使用锁的层次结构 比如 listing_3.8.cpp 中 hierarchical_mutex 的实现

thread_local

std::unique_lock vs std::lock_guard

std::once_flag

std::call_once

std::shared_mutex

std::shared_lock

std::recursive_mutex 嵌套锁，一般用在可并发访问的类上

## 同步并发操作

1. 等待事件
2. 带有期望的等待一次性事件
3. 在限定时间内等待
4. 使用同步操作简化代码

条件变量（condition variables）

期望（future）

std::condition_variable 和 std::condition_variable_any，两者都需要与一个互斥量一起才能工作（互斥量是为了同步）；前者仅限于与 std::mutex 一起工作，而后者可以和任何满足最低标准的互斥量一起工作。

使用队列在多个线程中转移数据是很常见的，做的好的话，同步操作可以限制在队列本身，
同步问题和条件竞争出现的改了也会降低。

cond.wait

cond.notify_one

cond.notify_all
