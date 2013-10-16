///////////////////////////////////////////////////////////////////////////////
// FileName:    WinMain.cpp
// Created:     2013/07/12
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: ³ÌÐòÈë¿Ú
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "AttachDlg.h"

static const CLSID CLSID_IEDTExplorerBar = {
    0x1a6fe369, 0xf28c, 0x4ad9, { 0xa3, 0xe6, 0x2b, 0xcb, 0x50, 0x80, 0x7c, 0xf1 }
};

static bool IsDevToolsSupported(void)
{
    bool b = true;

    CComPtr<IUnknown> pUnk;
    if (FAILED(pUnk.CoCreateInstance(CLSID_IEDTExplorerBar, NULL, CLSCTX_INPROC_SERVER)))
        b = false;

    return b;
}

static int RunApp(int nShowCmd)
{
    CAttachDlg dlg;
    if (IDOK == dlg.DoModal(NULL)) {
        HWND hTrident = dlg.TridentWindow();
        if (NULL != hTrident)
            ShowDevTools(hTrident);
    }

    return EXIT_SUCCESS;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR lpCmdLine, int nShowCmd)
{
    CoInitialize(NULL);

    if (!IsDevToolsSupported()) {
        MessageBox(NULL, _T("F12 developer tools are not supported."), _T("Error"),
            MB_ICONERROR);
        CoUninitialize();
        return EXIT_FAILURE;
    }

    AtlInitCommonControls(ICC_WIN95_CLASSES);
    _Module.Init(NULL, hInstance);

    InitSuperF12();
    //theLogger.SetPrefix("F12");

    int ret = RunApp(nShowCmd);

    _Module.Term();

    CoUninitialize();
    return ret;
}
