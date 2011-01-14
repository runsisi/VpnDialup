// VpnDialup2Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CVpnDialup2Dlg dialog
class CVpnDialup2Dlg : public CDialog
{
// Construction
public:
	CVpnDialup2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VPNDIALUP2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedCheck1();
private:
	CButton m_quietModeCheckBox;
public:
	afx_msg void OnClose();
	afx_msg LRESULT OnDialupTray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnVpnConnected(WPARAM wParam, LPARAM lParam);
};
