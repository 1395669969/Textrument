//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include <stdarg.h>
#include "FileDialog.h"


FileDialog *FileDialog::staticThis = NULL;
//int FileDialog::_dialogFileBoxId = (NppParameters::getInstance())->getWinVersion() < WV_W2K?edt1:cmb13;

FileDialog::FileDialog(HWND hwnd, HINSTANCE hInst) 
	: _nbCharFileExt(0), _nbExt(0)
{
	staticThis = this;
    for (int i = 0 ; i < nbExtMax ; i++)
        _extArray[i][0] = '\0';

    memset(_fileExt, 0x00, sizeof(_fileExt));
	_fileName[0] = '\0';
 
	_winVersion = (NppParameters::getInstance())->getWinVersion();

	_ofn.lStructSize = sizeof(_ofn);
	if (_winVersion < WV_W2K)
		_ofn.lStructSize = sizeof(OPENFILENAME);
	_ofn.hwndOwner = hwnd; 
	_ofn.hInstance = hInst;
	_ofn.lpstrFilter = _fileExt;
	_ofn.lpstrCustomFilter = (LPTSTR) NULL;
	_ofn.nMaxCustFilter = 0L;
	_ofn.nFilterIndex = 1L;
	_ofn.lpstrFile = _fileName;
	_ofn.nMaxFile = sizeof(_fileName)/sizeof(TCHAR);
	_ofn.lpstrFileTitle = NULL;
	_ofn.nMaxFileTitle = 0;
	_ofn.lpstrInitialDir = NULL;
	_ofn.lpstrTitle = NULL;
	_ofn.nFileOffset  = 0;
	_ofn.nFileExtension = 0;
	_ofn.lpfnHook = NULL;
	_ofn.lpstrDefExt = NULL;  // No default extension
	_ofn.lCustData = 0;
	_ofn.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_LONGNAMES | DS_CENTER | OFN_HIDEREADONLY;
	_ofn.pvReserved = NULL;
	_ofn.dwReserved = 0;
	_ofn.FlagsEx = 0;
}

// This function set and concatenate the filter into the list box of FileDialog.
// The 1st parameter is the description of the file type, the 2nd .. Nth parameter(s) is (are)
// the file extension which should be ".WHATEVER", otherwise it (they) will be considered as
// a file name to filter. Since the nb of arguments is variable, you have to add NULL at the end.
// example : 
// FileDialog.setExtFilter(TEXT("c/c++ src file"), TEXT(".c"), TEXT(".cpp"), TEXT(".cxx"), TEXT(".h"), NULL);
// FileDialog.setExtFilter(TEXT("Makeile"), TEXT("makefile"), TEXT("GNUmakefile"), NULL);
void FileDialog::setExtFilter(const TCHAR *extText, const TCHAR *ext, ...)
{
    // fill out the ext array for save as file dialog
    if (_nbExt < nbExtMax)
        lstrcpy(_extArray[_nbExt++], ext);
    // 
    std::generic_string extFilter = extText;
   
    va_list pArg;
    va_start(pArg, ext);

    std::generic_string exts;

	if (ext[0] == '.')
		exts += TEXT("*");
    exts += ext;
    exts += TEXT(";");

    const TCHAR *ext2Concat;

    while ((ext2Concat = va_arg(pArg, const TCHAR *)))
	{
        if (ext2Concat[0] == '.')
            exts += TEXT("*");
        exts += ext2Concat;
        exts += TEXT(";");
	}
	va_end(pArg);

	// remove the last ';'
    exts = exts.substr(0, exts.length()-1);

    extFilter += TEXT(" (L");
    extFilter += exts + TEXT(")");
    
    TCHAR *pFileExt = _fileExt + _nbCharFileExt;
    memcpy(pFileExt, extFilter.c_str(), extFilter.length() + 1);
    _nbCharFileExt += extFilter.length() + 1;
    
    pFileExt = _fileExt + _nbCharFileExt;
    memcpy(pFileExt, exts.c_str(), exts.length() + 1);
    _nbCharFileExt += exts.length() + 1;
}

int FileDialog::setExtsFilter(const TCHAR *extText, const TCHAR *exts)
{
    // fill out the ext array for save as file dialog
    if (_nbExt < nbExtMax)
        lstrcpy(_extArray[_nbExt++], exts);
    // 
    std::generic_string extFilter = extText;

    extFilter += TEXT(" (L");
    extFilter += exts;
	extFilter += TEXT(")");
    
    TCHAR *pFileExt = _fileExt + _nbCharFileExt;
    memcpy(pFileExt, extFilter.c_str(), extFilter.length() + 1);
    _nbCharFileExt += extFilter.length() + 1;
    
    pFileExt = _fileExt + _nbCharFileExt;
    memcpy(pFileExt, exts, lstrlen(exts) + 1);
    _nbCharFileExt += lstrlen(exts) + 1;

	return _nbExt;
}

TCHAR * FileDialog::doOpenSingleFileDlg() 
{
	TCHAR dir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, dir);
	NppParameters * params = NppParameters::getInstance();
	_ofn.lpstrInitialDir = params->getWorkingDir();

	_ofn.Flags |= OFN_FILEMUSTEXIST;

	TCHAR *fn = NULL;
	try {
		fn = ::GetOpenFileName((OPENFILENAME*)&_ofn)?_fileName:NULL;
		
		if (params->getNppGUI()._openSaveDir == dir_last)
		{
			::GetCurrentDirectory(MAX_PATH, dir);
			params->setWorkingDir(dir);
		}
	}
	catch(...) {
		::MessageBox(NULL, TEXT("GetSaveFileName crashes!!!"), TEXT(""), MB_OK);
	}

	::SetCurrentDirectory(dir); 

	return (fn);
}

stringVector * FileDialog::doOpenMultiFilesDlg()
{
	TCHAR dir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, dir);
	//_ofn.lpstrInitialDir = dir;

	NppParameters * params = NppParameters::getInstance();
	_ofn.lpstrInitialDir = params->getWorkingDir();

	_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

	BOOL res = ::GetOpenFileName((OPENFILENAME*)&_ofn);
	if (params->getNppGUI()._openSaveDir == dir_last)
	{
		::GetCurrentDirectory(MAX_PATH, dir);
		params->setWorkingDir(dir);
	}
	::SetCurrentDirectory(dir); 

	if (res)
	{
		TCHAR fn[MAX_PATH];
		TCHAR *pFn = _fileName + lstrlen(_fileName) + 1;
		if (!(*pFn))
			_fileNames.push_back(std::generic_string(_fileName));
		else
		{
			lstrcpy(fn, _fileName);
			if (fn[lstrlen(fn)-1] != '\\')
				lstrcat(fn, TEXT("\\"));
		}
		int term = int(lstrlen(fn));

		while (*pFn)
		{
			fn[term] = '\0';
			lstrcat(fn, pFn);
			_fileNames.push_back(std::generic_string(fn));
			pFn += lstrlen(pFn) + 1;
		}

		return &_fileNames;
	}
	else
		return NULL;
}

TCHAR * FileDialog::doSaveDlg() 
{
	TCHAR dir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, dir); 
	//_ofn.lpstrInitialDir = dir;

	NppParameters * params = NppParameters::getInstance();
	_ofn.lpstrInitialDir = params->getWorkingDir();

	_ofn.Flags |= OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_ENABLESIZING;

	_ofn.Flags |= OFN_ENABLEHOOK;
	_ofn.lpfnHook = OFNHookProc;

	TCHAR *fn = NULL;
	try {
		fn = ::GetSaveFileName((OPENFILENAME*)&_ofn)?_fileName:NULL;
		if (params->getNppGUI()._openSaveDir == dir_last)
		{
			::GetCurrentDirectory(MAX_PATH, dir);
			params->setWorkingDir(dir);
		}
	}
	catch(...) {
		::MessageBox(NULL, TEXT("GetSaveFileName crashes!!!"), TEXT(""), MB_OK);
	}

	::SetCurrentDirectory(dir); 

	return (fn);
}

static HWND hFileDlg = NULL;
static WNDPROC oldProc = NULL;
static generic_string currentExt = TEXT("");

static BOOL CALLBACK fileDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
    {
		case WM_COMMAND :
		{
			switch (wParam)
			{	
				case IDOK :
				{
					HWND fnControl = ::GetDlgItem(hwnd, FileDialog::_dialogFileBoxId);
					TCHAR fn[MAX_PATH];
					::GetWindowText(fnControl, fn, MAX_PATH);
					if (*fn == '\0')
						return oldProc(hwnd, message, wParam, lParam);

					if (currentExt != TEXT(""))
					{
						generic_string fnExt = changeExt(fn, currentExt);
						::SetWindowText(fnControl, fnExt.c_str());
					}
					return oldProc(hwnd, message, wParam, lParam);
				}

				default :
					break;
			}
		}
	}
	return oldProc(hwnd, message, wParam, lParam);
};

static TCHAR * get1stExt(TCHAR *ext) { // precondition : ext should be under the format : Batch (*.bat;*.cmd;*.nt)
	TCHAR *begin = ext;
	for ( ; *begin != '.' ; begin++);
	TCHAR *end = ++begin;
	for ( ; *end != ';' && *end != ')' ; end++);
	*end = '\0';
	if (*begin == '*')
		*begin = '\0';
	return begin;
};

static generic_string addExt(HWND textCtrl, HWND typeCtrl) {
	TCHAR fn[MAX_PATH];
	::GetWindowText(textCtrl, fn, MAX_PATH);
	
	int i = ::SendMessage(typeCtrl, CB_GETCURSEL, 0, 0);
	TCHAR ext[256];
	::SendMessage(typeCtrl, CB_GETLBTEXT, i, (LPARAM)ext);
	TCHAR *pExt = get1stExt(ext);
	if (*fn != '\0')
	{
		generic_string fnExt = changeExt(fn, pExt);
		::SetWindowText(textCtrl, fnExt.c_str());
	}
	return pExt;
};


UINT_PTR CALLBACK FileDialog::OFNHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG :
        {
			NppParameters *pNppParam = NppParameters::getInstance();
			int index = pNppParam->getFileSaveDlgFilterIndex();

			::SetWindowLongPtr(hWnd, GWL_USERDATA, (long)staticThis);
			hFileDlg = ::GetParent(hWnd);
			goToCenter(hFileDlg);

			if (index != -1)
			{
				HWND typeControl = ::GetDlgItem(hFileDlg, cmb1);
				::SendMessage(typeControl, CB_SETCURSEL, index, 0);
			}

			// Don't touch the following 3 lines, they are cursed !!!
			oldProc = (WNDPROC)::GetWindowLongPtr(hFileDlg, GWL_WNDPROC);
			if ((long)oldProc > 0)
				::SetWindowLongPtr(hFileDlg, GWL_WNDPROC, (LONG)fileDlgProc);

			return FALSE;
		}

		default :
		{
			FileDialog *pFileDialog = reinterpret_cast<FileDialog *>(::GetWindowLongPtr(hWnd, GWL_USERDATA));
			if (!pFileDialog)
			{
				return FALSE;
			}
			return pFileDialog->run(hWnd, uMsg, wParam, lParam);
		}
    }
    return FALSE;
}

BOOL APIENTRY FileDialog::run(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_NOTIFY :
		{
			LPNMHDR pNmhdr = (LPNMHDR)lParam;
			switch(pNmhdr->code)
			{
				case CDN_TYPECHANGE :
				{
					HWND fnControl = ::GetDlgItem(::GetParent(hWnd), _dialogFileBoxId);
					HWND typeControl = ::GetDlgItem(::GetParent(hWnd), cmb1);
					currentExt = addExt(fnControl, typeControl);
					return TRUE;
					//break;
				}

				case CDN_FILEOK :
				{
					HWND typeControl = ::GetDlgItem(::GetParent(hWnd), cmb1);
					int index = ::SendMessage(typeControl, CB_GETCURSEL, 0, 0);
					NppParameters *pNppParam = NppParameters::getInstance();
					pNppParam->setFileSaveDlgFilterIndex(index);
					return TRUE;
					//break;
				}

				default :
					return FALSE;
			}
			
		}
		default :
			return FALSE;
    }
}
