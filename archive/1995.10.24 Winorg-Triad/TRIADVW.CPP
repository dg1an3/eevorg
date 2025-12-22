// triadvw.cpp : implementation file
//

#include "stdafx.h"
#include "winorg.h"
#include "triadvw.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TriadView

IMPLEMENT_DYNCREATE(TriadView, CView)

TriadView::TriadView()
{
	sr = NULL;
	numRespThisTrial = 0;
}	// TriadView::TriadView

TriadView::~TriadView()
{
	delete sr;
}	// TriadView::~TriadView


BEGIN_MESSAGE_MAP(TriadView, CView)
	//{{AFX_MSG_MAP(TriadView)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TriadView drawing

void TriadView::OnDraw(CDC* pDC)
{
	Triad* pDoc = (Triad *)GetDocument();
	pDC->SetBkColor(RGB(0, 0, 0));
	
	if (sr == NULL)
	{
		sr = pDoc->genStimResp();
		sr->shownWhen = CTime::GetCurrentTime();
		// update message in the status bar
		CStatusBar *mainStatusBar = 
			&((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_wndStatusBar;
		char message[80];
		sprintf(message, "Eevorg: %i | Responses %i", pDoc->myPool.GetSize(),
			pDoc->myStimResp.GetSize());
		mainStatusBar->SetPaneText(0, message);
	}	// if
	ASSERT(sr->theEevorg[0] >= 0 && 
		sr->theEevorg[0] < pDoc->myPool.GetSize());
	ASSERT(sr->theEevorg[1] >= 0 && 
		sr->theEevorg[1] < pDoc->myPool.GetSize());
	ASSERT(sr->theEevorg[2] >= 0 && 
		sr->theEevorg[2] < pDoc->myPool.GetSize());
	ASSERT(pDoc->myPool.GetAt(sr->theEevorg[0]) != (void *)NULL);
	ASSERT(pDoc->myPool.GetAt(sr->theEevorg[1]) != (void *)NULL);
	ASSERT(pDoc->myPool.GetAt(sr->theEevorg[2]) != (void *)NULL);
	((Eevorg *)pDoc->myPool.GetAt(sr->theEevorg[0]))->
		drawAt(pDC, 160, 50, 125);
	((Eevorg *)pDoc->myPool.GetAt(sr->theEevorg[1]))->
		drawAt(pDC, 320, 200, 125);
	((Eevorg *)pDoc->myPool.GetAt(sr->theEevorg[2]))->
	 	drawAt(pDC, 480, 50, 125);
}	// TriadView::OnDraw

/////////////////////////////////////////////////////////////////////////////
// TriadView message handlers


void TriadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{      
	Triad* pDoc = (Triad *)GetDocument();

	// process the response
	sr->shownUntil = CTime::GetCurrentTime() - sr->shownWhen;
	switch(nChar)
	{
		case 'A' :	// upper right
			sr->userResponse = 0;
			break;
		case ' ' :	// lower middle
			sr->userResponse = 1;
			break;
		case 186 : // ';' :	// upper left - 186?
			sr->userResponse = 2;
			break;
		default :	// otherwise, do nothing
			if (sr->shownUntil.GetTotalSeconds() < 25)
				return;	
	}	// switch
	if (sr->shownUntil.GetTotalSeconds() < 25)
	{
		pDoc->myStimResp.Add(sr);
		if (++numRespThisTrial >= 20)
		{	// finished with trial
			// CString fileName = pDoc->GetPathName();
			// pDoc->OnSaveDocument(fileName);
			AfxMessageBox("Trial complete...");
		}	// if
	}	// if
	else
		delete sr;
	sr = NULL;
	Invalidate();
	// CView::OnKeyDown(nChar, nRepCnt, nFlags);
}	// TriadView::OnKeyDown
