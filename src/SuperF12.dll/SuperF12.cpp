///////////////////////////////////////////////////////////////////////////////
// FileName:    SuperF12.cpp
// Created:     2013/07/17
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: SuperF12.dll 导出函数实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "HookContext.h"

void WINAPI InitSuperF12(void)
{
    Globals::StopMonitor();
}

BOOL WINAPI ShowDevTools(HWND hTridentWindow)
{
    DWORD dwProcessID = 0;
    DWORD dwThreadID = GetWindowThreadProcessId(hTridentWindow, &dwProcessID);

    TCHAR szEvent[32];
    wsprintf(szEvent, _T("F12_PID_%d"), dwProcessID);
    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, szEvent);
    if (NULL != hEvent) {
        SharedParam param = { 0 };
        param.dwThreadID = dwThreadID;
        param.hWnd = hTridentWindow;
        Globals::PutParam(param, hEvent);
        CloseHandle(hEvent);
        return TRUE;
    }

    HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE, Globals::DummyHookProc,
        _Module.GetModuleInstance(), dwThreadID);
    if (NULL == hHook)
        return FALSE;

    wsprintf(szEvent, _T("F12_%d_Ready"), dwProcessID);
    HANDLE hEventReady = CreateEvent(NULL, TRUE, FALSE, szEvent);
    WaitForSingleObject(hEventReady, INFINITE);
    CloseHandle(hEventReady);

    wsprintf(szEvent, _T("F12_PID_%d"), dwProcessID);
    hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, szEvent);
    if (NULL != hEvent) {
        SharedParam param = { 0 };
        param.dwThreadID = dwThreadID;
        param.hWnd = hTridentWindow;
        Globals::PutParam(param, hEvent);
        CloseHandle(hEvent);
        return TRUE;
    } else {
        UnhookWindowsHookEx(hHook);
        return FALSE;
    }
}
