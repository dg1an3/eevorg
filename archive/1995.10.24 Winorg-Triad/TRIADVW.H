// triadvw.h : header file
//
    
#include "triad.h"

/////////////////////////////////////////////////////////////////////////////
// TriadView view

class TriadView : public CView
{
	DECLARE_DYNCREATE(TriadView)
protected:
	TriadView();			// protected constructor used by dynamic creation

// Attributes
public:
	StimResp *sr;	// store the current stimulus until response
	int numRespThisTrial;	// stores number of responses for this trial
		
// Operations
public:

// Implementation
protected:
	virtual ~TriadView();
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view

	// Generated message map functions
protected:
	//{{AFX_MSG(TriadView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
