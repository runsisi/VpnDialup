// VpnDialup2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VpnDialup2.h"
#include "VpnDialup2Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")

NOTIFYICONDATAW g_notify = {0};
static const UINT uId = 0x4444;
static const UINT WMU_DIALUPTRAY = 
	::RegisterWindowMessageW(L"Netspeed_runsisi@HUST");
static const wchar_t* wszTip = L"网络加速器 by runsisi@HUST\n"
	L"http://www.pathtome.com/";
static const wchar_t* wszInfoTitle = L"runsisi@163.com";
static const wchar_t* wszInfo = L"网络加速器 by runsisi@HUST\n"
	L"http://www.pathtome.com/";

extern UINT WMU_VPNCONNECTED;
//////////////////////////////////////////////////////////////////////////


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
private:
	RUNSISI_HUST::CHyperLinkCtrl m_vendor;
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VENDOR, m_vendor);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVpnDialup2Dlg dialog




CVpnDialup2Dlg::CVpnDialup2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVpnDialup2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVpnDialup2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_quietModeCheckBox);
	DDX_Control(pDX, IDC_BLOG, m_blog);
}

BEGIN_MESSAGE_MAP(CVpnDialup2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CVpnDialup2Dlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_CHECK1, &CVpnDialup2Dlg::OnBnClickedCheck1)
ON_WM_CLOSE()
ON_REGISTERED_MESSAGE(WMU_DIALUPTRAY, &CVpnDialup2Dlg::OnDialupTray)
ON_BN_CLICKED(IDOK, &CVpnDialup2Dlg::OnBnClickedOk)
ON_WM_SIZE()
ON_REGISTERED_MESSAGE(WMU_VPNCONNECTED, &CVpnDialup2Dlg::OnVpnConnected)
ON_COMMAND(ID_POPUPMENU_EXIT, &CVpnDialup2Dlg::OnPopupmenuExit)
ON_COMMAND(ID_POPUPMENU_MAINWINDOW, &CVpnDialup2Dlg::OnPopupmenuMainwindow)
ON_COMMAND(ID_POPUPMENU_ABOUT, &CVpnDialup2Dlg::OnPopupmenuAbout)
END_MESSAGE_MAP()


// CVpnDialup2Dlg message handlers

BOOL CVpnDialup2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_quietModeCheckBox.SetCheck(BST_CHECKED);
	g_notify.cbSize = sizeof(g_notify);
	g_notify.hWnd = this->m_hWnd;
	g_notify.uID = uId;
	g_notify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
	g_notify.uCallbackMessage = WMU_DIALUPTRAY;
	g_notify.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	StringCchCopyW(g_notify.szTip, 63, wszTip);
	StringCchCopyW(g_notify.szInfo, 255, wszInfo);
	g_notify.uTimeout = 20;
	StringCchCopyW(g_notify.szInfoTitle, 63, wszInfoTitle);
	g_notify.dwInfoFlags = NIIF_INFO;

	m_blog.SetURL(TEXT("http://www.cppblog.com/runsisi"));
	m_blog.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_CURSOR));
	m_blog.CorrectLinkWidth();

	m_popupMenu.LoadMenu(IDR_POPUPMENU);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVpnDialup2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVpnDialup2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVpnDialup2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//////////////////////////////////////////////////////////////////////////
#include <process.h>

struct param_t
{
	bool bQuiet;
	HWND hMainWindow;
};

param_t g_params = {0};
extern unsigned __stdcall VpnThreadProc(void* p);	//Parameter determine if we use
																		//quiet dial mode.

void CVpnDialup2Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//Get quiet mode check box state.
	
	bool bQuiet = true;
	(m_quietModeCheckBox.GetCheck() == BST_CHECKED) ? 
		0 : bQuiet = false;
	g_params.bQuiet = bQuiet;
	g_params.hMainWindow = m_hWnd;
	HANDLE hThread = (HANDLE)_beginthreadex(0, 0, VpnThreadProc, 
		0, 0, 0);
	if (hThread)
	{
		CWnd* hConnect = GetDlgItem(IDC_BUTTON1);
		hConnect->EnableWindow(FALSE);
		CloseHandle(hThread);
		GetDlgItem(IDC_STATUS)->SetWindowText(L"Connecting...Pls wait:)");
	}
	else
	{
		::MessageBoxW(0, L"BUG? Pls contact me!", L"BUG?", 
			MB_OK | MB_ICONINFORMATION);
	}
}

//void CVpnDialup2Dlg::OnBnClickedCheck1()
//{
//	// TODO: Add your control notification handler code here
//}

void CVpnDialup2Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	::Shell_NotifyIconW(NIM_ADD, &g_notify);
	ShowWindow(SW_HIDE);

	//CDialog::OnClose();
}


LRESULT CVpnDialup2Dlg::OnDialupTray(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN)
	{
		::Shell_NotifyIconW(NIM_DELETE, &g_notify);
		ShowWindow(SW_SHOWNORMAL);
		::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 
			0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	else if (lParam == WM_RBUTTONUP)
	{
		POINT pt = {0};
		::GetCursorPos(&pt);
		m_popupMenu.GetSubMenu(0)->TrackPopupMenu(
			TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

	return 0;
}
void CVpnDialup2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	::Shell_NotifyIconW(NIM_DELETE, &g_notify);

	OnOK();
}

void CVpnDialup2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (nType == SIZE_MINIMIZED)
	{
		::Shell_NotifyIconW(NIM_ADD, &g_notify);
		ShowWindow(SW_HIDE);
		OnClose();
	}
}

LRESULT CVpnDialup2Dlg::OnVpnConnected(
									   WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)		//Failed.
	{
		GetDlgItem(IDC_STATUS)->SetWindowText(L"Canceled or Failed.");
	}
	else if (wParam == 1)
	{
		GetDlgItem(IDC_STATUS)->SetWindowText(L"Enjoy:)");
	}

	return 0;
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_vendor.SetURL(TEXT("http://www.pathtome.com"));
	m_vendor.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_CURSOR));
	m_vendor.CorrectLinkWidth();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CVpnDialup2Dlg::OnPopupmenuExit()
{
	// TODO: Add your command handler code here
	::Shell_NotifyIconW(NIM_DELETE, &g_notify);

	OnOK();
}

void CVpnDialup2Dlg::OnPopupmenuMainwindow()
{
	// TODO: Add your command handler code here
	::Shell_NotifyIconW(NIM_DELETE, &g_notify);
	ShowWindow(SW_SHOWNORMAL);
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 
		0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void CVpnDialup2Dlg::OnPopupmenuAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
