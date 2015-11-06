// preferred-runway.h : main header file for the preferred-runway DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "PreferredRunwayPlugIn.h"

// CpreferredrunwayApp
// See preferred-runway.cpp for the implementation of this class
//

class CpreferredrunwayApp : public CWinApp
{
public:
	CpreferredrunwayApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
