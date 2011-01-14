#include "LoggingImp.h"
#include <ShlObj.h>
#pragma comment(lib,"shell32.lib")
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")
#pragma warning(disable:4995)
#pragma warning(disable:4996)
#include <stdarg.h>

/*!
* \file LoggingImp.cpp
* \brief Implement file for class CLoggingImp.
*
* Modified from COM-like DLL code to pure c++ code @2010/11/27
* \author runsisi@163.com
*/

namespace RUNSISI_HUST
{
	const int LOG_FILE_TOO_OLD = 60*60*24;		//one day
	const int LOG_HEADER_LEN = 64;

	void CLoggingImp::WriteLogging(int level, wchar_t* wszString)
	{
		//construct message header
		SYSTEMTIME tm;
		::GetLocalTime(&tm);

		wchar_t wszLogHeader[LOG_HEADER_LEN] = {0};

		if (level == 0x01)
		{
			::StringCchPrintfW(wszLogHeader, LOG_HEADER_LEN, 
				L"- DEBUG -  TID:%u Time:%02d:%02d:%02d  ", 
				GetCurrentThreadId(), 
				tm.wHour, tm.wMinute, tm.wSecond);
		}
		else if (level == 0x02)
		{
			::StringCchPrintfW(wszLogHeader, LOG_HEADER_LEN, 
				L"+ INFO  +  TID:%u Time:%02d:%02d:%02d  ", 
				GetCurrentThreadId(), 
				tm.wHour, tm.wMinute, tm.wSecond);
		}
		else if (level == 0x04)
		{
			::StringCchPrintfW(wszLogHeader, LOG_HEADER_LEN, 
				L"* ERROR *  TID:%u Time:%02d:%02d:%02d  ", 
				GetCurrentThreadId(), 
				tm.wHour, tm.wMinute, tm.wSecond);
		}

		::EnterCriticalSection(&m_csWriteFile);
		m_logwfstream << wszLogHeader << wszString << std::endl;
		m_logwfstream.flush();
		::LeaveCriticalSection(&m_csWriteFile);
	}

	void CLoggingImp::WriteLogging(int level, char* szString)
	{
		//construct message header
		wchar_t* wszString = 0;
		AnsiToUtf16(szString, &wszString);

		WriteLogging(level, wszString ? wszString : L"");

		delete[] wszString;	//no harm to delete null
	}

	bool CLoggingImp::InitLogging()
	{
		if (!SHGetSpecialFolderPathW(0, m_wszLogDir, CSIDL_APPDATA, TRUE))
		{
			return false;
		}
		PathAppendW(m_wszLogDir, L"Logging");
		if (!CreateDirectoryW(m_wszLogDir, 0) && 
			GetLastError() != ERROR_ALREADY_EXISTS)
		{
			return false;
		}

		CleanOldLog();

		wchar_t wszBaseName[MAX_PATH] = {0};
		if (!GetModuleFileNameW(0, wszBaseName, MAX_PATH))
		{
			return false;
		}
		PathRemoveExtensionW(wszBaseName);
		PathStripPathW(wszBaseName);
		wchar_t wszLogFile[MAX_PATH] = {0};
		HRESULT hr = StringCchPrintfW(wszLogFile, MAX_PATH, L"%s\\%s_PID%d_%d.log", 
			m_wszLogDir, wszBaseName, GetCurrentProcessId(), GetTickCount());
		if (FAILED(hr))
		{
			return false;
		}
		//create file
		std::locale loc("");
		m_logwfstream.imbue(loc);
		m_logwfstream.open(wszLogFile, std::ios_base::out);	//TODO: what mode?
		if (!m_logwfstream)
		{
			return false;
		}
		//initialize critical section
		bool bOk = true;
		try
		{
			InitializeCriticalSection(&m_csWriteFile);
		}
		catch (...)
		{
			bOk = false;
			m_logwfstream.close();
		}
		
		m_logwfstream << L"\t\tIF ANY ERROR OCCURRED, "
			L"PLS CONTACT ME: runsisi@163.com.\n\n\n";
		
		return bOk;
	}

	void CLoggingImp::CleanOldLog()
	{
		wchar_t wszToFindFile[MAX_PATH] = {0};
		HRESULT hr = ::StringCchPrintfW(wszToFindFile, MAX_PATH, 
			L"%s\\*.log", m_wszLogDir);
		if (FAILED(hr))
		{
			return;
		}

		WIN32_FIND_DATAW fd = {0};
		HANDLE hFindFile = ::FindFirstFileW(wszToFindFile, &fd);
		if (hFindFile == INVALID_HANDLE_VALUE)
			return;

		SYSTEMTIME stm = {0};
		::GetSystemTime(&stm);
		FILETIME ftm = {0};
		::SystemTimeToFileTime(&stm, &ftm);

		do 
		{
			LARGE_INTEGER liElapsed = {0}, liLastWrite = {0}, liCurrent = {0};
			liLastWrite.LowPart = fd.ftLastWriteTime.dwLowDateTime;
			liLastWrite.HighPart = fd.ftLastWriteTime.dwHighDateTime;
			liCurrent.LowPart = ftm.dwLowDateTime;
			liCurrent.HighPart = ftm.dwHighDateTime;

			liElapsed.QuadPart = liCurrent.QuadPart - liLastWrite.QuadPart;
			liElapsed.QuadPart /= 10000000;

			if (liElapsed.QuadPart > LOG_FILE_TOO_OLD || liElapsed.QuadPart <= 0)
			{
				StringCchPrintfW(wszToFindFile, MAX_PATH, L"%s\\%s", 
					m_wszLogDir, fd.cFileName);
				::DeleteFileW(wszToFindFile);
			}
		} while (::FindNextFileW(hFindFile, &fd));

		::FindClose(hFindFile);
	}

	bool AnsiToUtf16(const char* ansi, wchar_t** utf16)
	{
		//we have to use system API to do this, windows c runtime do not support 
		//convert between UTF-16 and char set that require more 
		//than two bytes per character, like UTF-8
		int len = ::MultiByteToWideChar(CP_ACP, 0, ansi, -1, 0, 0);	//include the terminal null!
		if (!len)
		{
			return false;
		}
		(*utf16) = new wchar_t[len];
		if (!(*utf16))
		{
			return false;
		}
		(*utf16)[len - 1] = 0;
		//should not fail
		::MultiByteToWideChar(CP_ACP, 0, ansi, -1, (*utf16), len);

		return true;
	}
}