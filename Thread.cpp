#include "Thread.h"
#include "ThreadPool.h"
using namespace std;
Thread::Thread() : m_thread(nullptr) , m_pauseFlag(false), m_stopFlag(false), m_state(Thread::State::none), m_isOver(false)
{
	m_thread = new thread(&Thread::Run, this);
}

Thread::Thread(const function<bool()>& runfunction) : Thread()
{
	SetRunFunction(runfunction);
}

Thread::~Thread()
{
	Stop();
}

void Thread::Start(const function<bool()>& runfunction)
{
	m_pauseFlag = false;
	m_stopFlag = false;
	m_state = Thread::State::runing;
	m_isOver = false;
	//m_thread = new thread(&Thread::Run, this);
	SetRunFunction(runfunction);
}
void Thread::Run()
{
	while (!m_stopFlag)
	{
		if (m_runFunction == nullptr)
		{
			unique_lock<mutex> locker(m_mtx);
			m_condition.wait(locker);
			locker.unlock();
		}
		bool isOver = m_runFunction();
		if (isOver)
		{
			threadpool->task.pop();
			threadpool->isFull = false;
			threadpool->cond.notify_one();
			threadpool->mp[this_thread::get_id()] = nullptr;
		}
		if (m_pauseFlag)
		{
			unique_lock<mutex> locker(m_mtx);
			while (m_pauseFlag)
			{
				m_condition.wait(locker);
			}
			locker.unlock();
		}
	}
	m_stopFlag = false;
	m_pauseFlag = false;
	m_isOver = true;
}

void Thread::Stop()
{
	if (m_thread == nullptr)
	{
		return;
	}
	m_stopFlag = true;
	m_pauseFlag = false;
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
	m_state = Thread::State::stop;
}

void Thread::Pause()
{
	if (m_thread == nullptr)
		return;
	m_state = Thread::State::pause;
	m_pauseFlag = true;
}

void Thread::Resume()
{
	if (m_thread == nullptr || m_state != Thread::State::pause)
		return;
	m_pauseFlag = false;
	m_condition.notify_all();
	m_state = Thread::State::runing;
}

void Thread::SetRunFunction(const function<bool()>& runFunction)
{
	m_runFunction = runFunction;
}

thread::id Thread::getThreadId()
{
	return m_thread->get_id();
}