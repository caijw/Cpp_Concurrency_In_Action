# mark

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

数据竞争，一种特殊的条件竞争：并发的去修改一个独立对象，数据竞争是（可怕的）未定义行为的起因。

std::mutex

std::lock_guard

c++ 标准库为互斥量提供了一个RAII语法的模板类std::lock_guard，其会在构造的时候提供已锁的互斥量，并在析构的时候进行解锁，从而保证了一个已锁的互斥量总是会被正确的解锁
