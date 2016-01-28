// This file is part of Notepad++ project
// Copyright (C)2003 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "fileBrowser.h"
#include "resource.h"
#include "tinyxml.h"
#include "FileDialog.h"
#include "localization.h"
#include "Parameters.h"
#include "ReadDirectoryChanges.h"

#define CX_BITMAP         16
#define CY_BITMAP         16

#define INDEX_OPEN_ROOT      0
#define INDEX_CLOSE_ROOT     1
#define INDEX_OPEN_NODE	     3
#define INDEX_CLOSED_NODE    4
#define INDEX_LEAF           5
#define INDEX_LEAF_INVALID   6

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

FileBrowser::~FileBrowser()
{
	for (size_t i = 0; i < _folderUpdaters.size(); ++i)
	{
		_folderUpdaters[i].stopWatcher();
	}
}

INT_PTR CALLBACK FileBrowser::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG :
        {
			FileBrowser::initMenus();

			// Create toolbar menu
			int style = WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_AUTOSIZE | TBSTYLE_FLAT | TBSTYLE_LIST;
			_hToolbarMenu = CreateWindowEx(0,TOOLBARCLASSNAME,NULL, style,
								   0,0,0,0,_hSelf,(HMENU)0, _hInst, NULL);
			TBBUTTON tbButtons[2];

			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance())->getNativeLangSpeaker();
			generic_string workspace_entry = pNativeSpeaker->getProjectPanelLangMenuStr("Entries", 0, PM_WORKSPACEMENUENTRY);
			generic_string edit_entry = pNativeSpeaker->getProjectPanelLangMenuStr("Entries", 1, PM_EDITMENUENTRY);

			tbButtons[0].idCommand = IDB_FILEBROWSER_BTN;
			tbButtons[0].iBitmap = I_IMAGENONE;
			tbButtons[0].fsState = TBSTATE_ENABLED;
			tbButtons[0].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
			tbButtons[0].iString = (INT_PTR)workspace_entry.c_str();

			tbButtons[1].idCommand = IDB_FILEBROWSER_EDIT_BTN;
			tbButtons[1].iBitmap = I_IMAGENONE;
			tbButtons[1].fsState = TBSTATE_ENABLED;
			tbButtons[1].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
			tbButtons[1].iString = (INT_PTR)edit_entry.c_str();

			SendMessage(_hToolbarMenu, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage(_hToolbarMenu, TB_ADDBUTTONS,       (WPARAM)sizeof(tbButtons) / sizeof(TBBUTTON),       (LPARAM)&tbButtons);
			SendMessage(_hToolbarMenu, TB_AUTOSIZE, 0, 0); 
			ShowWindow(_hToolbarMenu, SW_SHOW);

			_treeView.init(_hInst, _hSelf, ID_FILEBROWSERTREEVIEW);

			setImageList(IDI_PROJECT_WORKSPACE, IDI_PROJECT_WORKSPACEDIRTY, IDI_PROJECT_PROJECT, IDI_PROJECT_FOLDEROPEN, IDI_PROJECT_FOLDERCLOSE, IDI_PROJECT_FILE, IDI_PROJECT_FILEINVALID);
			_treeView.addCanNotDropInList(INDEX_LEAF);
			_treeView.addCanNotDropInList(INDEX_LEAF_INVALID);

			//_treeView.addCanNotDragOutList(INDEX_CLEAN_ROOT);
			//_treeView.addCanNotDragOutList(INDEX_DIRTY_ROOT);
			//_treeView.addCanNotDragOutList(INDEX_PROJECT);

			_treeView.display();

			/*
			if (!openWorkSpace(_workSpaceFilePath.c_str()))
				newWorkSpace();
			*/

            return TRUE;
        }

		
		case WM_MOUSEMOVE:
			if (_treeView.isDragging())
				_treeView.dragItem(_hSelf, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			if (_treeView.isDragging())
				if (_treeView.dropItem())
				{
				
				}
			break;

		case WM_NOTIFY:
		{
			notified((LPNMHDR)lParam);
		}
		return TRUE;

        case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            RECT toolbarMenuRect;
            ::GetClientRect(_hToolbarMenu, &toolbarMenuRect);

            ::MoveWindow(_hToolbarMenu, 0, 0, width, toolbarMenuRect.bottom, TRUE);

			HWND hwnd = _treeView.getHSelf();
			if (hwnd)
				::MoveWindow(hwnd, 0, toolbarMenuRect.bottom + 2, width, height - toolbarMenuRect.bottom - 2, TRUE);
            break;
        }

        case WM_CONTEXTMENU:
			if (!_treeView.isDragging())
				showContextMenu(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return TRUE;

		case WM_COMMAND:
		{
			popupMenuCmd(LOWORD(wParam));
			break;
		}

		case WM_DESTROY:
        {
			_treeView.destroy();
			destroyMenus();
			::DestroyWindow(_hToolbarMenu);
            break;
        }
		case WM_KEYDOWN:
			//if (wParam == VK_F2)
			{
				::MessageBoxA(NULL,"vkF2","",MB_OK);
			}
			break;

        default :
            return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
    }
	return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
}

void FileBrowser::initMenus()
{
	_hWorkSpaceMenu = ::CreatePopupMenu();
	
	NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance())->getNativeLangSpeaker();

	generic_string edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_MOVEUP, PM_MOVEUPENTRY);
	generic_string edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_MOVEDOWN, PM_MOVEDOWNENTRY);
	generic_string edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_RENAME, PM_EDITRENAME);
	generic_string edit_addfolder = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_NEWFOLDER, PM_EDITNEWFOLDER);
	generic_string edit_addfiles = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_ADDFILES, PM_EDITADDFILES);
	//generic_string edit_addfilesRecursive = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_ADDFILESRECUSIVELY, PM_EDITADDFILESRECUSIVELY);
	generic_string edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("ProjectMenu", IDM_FILEBROWSER_DELETEFOLDER, PM_EDITREMOVE);

	_hProjectMenu = ::CreatePopupMenu();
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEUP, edit_moveup.c_str());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEDOWN, edit_movedown.c_str());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_RENAME, edit_rename.c_str());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_NEWFOLDER, edit_addfolder.c_str());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_ADDFILES, edit_addfiles.c_str());
	//::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_ADDFILESRECUSIVELY, edit_addfilesRecursive.c_str());
	::InsertMenu(_hProjectMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_DELETEFOLDER, edit_remove.c_str());

	edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_MOVEUP, PM_MOVEUPENTRY);
	edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_MOVEDOWN, PM_MOVEDOWNENTRY);
	edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_RENAME, PM_EDITRENAME);
	edit_addfolder = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_NEWFOLDER, PM_EDITNEWFOLDER);
	edit_addfiles = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_ADDFILES, PM_EDITADDFILES);
	//edit_addfilesRecursive = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_ADDFILESRECUSIVELY, PM_EDITADDFILESRECUSIVELY);
	edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("FolderMenu", IDM_FILEBROWSER_DELETEFOLDER, PM_EDITREMOVE);

	_hFolderMenu = ::CreatePopupMenu();
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEUP,        edit_moveup.c_str());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEDOWN,      edit_movedown.c_str());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_RENAME,        edit_rename.c_str());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_NEWFOLDER,     edit_addfolder.c_str());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_ADDFILES,      edit_addfiles.c_str());
	//::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_ADDFILESRECUSIVELY, edit_addfilesRecursive.c_str());
	::InsertMenu(_hFolderMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_DELETEFOLDER,  edit_remove.c_str());

	edit_moveup = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_FILEBROWSER_MOVEUP, PM_MOVEUPENTRY);
	edit_movedown = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_FILEBROWSER_MOVEDOWN, PM_MOVEDOWNENTRY);
	edit_rename = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_FILEBROWSER_RENAME, PM_EDITRENAME);
	edit_remove = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_FILEBROWSER_DELETEFILE, PM_EDITREMOVE);
	generic_string edit_modifyfile = pNativeSpeaker->getProjectPanelLangMenuStr("FileMenu", IDM_FILEBROWSER_MODIFYFILEPATH, PM_EDITMODIFYFILE);

	_hFileMenu = ::CreatePopupMenu();
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEUP, edit_moveup.c_str());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MOVEDOWN, edit_movedown.c_str());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, UINT(-1), 0);
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_RENAME, edit_rename.c_str());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_DELETEFILE, edit_remove.c_str());
	::InsertMenu(_hFileMenu, 0, MF_BYCOMMAND, IDM_FILEBROWSER_MODIFYFILEPATH, edit_modifyfile.c_str());
}


BOOL FileBrowser::setImageList(int root_clean_id, int root_dirty_id, int project_id, int open_node_id, int closed_node_id, int leaf_id, int ivalid_leaf_id) 
{
	HBITMAP hbmp;
	COLORREF maskColour = RGB(192, 192, 192);
	const int nbBitmaps = 7;

	// Creation of image list
	if ((_hImaLst = ImageList_Create(CX_BITMAP, CY_BITMAP, ILC_COLOR32 | ILC_MASK, nbBitmaps, 0)) == NULL) 
		return FALSE;

	// Add the bmp in the list
	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(root_clean_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(root_dirty_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(project_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(open_node_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(closed_node_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(leaf_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(_hInst, MAKEINTRESOURCE(ivalid_leaf_id));
	if (hbmp == NULL)
		return FALSE;
	ImageList_AddMasked(_hImaLst, hbmp, maskColour);
	DeleteObject(hbmp);

	if (ImageList_GetImageCount(_hImaLst) < nbBitmaps)
		return FALSE;

	// Set image list to the tree view
	TreeView_SetImageList(_treeView.getHSelf(), _hImaLst, TVSIL_NORMAL);

	return TRUE;
}


void FileBrowser::destroyMenus() 
{
	::DestroyMenu(_hWorkSpaceMenu);
	::DestroyMenu(_hProjectMenu);
	::DestroyMenu(_hFolderMenu);
	::DestroyMenu(_hFileMenu);
}

void FileBrowser::buildProjectXml(TiXmlNode *node, HTREEITEM hItem, const TCHAR* fn2write)
{
	TCHAR textBuffer[MAX_PATH];
	TVITEM tvItem;
	tvItem.mask = TVIF_TEXT | TVIF_PARAM;
	tvItem.pszText = textBuffer;
	tvItem.cchTextMax = MAX_PATH;

    for (HTREEITEM hItemNode = _treeView.getChildFrom(hItem);
		hItemNode != NULL;
		hItemNode = _treeView.getNextSibling(hItemNode))
	{
		tvItem.hItem = hItemNode;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0,(LPARAM)&tvItem);
		if (tvItem.lParam != NULL)
		{
			generic_string *fn = (generic_string *)tvItem.lParam;
			generic_string newFn = getRelativePath(*fn, fn2write);
			TiXmlNode *fileLeaf = node->InsertEndChild(TiXmlElement(TEXT("File")));
			fileLeaf->ToElement()->SetAttribute(TEXT("name"), newFn.c_str());
		}
		else
		{
			TiXmlNode *folderNode = node->InsertEndChild(TiXmlElement(TEXT("Folder")));
			folderNode->ToElement()->SetAttribute(TEXT("name"), tvItem.pszText);
			buildProjectXml(folderNode, hItemNode, fn2write);
		}
	}
}

generic_string FileBrowser::getRelativePath(const generic_string & filePath, const TCHAR *workSpaceFileName)
{
	TCHAR wsfn[MAX_PATH];
	lstrcpy(wsfn, workSpaceFileName);
	::PathRemoveFileSpec(wsfn);

	size_t pos_found = filePath.find(wsfn);
	if (pos_found == generic_string::npos)
		return filePath;
	const TCHAR *relativeFile = filePath.c_str() + lstrlen(wsfn);
	if (relativeFile[0] == '\\')
		++relativeFile;
	return relativeFile;
}

void FileBrowser::openSelectFile()
{
	TVITEM tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = _treeView.getSelection();
	::SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0,(LPARAM)&tvItem);

	BrowserNodeType nType = getNodeType(tvItem.hItem);
	generic_string *fn = (generic_string *)tvItem.lParam;
	if (nType == browserNodeType_file && fn)
	{
		tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		if (::PathFileExists(fn->c_str()))
		{
			::SendMessage(_hParent, NPPM_DOOPEN, 0, (LPARAM)(fn->c_str()));
			tvItem.iImage = INDEX_LEAF;
			tvItem.iSelectedImage = INDEX_LEAF;
		}
		else
		{
			tvItem.iImage = INDEX_LEAF_INVALID;
			tvItem.iSelectedImage = INDEX_LEAF_INVALID;
		}
		TreeView_SetItem(_treeView.getHSelf(), &tvItem);
	}
}


void FileBrowser::notified(LPNMHDR notification)
{
	if ((notification->hwndFrom == _treeView.getHSelf()))
	{
		TCHAR textBuffer[MAX_PATH];
		TVITEM tvItem;
		tvItem.mask = TVIF_TEXT | TVIF_PARAM;
		tvItem.pszText = textBuffer;
		tvItem.cchTextMax = MAX_PATH;

		switch (notification->code)
		{
			case NM_DBLCLK:
			{
				openSelectFile();
			}
			break;
	
			case TVN_ENDLABELEDIT:
			{
				LPNMTVDISPINFO tvnotif = (LPNMTVDISPINFO)notification;
				if (!tvnotif->item.pszText)
					return;
				if (getNodeType(tvnotif->item.hItem) == browserNodeType_root)
					return;

				// Processing for only File case
				if (tvnotif->item.lParam) 
				{
					// Get the old label
					tvItem.hItem = _treeView.getSelection();
					::SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0,(LPARAM)&tvItem);
					size_t len = lstrlen(tvItem.pszText);

					// Find the position of old label in File path
					generic_string *filePath = (generic_string *)tvnotif->item.lParam;
					size_t found = filePath->rfind(tvItem.pszText);

					// If found the old label, replace it with the modified one
					if (found != generic_string::npos)
						filePath->replace(found, len, tvnotif->item.pszText);

					// Check the validity of modified file path
					tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					if (::PathFileExists(filePath->c_str()))
					{
						tvItem.iImage = INDEX_LEAF;
						tvItem.iSelectedImage = INDEX_LEAF;
					}
					else
					{
						tvItem.iImage = INDEX_LEAF_INVALID;
						tvItem.iSelectedImage = INDEX_LEAF_INVALID;
					}
					TreeView_SetItem(_treeView.getHSelf(), &tvItem);
				}

				// For File, Folder and Project
				::SendMessage(_treeView.getHSelf(), TVM_SETITEM, 0,(LPARAM)(&(tvnotif->item)));
			}
			break;

			case TVN_KEYDOWN:
			{
				//tvItem.hItem = _treeView.getSelection();
				//::SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0,(LPARAM)&tvItem);
				LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)notification;
				
				if (ptvkd->wVKey == VK_DELETE)
				{
					HTREEITEM hItem = _treeView.getSelection();
					BrowserNodeType nType = getNodeType(hItem);
					if (nType == browserNodeType_folder)
						popupMenuCmd(IDM_FILEBROWSER_DELETEFOLDER);
					else if (nType == browserNodeType_file)
						popupMenuCmd(IDM_FILEBROWSER_DELETEFILE);
				}
				else if (ptvkd->wVKey == VK_RETURN)
				{
					HTREEITEM hItem = _treeView.getSelection();
					BrowserNodeType nType = getNodeType(hItem);
					if (nType == browserNodeType_file)
						openSelectFile();
					else
						_treeView.toggleExpandCollapse(hItem);
				}
				else if (ptvkd->wVKey == VK_UP)
				{
					if (0x80 & GetKeyState(VK_CONTROL))
					{
						popupMenuCmd(IDM_FILEBROWSER_MOVEUP);
					}
				}
				else if (ptvkd->wVKey == VK_DOWN)
				{
					if (0x80 & GetKeyState(VK_CONTROL))
					{
						popupMenuCmd(IDM_FILEBROWSER_MOVEDOWN);
					}
				}
				else if (ptvkd->wVKey == VK_F2)
					popupMenuCmd(IDM_FILEBROWSER_RENAME);
				
			}
			break;

			case TVN_ITEMEXPANDED:
			{
				LPNMTREEVIEW nmtv = (LPNMTREEVIEW)notification;
				tvItem.hItem = nmtv->itemNew.hItem;
				tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;

				if (getNodeType(nmtv->itemNew.hItem) == browserNodeType_folder)
				{
					if (nmtv->action == TVE_COLLAPSE)
					{
						_treeView.setItemImage(nmtv->itemNew.hItem, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
					}
					else if (nmtv->action == TVE_EXPAND)
					{
						_treeView.setItemImage(nmtv->itemNew.hItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);
					}
				}
			}
			break;

			case TVN_BEGINDRAG:
			{
				//printStr(TEXT("hello"));
				_treeView.beginDrag((LPNMTREEVIEW)notification);
				
			}
			break;
		}
	}
}

BrowserNodeType FileBrowser::getNodeType(HTREEITEM hItem)
{
	TVITEM tvItem;
	tvItem.hItem = hItem;
	tvItem.mask = TVIF_IMAGE | TVIF_PARAM;
	SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0,(LPARAM)&tvItem);

	// Root
	if (tvItem.iImage == INDEX_CLOSE_ROOT || tvItem.iImage == INDEX_OPEN_ROOT)
	{
		return browserNodeType_root;
	}
	// Folder
	else if (tvItem.lParam == NULL)
	{
		return browserNodeType_folder;
	}
	// File
	else
	{
		return browserNodeType_file;
	}
}

void FileBrowser::showContextMenu(int x, int y)
{
	TVHITTESTINFO tvHitInfo;
	HTREEITEM hTreeItem;

	// Detect if the given position is on the element TVITEM
	tvHitInfo.pt.x = x;
	tvHitInfo.pt.y = y;
	tvHitInfo.flags = 0;
	ScreenToClient(_treeView.getHSelf(), &(tvHitInfo.pt));
	hTreeItem = TreeView_HitTest(_treeView.getHSelf(), &tvHitInfo);

	if (tvHitInfo.hItem != NULL)
	{
		// Make item selected
		_treeView.selectItem(tvHitInfo.hItem);

		// get clicked item type
		BrowserNodeType nodeType = getNodeType(tvHitInfo.hItem);
		HMENU hMenu = NULL;
		if (nodeType == browserNodeType_root)
			hMenu = _hWorkSpaceMenu;
		else if (nodeType == browserNodeType_folder)
			hMenu = _hFolderMenu;
		else //nodeType_file
			hMenu = _hFileMenu;
		TrackPopupMenu(hMenu, TPM_LEFTALIGN, x, y, 0, _hSelf, NULL);
	}
}

POINT FileBrowser::getMenuDisplayPoint(int iButton)
{
	POINT p;
	RECT btnRect;
	SendMessage(_hToolbarMenu, TB_GETITEMRECT, iButton, (LPARAM)&btnRect);

	p.x = btnRect.left;
	p.y = btnRect.top + btnRect.bottom;
	ClientToScreen(_hToolbarMenu, &p);
	return p;
}

HTREEITEM FileBrowser::createNewFolder(HTREEITEM hTreeItem, const TCHAR *folderName)
{
	HTREEITEM addedItem = _treeView.addItem(folderName, hTreeItem, INDEX_CLOSED_NODE);
	
	TreeView_Expand(_treeView.getHSelf(), hTreeItem, TVE_EXPAND);
	TreeView_EditLabel(_treeView.getHSelf(), addedItem);
	if (getNodeType(hTreeItem) == browserNodeType_folder)
		_treeView.setItemImage(hTreeItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);

	return addedItem;
}


void FileBrowser::popupMenuCmd(int cmdID)
{
	// get selected item handle
	HTREEITEM hTreeItem = _treeView.getSelection();
	if (!hTreeItem)
		return;

	switch (cmdID)
	{
		//
		// Toolbar menu buttons
		//
		case IDB_FILEBROWSER_BTN:
		{
		  POINT p = getMenuDisplayPoint(0);
		  TrackPopupMenu(_hWorkSpaceMenu, TPM_LEFTALIGN, p.x, p.y, 0, _hSelf, NULL);
		}
		break;

		case IDB_FILEBROWSER_EDIT_BTN:
		{
			POINT p = getMenuDisplayPoint(1);
			HMENU hMenu = NULL;
			BrowserNodeType nodeType = getNodeType(hTreeItem);

			if (nodeType == browserNodeType_folder)
				hMenu = _hFolderMenu;
			else if (nodeType == browserNodeType_file)
				hMenu = _hFileMenu;
			if (hMenu)
				TrackPopupMenu(hMenu, TPM_LEFTALIGN, p.x, p.y, 0, _hSelf, NULL);
		}
		break;

		//
		// Toolbar menu commands
		//

		case IDM_FILEBROWSER_RENAME :
			TreeView_EditLabel(_treeView.getHSelf(), hTreeItem);
		break;
		
		case IDM_FILEBROWSER_NEWFOLDER :
		{
			NativeLangSpeaker *pNativeSpeaker = (NppParameters::getInstance())->getNativeLangSpeaker();
			generic_string newFolderLabel = pNativeSpeaker->getAttrNameStr(PM_NEWFOLDERNAME, "ProjectManager", "NewFolderName");
			createNewFolder(hTreeItem, newFolderLabel.c_str());
		}
		break;

		case IDM_FILEBROWSER_MOVEDOWN :
		{
			_treeView.moveDown(hTreeItem);
		}
		break;

		case IDM_FILEBROWSER_MOVEUP :
		{
			_treeView.moveUp(hTreeItem);
		}
		break;
		
		/*
		case IDM_FILEBROWSER_ADDFILES :
		{
			addFiles(hTreeItem);
			if (getNodeType(hTreeItem) == browserNodeType_folder)
				_treeView.setItemImage(hTreeItem, INDEX_OPEN_NODE, INDEX_OPEN_NODE);
		}
		break;
		*/

		case IDM_FILEBROWSER_DELETEFOLDER :
		{
			HTREEITEM parent = _treeView.getParent(hTreeItem);

			if (_treeView.getChildFrom(hTreeItem) != NULL)
			{
				TCHAR str2display[MAX_PATH] = TEXT("All the sub-items will be removed.\rAre you sure you want to remove this folder from the project?");
				if (::MessageBox(_hSelf, str2display, TEXT("Remove folder from project"), MB_YESNO) == IDYES)
				{
					_treeView.removeItem(hTreeItem);
					//_folderUpdaters[0].stopWatcher();
				}
			}
			else
			{
				_treeView.removeItem(hTreeItem);
			}
			if (getNodeType(parent) == browserNodeType_folder)
				_treeView.setItemImage(parent, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
		}
		break;

		case IDM_FILEBROWSER_DELETEFILE :
		{
			HTREEITEM parent = _treeView.getParent(hTreeItem);

			TCHAR str2display[MAX_PATH] = TEXT("Are you sure you want to remove this file from the project?");
			if (::MessageBox(_hSelf, str2display, TEXT("Remove file from project"), MB_YESNO) == IDYES)
			{
				_treeView.removeItem(hTreeItem);
				if (getNodeType(parent) == browserNodeType_folder)
					_treeView.setItemImage(parent, INDEX_CLOSED_NODE, INDEX_CLOSED_NODE);
			}
		}
		break;
	}
}



void FileBrowser::getDirectoryStructure(const TCHAR *dir, const std::vector<generic_string> & patterns, FolderInfo & directoryStructure, bool isRecursive, bool isInHiddenDir)
{
	if (directoryStructure._parent == nullptr) // Root!
		directoryStructure.setRootPath(dir);

	generic_string dirFilter(dir);
	dirFilter += TEXT("*.*");
	WIN32_FIND_DATA foundData;

	HANDLE hFile = ::FindFirstFile(dirFilter.c_str(), &foundData);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		if (foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!isInHiddenDir && (foundData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				// do nothing
			}
			else if (isRecursive)
			{
				if ((lstrcmp(foundData.cFileName, TEXT("."))) && (lstrcmp(foundData.cFileName, TEXT(".."))))
				{
					generic_string pathDir(dir);
					pathDir += foundData.cFileName;
					pathDir += TEXT("\\");

					FolderInfo subDirectoryStructure(foundData.cFileName, &directoryStructure);
					getDirectoryStructure(pathDir.c_str(), patterns, subDirectoryStructure, isRecursive, isInHiddenDir);
					directoryStructure.addSubFolder(subDirectoryStructure);
				}
			}
		}
		else
		{
			if (matchInList(foundData.cFileName, patterns))
			{
				directoryStructure.addFile(foundData.cFileName);
			}
		}
	}

	while (::FindNextFile(hFile, &foundData))
	{
		if (foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!isInHiddenDir && (foundData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				// do nothing
			}
			else if (isRecursive)
			{
				if ((lstrcmp(foundData.cFileName, TEXT("."))) && (lstrcmp(foundData.cFileName, TEXT(".."))))
				{
					generic_string pathDir(dir);
					pathDir += foundData.cFileName;
					pathDir += TEXT("\\");

					FolderInfo subDirectoryStructure(foundData.cFileName, &directoryStructure);
					getDirectoryStructure(pathDir.c_str(), patterns, subDirectoryStructure, isRecursive, isInHiddenDir);
					directoryStructure.addSubFolder(subDirectoryStructure);
				}
			}
		}
		else
		{
			if (matchInList(foundData.cFileName, patterns))
			{
				directoryStructure.addFile(foundData.cFileName);
			}
		}
	}
	::FindClose(hFile);
}

void FileBrowser::addRootFolder(generic_string rootFolderPath)
{
	size_t nbFolderUpdaters = _folderUpdaters.size();
	for (size_t i = 0; i < nbFolderUpdaters; ++i)
	{
		if (_folderUpdaters[i]._rootFolder._rootPath == rootFolderPath)
			return;
	}

	std::vector<generic_string> patterns2Match;
	patterns2Match.push_back(TEXT("*.*"));

	TCHAR *label = ::PathFindFileName(rootFolderPath.c_str());
	FolderInfo directoryStructure(label, nullptr);
	getDirectoryStructure(rootFolderPath.c_str(), patterns2Match, directoryStructure, true, false);
	HTREEITEM hRootItem = createFolderItemsFromDirStruct(nullptr, directoryStructure);
	_treeView.expand(hRootItem);
	_folderUpdaters.push_back(FolderUpdater(directoryStructure, this));
	_folderUpdaters[_folderUpdaters.size() - 1].startWatcher();
}

HTREEITEM FileBrowser::createFolderItemsFromDirStruct(HTREEITEM hParentItem, const FolderInfo & directoryStructure)
{
	HTREEITEM hFolderItem = nullptr;
	if (directoryStructure._parent == nullptr && hParentItem == nullptr)
	{
		TCHAR rootPath[MAX_PATH];
		lstrcpy(rootPath, directoryStructure._rootPath.c_str());
		size_t len = lstrlen(rootPath);
		if (rootPath[len - 1] == '\\')
			rootPath[len - 1] = '\0';
		hFolderItem = _treeView.addItem(directoryStructure._name.c_str(), TVI_ROOT, INDEX_CLOSED_NODE, rootPath);
	}
	else
	{
		hFolderItem = _treeView.addItem(directoryStructure._name.c_str(), hParentItem, INDEX_CLOSED_NODE);
	}

	for (size_t i = 0; i < directoryStructure._subFolders.size(); ++i)
	{
		createFolderItemsFromDirStruct(hFolderItem, directoryStructure._subFolders[i]);
	}

	for (size_t i = 0; i < directoryStructure._files.size(); ++i)
	{
		_treeView.addItem(directoryStructure._files[i]._name.c_str(), hFolderItem, INDEX_LEAF);
	}
	_treeView.fold(hParentItem);

	return hFolderItem;
}

HTREEITEM FileBrowser::getRootFromFullPath(generic_string rootPath)
{
	HTREEITEM node = nullptr;
	for (HTREEITEM hItemNode = _treeView.getRoot();
		hItemNode != nullptr && node == nullptr;
		hItemNode = _treeView.getNextSibling(hItemNode))
	{
		TVITEM tvItem;
		tvItem.mask = TVIF_PARAM;
		tvItem.cchTextMax = MAX_PATH;
		tvItem.hItem = hItemNode;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, (LPARAM)&tvItem);

		if (tvItem.lParam != 0 && rootPath == *((generic_string *)tvItem.lParam))
			node = hItemNode;
	}
	return node;
}

HTREEITEM FileBrowser::findChildNodeFromName(HTREEITEM parent, generic_string label)
{
	HTREEITEM childNodeFound = nullptr;

	for (HTREEITEM hItemNode = _treeView.getChildFrom(parent);
		hItemNode != NULL && childNodeFound == nullptr;
		hItemNode = _treeView.getNextSibling(hItemNode))
	{
		TCHAR textBuffer[MAX_PATH];
		TVITEM tvItem;
		tvItem.mask = TVIF_TEXT;
		tvItem.pszText = textBuffer;
		tvItem.cchTextMax = MAX_PATH;
		tvItem.hItem = hItemNode;
		SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, (LPARAM)&tvItem);

		if (label == tvItem.pszText)
		{
			childNodeFound = hItemNode;
		}
	}
	return childNodeFound;
}

bool FileBrowser::addInTree(generic_string rootPath, generic_string addItemFullPath, HTREEITEM node, vector<generic_string> linarPathArray)
{
	if (node == nullptr) // it's a root. Search the right root with rootPath
	{
		// Search
		if ((node = getRootFromFullPath(rootPath)) == nullptr)
			return false;
	}

	if (linarPathArray.size() == 1)
	{
		// Search : if no found, add
		HTREEITEM childNodeFound = findChildNodeFromName(node, linarPathArray[0]);
		if (childNodeFound != nullptr)
			return false;

		// No found, good - Action
		if (::PathIsDirectory(addItemFullPath.c_str()))
		{
			_treeView.addItem(linarPathArray[0].c_str(), node, INDEX_CLOSED_NODE);
		}
		else
		{
			_treeView.addItem(linarPathArray[0].c_str(), node, INDEX_LEAF);
		}
		return true;
	}
	else
	{
		HTREEITEM childNodeFound = nullptr;
		for (HTREEITEM hItemNode = _treeView.getChildFrom(node);
			hItemNode != NULL && childNodeFound == nullptr;
			hItemNode = _treeView.getNextSibling(hItemNode))
		{
			TCHAR textBuffer[MAX_PATH];
			TVITEM tvItem;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = textBuffer;
			tvItem.cchTextMax = MAX_PATH;
			tvItem.hItem = hItemNode;
			SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, (LPARAM)&tvItem);

			if (linarPathArray[0] == tvItem.pszText)
			{
				// search recursively the node for an action
				linarPathArray.erase(linarPathArray.begin());
				return addInTree(rootPath, addItemFullPath, hItemNode, linarPathArray);
			}
		}
		return false;
	}
}

bool FileBrowser::deleteFromTree(generic_string rootPath, HTREEITEM node, std::vector<generic_string> linarPathArray)
{
	if (node == nullptr) // it's a root. Search the right root with rootPath
	{
		// Search
		if ((node = getRootFromFullPath(rootPath)) == nullptr)
			return false;
	}

	if (linarPathArray.size() == 1)
	{
		// Search
		HTREEITEM childNodeFound = findChildNodeFromName(node, linarPathArray[0]);
		if (childNodeFound == nullptr)
			return false;

		// found it, delete it
		_treeView.removeItem(childNodeFound);
		return true;
	}
	else
	{
		HTREEITEM childNodeFound = nullptr;
		for (HTREEITEM hItemNode = _treeView.getChildFrom(node);
			hItemNode != NULL && childNodeFound == nullptr;
			hItemNode = _treeView.getNextSibling(hItemNode))
		{
			TCHAR textBuffer[MAX_PATH];
			TVITEM tvItem;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = textBuffer;
			tvItem.cchTextMax = MAX_PATH;
			tvItem.hItem = hItemNode;
			SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, (LPARAM)&tvItem);

			if (linarPathArray[0] == tvItem.pszText)
			{
				// search recursively the node for an action
				linarPathArray.erase(linarPathArray.begin());
				return deleteFromTree(rootPath, hItemNode, linarPathArray);
			}
		}
		return false;
	}
}

bool FileBrowser::renameInTree(generic_string rootPath, HTREEITEM node, std::vector<generic_string> linarPathArrayFrom, std::vector<generic_string> linarPathArrayTo)
{
	if (node == nullptr) // it's a root. Search the right root with rootPath
	{
		// Search
		if ((node = getRootFromFullPath(rootPath)) == nullptr)
			return false;
	}

	if (linarPathArrayFrom.size() == 1)
	{
		// Search
		HTREEITEM childNodeFound = findChildNodeFromName(node, linarPathArrayFrom[0]);
		if (childNodeFound == nullptr)
			return false;

		// found it, rename it
		_treeView.renameItem(childNodeFound, linarPathArrayTo[0].c_str());
		return true;
	}
	else
	{
		HTREEITEM childNodeFound = nullptr;
		for (HTREEITEM hItemNode = _treeView.getChildFrom(node);
			hItemNode != NULL && childNodeFound == nullptr;
			hItemNode = _treeView.getNextSibling(hItemNode))
		{
			TCHAR textBuffer[MAX_PATH];
			TVITEM tvItem;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = textBuffer;
			tvItem.cchTextMax = MAX_PATH;
			tvItem.hItem = hItemNode;
			SendMessage(_treeView.getHSelf(), TVM_GETITEM, 0, (LPARAM)&tvItem);

			if (linarPathArrayFrom[0] == tvItem.pszText)
			{
				// search recursively the node for an action
				linarPathArrayFrom.erase(linarPathArrayFrom.begin());
				linarPathArrayTo.erase(linarPathArrayTo.begin());
				return renameInTree(rootPath, hItemNode, linarPathArrayFrom, linarPathArrayTo);
			}
		}
		return false;
	}
}

bool FolderInfo::addToStructure(generic_string & fullpath, std::vector<generic_string> linarPathArray)
{
	if (linarPathArray.size() == 1) // could be file or folder
	{
		fullpath += TEXT("\\");
		fullpath += linarPathArray[0];
		if (PathIsDirectory(fullpath.c_str()))
		{
			// search in folders, if found - no good
			size_t nbFolder = _subFolders.size();
			for (size_t i = 0; i < nbFolder; ++i)
			{
				if (linarPathArray[0] == _subFolders[i].getName())
					return false; // Maybe already added?
			}
			_subFolders.push_back(FolderInfo(linarPathArray[0], this));
			return true;
		}
		else
		{
			// search in files, if found - no good
			size_t nbFile = _files.size();
			for (size_t i = 0; i < nbFile; ++i)
			{
				if (linarPathArray[0] == _files[i].getName())
					return false; // Maybe already added?
			}
			_files.push_back(FileInfo(linarPathArray[0], this));
			return true;
		}	
	}
	else // folder
	{
		size_t nbFolder = _subFolders.size();
		for (size_t i = 0; i < nbFolder; ++i)
		{
			if (_subFolders[i].getName() == linarPathArray[0])
			{
				fullpath += TEXT("\\");
				fullpath += linarPathArray[0];
				linarPathArray.erase(linarPathArray.begin());
				return _subFolders[i].addToStructure(fullpath, linarPathArray);
			}
		}
		return false;
	}
}

bool FolderInfo::removeFromStructure(std::vector<generic_string> linarPathArray)
{
	if (linarPathArray.size() == 1) // could be file or folder
	{
		for (size_t i = 0; i < _files.size(); ++i)
		{
			if (_files[i].getName() == linarPathArray[0])
			{
				// remove this file
				_files.erase(_files.begin() + i);
				return true;
			}
		}

		for (size_t i = 0; i < _subFolders.size(); ++i)
		{
			if (_subFolders[i].getName() == linarPathArray[0])
			{
				// remove this folder
				_subFolders.erase(_subFolders.begin() + i);
				return true;
			}
		}
	}
	else // folder
	{
		for (size_t i = 0; i < _subFolders.size(); ++i)
		{
			if (_subFolders[i].getName() == linarPathArray[0])
			{
				linarPathArray.erase(linarPathArray.begin());
				return _subFolders[i].removeFromStructure(linarPathArray);
			}
		}
	}
	return false;
}

bool FolderInfo::renameInStructure(std::vector<generic_string> linarPathArrayFrom, std::vector<generic_string> linarPathArrayTo)
{
	if (linarPathArrayFrom.size() == 1) // could be file or folder
	{
		for (size_t i = 0; i < _files.size(); ++i)
		{
			if (_files[i].getName() == linarPathArrayFrom[0])
			{
				// rename this file
				_files[i].setName(linarPathArrayTo[0]);
				return true;
			}
		}

		for (size_t i = 0; i < _subFolders.size(); ++i)
		{
			if (_subFolders[i].getName() == linarPathArrayFrom[0])
			{
				// rename this folder
				_subFolders[i].setName(linarPathArrayTo[0]);
				return true;
			}
		}
		return false;
	}
	else // folder
	{
		for (size_t i = 0; i < _subFolders.size(); ++i)
		{
			if (_subFolders[i].getName() == linarPathArrayFrom[0])
			{
				linarPathArrayFrom.erase(linarPathArrayFrom.begin());
				linarPathArrayTo.erase(linarPathArrayTo.begin());
				return _subFolders[i].renameInStructure(linarPathArrayFrom, linarPathArrayTo);
			}
		}
		return false;
	}
}

void FolderUpdater::startWatcher()
{
	// no thread yet, create a event with non-signaled, to block all threads
	_EventHandle = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
	_watchThreadHandle = ::CreateThread(NULL, 0, watching, this, 0, NULL);
}

void FolderUpdater::stopWatcher()
{
	::SetEvent(_EventHandle);
	::CloseHandle(_watchThreadHandle);
	::CloseHandle(_EventHandle);
}

LPCWSTR explainAction(DWORD dwAction)
{
	switch (dwAction)
	{
	case FILE_ACTION_ADDED:
		return L"Added";
	case FILE_ACTION_REMOVED:
		return L"Deleted";
	case FILE_ACTION_MODIFIED:
		return L"Modified";
	case FILE_ACTION_RENAMED_OLD_NAME:
		return L"Renamed From";
	case FILE_ACTION_RENAMED_NEW_NAME:
		return L"Renamed ";
	default:
		return L"BAD DATA";
	}
};

vector<generic_string> split(const generic_string & string2split, TCHAR sep)
{
	vector<generic_string> splitedStrings;
	size_t len = string2split.length();
	size_t beginPos = 0;
	for (size_t i = 0; i < len + 1; ++i)
	{
		if (string2split[i] == sep || string2split[i] == '\0')
		{
			splitedStrings.push_back(string2split.substr(beginPos, i - beginPos));
			beginPos = i + 1;
		}
	}
	return splitedStrings;
};

bool FolderUpdater::updateTree(DWORD action, const std::vector<generic_string> & file2Change)
{
	/*
	TCHAR msg2show[1024];
	switch (action)
	{
	case FILE_ACTION_ADDED:
		swprintf(msg2show, L"%s %s\n", explainAction(action), file2Change[0].c_str());
		printStr(msg2show);
		//::PostMessage(thisFolderUpdater->_hFileBrowser, FB_ADDFILE, nullptr, (LPARAM)wstrFilename.GetString());
		break;

	case FILE_ACTION_REMOVED:
		swprintf(msg2show, L"%s %s\n", explainAction(action), file2Change[0].c_str());
		printStr(msg2show);

		break;

	case FILE_ACTION_RENAMED_NEW_NAME:
		swprintf(msg2show, L"%s from %s \rto %s", explainAction(action), file2Change[0].c_str(), file2Change[1].c_str());
		printStr(msg2show);

		break;

	default:
		break;
	}
	*/
	generic_string separator = TEXT("\\\\");

	size_t sepPos = file2Change[0].find(separator);
	if (sepPos == generic_string::npos)
		return false;

	generic_string pathSuffix = file2Change[0].substr(sepPos + separator.length(), file2Change[0].length() - 1);

	// remove prefix of file/folder in changeInfo, splite the remained path
	vector<generic_string> linarPathArray = split(pathSuffix, '\\');

	if (action == FILE_ACTION_ADDED)
	{
		generic_string rootPath = file2Change[0].substr(0, sepPos);
		generic_string path = rootPath;
		// search recursively and modify the tree structure
		bool foundAndModified = _rootFolder.addToStructure(path, linarPathArray);
		if (foundAndModified)
		{
			generic_string addedFilePath = file2Change[0].substr(0, sepPos + 1);
			addedFilePath += pathSuffix;
			bool isAdded = _pFileBrowser->addInTree(rootPath, addedFilePath, nullptr, linarPathArray);
			if (not isAdded)
				MessageBox(NULL, addedFilePath.c_str(), TEXT("file/folder is not added"), MB_OK);
		}
		else
			printStr(TEXT("addToStructure pb"));
	}
	else if (action == FILE_ACTION_REMOVED)
	{
		generic_string rootPath = file2Change[0].substr(0, sepPos);
		// search recursively and modify the tree structure
		bool foundAndModified = _rootFolder.removeFromStructure(linarPathArray);
		if (foundAndModified)
		{
			bool isRemoved = _pFileBrowser->deleteFromTree(rootPath, nullptr, linarPathArray);
			if (not isRemoved)
				MessageBox(NULL, file2Change[0].c_str(), TEXT("file/folder is not removed"), MB_OK);
		}
		else
			printStr(TEXT("removeFromStructure pb"));
	}
	else if (action == FILE_ACTION_RENAMED_NEW_NAME)
	{
		generic_string rootPath = file2Change[0].substr(0, sepPos);

		size_t sepPos2 = file2Change[1].find(separator);
		if (sepPos2 == generic_string::npos)
			return false;

		generic_string pathSuffix2 = file2Change[1].substr(sepPos2 + separator.length(), file2Change[1].length() - 1);
		vector<generic_string> linarPathArray2 = split(pathSuffix2, '\\');

		bool foundAndModified = _rootFolder.renameInStructure(linarPathArray, linarPathArray2);
		if (foundAndModified)
		{
			bool isRenamed = _pFileBrowser->renameInTree(rootPath, nullptr, linarPathArray, linarPathArray2);
			if (not isRenamed)
				MessageBox(NULL, file2Change[0].c_str(), TEXT("file/folder is not removed"), MB_OK);
		}
		else
			printStr(TEXT("removeFromStructure pb"));
	}

	return true;
}


DWORD WINAPI FolderUpdater::watching(void *params)
{
	FolderUpdater *thisFolderUpdater = (FolderUpdater *)params;
	const TCHAR *dir2Watch = (thisFolderUpdater->_rootFolder)._rootPath.c_str();

	const DWORD dwNotificationFlags = FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME;

	// Create the monitor and add directory to watch.
	CReadDirectoryChanges changes;
	changes.AddDirectory(dir2Watch, true, dwNotificationFlags);

	HANDLE changeHandles[] = { thisFolderUpdater->_EventHandle, changes.GetWaitHandle() };

	bool toBeContinued = true;

	while (toBeContinued)
	{
		DWORD waitStatus = ::WaitForMultipleObjects(_countof(changeHandles), changeHandles, FALSE, INFINITE);
		switch (waitStatus)
		{
			case WAIT_OBJECT_0 + 0:
			// Mutex was signaled. User removes this folder or file browser is closed
				toBeContinued = false;
				break;

			case WAIT_OBJECT_0 + 1:
			// We've received a notification in the queue.
			{
				DWORD dwAction;
				CStringW wstrFilename;
				if (changes.CheckOverflow())
					printStr(L"Queue overflowed.");
				else
				{
					changes.Pop(dwAction, wstrFilename);
					static generic_string oldName;

					std::vector<generic_string> file2Change;

					switch (dwAction)
					{
						case FILE_ACTION_ADDED:
							file2Change.push_back(wstrFilename.GetString());
							thisFolderUpdater->updateTree(dwAction, file2Change);
							//::PostMessage(thisFolderUpdater->_hFileBrowser, FB_ADDFILE, nullptr, (LPARAM)wstrFilename.GetString());
							oldName = TEXT("");
							break;

						case FILE_ACTION_REMOVED:
							file2Change.push_back(wstrFilename.GetString());
							thisFolderUpdater->updateTree(dwAction, file2Change);
							oldName = TEXT("");
							break;

						case FILE_ACTION_MODIFIED:
							oldName = TEXT("");
							break;

						case FILE_ACTION_RENAMED_OLD_NAME:
							oldName = wstrFilename.GetString();
							break;

						case FILE_ACTION_RENAMED_NEW_NAME:
							if (not oldName.empty())
							{
								file2Change.push_back(oldName);
								file2Change.push_back(wstrFilename.GetString());
								thisFolderUpdater->updateTree(dwAction, file2Change);
							}
							oldName = TEXT("");
							break;

						default:
							oldName = TEXT("");
							break;
					}
				}
			}
			break;

			case WAIT_IO_COMPLETION:
				// Nothing to do.
				break;
		}
	}

	// Just for sample purposes. The destructor will
	// call Terminate() automatically.
	changes.Terminate();
	//printStr(L"Quit watching thread");
	return EXIT_SUCCESS;
}

/*

DWORD WINAPI FolderUpdater::watching(void *params)
{
	FolderUpdater *thisFolderUpdater = (FolderUpdater *)params;
	const TCHAR *lpDir = (thisFolderUpdater->_rootFolder)._path.c_str();
	DWORD dwWaitStatus;
	HANDLE dwChangeHandles[3];

	// Watch the directory for file creation and deletion. 
	dwChangeHandles[0] = FindFirstChangeNotification(
		lpDir,                         // directory to watch 
		TRUE,                         // do watch subtree 
		FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

	if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
	{
		printStr(TEXT("\n ERROR: FindFirstChangeNotification function failed.\n"));
		//ExitProcess(GetLastError());
		return 1;
	}

	// Watch the subtree for directory creation and deletion. 
	dwChangeHandles[1] = FindFirstChangeNotification(
		lpDir,                       // directory to watch 
		TRUE,                          // watch the subtree 
		FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes 

	if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
	{
		printStr(TEXT("\n ERROR: FindFirstChangeNotification function failed.\n"));
		return 1;
	}

	dwChangeHandles[2] = thisFolderUpdater->_mutex;


	// Make a final validation check on our handles.
	if ((dwChangeHandles[0] == NULL) || (dwChangeHandles[1] == NULL))
	{
		printStr(TEXT("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n"));
		//ExitProcess(GetLastError());
		return 1;
	}

	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 

	while (thisFolderUpdater->_toBeContinued)
	{
		// Wait for notification.
		dwWaitStatus = WaitForMultipleObjects(3, dwChangeHandles, FALSE, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// A file was created, renamed, or deleted in the directory.
			// Refresh this directory and restart the notification.
			printStr(lpDir);
			//PostMessage()
			if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
			{
				printStr(TEXT("\n ERROR: FindNextChangeNotification function failed.\n"));
				return 1;
			}
			break;

		case WAIT_OBJECT_0 + 1:

			// A directory was created, renamed, or deleted.
			// Refresh the tree and restart the notification.
			printStr(TEXT("A directory was created, renamed, or deleted."));
			if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
			{
				printStr(TEXT("\n ERROR: FindNextChangeNotification function failed.\n"));
				return 1;
			}
			break;

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			printStr(TEXT("\nNo changes in the timeout period.\n"));
			break;

		default:
			printStr(TEXT("\n ERROR: Unhandled dwWaitStatus.\n"));
			return 1;
			break;
		}
	}
	printStr(TEXT("youpi!"));
	return 0;
}
*/