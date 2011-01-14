#ifndef EXCEPTION_RUNSISI_HUST_H_
#define EXCEPTION_RUNSISI_HUST_H_

/*!
* \file Exception.h
* \brief header file for user defined exception class.
* \author runsisi@163.com
*/

#include <string>

/*!
* \defgroup exception_group Exception
* \{
*/

#if defined(UNICODE) || defined(_UNICODE)
#define CRunsisiException RUNSISI_HUST::CRunsisiExceptionW
#else
#define CRunsisiException RUNSISI_HUST::CRunsisiExceptionA
#endif

namespace RUNSISI_HUST
{
	/*! \brief If something failed, throw this exception.
	*
	* This is the unicode version of CRunsisiException, when throw
	* this exception object, you can construct detailed explanation.
	*/
	class CRunsisiExceptionW
	{
	public:
		/*! \brief Constructor, its parameters are identical to CRT function wprintf().
		*
		* \param wszFormat Format string.
		*/
		CRunsisiExceptionW(const wchar_t* wszFormat, ...) throw();

		/*! \brief Destructor.
		*/
		virtual ~CRunsisiExceptionW() throw();

		/*! \brief Use this member function to get detail explanation form exception object.
		*
		* \return Wide constant string contains detailed explanation.
		*/
		virtual const wchar_t* what() const throw();
	protected:
		std::wstring m_wstrDescription;
	};

	/*! \brief If something failed, throw this exception.
	*
	* This is the ansi version of CRunsisiException, when throw
	* this exception object, you can construct detailed explanation.
	*/
	class CRunsisiExceptionA
	{
	public:
		/*! \brief Constructor, its parameters are identical to CRT function printf().
		*
		* \param szFormat Format string.
		*/
		CRunsisiExceptionA(const char* szFormat, ...) throw();

		/*! \brief Destructor.
		*/
		virtual ~CRunsisiExceptionA() throw();

		/*! \brief Use this member function to get detail explanation form exception object.
		*
		* \return Ansi constant string contains detailed explanation.
		*/
		virtual const char* what() const throw();
	protected:
		std::string m_strDescription;
	};
}

/*!
* \}
*/

#endif