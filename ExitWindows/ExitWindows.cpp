#include "ExitWindows.h"
#include "../Privilege/Privilege.h"
#include "../Exception/Exception.h"
#include "../Logging/Logging.h"

namespace RUNSISI_HUST
{
	LONG ExitWindowsSystem(ExitWindowsType::ExitWindowsType exitType)
	{
		LONG lRet = ERROR_SUCCESS;

		HANDLE hToken = 0; 
		TOKEN_PRIVILEGES oldtp = {0};
		
		try
		{
			//Acquire shut down privilege.
			lRet = AcquirePrivilege(hToken, oldtp, SE_SHUTDOWN_NAME);
			if (lRet != ERROR_SUCCESS)
			{
				throw CRunsisiExceptionW(L"Failed to acquire shut down "
					L"privilege, code: %d", lRet);
			}
			//Exit.
			if (!::ExitWindowsEx(exitType, 
				SHTDN_REASON_MAJOR_APPLICATION))
			{
				lRet = GetLastError();
				throw CRunsisiExceptionW(L"Failed to shut down, "
					L"code: %d", lRet);
			}
		}
		catch (CRunsisiExceptionW& e)
		{
			ERRW(e.what());
		}

		if (hToken)
		{
			CloseHandle(hToken);
		}

		return lRet;
	}
}