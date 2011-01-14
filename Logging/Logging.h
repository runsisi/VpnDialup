#ifndef LOGGING_RUNSISI_HUST_H_
#define LOGGING_RUNSISI_HUST_H_

/*!
* \file Logging.h
* \brief header file for Logging class.
*
* Modified to pure c++ code from COM-like DLL code @2010/11/27.
* \author runsisi@163.com
*/

/*!
* \defgroup logging_group Logging
* \{
*/

namespace RUNSISI_HUST
{
	/*! \brief Interface class for logging.
	*
	* Do not use it directly, you have to use logging macros instead.
	*/
	class CLogging
	{
	public:
		virtual void WriteLogging(int level, wchar_t* wszString) = 0;
		virtual void WriteLogging(int level, char* szString) = 0;
	};

// 	void InitializeLogging(wchar_t* wszLoggingDllPath = L"Logging.dll");
// 	void UninitializeLogging();

	void WriteLogging(int level, const wchar_t* pFormat, ...);
	void WriteLogging(int level, const char* pFormat, ...);

// #if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_LOG) || defined(RUNSISI_ERROR)
// 	extern CLogging* g_pLoggingInstance;
// #endif
}

//macro definitions
//write log
#if defined(_DEBUG) || defined(RUNSISI_DEBUG) || defined(RUNSISI_INFO) || defined(RUNSISI_ERROR)
#define RUNSISI_HUST_TRCLOGW(level, ...) \
	RUNSISI_HUST::WriteLogging(level, __VA_ARGS__)
#define RUNSISI_HUST_TRCLOGA(level, ...) \
	RUNSISI_HUST::WriteLogging(level, __VA_ARGS__)
#else
#define RUNSISI_HUST_TRCLOGW(level, ...)
#define RUNSISI_HUST_TRCLOGA(level, ...)
#endif
//end write log



//_DEBUG or RUNSISI_DEBUG
#if defined(_DEBUG) || defined(RUNSISI_DEBUG)
#define RUNSISI_TRC_DEBUGW(...) RUNSISI_HUST_TRCLOGW(0x01, __VA_ARGS__)
#define RUNSISI_TRC_INFOW(...) RUNSISI_HUST_TRCLOGW(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERRW(...) RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#define RUNSISI_TRC_DEBUGA(...) RUNSISI_HUST_TRCLOGA(0x01, __VA_ARGS__)
#define RUNSISI_TRC_INFOA(...) RUNSISI_HUST_TRCLOGA(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERRA(...) RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)

#if defined(UNICODE) || defined(_UNICODE)	//UNICODE
#define RUNSISI_TRC_DEBUG(...) RUNSISI_HUST_TRCLOGW(0x01, __VA_ARGS__)
#define RUNSISI_TRC_INFO(...) RUNSISI_HUST_TRCLOGW(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERR(...) RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#else	
#define RUNSISI_TRC_DEBUG(...) RUNSISI_HUST_TRCLOGA(0x01, __VA_ARGS__)
#define RUNSISI_TRC_INFO(...) RUNSISI_HUST_TRCLOGA(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERR(...) RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)
#endif	//end UNICODE
#ifdef DEBUGW
#error DEBUGW already defined!
#else
#define DEBUGW(...) RUNSISI_TRC_DEBUGW(__VA_ARGS__)
#endif
#ifdef DEBUGA
#error DEBUGA already defined!
#else
#define DEBUGA(...) RUNSISI_TRC_DEBUGA(__VA_ARGS__)
#endif
#ifdef INFOW
#error INFOW already defined!
#else
#define INFOW(...) RUNSISI_TRC_INFOW(__VA_ARGS__)
#endif
#ifdef INFOA
#error INFOA already defined!
#else
#define INFOA(...) RUNSISI_TRC_INFOA(__VA_ARGS__)
#endif
#ifdef ERRW
#error ERRW already defined!
#else
#define ERRW(...) RUNSISI_TRC_ERRW(__VA_ARGS__)
#endif
#ifdef ERRA
#error ERRA already defined!
#else
#define ERRA(...) RUNSISI_TRC_ERRA(__VA_ARGS__)
#endif
//end _DEBUG or RUNSISI_DEBUG


//RUNSISI_INFO
#elif !defined(_DEBUG) && defined(RUNSISI_INFO)
#define RUNSISI_TRC_DEBUGW(...)
#define RUNSISI_TRC_INFOW(...) RUNSISI_HUST_TRCLOGW(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERRW(...)	RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#define RUNSISI_TRC_DEBUGA(...)
#define RUNSISI_TRC_INFOA(...) RUNSISI_HUST_TRCLOGA(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERRA(...)	RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)

#define RUNSISI_TRC_DEBUG(...)		//UNICODE
#if defined(UNICODE) || defined(_UNICODE)
#define RUNSISI_TRC_INFO(...) RUNSISI_HUST_TRCLOGW(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERR(...)	RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#else
#define RUNSISI_TRC_INFO(...) RUNSISI_HUST_TRCLOGA(0x02, __VA_ARGS__)
#define RUNSISI_TRC_ERR(...)	RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)
#endif
#ifdef DEBUGW
#error DEBUGW already defined!
#else
#define DEBUGW(...)
#endif
#ifdef DEBUGA
#error DEBUGA already defined!
#else
#define DEBUGA(...)
#endif
#ifdef INFOW
#error INFOW already defined!
#else
#define INFOW(...) RUNSISI_TRC_INFOW(__VA_ARGS__)
#endif
#ifdef INFOA
#error INFOA already defined!
#else
#define INFOA(...) RUNSISI_TRC_INFOA(__VA_ARGS__)
#endif
#ifdef ERRW
#error ERRW already defined!
#else
#define ERRW(...) RUNSISI_TRC_ERRW(__VA_ARGS__)
#endif
#ifdef ERRA
#error ERRA already defined!
#else
#define ERRA(...) RUNSISI_TRC_ERRA(__VA_ARGS__)
#endif



//RUNSISI_ERROR
#elif !defined(_DEBUG) && defined(RUNSISI_ERROR)
#define RUNSISI_TRC_DEBUGW(...)
#define RUNSISI_TRC_INFOW(...)
#define RUNSISI_TRC_ERRW(...) RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#define RUNSISI_TRC_DEBUGA(...)
#define RUNSISI_TRC_INFOA(...)
#define RUNSISI_TRC_ERRA(...) RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)

#define RUNSISI_TRC_DEBUG(...)		//UNICODE
#define RUNSISI_TRC_INFO(...)
#if defined(UNICODE) || defined(_UNICODE)
#define RUNSISI_TRC_ERR(...)	RUNSISI_HUST_TRCLOGW(0x04, __VA_ARGS__)
#else	
#define RUNSISI_TRC_ERR(...)	RUNSISI_HUST_TRCLOGA(0x04, __VA_ARGS__)
#endif
#ifdef DEBUGW
#error DEBUGW already defined!
#else
#define DEBUGW(...)
#endif
#ifdef DEBUGA
#error DEBUGA already defined!
#else
#define DEBUGA(...)
#endif
#ifdef INFOW
#error INFOW already defined!
#else
#define INFOW(...)
#endif
#ifdef INFOA
#error INFOA already defined!
#else
#define INFOA(...)
#endif
#ifdef ERRW
#error ERRW already defined!
#else
#define ERRW(...) RUNSISI_TRC_ERRW(__VA_ARGS__)
#endif
#ifdef ERRA
#error ERRA already defined!
#else
#define ERRA(...) RUNSISI_TRC_ERRA(__VA_ARGS__)
#endif


//otherwise
#else	
#define RUNSISI_TRC_DEBUGW(...)
#define RUNSISI_TRC_INFOW(...)
#define RUNSISI_TRC_ERRW(...)
#define RUNSISI_TRC_DEBUGA(...)
#define RUNSISI_TRC_INFOA(...)
#define RUNSISI_TRC_ERRA(...)

#define RUNSISI_TRC_DEBUG(...)		//UNICODE
#define RUNSISI_TRC_INFO(...)
#define RUNSISI_TRC_ERR(...)
#ifdef DEBUGW
#error DEBUGW already defined!
#else
#define DEBUGW(...)
#endif
#ifdef DEBUGA
#error DEBUGA already defined!
#else
#define DEBUGA(...)
#endif
#ifdef INFOW
#error INFOW already defined!
#else
#define INFOW(...)
#endif
#ifdef INFOA
#error INFOA already defined!
#else
#define INFOA(...)
#endif
#ifdef ERRW
#error ERRW already defined!
#else
#define ERRW(...)
#endif
#ifdef ERRA
#error ERRA already defined!
#else
#define ERRA(...)
#endif

#endif

/*!
* \}
*/

#endif	//header file