#ifndef CUSTOMMENU_RUNSISI_HUST_H_
#define CUSTOMMENU_RUNSISI_HUST_H_

/*!
* \file CustomMenu.h
* \author runsisi@163.com
*/

/*! \defgroup custommenu_group Custom Menu
* \{
*/

namespace RUNSISI_HUST
{
	const int MAX_MENU_ITEM_COUNT = 50;
	const int MENU_ITEM_HEIGHT = 28;
	const int MENU_ITEM_WIDTH = 120;

	class CCustomMenu : public CMenu
	{
	private:
		/*! \brief Menu item information struct definition.
		*
		* If this is a common menu item, m_miiId stands for menu item ID, 
		* otherwise, it is used to identify various menu item type, they are 
		* 4 kinds of menu items, -2 stands for topmost menu item(menu 
		* item at the root of application main menu or context menu), -1 stands 
		* for child menu, 0 stands for separator, and >0 stands for 
		* common menu item.
		*/
		typedef struct MenuItemInfo_s
		{
			CString m_miiItemText;	//!< Menu item text.
			UINT m_miiItemId;			//!< Menu item type / ID.
			UINT m_miiIconIndex;		//!< Menu item associated icon index.
		} MenuItemInfo_t;

		enum {TOPMOST_MENU = -2, CHILD_MENU = -1, 
			SEPARATOR_MENU = 0};

		// Construction
	public:
		CCustomMenu();
		virtual ~CCustomMenu();

		// Overrides
	public:
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

		// Implementation
	public:
		/*! \brief Set menu item image list.
		*
		* \param pImageList This menu will use the specified image list to draw.
		*/
		void SetMenuImageList(CImageList* pImageList);

		/*! \brief Change menu item.
		* \param menu Menu to be modified.
		* \param bTop If menu is application main menu or context menu.
		*/
		BOOL ChangeMenuItem(CMenu* menu, BOOL bTop = FALSE);

		void DrawTopMenuItem(CDC* dc, CRect rect, BOOL bSelect);
		void DrawCommonMenuItem(CDC* dc, CRect rect, BOOL bSelect, BOOL bGray);
		void DrawSeparate(CDC* dc, CRect rect);

		void DrawMenuIcon(CDC* dc, CRect rect, UINT iconIndex);
		void DrawItemText(CDC* dc, CString str, CRect rect, 
			BOOL bCom, BOOL bGray, BOOL bTop);
		void DrawMenuTitle(CDC* dc, CRect rect, CString title);

	protected:
		//Check if the drawing item is owned by specified menu, 
		//hwnd is a input parameter from LPDRAWITEMSTRUCT.
		BOOL IsDrawingItem(CMenu* menu, HWND hwnd);

		//Data members.
		MenuItemInfo_t m_MenuInfoList[MAX_MENU_ITEM_COUNT];	//TODO: Fix this limitation.
		UINT m_uItemIndex;	//Includes topmost, pop up, separator and common menu item.
		UINT m_uIconIndex;	//Icon is only for child menu item or common menu item.
		CImageList* m_pImageList;	//Icon image list.
		CFont m_titleFont;
	};
}

/*! \}
*/

#endif
