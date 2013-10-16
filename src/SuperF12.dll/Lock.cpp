#include "Common.h"
#include "Lock.h"

Lock::Lock(PCTSTR pszName) : m_cntReaders(0), m_hDataLock(CreateSemaphore(NULL, 1, 1, pszName))
{
    ::InitializeCriticalSection(&m_cs);
}

Lock::~Lock(void)
{
    CloseHandle(m_hDataLock);
    ::DeleteCriticalSection(&m_cs);
}

void Lock::AcquireForReading(void)
{
    EnterCS();
    if (1 == ++m_cntReaders)
    {
        WaitForSingleObject(m_hDataLock, INFINITE);
    }
    LeaveCS();
}

void Lock::AcquireForWriting(void)
{
    WaitForSingleObject(m_hDataLock, INFINITE);
}

void Lock::EnterCS(void)
{
    ::EnterCriticalSection(&m_cs);
}

void Lock::LeaveCS(void)
{
    ::LeaveCriticalSection(&m_cs);
}

void Lock::ReleaseForReading(void)
{
    EnterCS();
    if (0 == --m_cntReaders)
        ReleaseSemaphore(m_hDataLock, 1, NULL);
    LeaveCS();
}

void Lock::ReleaseForWriting(void)
{
    ReleaseSemaphore(m_hDataLock, 1, NULL);
}
