#ifndef ACQUIREPRIVILEGE_RUNSISI_HUST_H_
#define ACQUIREPRIVILEGE_RUNSISI_HUST_H_

/*!
* \file AcquirePrivilege.h
* \author runsisi@163.com
*/

/*!
* \defgroup privilege_group Privilege
* \{
*/

namespace RUNSISI_HUST
{
	LONG AcquirePrivilege(HANDLE& hToken, 
		TOKEN_PRIVILEGES& oldtp, LPCWSTR wszPrivilege);
	LONG RestorePrivilege(HANDLE& hToken, 
		TOKEN_PRIVILEGES& oldtp);
	LONG AcquireDebugPrivilege(HANDLE& hToken, 
		TOKEN_PRIVILEGES& oldtp);
}

/*! \}
*/

#endif