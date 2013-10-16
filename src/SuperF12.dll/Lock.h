#ifndef LOCK_H
#define LOCK_H

#pragma once

class Lock
{
public:
    Lock(PCTSTR pszName = NULL);
    ~Lock(void);
public:
    void AcquireForReading(void);
    void AcquireForWriting(void);
    void AssertIsLocked(void);
    void ReleaseForReading(void);
    void ReleaseForWriting(void);
private:
    void EnterCS(void);
    void LeaveCS(void);
private:
    CRITICAL_SECTION m_cs;
    size_t m_cntReaders;
    HANDLE m_hDataLock;
};

class AutoReadLock
{
public:
    AutoReadLock(Lock& lock) : m_lock(lock)
    {
        m_lock.AcquireForReading();
    }
    ~AutoReadLock(void)
    {
        m_lock.ReleaseForReading();
    }
private:
    Lock& m_lock;
};

class AutoWriteLock
{
public:
    AutoWriteLock(Lock& lock) : m_lock(lock)
    {
        m_lock.AcquireForWriting();
    }
    ~AutoWriteLock(void)
    {
        m_lock.ReleaseForWriting();
    }
private:
    Lock& m_lock;
};

#endif // LOCK_H
