#pragma once

#include <iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<functional>
#include<queue>
#include<map>
#include"Thread.h"
#define N 2
using namespace std;
class Thread;
class ThreadPool {

public:
    //自定义void()的函数类型
    typedef function<bool()>Task;

    ThreadPool();
    ~ThreadPool();

public:
    size_t initnum;
    //线程数组
    vector<Thread*>threads;

    //任务队列
    std::queue<Task>task;
    
    std::map<thread::id, Task>mp;
    //互斥锁条件变量
    mutex _mutex;
    condition_variable cond;

    //线程池工作结束时为真
    bool done;

    //队列是否为空
    bool isEmpty;
    //队列是否为满
    bool isFull;

    bool flag;

    inline void FreeCond()
    {
        cond.notify_one();
    }
public:
    void addTask(const Task& f);
    void start(int num, ThreadPool* threadpool);
    void setSize(int num);
    void runTask();
    void finish();
    void stop(const Task& f);
    void addTaskId(const Task& f);
    void pause(const thread::id threadId);
    void resume(const thread::id threadId);
};
