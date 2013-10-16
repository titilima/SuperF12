///////////////////////////////////////////////////////////////////////////////
// FileName:    AttachDlg.h
// Created:     2013/07/14
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: Attach ¶Ô»°¿ò
///////////////////////////////////////////////////////////////////////////////

#ifndef ATTACHDLG_H
#define ATTACHDLG_H

#pragma once

class CAttachDlg : public CDialogImpl<CAttachDlg>, public CWinDataExchange<CAttachDlg>
{
public:
    CAttachDlg(void);
    enum { IDD = IDD_DLG_ATTACH };
public:
    HWND TridentWindow(void) const { return m_hTrident; }
private:
    void ReleaseCapture(bool bCancelMode);
public:
    BEGIN_MSG_MAP_EX(CAttachDlg)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_RBUTTONDOWN(OnRButtonDown)

        COMMAND_ID_HANDLER_EX(IDCANCEL, OnOKCancel)
        COMMAND_ID_HANDLER_EX(IDOK, OnOKCancel)
    END_MSG_MAP()
    BEGIN_DDX_MAP(CAttachDlg)
        DDX_CONTROL_HANDLE(IDC_ST_ARROW, m_stArrow)
        DDX_CONTROL_HANDLE(IDC_ST_HWND, m_stHWND)
    END_DDX_MAP()
private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnRButtonDown(UINT nFlags, CPoint point);

    void OnOKCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
    bool m_bCapture;
    CCursorHandle m_cursor, m_oldCursor;
    CStatic m_stArrow, m_stHWND;
    HWND m_hTrident;
};

#endif // ATTACHDLG_H
