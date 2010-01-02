#define _WIN32_DCOM
#include "mutex.h"

// *************************
//		SimpleMutex
// *************************

SimpleMutex::SimpleMutex()
{
#ifdef _WIN32
	InitializeCriticalSectionAndSpinCount(&m_criticalSection, 0x80000020);
#else
	pthread_mutexattr_init(&m_criticalSectionAttr);
	pthread_mutexattr_settype(&m_criticalSectionAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&m_criticalSection, &m_criticalSectionAttr);
#endif
}

SimpleMutex::~SimpleMutex()
{
#ifdef _WIN32
	DeleteCriticalSection(&m_criticalSection);
#else
	pthread_mutexattr_destroy(&m_criticalSectionAttr);
	pthread_mutex_destroy(&m_criticalSection);
#endif
}

//lock
void SimpleMutex::lock()
{
#ifdef _WIN32
	EnterCriticalSection(&m_criticalSection);
#else
	pthread_mutex_lock(&m_criticalSection);
#endif
}

//try lock
bool SimpleMutex::tryLock()
{
#ifdef _WIN32
	return TryEnterCriticalSection(&m_criticalSection) == TRUE;
#else
	return pthread_mutex_trylock(&m_criticalSection) == 0;
#endif
}

//unlock
void SimpleMutex::unlock()
{
#ifdef _WIN32
	LeaveCriticalSection(&m_criticalSection);
#else
	pthread_mutex_unlock(&m_criticalSection);
#endif
}

// ****************************
//		SimpleThreadLock
// ****************************

SimpleThreadLock::SimpleThreadLock(SimpleMutex &mutex) : m_mutex(mutex), m_locked(true)
{
	mutex.lock();
}

//the destructor
SimpleThreadLock::~SimpleThreadLock()
{
	m_mutex.unlock();
}

//report the state of locking when used as a boolean
SimpleThreadLock::operator bool() const
{
	return m_locked;
}

// ****************************
//		ManualThreadLock
// ****************************

ManualThreadLock::ManualThreadLock() : m_mutex(NULL), m_locked(false)
{
}

ManualThreadLock::ManualThreadLock(SimpleMutex * mutex) : m_locked(false)
{
	setMutex(mutex);
}

//the destructor
ManualThreadLock::~ManualThreadLock()
{
	if (m_mutex != NULL)
		doUnlock();
}

void ManualThreadLock::setMutex(SimpleMutex * mutex)
{
	m_mutex = mutex;
}

//report the state of locking when used as a boolean
ManualThreadLock::operator bool() const
{
	return m_locked;
}

void ManualThreadLock::doLock()
{
	m_mutex->lock();
	m_locked = true;
}

bool ManualThreadLock::doTryLock()
{
	if (m_mutex->tryLock() == false)
		return false;

	m_locked = true;
	return true;
}

void ManualThreadLock::doUnlock()
{
	m_locked = false;
	m_mutex->unlock();
}