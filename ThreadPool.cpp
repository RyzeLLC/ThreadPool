#include"ThreadPool.h"
#include"Thread.h"
using namespace std;
ThreadPool::ThreadPool() :done(false), isEmpty(true), isFull(false), flag(false) {
}


//���ó��г�ʼ�߳���
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
//�������
void ThreadPool::addTask(const Task& f) {

    if (!done) {
        //����������Դ    
        unique_lock<mutex>lk(_mutex);

        //Ҫ�����������������,�͵ȴ������������
        while (isFull) {
            cond.wait(lk);
        }

        //���������������
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

    //�̳߳ؽ�������
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i]->Stop();
    }
    done = true;
}

//void ThreadPool::runTask() {
//
//    //���ϱ�������,�ж�Ҫ��������Ļ�,��ִ��
//    while (!done) {
//
//        unique_lock<mutex>lk(_mutex);
//        //����Ϊ�յĻ�,�͵ȴ�����
//        while (isEmpty) {
//            cond.wait(lk);
//        }
//        thread::id a = std::this_thread::get_id();
//        Task ta;
//        //ת�ƿ��ƿ�,����ֵ����ת��Ϊ��ֵ����
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
