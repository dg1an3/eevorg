
// EevorgWnd.h : main header file for the EevorgWnd application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEevorgWndApp:
// See EevorgWnd.cpp for the implementation of this class
//

class CEevorgWndApp : public CWinApp
{
public:
	CEevorgWndApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEevorgWndApp theApp;
