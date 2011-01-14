#ifndef LOGGINGIMP_RUNSISI_HUST_H_
#define LOGGINGIMP_RUNSISI_HUST_H_

/*!
* \file LoggingImp.h
* \brief implement header for class CLogging.
*
* modified from COM-like DLL code to pure c++ code @2010/11/27
* runsisi@163.com
*/

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500	//win2k and above
#endif
#ifndef WINVER
#define WINVER _WIN32_WINNT	//win2k and above
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>

#include "Logging.h"

namespace RUNSISI_HUST
{
	class CLoggingImp : public CLogging
	{
	public:
		CLoggingImp() {InitLogging();}
		~CLoggingImp() {m_logwfstream.close();}
	public:
		virtual void WriteLogging(int level, wchar_t* wszString);
		virtual void WriteLogging(int level, char* szString);		
	public:
		bool InitLogging();
	private:
		void CleanOldLog();
	private:
		wchar_t m_wszLogDir[MAX_PATH];
		CRITICAL_SECTION m_csWriteFile;
	private:
		std::wofstream m_logwfstream;
	};

	bool AnsiToUtf16(const char* ansi, wchar_t** utf16);
}

#endif