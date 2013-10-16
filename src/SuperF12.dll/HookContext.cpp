///////////////////////////////////////////////////////////////////////////////
// FileName:    HookContext.cpp
// Created:     2013/07/17
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 挂钩上下文实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "HookContext.h"
#include "DevToolsHost.h"

CHookContext::CHookContext(DWORD dwThreadID)
{
    m_hHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, _Module.GetModuleInstance(),
        dwThreadID);
}

LRESULT CALLBACK CHookContext::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
    CHookContext *ctx = Globals::GetContext();
    ATLASSERT(NULL != ctx);

    PCWPSTRUCT cwp = reinterpret_cast<PCWPSTRUCT>(lParam);
    if (HC_ACTION == code) {
        if (Globals::IsF12Notify(cwp->message)) {
            switch (cwp->wParam)
            {
            case Globals::ShowDevTools:
                {
                    CDevToolsHost *host = ctx->GetDevToolsHost(cwp->hwnd);
                    host->Show();
                    break;
                }
            }
        } else if (WM_NCDESTROY == cwp->message) {
            ctx->RemoveHost(cwp->hwnd);
        }
    }

    ATLASSERT(NULL != ctx->m_hHook);
    return CallNextHookEx(ctx->m_hHook, code, wParam, lParam);
}

CDevToolsHost* CHookContext::GetDevToolsHost(HWND hTridentWindow)
{
    map<HWND, CDevToolsHost *>::const_iterator it = m_map.find(hTridentWindow);
    if (it != m_map.end())
        return it->second;

    CDevToolsHost *host = new CDevToolsHost(hTridentWindow);
    m_map[hTridentWindow] = host;
    m_parentMap[GetParent(hTridentWindow)] = hTridentWindow;
    return host;
}

bool CHookContext::RemoveHost(HWND hWindow)
{
    map<HWND, HWND>::const_iterator it = m_parentMap.find(hWindow);
    if (it == m_parentMap.end())
        return false;

    CDevToolsHost *host = m_map[it->second];
    host->Hide();
    delete host;

    m_map.erase(it->second);
    m_parentMap.erase(hWindow);
    return true;
}
