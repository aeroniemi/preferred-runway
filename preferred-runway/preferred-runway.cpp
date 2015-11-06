// preferred-runway.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "preferred-runway.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CpreferredrunwayApp

BEGIN_MESSAGE_MAP(CpreferredrunwayApp, CWinApp)
END_MESSAGE_MAP()


// CpreferredrunwayApp construction

CpreferredrunwayApp::CpreferredrunwayApp(){

}

CpreferredrunwayApp theApp;

BOOL CpreferredrunwayApp::InitInstance(){
	CWinApp::InitInstance();

	return TRUE;
}

PreferredRunwayPlugIn *pMyPlugIn = NULL;

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance) {

	*ppPlugInInstance = pMyPlugIn = new PreferredRunwayPlugIn();
}

void __declspec (dllexport) EuroScopePlugInExit(void) {

	delete pMyPlugIn;
}