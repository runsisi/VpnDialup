#ifndef SEHTOCPPEH_RUNSISI_HUST_H_
#define SEHTOCPPEH_RUNSISI_HUST_H_

/*!
* \file SehToCppeh.h
* \author runsisi@163.com
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <eh.h>

/*! \addtogroup exception_group
* \{
*/

namespace RUNSISI_HUST
{
	/*! \brief If SE occurred, it will throw this C++ exception. 
	*
	* Do not throw this exception manually, if SE occurred, 
	* it will be thrown automatically.
	*/
	class CSehException
	{
	public:
		/*! \brief Windows SE handler will construct this object.
		*/
		CSehException(unsigned int uCode, PEXCEPTION_POINTERS pException)
		{
			m_uExceptionCode = uCode;
			m_exceptionRecord = *pException->ExceptionRecord;
			m_processorContext = *pException->ContextRecord;
		}
	public:
		unsigned int m_uExceptionCode;
		EXCEPTION_RECORD m_exceptionRecord;
		CONTEXT m_processorContext;
	};

	static inline void TranslateSehtoCppeh(unsigned int uCode, 
		PEXCEPTION_POINTERS pException)
	{
		throw CSehException(uCode, pException);
	}
}

/*!
* \}
*/

#endif