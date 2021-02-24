#include <iostream>
#include<Windows.h>
#include"ThreadPool.h"

using namespace std;
int i = 0;
ThreadPool p;
void func() {
    cout << "task finish" << "------>" << endl;
    //return false;
}

bool func_for()
{
    ++i;
    cout << i << endl;
    if (i == 1000)
    {
        p.pause(p.threads[0]->getThreadId());
    }

    return false;
} 
int main()
{

    p.start(N, &p);
    int i = 0;
    p.addTask(func_for);
    //p.addTask(func);
    //p.stop(func_for);
    while (1)
    {
        if (p.threads[0]->Getstate() == Thread::State::pause)
        {
            Sleep(500);
            p.resume(p.threads[0]->getThreadId());
        }
    }
    system("pause");
    return 0;
}
