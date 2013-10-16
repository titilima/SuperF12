///////////////////////////////////////////////////////////////////////////////
// FileName:    Globals.h
// Created:     2013/07/15
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 单件管理器
///////////////////////////////////////////////////////////////////////////////

#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once

#include "Lock.h"

class CHookContext;

struct SharedParam {
    DWORD dwThreadID;
    HWND hWnd;
};

class Globals
{
public:
    enum NotifyCode {
        ShowDevTools,
    };
private:
    Globals(void);
    ~Globals(void);
public:
    static LRESULT CALLBACK DummyHookProc(int code, WPARAM wParam, LPARAM lParam);
    static CHookContext* GetContext(void);
    static void GetParam(SharedParam *param);
    static void Init(void);
    static bool IsF12Notify(UINT uMsg);
    static void PutParam(const SharedParam &param, HANDLE hEvent = NULL);
    static void StopMonitor(void);
    static void Uninit(void);
private:
    bool IsThreadAttached(DWORD dwThreadID);
    static DWORD WINAPI MonitorThread(PVOID pvParam);
private:
    UINT m_uMsgNotify;
    HANDLE m_hLock, m_hEvent, m_hThread;

    Lock m_lock;
    map<DWORD, CHookContext *> m_ctxMap;
};

#define DECLARE_SINGLETON_HEADERS(Class)    \
    private:                                \
        Class(void);                        \
        Class(const Class& obj);            \
        ~Class(void);                       \
        friend class Globals;               \
        friend class std::auto_ptr<Class>

#endif // GLOBALS_H
