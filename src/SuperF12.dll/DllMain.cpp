///////////////////////////////////////////////////////////////////////////////
// FileName:    DllMain.cpp
// Created:     2013/07/15
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: Ä£¿éÈë¿Ú
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Detours\detours.h"

typedef HMODULE (WINAPI * FNLoadLibraryW)(PCWSTR);

static FNLoadLibraryW RawLoadLibraryW = NULL;

static HMODULE WINAPI HookLoadLibraryW(PCWSTR lpLibFileName)
{
    PCWSTR pwzName = wcsrchr(lpLibFileName, L'\\');
    if (NULL != pwzName)
        ++pwzName;
    else
        pwzName = lpLibFileName;

    static const PCWSTR pwzDlls[] = {
        L"jsdbgui.dll", L"jsprofilerui.dll", L"networkinspection.dll"
    };

    for (size_t i = 0; i < ARRAYSIZE(pwzDlls); ++i) {
        if (0 == _wcsicmp(pwzName, pwzDlls[i])) {
            static WCHAR wzIEPath[MAX_PATH] = { 0 };
            if (L'\0' == wzIEPath[0]) {
                CStaticDataInitCriticalSectionLock lock;
                ATLVERIFY(SUCCEEDED(lock.Lock()));
                if (L'\0' == wzIEPath[0]) {
                    HMODULE hDT = GetModuleHandle(_T("iedvtool.dll"));
                    ATLASSERT(NULL != hDT);
                    GetModuleFileName(hDT, wzIEPath, MAX_PATH);
                    *(wcsrchr(wzIEPath, L'\\') + 1) = L'\0';
                }
                lock.Unlock();
            }

            wstring wsDll(wzIEPath);
            wsDll.append(pwzName);
            return RawLoadLibraryW(wsDll.c_str());
        }
    }

    return RawLoadLibraryW(lpLibFileName);
}

BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD dwReason, PVOID)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            _Module.Init(NULL, hDllHandle);

            HMODULE hKernel32 = GetModuleHandle(_T("kernel32.dll"));
            RawLoadLibraryW = reinterpret_cast<FNLoadLibraryW>(GetProcAddress(hKernel32,
                "LoadLibraryW"));
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(reinterpret_cast<PVOID *>(&RawLoadLibraryW), HookLoadLibraryW);
            DetourTransactionCommit();

            Globals::Init();
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            Globals::Uninit();

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourDetach(reinterpret_cast<PVOID *>(&RawLoadLibraryW), HookLoadLibraryW);
            DetourTransactionCommit();

            _Module.Term();
            break;
        }
    }
    return TRUE;
}
