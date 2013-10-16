///////////////////////////////////////////////////////////////////////////////
// FileName:    Globals.cpp
// Created:     2013/07/15
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 单件管理器实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "HookContext.h"

static Globals *theGlobals = NULL;

#pragma data_seg("shared")
static DWORD g_dwParamThreadID = 0;
static HWND g_hParamWindow = NULL;
static HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:shared,rws")

Globals::Globals(void)
{
    theGlobals = this;
    m_uMsgNotify = RegisterWindowMessage(_T("WM_F12_NOTIFY"));

    m_hLock = CreateEvent(NULL, TRUE, TRUE, _T("F12_ParamLock"));

    TCHAR szEvent[32];
    wsprintf(szEvent, _T("F12_PID_%d"), GetCurrentProcessId());
    m_hEvent = CreateEvent(NULL, TRUE, FALSE, szEvent);

    m_hThread = CreateThread(NULL, 0, MonitorThread, NULL, 0, NULL);
}

Globals::~Globals(void)
{
    if (NULL != m_hThread) {
        SharedParam param = { 0 }; // Set TID to 0 to exit thread.
        PutParam(param);
        StopMonitor();
    }
    CloseHandle(m_hLock);

    theGlobals = NULL;
}

LRESULT CALLBACK Globals::DummyHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

CHookContext* Globals::GetContext(void)
{
    AutoReadLock lock(theGlobals->m_lock);
    map<DWORD, CHookContext *>::const_iterator it = theGlobals->m_ctxMap.find(GetCurrentThreadId());
    if (it != theGlobals->m_ctxMap.end())
        return it->second;
    else
        return NULL;
}

void Globals::GetParam(SharedParam *param)
{
    param->dwThreadID = g_dwParamThreadID;
    param->hWnd = g_hParamWindow;
    SetEvent(theGlobals->m_hLock);
}

void Globals::Init(void)
{
    new Globals;
}

bool Globals::IsF12Notify(UINT uMsg)
{
    return uMsg == theGlobals->m_uMsgNotify;
}

bool Globals::IsThreadAttached(DWORD dwThreadID)
{
    AutoReadLock lock(m_lock);
    map<DWORD, CHookContext *>::const_iterator it = m_ctxMap.find(dwThreadID);
    return it != m_ctxMap.end();
}

DWORD WINAPI Globals::MonitorThread(PVOID pvParam)
{
    TCHAR szEvent[32];
    wsprintf(szEvent, _T("F12_%d_Ready"), GetCurrentProcessId());
    HANDLE hReady = OpenEvent(EVENT_MODIFY_STATE, FALSE, szEvent);
    if (NULL == hReady)
        return EXIT_SUCCESS; // SuperF12.exe

    SetEvent(hReady);
    CloseHandle(hReady);

    for (;;) {
        WaitForSingleObject(theGlobals->m_hEvent, INFINITE);

        SharedParam param = { 0 };
        GetParam(&param);
        if (0 == param.dwThreadID)
            break;

        if (!theGlobals->IsThreadAttached(param.dwThreadID)) {
            AutoWriteLock lock(theGlobals->m_lock);
            theGlobals->m_ctxMap[param.dwThreadID] = new CHookContext(param.dwThreadID);
        }

        SendMessage(param.hWnd, theGlobals->m_uMsgNotify, Globals::ShowDevTools, 0);
        ResetEvent(theGlobals->m_hEvent);
    }

    return EXIT_SUCCESS;
}

void Globals::PutParam(const SharedParam &param, HANDLE hEvent)
{
    WaitForSingleObject(theGlobals->m_hLock, INFINITE);
    ResetEvent(theGlobals->m_hLock);

    g_dwParamThreadID = param.dwThreadID;
    g_hParamWindow = param.hWnd;

    if (NULL == hEvent)
        hEvent = theGlobals->m_hEvent;
    SetEvent(hEvent);
}

void Globals::StopMonitor(void)
{
    if (NULL != theGlobals->m_hThread) {
        WaitForSingleObject(theGlobals->m_hThread, INFINITE);

        CloseHandle(theGlobals->m_hThread);
        theGlobals->m_hThread = NULL;
        CloseHandle(theGlobals->m_hEvent);
        theGlobals->m_hEvent = NULL;
    }
}

void Globals::Uninit(void)
{
    delete theGlobals;
}
