///////////////////////////////////////////////////////////////////////////////
// FileName:    HookContext.h
// Created:     2013/07/17
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 挂钩上下文
///////////////////////////////////////////////////////////////////////////////

#ifndef HOOKCONTEXT_H
#define HOOKCONTEXT_H

#pragma once

class CDevToolsHost;

class CHookContext
{
public:
    CHookContext(DWORD dwThreadID);
private:
    static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
    CDevToolsHost* GetDevToolsHost(HWND hTridentWindow);
    bool RemoveHost(HWND hWindow);
private:
    HHOOK m_hHook;
    map<HWND, CDevToolsHost *> m_map;
    // 显示开发者工具的 Trident 窗口收不到 WM_DESTROY,
    // 只得建立一个父窗口映射, 由父窗口代劳.
    map<HWND, HWND> m_parentMap;
};

#endif // HOOKCONTEXT_H
