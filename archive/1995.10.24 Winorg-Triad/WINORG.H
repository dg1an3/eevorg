// winorg.h : main header file for the WINORG application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinorgApp:
// See winorg.cpp for the implementation of this class
//

class CWinorgApp : public CWinApp
{
public:
	CWinorgApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CWinorgApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
