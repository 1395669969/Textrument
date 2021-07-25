// Scintilla source code edit control
/** @file LexNull.cxx
 ** Lexer for no language. Used for plain text and unrecognized files.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"
#include "windows.h"

using namespace Scintilla;

static void ColouriseNullDoc(Sci_PositionU startPos, Sci_Position length, int, WordList *[],
                            Accessor &styler) {
	// Null language means all style bytes are 0 so just mark the end - no need to fill in.
	if (length > 0) {
		styler.StartAt(startPos + length - 1);
		styler.StartSegment(startPos + length - 1);
		styler.ColourTo(startPos + length - 1, 0);
	}
}

static void FoldNULLDoc(Sci_PositionU startPos, Sci_Position length, int /* initStyle */, WordList *[], Accessor &styler) {

	//::MessageBoxA(NULL, ("111"), (""), MB_OK);
	int visibleCharsCurrent, visibleCharsNext;
	int levelCurrent, levelNext;
	Sci_PositionU i, lineEnd;
	Sci_PositionU lengthDoc   = startPos + length;
	Sci_Position  lineCurrent = styler.GetLine(startPos);

	i       = styler.LineStart(lineCurrent  );
	lineEnd = styler.LineStart(lineCurrent+1)-1;
	if(lineEnd>=lengthDoc) lineEnd = lengthDoc-1;
	while(styler[lineEnd]=='\n' || styler[lineEnd]=='\r') lineEnd--;
	for(visibleCharsCurrent=0, levelCurrent=SC_FOLDLEVELBASE; !visibleCharsCurrent && i<=lineEnd; i++){
		if(isspacechar(styler[i])) levelCurrent++;
		else                       visibleCharsCurrent=1;
	}

	for(; i<lengthDoc; lineCurrent++) {
		i       = styler.LineStart(lineCurrent+1);
		lineEnd = styler.LineStart(lineCurrent+2)-1;
		if(lineEnd>=lengthDoc) lineEnd = lengthDoc-1;
		while(styler[lineEnd]=='\n' || styler[lineEnd]=='\r') lineEnd--;
		for(visibleCharsNext=0, levelNext=SC_FOLDLEVELBASE; !visibleCharsNext && i<=lineEnd; i++){
			if(isspacechar(styler[i])) levelNext++;
			else                       visibleCharsNext=1;
		}
		int lev = levelCurrent;
		if(!visibleCharsCurrent) lev |= SC_FOLDLEVELWHITEFLAG;
		else if(levelNext > levelCurrent) lev |= SC_FOLDLEVELHEADERFLAG;
		styler.SetLevel(lineCurrent, lev);
		levelCurrent = levelNext;
		visibleCharsCurrent = visibleCharsNext;
	}
}

//LexerModule lmNull(SCLEX_NULL, ColouriseNullDoc, "null");

LexerModule lmNull(SCLEX_NULL, ColouriseNullDoc, "null", FoldNULLDoc);
