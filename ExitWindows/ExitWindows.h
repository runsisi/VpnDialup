#ifndef EXITWINDOWS_RUNSISI_HUST_H_
#define EXITWINDOWS_RUNSISI_HUST_H_

/*!
* \file ExitWindows.h
* \author runsisi@163.com
*/

#include "../WinVer/WinVer.h"
#include <Windows.h>

/*!
* \defgroup exitwindows_group Exit Windows
* \{
*/

namespace RUNSISI_HUST
{
	/*! \brief Contains exit windows enumeration type.
	*/
	namespace ExitWindowsType
	{
		/*! \brief Defines exit windows type.
		*
		* Refer to ExitWindowsEx() on MSDN for exit type.
		*/
		enum ExitWindowsType
		{
			LOGOFF = EWX_LOGOFF, 
			POWEROFF = EWX_POWEROFF, 
			REBOOT = EWX_REBOOT, 
			//RESTARTAPPS = ::EWX_RESTARTAPPS, 
			SHUTDOWN = EWX_SHUTDOWN,
			FORCE = EWX_FORCE,
			//FORCEIFHUNG = EWX_FORCEIFHUNG
		};
	}

	/*! \brief Use this function to shutdown or boot .etc.
	* \param exitType Exit enumeration type.
	* \return ERROR_SUCCESS on succeeded.
	* \note ExitWindows is a system defined macro, so we can not define
	* the function the same name as the macro.
	*/
	LONG ExitWindowsSystem(ExitWindowsType::ExitWindowsType exitType);
}

/*! \}
*/

#endif