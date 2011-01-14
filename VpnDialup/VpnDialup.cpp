#pragma warning(disable:4995)
#include "VpnDialup.h"
#pragma comment(lib,"rasapi32.lib")
#include <RasDlg.h>
#pragma comment(lib,"rasdlg.lib")
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")
#include "../Logging/Logging.h"
#include "../Exception/Exception.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#define RUNSISI_HUST_VPN_PHONE_BOOK_NAME \
	L"RUNSISI_HUST_VpnPhoneBook.book"

namespace RUNSISI_HUST
{
	LONG GetVpnPhoneBookPath(wchar_t wszPath[], int size)
	{
		LONG lRet = ERROR_SUCCESS;

		wchar_t wszTempDir[MAX_PATH] = {0};
		if (!GetTempPathW(MAX_PATH, wszTempDir))
		{
			lRet = GetLastError();
			return lRet;
		}
		if (!PathAppendW(wszTempDir, RUNSISI_HUST_VPN_PHONE_BOOK_NAME))
		{
			lRet = ERROR_INVALID_PARAMETER;
			return lRet;
		}
		HRESULT hr = StringCchCopyW(wszPath, size, wszTempDir);
		if (FAILED(hr))
		{
			lRet = hr;
		}

		return lRet;
	}

	LONG DeleteVpnPhoneBook()
	{
		LONG lRet = ERROR_SUCCESS;

		//Get phone book path.
		wchar_t wszPhoneBook[MAX_PATH] = {0};
		if (GetVpnPhoneBookPath(wszPhoneBook, MAX_PATH) == 
			ERROR_SUCCESS)	//Failed or did not implemented.
		{
			if (!DeleteFileW(wszPhoneBook))
			{
				lRet = GetLastError();
			}
		}

		return lRet;
	}

	LONG InitVpnEntry(RASENTRYW& entry, const wchar_t* wszAddr)
	{
		LONG lRet = ERROR_SUCCESS;

		try
		{
			entry.dwSize = sizeof(entry);
			entry.dwfOptions = RASEO_RemoteDefaultGateway | 
				RASEO_RequirePAP | 
				RASEO_RequireCHAP | 
				RASEO_RequireMsCHAP | 
				RASEO_RequireMsCHAP2 | 
				RASEO_ShowDialingProgress | 
				RASEO_PreviewUserPw | 
				RASEO_ModemLights | 
				RASEO_SwCompression;
			entry.dwCountryID = 1;
			entry.dwCountryCode = 1;
			HRESULT hr = StringCchCopyW(entry.szLocalPhoneNumber, 
				RAS_MaxPhoneNumber, wszAddr);
			if (FAILED(hr))
			{
				lRet = hr;
				throw CRunsisiExceptionW(L"Failed to initialize RASENTRY "
					L"member szLocalPhoneNumber, code: %d.", hr);
			}
			entry.dwfNetProtocols = RASNP_Ip;
			entry.dwFramingProtocol = RASFP_Ppp;	//RASFP_Slip;	 //RASFP_Ras;
			hr = StringCchCopyW(entry.szDeviceType, RAS_MaxDeviceType, 
				RASDT_Vpn);
			if (FAILED(hr))
			{
				lRet = hr;
				throw CRunsisiExceptionW(L"Failed to initialize RASENTRY "
					L"member szDeviceType, code: %d.", hr);
			}
			entry.dwDialMode = RASEDM_DialAll;
			entry.dwDialExtraPercent = 75;
			entry.dwDialExtraSampleSeconds = 120;
			entry.dwHangUpExtraPercent = 10;
			entry.dwHangUpExtraSampleSeconds = 120;
			entry.dwIdleDisconnectSeconds = RASIDS_UseGlobalValue;	//RASIDS_Disabled;
			//Win2k
			entry.dwType = RASET_Vpn;
			entry.dwEncryptionType = ET_None;
			entry.dwVpnStrategy = VS_L2tpOnly;
			//WinXP
			entry.dwfOptions2 = RASEO2_ReconnectIfDropped | 
				RASEO2_DontNegotiateMultilink;
			entry.dwRedialCount = 3;
			entry.dwRedialPause = 60;
		}
		catch (CRunsisiExceptionW& e)
		{
			ERRW(e.what());
		}

		return lRet;
	}

	LONG CreateVpnEntry(RASENTRY& entry, const wchar_t* wszEntryName)
	{
		LONG lRet = ERROR_SUCCESS;

		try
		{
			//Validate.
			DWORD dwRet = RasValidateEntryNameW(0, wszEntryName);
			if (dwRet != ERROR_SUCCESS)
			{
				lRet = dwRet;
				if (dwRet == ERROR_ALREADY_EXISTS)
				{
					throw CRunsisiExceptionW(L"Connection name %s already exists.", 
						wszEntryName);
				}
				else
				{
					throw CRunsisiExceptionW(L"Invalid connection name, code: %d.", 
						dwRet);
				}
			}
			//Get phone book path.
			wchar_t wszPhoneBook[MAX_PATH] = {0};
			if (GetVpnPhoneBookPath(wszPhoneBook, MAX_PATH) != 
				ERROR_SUCCESS)	//Failed or did not implemented.
			{
				wszPhoneBook[0] = 0;
			}
			//Create.
			dwRet = RasSetEntryPropertiesW(wszPhoneBook[0] ? wszPhoneBook : 0, 
				wszEntryName, &entry, entry.dwSize, 0, 0);
			if (dwRet != ERROR_SUCCESS)
			{
				lRet = dwRet;
				throw CRunsisiExceptionW(L"Failed to create VPN entry, code: %d.", 
					dwRet);
			}
		}
		catch (CRunsisiExceptionW& e)
		{
			ERRW(e.what());
		}

		return lRet;
	}

	LONG DeleteVpnEntry(const wchar_t* wszEntryName)
	{
		LONG lRet = ERROR_SUCCESS;

		DWORD dwRet = RasDeleteEntryW(0, wszEntryName);
		if (dwRet != ERROR_SUCCESS)
		{
			lRet = dwRet;
			ERRW(L"Failed to delete VPN entry %s, code: %d.", 
				wszEntryName, dwRet);
		}

		return lRet;
	}

	LONG ConnectVpn(HRASCONN& hVpnConn, 
		bool bQuiet,
		const wchar_t* wszEntryName, 
		const wchar_t* wszUserName, 
		const wchar_t* wszPassword)
	{
		LONG lRet = ERROR_SUCCESS;

		try
		{
			RASDIALPARAMS dialParams = {0};
			dialParams.dwSize = sizeof(dialParams);
			HRESULT hr = StringCchCopyW(dialParams.szEntryName, 
				RAS_MaxEntryName, wszEntryName);
			if (FAILED(hr))
			{
				lRet = hr;
				throw CRunsisiExceptionW(L"Failed to initialize RASDIALPARAMS "
					L"member szEntryName, code: %d.", hr);
			}
			hr = StringCchCopyW(dialParams.szUserName, UNLEN, wszUserName);
			if (FAILED(hr))
			{
				lRet = hr;
				throw CRunsisiExceptionW(L"Failed to initialize RASDIALPARAMS "
					L"member szEntryName, code: %d.", hr);
			}
			hr = StringCchCopyW(dialParams.szPassword, UNLEN, wszPassword);
			if (FAILED(hr))
			{
				lRet = hr;
				throw CRunsisiExceptionW(L"Failed to initialize RASDIALPARAMS "
					L"member szPassword, code: %d.", hr);
			}
			//Get phone book path.
			wchar_t wszPhoneBook[MAX_PATH] = {0};
			if (GetVpnPhoneBookPath(wszPhoneBook, MAX_PATH) != 
				ERROR_SUCCESS)	//Failed or did not implemented.
			{
				wszPhoneBook[0] = 0;
			}
			//Dial.
			if (bQuiet)
			{
				DWORD dwRet = RasDialW(0, wszPhoneBook[0] ? wszPhoneBook : 0, 
					&dialParams, 0, 0, &hVpnConn);
				lRet = dwRet;
			}
			else
			{
				RASDIALDLG rasDialDlg = {0};
				rasDialDlg.dwSize = sizeof(rasDialDlg);
				wchar_t wszEntry[MAX_PATH] = {0};
				HRESULT hr = StringCchCopyW(wszEntry, MAX_PATH, wszEntryName);
				if (FAILED(hr))
				{
					lRet = hr;
					throw CRunsisiExceptionW(L"Failed to copy entry name to "
						L"stack char array, code: %d.", lRet);
				}
				BOOL bRet = RasDialDlgW(wszPhoneBook[0] ? wszPhoneBook : 0, wszEntry, 0, &rasDialDlg);
				if (!bRet)	//Error.
				{
					lRet = ERROR_TIMEOUT;
				}
			}
		}
		catch (CRunsisiExceptionW& e)
		{
			ERRW(e.what());
		}
		
		return lRet;
	}

	LONG DisconnectVpn(HRASCONN hRasConn)
	{
		LONG lRet = ERROR_SUCCESS;

		DWORD dwRet = RasHangUpW(hRasConn);
		if (dwRet != ERROR_SUCCESS)
		{
			lRet = dwRet;
			ERRW(L"Failed to terminate the VPN connection, code %d.", 
				dwRet);
		}

		return lRet;
	}

	LONG DisableIpSec()
	{
		LONG lRet = ERROR_SUCCESS;

		HKEY hkeyIpSec = 0;
		const wchar_t* wszIpSecKey = L"SYSTEM\\CurrentControlSet"
			L"\\Services\\RasMan\\Parameters";

		try
		{
			//Open.
			lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, wszIpSecKey, 0, 
				KEY_SET_VALUE | KEY_QUERY_VALUE, &hkeyIpSec);
			if (lRet != ERROR_SUCCESS)
			{
				throw CRunsisiExceptionW(L"Failed to open IPSec key, code: %d.", 
					lRet);
			}
			//Query.
			DWORD dwIpSec = 0;
			DWORD dwSize = sizeof(dwIpSec);
			lRet = RegQueryValueExW(hkeyIpSec, L"ProhibitIpSec", 0, 0, 
				(BYTE*)&dwIpSec, &dwSize);
			if (lRet == ERROR_SUCCESS && dwIpSec == 1)	//Exists.
			{
				lRet = ERROR_ALREADY_EXISTS;
				throw CRunsisiExceptionW(L"IPSec key is already set.");
			}
			else if (lRet != ERROR_SUCCESS && lRet != ERROR_FILE_NOT_FOUND)	//Error.
			{
				throw CRunsisiExceptionW(L"Failed to query IPSec key, code: %d.", 
					lRet);
			}
			
			//Else set it.
			dwIpSec = 1;
			lRet = RegSetValueExW(hkeyIpSec, L"ProhibitIpSec", 0, 
				REG_DWORD, (BYTE*)&dwIpSec, sizeof(DWORD));
			if (lRet != ERROR_SUCCESS)
			{
				throw CRunsisiExceptionW(L"Failed to set IPSec key, code: %d.", 
					lRet);
			}
		}
		catch (CRunsisiExceptionW& e)
		{
			ERRW(e.what());
		}
		if (hkeyIpSec)
		{
			//Close.
			RegCloseKey(hkeyIpSec);
		}

		return lRet;
	}
}