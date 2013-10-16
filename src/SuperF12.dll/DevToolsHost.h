///////////////////////////////////////////////////////////////////////////////
// FileName:    DevToolsHost.h
// Created:     2013/07/20
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 开发者工具容器
///////////////////////////////////////////////////////////////////////////////

#ifndef DEVTOOLSHOST_H
#define DEVTOOLSHOST_H

#pragma once

class IEDevToolsSite : public IUnknown
{
public:
    STDMETHOD(GetBrowser)(IUnknown **) PURE;
    STDMETHOD(PinUnPin)(BOOL, int) PURE;
    STDMETHOD(OnCloseDevTools)(void) PURE;
    STDMETHOD(OnLoseFocus)(void) PURE;
    STDMETHOD(JITShowDevTools)(int) PURE;
    STDMETHOD(DevToolsOpened)(int *) PURE;
    STDMETHOD(CloseTools)(void) PURE;
    virtual int v_HandleDelete(long *) PURE;
};

class IEDevTools : public IUnknown
{
public:
    STDMETHOD(SetDevToolSite)(IEDevToolsSite *site) PURE;
    STDMETHOD(ShowDevTools)(BOOL bShow) PURE;
    STDMETHOD(JITShowDevTools)(int) PURE;
    STDMETHOD(CloseDevTools)(void) PURE;
    STDMETHOD(IsVisible)(BOOL *) PURE;
    STDMETHOD(GetPinState)(BOOL *bPin) PURE;
    STDMETHOD(SetPinState)(BOOL bPin) PURE;
    STDMETHOD(IsMinimized)(BOOL *) PURE;
    STDMETHOD(DevToolsReady)(void) PURE;
};

class CDevToolsHost : public IEDevToolsSite
{
public:
    CDevToolsHost(HWND hTrident);
public:
    void Hide(void);
    HRESULT Show(void);
public:
    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
    STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);
    // IEDevToolsSite
    STDMETHOD(GetBrowser)(IUnknown **) OVERRIDE;
    STDMETHOD(PinUnPin)(BOOL, int) OVERRIDE;
    STDMETHOD(OnCloseDevTools)(void) OVERRIDE;
    STDMETHOD(OnLoseFocus)(void) OVERRIDE;
    STDMETHOD(JITShowDevTools)(int) OVERRIDE;
    STDMETHOD(DevToolsOpened)(int *) OVERRIDE;
    STDMETHOD(CloseTools)(void) OVERRIDE;
    virtual int v_HandleDelete(long *) OVERRIDE;
private:
    HWND m_hTrident;
    CComPtr<IEDevTools> m_dt;
};

#endif // DEVTOOLSHOST_H
