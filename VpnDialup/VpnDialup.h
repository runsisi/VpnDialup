#ifndef VPNDIALUP_RUNSISI_HUST_H_
#define VPNDIALUP_RUNSISI_HUST_H_

/*!
* \file VpnDialup
* \author runsisi@163.com
*/

//#include "WinVer/WinVer.h"
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ras.h>

/*!
* \defgroup vpndialup_group Vpn Dailup
* \{
*/

namespace RUNSISI_HUST
{
	/*! \brief Get VPN dial up phone book full path file name.
	* \param wszPath String address to receive path.
	* \param size Buffer size of wszPath.
	* \return ERROR_SUCCESS on success.
	*/
	LONG GetVpnPhoneBookPath(wchar_t wszPath[], int size);

	/*! \brief Delete VPN dial up phone book file.
	* \return ERROR_SUCCESS on success.
	*/
	LONG DeleteVpnPhoneBook();

	/*! \brief Use this function to initialize a VPN entry.
	* \param entry Output parameter to receive initialized RASENTRY.
	* \param wszAddr VPN server IP / host name.
	* \return ERROR_SUCCESS on success, to get extended 
	* error information, call GetLastError.
	*/
	LONG InitVpnEntry(RASENTRYW& entry, 
		const wchar_t* wszAddr = L"221.174.16.81");

	/*! \brief Create a VPN connection.
	* \param entry VPN parameters.
	* \param wszEntryName VPN connection name.
	* \return ERROR_SUCCESS on success, to get extended 
	* error information, call GetLastError.
	*/
	LONG CreateVpnEntry(RASENTRY& entry, 
		const wchar_t* wszEntryName = L"runsisi@HUST");

	/*! \brief Delete a VPN connection.
	* \param wszEntryName VPN connection name.
	* \return ERROR_SUCCESS on success, to get extended 
	* error information, call GetLastError.
	*/
	LONG DeleteVpnEntry(const wchar_t* wszEntryName = L"runsisi@HUST");

	/*! \brief Connect.
	* \param hVpnConn If this function succeeded, it stores 
	* a handle to the VPN connection to this parameter.
	* \param wszEntryName VPN connection name.
	* \param wszUserName VPN user name.
	* \param wszPassword VPN password.
	* \return ERROR_SUCCESS on success, to get extended 
	* error information, call GetLastError.
	*/
	LONG ConnectVpn(HRASCONN& hVpnConn, 
		bool bQuiet = true, 
		const wchar_t* wszEntryName = L"runsisi@HUST", 
		const wchar_t* wszUserName = L"demo@gjw", 
		const wchar_t* wszPassword = L"demo123");

	/*! \brief Disconnect.
	* \param hRasConn A handle to the VPN connection.
	* \return ERROR_SUCCESS is returned if succeeded.
	*/
	LONG DisconnectVpn(HRASCONN hRasConn);

	/*! \brief Disable IpSec.
	* \return ERROR_SUCCESS is returned if succeeded.
	*/
	LONG DisableIpSec();
}

/*! \}
*/

#endif