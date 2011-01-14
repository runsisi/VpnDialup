#include "Logging.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#pragma warning(disable:4996)
#include "LoggingImp.h"

/*!
* \file Logging.cpp
* \brief Modified from COM-like DLL code to pure c++ code @2010/11/27
* \author runsisi@163.com
*/

namespace RUNSISI_HUST
{
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
	CLoggingImp g_Logging;
#endif

	void WriteLogging(int level, const wchar_t* pFormat, ...)
	{
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
		try
		{
			wchar_t* wszMsg = 0;
			int len = 0;

			va_list pArg;
			va_start(pArg, pFormat);
			len = _vscwprintf(pFormat, pArg) + 1;
			wszMsg = new wchar_t[len];		//if this fails, this app may have corrupted
			if (!wszMsg)
			{
				return;
			}
			_vswprintf(wszMsg, pFormat, pArg);
			va_end(pArg);

			g_Logging.WriteLogging(level, wszMsg);
			OutputDebugStringW(wszMsg);

			delete[] wszMsg;
		}
		catch (...)
		{
			OutputDebugStringW(L"- ERROR - Failed to write log!");
		}
#endif
	}

	void WriteLogging(int level, const char* pFormat, ...)
	{
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
		try
		{
			char* szMsg = 0;	
			int len = 0;

			va_list pArg;
			va_start(pArg, pFormat);
			len = _vscprintf(pFormat, pArg) + 1;
			szMsg = new char[len];
			if (!szMsg)
			{
				return;
			}
			vsprintf(szMsg, pFormat, pArg);
			va_end(pArg);

			g_Logging.WriteLogging(level, szMsg);
			OutputDebugStringA(szMsg);

			delete[] szMsg;
		}
		catch (...)
		{
			OutputDebugStringW(L"- ERROR - Failed to write log!");
		}
#endif
	}

/*	void InitializeLogging(wchar_t* wszLoggingDllPath)
	{
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
		if (g_pLoggingInstance)
		{
			return;
		}
		HMODULE hLoggingDll = LoadLibraryW(wszLoggingDllPath);
		if (!hLoggingDll)
		{
			return;
		}
		typedef CLogging* (*CreateLoggingInstance_t)();
		CreateLoggingInstance_t CreateLoggingInstance = 
			(CreateLoggingInstance_t)GetProcAddress(hLoggingDll, "CreateLoggingInstance");
		if (!CreateLoggingInstance)
		{
			FreeLibrary(hLoggingDll);
			return;
		}
		
		g_pLoggingInstance = CreateLoggingInstance();
#endif
	}

	void UninitializeLogging()
	{
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
		if (g_pLoggingInstance)
		{
			g_pLoggingInstance->Delete();
			g_pLoggingInstance = 0;
		}
#endif
	}*/
}