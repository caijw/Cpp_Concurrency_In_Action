#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

bool more_data_to_prepare()
{
    return false;
}

struct data_chunk
{};

data_chunk prepare_data()
{
    return data_chunk();
}

void process(data_chunk&)
{}

bool is_last_chunk(data_chunk&)
{
    return true;
}

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
        data_chunk const data=prepare_data();
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
}

void data_processing_thread()
{
    while(true)
    {
        // [by kingwei] wait() 会去检查这些条件，当条件满足时（lambda函数返回true）返回。
        // 如果条件不满足（返回false），wait 函数将解锁互斥量，
        // 并且将这个线程（处理数据的线程）处于阻塞或者等待状态。
        // 当准备数据的线程调用 notify_one() 通知条件变量时，
        // 处理数据的线程从睡眠状态中苏醒，重新获取互斥锁，
        // 并且对条件再次检查，在条件满足的情况下，从 wait() 返回并继续持有锁。
        // 当条件不满足时，线程将对互斥量解锁，并且重新开始等待。
        // 等待中的线程必须在等待期间解锁互斥量，并且在这之后对互斥量重新上锁，
        // std::lock_guard 没有那么灵活，
        // 这就是为什么用 std::unique_lock 而不使用 std::lock_guard 的原因
        // 如果在线程休眠期间报错锁住状态准备数据的线程将无法锁住互斥量，
        // 也无法添加数据到队列，同样的，等待线程也永远不会知道条件何时满足
        // 在调用 wait() 的过程中，一个条件变量可能会去检查给定条件若干次；
        // 然而，他总是在互斥量被锁定时这样做，并且当条件返回true，立即返回。
        // 当等待线程重新获取互斥量并检查条件时，如果它并非直接响应另一个线程
        // 的通知，这就是所谓的伪唤醒，任何伪唤醒的数量和频率都是不确定的。
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[]{return !data_queue.empty();});
        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main()
{
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}
