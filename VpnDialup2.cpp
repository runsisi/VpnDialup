// VpnDialup2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VpnDialup2.h"
#include "VpnDialup2Dlg.h"

#include "VpnDialup/VpnDialup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVpnDialup2App

BEGIN_MESSAGE_MAP(CVpnDialup2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVpnDialup2App construction

CVpnDialup2App::CVpnDialup2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVpnDialup2App object

CVpnDialup2App theApp;


// CVpnDialup2App initialization

BOOL CVpnDialup2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Dialup_runsisi_HUST"));

	HANDLE hEvent = CreateEventW(0, 0, 0, L"RUNSISI_HUST_NETSPEED");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::MessageBoxW(0, L"已经运行了一个实例", 
			L"Oops!", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	//Time limit.
	SYSTEMTIME stm = {0};
	GetLocalTime(&stm);
	//Compare.
	if (stm.wYear != 2011 || stm.wMonth != 1)
	{
		::MessageBoxW(0, L"This is a test version, Contact me for more information!", 
			L"Expiration", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	

	RUNSISI_HUST::DeleteVpnEntry();

	CVpnDialup2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CVpnDialup2App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	using namespace RUNSISI_HUST;
	extern HRASCONN g_hRasConn;

	if (g_hRasConn)
	{
		DisconnectVpn(g_hRasConn);
	}
	DeleteVpnEntry();
	DeleteVpnPhoneBook();

	return CWinApp::ExitInstance();
}
