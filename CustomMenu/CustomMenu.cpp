#include "../WinVer/WinVer.h"	//Use precompiled header if you want.
#include <afxwin.h>
#include <afxcmn.h>
#include "CustomMenu.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace RUNSISI_HUST
{
	CCustomMenu::CCustomMenu()
	{
		m_uIconIndex = 0;
		m_uItemIndex = 0;
		m_pImageList = 0;
	}

	CCustomMenu::~CCustomMenu()
	{
	}

	void CCustomMenu::SetMenuImageList(CImageList* pImageList)
	{
		this->m_pImageList = pImageList;
	}

	BOOL CCustomMenu::IsDrawingItem(CMenu* menu, HWND hwnd)
	{
		BOOL bDrawing = FALSE;

		if (hwnd == (HWND)menu->m_hMenu)
		{
			return TRUE;
		}
		int count = menu->GetMenuItemCount();
		CMenu* pSubMenu = 0;
		for (int i = 0; i < count; ++i)
		{
			pSubMenu = menu->GetSubMenu(i);
			if (pSubMenu)
			{
				bDrawing = IsDrawingItem(pSubMenu, hwnd);
				if (bDrawing)
				{
					break;
				}
			}
		}

		return bDrawing;
	}

	void CCustomMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		// TODO: Add your code to draw the specified item
		if(!lpDrawItemStruct->itemData)
		{
			return;
		}
		//To check if the drawing menu item that owned by this menu.
		if (!IsDrawingItem(this, lpDrawItemStruct->hwndItem))
		{
			return;
		}

		CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		MenuItemInfo_t* itemInfo=(MenuItemInfo_t*)(lpDrawItemStruct->itemData);
		UINT itemTypeOrId = itemInfo->m_miiItemId;
		CString itemText = itemInfo->m_miiItemText;
		UINT iconIndex = itemInfo->m_miiIconIndex;
		UINT itemState = lpDrawItemStruct->itemState;
		CRect rect = lpDrawItemStruct->rcItem;
		pDC->SetBkMode(TRANSPARENT);

		switch(itemTypeOrId)
		{
		case TOPMOST_MENU://顶层菜单
			DrawTopMenuItem(pDC, rect, 
				(itemState & ODS_SELECTED) || (itemState & 0x0040));
			//这里也不要用(m_state& ODS_SELECTED)||
	//			(lpDrawItemStruct->itemAction & (ODA_SELECT)));
			//(m_state&0x0040)也要加上，否则鼠标移入没反应
			//0x0040 ==ODS_HOTLIGHT
			DrawItemText(pDC, itemText, rect, FALSE, FALSE, TRUE);
			break;
		case CHILD_MENU:
			DrawCommonMenuItem(pDC,rect,(itemState & ODS_SELECTED)||(itemState & 0x0040),FALSE);
			DrawItemText(pDC,itemText,rect,TRUE,FALSE,FALSE);
			DrawMenuIcon(pDC,rect,iconIndex + 1);
			break;
		case SEPARATOR_MENU:
			DrawSeparate(pDC, rect);
			break;
		default:	//Common menu item.
			//||(lpDrawItemStruct->itemAction & (ODA_SELECT))
			//这个不要加，加了之后会出现一直处于高亮显示
			if(itemState & ODS_GRAYED)
			{
				DrawCommonMenuItem(pDC, rect, (itemState & ODS_SELECTED), TRUE);
				DrawItemText(pDC, itemText, rect, TRUE, TRUE, FALSE);
			}
			else
			{
				DrawCommonMenuItem(pDC, rect, (itemState & ODS_SELECTED), FALSE);
				DrawItemText(pDC, itemText, rect, TRUE, FALSE, FALSE);
			}

			if(itemState & ODS_CHECKED)
				DrawMenuIcon(pDC, rect, 0);		//Use the first icon!
			else
				DrawMenuIcon(pDC, rect, iconIndex + 1);

			//DrawMenuTitle(pDC, rect, TEXT("华中科技大学"));

			break;
		}
	}

	void CCustomMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		// TODO: Add your code to determine the size of specified item
		if(lpMeasureItemStruct->CtlType == ODT_MENU)
		{
			lpMeasureItemStruct->itemHeight = MENU_ITEM_HEIGHT;
			lpMeasureItemStruct->itemWidth = MENU_ITEM_WIDTH;
			MenuItemInfo_t* info = (MenuItemInfo_t*)lpMeasureItemStruct->itemData;

			if(lpMeasureItemStruct->itemID == TOPMOST_MENU)
			{
				lpMeasureItemStruct->itemWidth = 
					info->m_miiItemText.GetLength() * 6;
			}
			else	//Child menu or common menu item or separator.
			{
				lpMeasureItemStruct->itemWidth = 
					info->m_miiItemText.GetLength() * 16 + 32;
			}
			if(info->m_miiItemId == SEPARATOR_MENU)
			{
					lpMeasureItemStruct->itemHeight = 1;
			}
		}
	}

	BOOL CCustomMenu::ChangeMenuItem(CMenu* pMenu, BOOL bTop)
	{
		if(pMenu != NULL)
		{
			int itemCount = pMenu->GetMenuItemCount();

			for (int i = 0 ; i < itemCount ; i++)
			{
				int itemId = pMenu->GetMenuItemID(i);

				CMenu* pSubMenu = pMenu->GetSubMenu(i);

				if(itemId == -1 && bTop)		//Application menu or context menu.
				{
					m_MenuInfoList[m_uItemIndex].m_miiItemId = 
						TOPMOST_MENU;
				}
				else if (itemId == -1)	//Child menu.
				{
					m_MenuInfoList[m_uItemIndex].m_miiItemId = CHILD_MENU;
					m_MenuInfoList[m_uItemIndex].m_miiIconIndex = m_uIconIndex;
					m_uIconIndex++;
				}
				else if (itemId == 0)	//Separator.
				{
					m_MenuInfoList[m_uItemIndex].m_miiItemId = SEPARATOR_MENU;
				}
				else if(itemId > 0)	//Common menu item.
				{
					m_MenuInfoList[m_uItemIndex].m_miiItemId = itemId;
					m_MenuInfoList[m_uItemIndex].m_miiIconIndex = m_uIconIndex;
					m_uIconIndex++;
				}

				pMenu->GetMenuString(i, 
					m_MenuInfoList[m_uItemIndex].m_miiItemText, MF_BYPOSITION);
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_OWNERDRAW | MF_STRING,
					m_MenuInfoList[m_uItemIndex].m_miiItemId, 
					(LPCTSTR)(&m_MenuInfoList[m_uItemIndex]));

				m_uItemIndex++;	//Item index increase.

				if(pSubMenu)
				{
					ChangeMenuItem(pSubMenu);
				}
			}
		}
		return TRUE;
	}

	void CCustomMenu::DrawSeparate(CDC* dc, CRect rc)
	{
		rc.left = rc.left + 0;
		dc->Draw3dRect(rc, GetSysColor(COLOR_3DDKSHADOW), 
			GetSysColor(COLOR_3DDKSHADOW));
	}

	void CCustomMenu::DrawCommonMenuItem(CDC* dc, 
		CRect rc, BOOL bSelect, BOOL bGray)
	{
		CRect rect(rc);
		//rect.DeflateRect(30,1,0,1);

		if(bSelect)
		{
			dc->FillSolidRect(rect, RGB(186,211,252));//208,235,255
		}
		else
		{
			if(bGray)
			{
				dc->FillSolidRect(rect, RGB(240,238,225));
			}
			else
			{
				dc->FillSolidRect(rect, RGB(255,255,255));
			}
		}
	}

	void CCustomMenu::DrawTopMenuItem(CDC* dc, CRect rc, BOOL bSelect)
	{
		if(bSelect)
		{
			dc->SelectStockObject(BLACK_PEN);
			CRect rect(rc);
			dc->Rectangle(rect);
			rect.DeflateRect(1,1,1,1);
			//dc->FillSolidRect(rect, RGB(255,235,181));
			dc->FillSolidRect(rect, GetSysColor(COLOR_WINDOW));
		}
		else
		{
			//dc->FillSolidRect(rc,RGB(186,211,252));
			dc->FillSolidRect(rc,GetSysColor(COLOR_3DFACE));
// 
// 			CRect rect;
// 			AfxGetMainWnd()->GetClientRect(&rect);
// 			rect.left = 300;
// 			rect.bottom = rc.bottom;
// 			rect.top = rc.top;
// 			rect.right += 4;
// 			dc->FillSolidRect(rect,RGB(186,211,252));//画出射剩余的部分
		}
	}

	void CCustomMenu::DrawItemText(CDC* dc, CString str, 
		CRect rc, BOOL bCommon, BOOL bGray, BOOL bTop)
	{
		if(bGray)	//Disabled menu item.
		{
			dc->SetTextColor(RGB(127, 127, 127));
		}

		CRect rect(rc);

		if(bCommon)
		{
			rect.DeflateRect(40, 0, 0, 0);
		}

		if(bTop)	//Application menu.
		{
			dc->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else	//Common menu or child menu.
		{
			dc->DrawText(str, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
	}

	void CCustomMenu::DrawMenuIcon(CDC* dc, CRect rc, UINT iconIndex)
	{
		m_pImageList->Draw(dc, iconIndex, 
			CPoint(rc.left + 2, rc.top + 2), ILD_TRANSPARENT);
		//ILD_MASK不要加
	}

	void CCustomMenu::DrawMenuTitle(CDC* dc, CRect rc,CString title)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_MENU_LEFT);
		BITMAP size;
		bitmap.GetBitmap(&size);
		CDC memDc;
		memDc.CreateCompatibleDC(dc);
		CGdiObject* oldObject;
		oldObject = memDc.SelectObject(&bitmap);

		//m_save = m_pdc->SaveDC();
		dc->StretchBlt(0, 0, 24, rc.bottom, &memDc, 0, 0, 
			size.bmWidth, size.bmHeight, SRCCOPY);
		bitmap.DeleteObject();
		CFont* oldFont = dc->SelectObject(&m_titleFont);


		dc->TextOut(rc.left + 5, rc.Height() + 65, title);
		//m_pdc->DrawText(m_title,m_rect,DT_SINGLELINE|DT_VCENTER |DT_LEFT);
		dc->SelectObject(oldFont);
	}
}