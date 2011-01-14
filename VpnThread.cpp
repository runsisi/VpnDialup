#include <process.h>
#include "VpnDialup/VpnDialup.h"
#include "Exception/Exception.h"
#include "Logging/Logging.h"
#include "ExitWindows/ExitWindows.h"

unsigned __stdcall VpnThreadProc(void* p);
HRASCONN g_hRasConn = 0;	//VPN connection.

struct param_t
{
	bool bQuiet;
	HWND hMainWindow;
};

extern param_t g_params;

UINT WMU_VPNCONNECTED = 
	RegisterWindowMessageW(L"RUNSISI_HUST_VPNCONNECTED");

unsigned __stdcall VpnThreadProc(void* p)
{
	using namespace RUNSISI_HUST;

	try
	{
		RASENTRYW RasEntry = {0};

		LONG lRet = DisableIpSec();
		if (lRet != ERROR_SUCCESS)
		{
			if (lRet != ERROR_ALREADY_EXISTS)
			{
				throw CRunsisiExceptionW(L"Failed to disable IPSec, code: %d.", 
					lRet);
			}
		}
		else	//Reboot.
		{
			int iRet = ::MessageBoxW(0, L"Reboot is needed, reboot it now?", 
				L"REBOOT", MB_YESNO | MB_ICONINFORMATION);
			(iRet == IDYES) ? ExitWindowsSystem(
				ExitWindowsType::ExitWindowsType(
				ExitWindowsType::REBOOT | ExitWindowsType::FORCE)) : 
			(throw CRunsisiExceptionW(
				L"Reboot canceled, it will take effect next time."));
		}		

		lRet = InitVpnEntry(RasEntry);
		if (lRet != ERROR_SUCCESS)
		{
			throw CRunsisiExceptionW(L"Failed to initialize VPN entry, code: %d.", 
				lRet);
		}
		lRet = CreateVpnEntry(RasEntry);
		if (lRet != ERROR_SUCCESS)
		{
			throw CRunsisiExceptionW(L"Failed to create VPN entry, code: %d.", 
				lRet);
		}	

		lRet = ConnectVpn(g_hRasConn, g_params.bQuiet);
		if (lRet != ERROR_SUCCESS)
		{
			//Notify main window.
			PostMessageW(g_params.hMainWindow, WMU_VPNCONNECTED, 0, 0);
			throw CRunsisiExceptionW(L"Failed to connect VPN server, code: %d.", 
				lRet);
		}
		//Notify main window.
		PostMessageW(g_params.hMainWindow, WMU_VPNCONNECTED, 1, 0);
	}
	catch (CRunsisiExceptionW& e)
	{
		ERRW(e.what());
	}

	return 0;
}