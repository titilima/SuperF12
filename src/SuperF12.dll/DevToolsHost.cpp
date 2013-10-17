///////////////////////////////////////////////////////////////////////////////
// FileName:    DevToolsHost.cpp
// Created:     2013/07/20
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 开发者工具容器实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DevToolsHost.h"
#include <ShlGuid.h>

static const CLSID CLSID_IEDTExplorerBar = {
    0x1a6fe369, 0xf28c, 0x4ad9, { 0xa3, 0xe6, 0x2b, 0xcb, 0x50, 0x80, 0x7c, 0xf1 }
};

static const IID IID_IEDevTools = {
    0x181e3828, 0xfe6e, 0x4602, { 0xa3, 0x27, 0x78, 0x6a, 0x76, 0xfd, 0xfb, 0x3a }
};

static const IID IID_IEDevToolsOld = {
    0x059055df, 0x6bb0, 0x402a, { 0xba, 0x48, 0x58, 0xbf, 0xa3, 0x43, 0x71, 0x9c }
};

///////////////////////////////////////////////////////////////////////////////

static UINT WM_HTML_GETOBJECT = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));

CDevToolsHost::CDevToolsHost(HWND hTrident) : m_hTrident(hTrident)
{
    // IE 11
    CoCreateInstance(CLSID_IEDTExplorerBar, NULL, CLSCTX_INPROC_SERVER,
        IID_IEDevTools, reinterpret_cast<PVOID *>(&m_dt));
    if (NULL == m_dt) {
        // IE 8 ~ IE 10
        CoCreateInstance(CLSID_IEDTExplorerBar, NULL, CLSCTX_INPROC_SERVER,
            IID_IEDevToolsOld, reinterpret_cast<PVOID *>(&m_dt));

    }
}

STDMETHODIMP_(ULONG) CDevToolsHost::AddRef(void)
{
    return 1;
}

STDMETHODIMP CDevToolsHost::CloseTools(void)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDevToolsHost::DevToolsOpened(int *)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDevToolsHost::GetBrowser(IUnknown **unk)
{
    LRESULT lr = SendMessage(m_hTrident, WM_HTML_GETOBJECT, 0, 0);

    CComPtr<IServiceProvider> sp;
    HRESULT hr = ObjectFromLresult(lr, IID_IServiceProvider, 0, reinterpret_cast<PVOID *>(&sp));
    if (FAILED(hr))
        return hr;

    CComPtr<IWebBrowser2> wb2;
    hr = sp->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, reinterpret_cast<PVOID *>(&wb2));
    if (SUCCEEDED(hr))
        *unk = wb2.Detach();
    return hr;
}

void CDevToolsHost::Hide(void)
{
    m_dt->ShowDevTools(FALSE);
}

STDMETHODIMP CDevToolsHost::JITShowDevTools(int)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDevToolsHost::OnCloseDevTools(void)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDevToolsHost::OnLoseFocus(void)
{
    return S_OK;
}

STDMETHODIMP CDevToolsHost::PinUnPin(BOOL, int )
{
    return S_OK;
}

STDMETHODIMP CDevToolsHost::QueryInterface(REFIID riid, void **ppvObject)
{
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CDevToolsHost::Release(void)
{
    return 1;
}

HRESULT CDevToolsHost::Show(void)
{
    HRESULT hr = m_dt->SetDevToolSite(this);
    if (FAILED(hr))
        return hr;

    m_dt->SetPinState(FALSE);
    hr = m_dt->ShowDevTools(TRUE);
    if (SUCCEEDED(hr))
        hr = m_dt->DevToolsReady();

    CComQIPtr<IOleWindow> ow(m_dt);
    HWND hDevTools = NULL;
    ATLVERIFY(SUCCEEDED(ow->GetWindow(&hDevTools)));
    SetForegroundWindow(hDevTools);
    return hr;
}

int CDevToolsHost::v_HandleDelete(long *)
{
    return 0;
}
