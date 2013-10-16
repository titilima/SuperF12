///////////////////////////////////////////////////////////////////////////////
// FileName:    Utility.cpp
// Created:     2013/07/14
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 工具类、函数实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Utility.h"

BOOL WINAPI EnableDlgItem(HWND hDlg, UINT id, BOOL bEnable)
{
    return ::EnableWindow(::GetDlgItem(hDlg, id), bEnable);
}

BOOL WINAPI GetRectInParent(HWND hChild, LPRECT rc)
{
    ATLASSERT(::IsWindow(hChild) && NULL != rc);

    HWND hParent = GetParent(hChild);
    if (NULL == hParent)
        hParent = ::GetDesktopWindow();

    // 将父窗口的左上角转换为屏幕坐标
    POINT ptParent = { 0, 0 };
    if (!::ClientToScreen(hParent, &ptParent))
        return FALSE;

    // 取窗口的屏幕坐标
    RECT rcCtrl;
    if (!::GetWindowRect(hChild, &rcCtrl))
        return FALSE;

    rc->left = rcCtrl.left - ptParent.x;
    rc->top = rcCtrl.top - ptParent.y;
    rc->right = rcCtrl.right - ptParent.x;
    rc->bottom = rcCtrl.bottom - ptParent.y;
    return TRUE;
}

void WINAPI HighlightWindow(HWND hWnd)
{
    ATLASSERT(NULL != hWnd && IsWindow(hWnd));

    CWindowDC dc(hWnd);

    CRect rc;
    ::GetWindowRect(hWnd, &rc);
    rc.OffsetRect(-rc.left, -rc.top);

    if (rc.IsRectEmpty())
        return;

    static const int DINV = 3;
    dc.PatBlt(rc.left, rc.top, rc.Width(), DINV, DSTINVERT);
    dc.PatBlt(rc.left, rc.bottom - DINV, DINV, -(rc.Height() - 2 * DINV), DSTINVERT);
    dc.PatBlt(rc.right - DINV, rc.top + DINV, DINV, rc.Height() - 2 * DINV, DSTINVERT);
    dc.PatBlt(rc.right, rc.bottom - DINV, -rc.Width(), DINV, DSTINVERT);
}

void WINAPI SetWindowIcon(HWND hWnd, HMODULE hMod, UINT idIcon)
{
    HICON hIconBig, hIconSmall;

    hIconBig = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(idIcon), IMAGE_ICON,
        GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
        LR_SHARED);
    hIconSmall = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(idIcon), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
        LR_SHARED);

    ::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIconBig);
    ::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);
}
