#include <stdarg.h>
#include "Exception.h"

/*!
* \file Exception.cpp
* \author runsisi@163.com
*/

#pragma warning(disable:4996)

namespace RUNSISI_HUST
{
	//UNICODE
	CRunsisiExceptionW::CRunsisiExceptionW(const wchar_t* wszFormat, ...)
	{
		wchar_t* wszDescription = 0;
		int len = 0;

		va_list pArg;
		va_start(pArg, wszFormat);
		len = _vscwprintf(wszFormat, pArg) + 1;
		wszDescription = new wchar_t[len];		//if this fails, this app may has been corrupted
		_vswprintf(wszDescription, wszFormat, pArg);
		va_end(pArg);

		m_wstrDescription = wszDescription;

		delete[] wszDescription;
	}

	CRunsisiExceptionW::~CRunsisiExceptionW()
	{

	}

	const wchar_t* CRunsisiExceptionW::what() const
	{
		return m_wstrDescription.c_str();
	}

	//ANSI
	CRunsisiExceptionA::CRunsisiExceptionA(const char* szFormat, ...)
	{
		char* szDescription = 0;
		int len = 0;

		va_list pArg;
		va_start(pArg, szFormat);
		len = _vscprintf(szFormat, pArg) + 1;
		szDescription = new char[len];		//if this fails, this app may has been corrupted
		vsprintf(szDescription, szFormat, pArg);
		va_end(pArg);

		m_strDescription = szDescription;

		delete[] szDescription;
	}

	CRunsisiExceptionA::~CRunsisiExceptionA()
	{

	}

	const char* CRunsisiExceptionA::what() const
	{
		return m_strDescription.c_str();
	}
}