// VpnDialup2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVpnDialup2App:
// See VpnDialup2.cpp for the implementation of this class
//

class CVpnDialup2App : public CWinApp
{
public:
	CVpnDialup2App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CVpnDialup2App theApp;