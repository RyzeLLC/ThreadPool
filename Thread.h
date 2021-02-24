#pragma once
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include<stdint.h>
#include<functional>
#include "ThreadPool.h"
using namespace std;
class ThreadPool;
class Thread
{
public:
	Thread();
	Thread(const function<bool()>& runfuction);
	~Thread();

	enum class State
	{
		none,
		stop,
		runing,
		pause
	};
	ThreadPool* threadpool;
	inline State Getstate() const { return m_state; };
	inline bool IsRunOver() const
	{
		return m_state == State::runing && m_isOver == true;
	}
	//inline thread::id getThreadId() const
	//{
	//	return this_thread::get_id();
	//}
	void Start(const function<bool()>& runFunction);
	void Stop();
	void Pause();
	void Resume();
	void SetRunFunction(const function<bool()> &runFunction);
	void Run();
	thread::id getThreadId();
	condition_variable m_condition;
	
private:
	thread* m_thread;
	mutex m_mtx;
	
	State m_state;
	function<bool()>m_runFunction;
	atomic_bool m_isOver;
	atomic_bool m_pauseFlag;
	atomic_bool m_stopFlag;
};

