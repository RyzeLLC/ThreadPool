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
    //�Զ���void()�ĺ�������
    typedef function<bool()>Task;

    ThreadPool();
    ~ThreadPool();

public:
    size_t initnum;
    //�߳�����
    vector<Thread*>threads;

    //�������
    std::queue<Task>task;
    
    std::map<thread::id, Task>mp;
    //��������������
    mutex _mutex;
    condition_variable cond;

    //�̳߳ع�������ʱΪ��
    bool done;

    //�����Ƿ�Ϊ��
    bool isEmpty;
    //�����Ƿ�Ϊ��
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
