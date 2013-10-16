///////////////////////////////////////////////////////////////////////////////
// FileName:    AttachDlg.cpp
// Created:     2013/07/14
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: Attach 对话框实现
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "AttachDlg.h"
#include "Utility.h"

CAttachDlg::CAttachDlg(void) : m_bCapture(false), m_hTrident(NULL)
{
    m_cursor.LoadCursor(IDC_CUR_F12);
}

void CAttachDlg::ReleaseCapture(bool bCancelMode)
{
    ::ReleaseCapture();
    m_bCapture = false;

    m_stArrow.SetCursor(m_cursor);
    SetCursor(m_oldCursor);

    if (NULL != m_hTrident) {
        HighlightWindow(m_hTrident);
        if (bCancelMode)
            m_hTrident = NULL;
    }
    EnableDlgItem(*this, IDOK, NULL != m_hTrident);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CAttachDlg::OnInitDialog(CWindow, LPARAM)
{
    SetWindowIcon(*this, _Module.GetResourceInstance(), IDI_ICON_MAIN);

    DoDataExchange(FALSE);

    m_stArrow.SetCursor(m_cursor);
    m_stHWND.ShowWindow(SW_HIDE);
    EnableDlgItem(*this, IDOK, FALSE);
    return FALSE;
}

void CAttachDlg::OnLButtonDown(UINT, CPoint point)
{
    CRect rcArrow;
    GetRectInParent(m_stArrow, &rcArrow);
    if (!::PtInRect(&rcArrow, point)) {
        SetMsgHandled(FALSE);
        return;
    }

    if (NULL != m_hTrident) {
        m_hTrident = NULL;
        m_stHWND.ShowWindow(SW_HIDE);
        EnableDlgItem(*this, IDOK, FALSE);
    }

    m_stArrow.SetIcon(NULL);
    m_oldCursor = SetCursor(m_cursor);

    m_bCapture = true;
    SetCapture();
}

void CAttachDlg::OnLButtonUp(UINT, CPoint)
{
    if (m_bCapture)
        ReleaseCapture(false);
    else
        SetMsgHandled(FALSE);
}

void CAttachDlg::OnMouseMove(UINT, CPoint point)
{
    do {
        if (!m_bCapture)
            break;

        ClientToScreen(&point);
        HWND hWnd = ::WindowFromPoint(point);
        if (NULL == hWnd || m_hTrident == hWnd)
            break;

        if (NULL != m_hTrident) {
            HighlightWindow(m_hTrident);
            m_hTrident = NULL;
            m_stHWND.ShowWindow(SW_HIDE);
        }

        TCHAR tzClass[32];
        ::GetClassName(hWnd, tzClass, ARRAYSIZE(tzClass));
        if (0 != lstrcmp(tzClass, _T("Internet Explorer_Server")))
            break;

        m_hTrident = hWnd;
        HighlightWindow(m_hTrident);

        TCHAR tzHWND[32];
        wsprintf(tzHWND, _T("Window Handle: 0x%08X"), m_hTrident);
        m_stHWND.SetWindowText(tzHWND);
        m_stHWND.ShowWindow(SW_SHOW);
        return;
    } while (false);

    SetMsgHandled(FALSE);
}

void CAttachDlg::OnOKCancel(UINT, int nID, CWindow)
{
    EndDialog(nID);
}

void CAttachDlg::OnRButtonDown(UINT, CPoint)
{
    if (m_bCapture)
        ReleaseCapture(true);
    else
        SetMsgHandled(FALSE);
}
