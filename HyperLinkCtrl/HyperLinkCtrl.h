#ifndef HYPERLINKCTRL_RUNSISI_HUST_H_
#define HYPERLINKCTRL_RUNSISI_HUST_H_

/*!
* \file HyperLinkCtrl.h
* \brief Original author: http://www.angelfire.com/blues2/javeline.
* \author runsisi@163.com
*/

/*!
* \defgroup hyperlinkctrl_group Hyper Link Control
* \{
*/

namespace RUNSISI_HUST
{
	class CHyperLinkCtrl : public CStatic
	{
	// Construction
	public:
		CHyperLinkCtrl();
		virtual ~CHyperLinkCtrl();

	// Attributes
	public:
		enum eUnderline {ALWAYS, ONHOVER, NONE};

		/*! \brief Gets the underline mode. 
		*
		* Values are ALWAYS = 0, ONHOVER = 1, NONE = 2.
		* Default is ALWAYS.
		*/
		int GetUnderline()
		{ 
			return m_underline; 
		}

		/*! \brief Sets the underline mode.
		*/
		void SetUnderline(int underline);
		
		/*! \brief Gets the link color.
		*/
		COLORREF GetLinkColor()
		{ 
			return m_linkColor; 
		}

		/*! \brief Sets the link color.
		*/
		void SetLinkColor(COLORREF c)
		{ 
			m_linkColor = c;
			m_color = m_bVisited ? m_visitedColor : m_linkColor; 
			InvalidateRect(NULL); 
		}

		/*! \brief  Gets the hover color of the link(when the mouse is over the control).
		*/
		COLORREF GetHoverColor()
		{ 
			return m_hoverColor; 
		}

		/*! \brief Sets the hover color of the link.
		*/
		void SetHoverColor(COLORREF c)
		{ 
			m_hoverColor = c; 
		}

		/*! \brief Gets the color of the active link(when the link is being opened).
		*/
		COLORREF GetActiveColor()
		{ 
			return m_activeColor; 
		}

		/*! \brief Sets the color of the active link.
		*/
		void SetActiveColor(COLORREF c)
		{ 
			m_activeColor = c; 
		}

		/*! \brief Gets the color of already visited link.
		*/
		COLORREF GetVisitedColor(){ return m_visitedColor; }

		/*! \brief Sets the color of already visited link.
		*/
		void SetVisitedColor(COLORREF c)
		{ 
			m_visitedColor = c;
			m_color = m_bVisited ? m_visitedColor : m_linkColor; 
			InvalidateRect(NULL); 
		}
		
		/*! \brief Gets the background color.
		*/
		COLORREF GetBkColor()
		{ 
			return m_bkColor; 
		}

		/*! \brief Sets the background color. 
		* 
		* Automatically sets the background mode to OPAQUE.
		* Values for this includes: OPAQUE, TRANSPARENT.
		*/
		void SetBkColor(COLORREF c);

		/*! \brief Gets the background mode. 
		*
		* If the mode is OPAQUE, the background is filled first with 
		current background color.
		*/
		int GetBkMode()
		{ 
			return m_bkMode; 
		}

		/*! \brief Sets the background mode.
		*/
		void SetBkMode(int mode);

		/*! \brief Gets the tool tip text.
		*/
		CString GetToolTipText();

		/*! \brief  Sets the tool tip text.
		*/
		void SetToolTipText(CString toolTipText);

		/*! \brief Gets the HCURSOR used as the current mouse cursor for the control.
		*/
		HCURSOR GetLinkCursor()
		{ 
			return m_hCursor; 
		}

		/*! \brief Sets the HCURSOR used as the current mouse cursor for the control.
		*/
		void SetLinkCursor(HCURSOR hCursor)
		{ 
			m_hCursor = hCursor; 
		}

		/*! \brief Gets the URL (Uniform Resource Locator) string.
		*/
		CString GetURL()
		{ 
			return m_url; 
		}

		/*! \brief Sets the URL (Uniform Resource Locator) string.
		*/
		BOOL SetURL(CString url);

		void CorrectLinkWidth();	//Added @2011/01/14

	// Overrides
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		protected:
		virtual void PreSubclassWindow();

		// Generated message map functions
	protected:
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

		DECLARE_MESSAGE_MAP()

	protected:
		virtual BOOL FollowLink(CString url);
		virtual BOOL MailTo(CString url, CString name = TEXT(""));
		virtual void EraseBackground();

	private:
		COLORREF	m_linkColor;
		COLORREF	m_hoverColor;
		COLORREF	m_activeColor;
		COLORREF	m_visitedColor;
		COLORREF	m_color;
		COLORREF	m_bkColor;
		int			m_bkMode;
		eUnderline	m_underline;
		BOOL		m_bUnderline;
		CFont		m_font;
		CBrush		m_brBack;
		CBrush		m_brLastBack;
		BOOL		m_bHovering;
		BOOL		m_bVisited;
		HCURSOR		m_hCursor;
		CString		m_url;
		CToolTipCtrl m_toolTip;
	};
}

/*! \}
*/

#endif