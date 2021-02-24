#include"ThreadPool.h"
#include"Thread.h"
using namespace std;
ThreadPool::ThreadPool() :done(false), isEmpty(true), isFull(false), flag(false) {
}


//设置池中初始线程数
void ThreadPool::setSize(int num) {
    (*this).initnum = num;
}

//void ThreadPool::addTaskId(const Task& f)
//{
//    for (int32_t i = 0; i < N; ++i)
//    {
//        if (numId[i] == 0)
//        {
//            
//            //TaskId[(void*)f] = f;
//            numId[i] = i;
//        }
//    }
//    return;
//}
//添加任务
void ThreadPool::addTask(const Task& f) {

    if (!done) {
        //保护共享资源    
        unique_lock<mutex>lk(_mutex);

        //要是任务数量到了最大,就等待处理完再添加
        while (isFull) {
            cond.wait(lk);
        }

        //给队列中添加任务
        task.push(f);
        if (task.size() == initnum)
            isFull = true;

        for (int i = 0; i < threads.size(); ++i)
        {
            if (threads[i]->Getstate() == Thread::State::none)
            {
                function<bool()> runFunction = [=]() -> bool
                {
                    f();
                    return false;
                };
                mp[threads[i]->getThreadId()] = f;
                threads[i]->Start(runFunction);
                threads[i]->m_condition.notify_one();
                cout << threads[i]->getThreadId() << endl;
                break;
            }
        }
        lk.unlock();
    }
}

void ThreadPool::finish() {

    //线程池结束工作
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i]->Stop();
    }
    done = true;
}

//void ThreadPool::runTask() {
//
//    //不断遍历队列,判断要是有任务的话,就执行
//    while (!done) {
//
//        unique_lock<mutex>lk(_mutex);
//        //队列为空的话,就等待任务
//        while (isEmpty) {
//            cond.wait(lk);
//        }
//        thread::id a = std::this_thread::get_id();
//        Task ta;
//        //转移控制快,将左值引用转换为右值引用
//        ta = move(task.front());
//        task.pop();
//        mp[a] = ta;
//        if (task.empty()) {
//            isEmpty = true;
//        }
//        lk.unlock();
//        isFull = false;
//        ta();
//        cond.notify_one();
//    }
//    done = false;
//}

void ThreadPool::start(int num,ThreadPool *threadPool) {

    setSize(num);

    for (int i = 0; i < num; i++) {
        
        Thread* m_thread = new Thread(nullptr);
        m_thread->threadpool = threadPool;
        //cout << m_thread->getThreadId()<< endl;
        threads.push_back(m_thread);
    }
}

void ThreadPool::pause(const thread::id threadId)
{
    for (std::map<thread::id, Task> ::iterator it = mp.begin(); it != mp.end(); ++it)
    {
        for (int i = 0; i < N; ++i)
        {
            if (it->first == threads[i]->getThreadId())
            {
                threads[i]->Pause();
            }
        }
    }
}

void ThreadPool::resume(const thread::id threadId)
{
    for (std::map<thread::id, Task> ::iterator it = mp.begin(); it != mp.end(); ++it)
    {
        for (int i = 0; i < N; ++i)
        {
            if (it->first == threads[i]->getThreadId())
            {
                threads[i]->Resume();
            }
        }
    }
}
//void ThreadPool::stop(const Task& f)
//{
//    for (std::map<thread::id, Task> ::iterator it = mp.begin(); it != mp.end(); ++it)
//    {
//        if (&(it->second) == &(f))
//        {
//            for (int i = 0; i < N; ++i)
//            {
//                if (it->first == threads[i].get_id())
//                {
//                    threads[i].join();
//                    //numId[TaskId[f]] = 0;
//                }
//            }
//        }
//    }
//}
ThreadPool::~ThreadPool() {
}
