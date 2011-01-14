#include "../WinVer/WinVer.h"
#include <windows.h>
#include "Privilege.h"
#include "../Exception/Exception.h"
#include "../Logging/Logging.h"

namespace RUNSISI_HUST
{
	LONG AcquirePrivilege(HANDLE& hToken, 
		TOKEN_PRIVILEGES& oldtp, LPCWSTR wszPrivilege)
	{
		LONG lRet = ERROR_SUCCESS;

		//Initialize.
		hToken = 0;
		try
		{
			TOKEN_PRIVILEGES tp = {0};
			DWORD dwSize = sizeof(TOKEN_PRIVILEGES);
			LUID luid = {0};

			if (!OpenProcessToken(GetCurrentProcess(), 
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, 
				&hToken))
			{
				lRet = GetLastError();
				throw CRunsisiExceptionW(L"Failed to open current "
					L"process token, code: %d.", lRet);
			}
			if (!LookupPrivilegeValue(0, wszPrivilege, &luid))
			{
				lRet = GetLastError();
				throw CRunsisiExceptionW(L"Failed to look up required privilege, "
					L"code: %d.", lRet);
			}
			ZeroMemory(&tp, sizeof(tp));
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 
				sizeof(TOKEN_PRIVILEGES), 	&oldtp, &dwSize))
			{
				lRet = GetLastError();
				throw CRunsisiExceptionW(L"Failed to adjust token privilege, "
					L"code: %d.", lRet);
			}
		}
		catch (CRunsisiExceptionW& e)
		{
			if (hToken)
			{
				CloseHandle(hToken);
			}
			ERRW(e.what());
		}

		return lRet;
	}

	LONG RestorePrivilege(HANDLE& hToken, TOKEN_PRIVILEGES& oldtp)
	{
		LONG lRet = ERROR_SUCCESS;

		if (!AdjustTokenPrivileges(hToken, FALSE, &oldtp, 
			sizeof(TOKEN_PRIVILEGES), 0, 0))
		{
			lRet = GetLastError();
		}

		return lRet;
	}

	LONG AcquireDebugPrivilege(HANDLE& hToken, TOKEN_PRIVILEGES& oldtp)
	{
		return AcquirePrivilege(hToken, oldtp, SE_DEBUG_NAME);
	}
}