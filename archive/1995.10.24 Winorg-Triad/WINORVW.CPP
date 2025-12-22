// winorvw.cpp : implementation of the CWinorgView class
//

#include "stdafx.h"
#include "winorg.h"
#include "eevorg.h"
#include "winorvw.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinorgView

IMPLEMENT_DYNCREATE(CWinorgView, CView)

BEGIN_MESSAGE_MAP(CWinorgView, CView)
	//{{AFX_MSG_MAP(CWinorgView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinorgView construction/destruction

CWinorgView::CWinorgView()
{
	SetWindowPos(&wndTop, 0, 0, 100, 160, SWP_NOMOVE);
}

CWinorgView::~CWinorgView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWinorgView drawing

void CWinorgView::DrawScale(CDC* pDC, Eevorg *Eev, int atX, int atY, int hgt)
{   
	if (hgt < 2)
		return;

	CByteArray *curGen;
	int c, at;
	int actHgt = hgt*2/3;
	for (int n = 0; n < actHgt;)
	{ 
		curGen = Eev->getGen(n);
		for (int m = curGen->GetSize()-1; m > 0; m--)
		{                           
			c = curGen->GetAt(m)*32;
			at = (int)((float)m*(1.0-(float)n*(float)n*(float)n/
			     ((float)actHgt*(float)actHgt*(float)actHgt)));
			pDC->SetPixel(atX+at, atY+n, RGB(c*4,c,c*2));
			pDC->SetPixel(atX-at, atY+n, RGB(c*4,c,c*2));
		}
		
		pDC->SetPixel(atX, atY+n, RGB(c*4,c,c*2));
		
		if (++n >= Eev->numOfGen())
			n = Eev->nextGen();
	}
	actHgt = hgt*33/48;
	int childStartY = (int)pow((float)actHgt*(float)actHgt*(float)actHgt/4.0,
			1.0/3.0);
	DrawScale(pDC, Eev->getLeftChild(), 
	        atX-(int)((float)childStartY*(1.0-(float)childStartY*
			(float)childStartY*(float)childStartY/
			((float)actHgt*(float)actHgt*(float)actHgt))), 
			atY+(int)((float)childStartY*1.2),
			hgt/3);
	DrawScale(pDC, Eev->getRightChild(),
	        atX+(int)((float)childStartY*(1.0-(float)childStartY*
			(float)childStartY*(float)childStartY/
			((float)actHgt*(float)actHgt*(float)actHgt))), 
			atY+(int)((float)childStartY*1.2),
			hgt/3);
	DrawScale(pDC, Eev->getMidChild(),
	        atX, atY+hgt*33/48, hgt/3);
}

void CWinorgView::OnDraw(CDC* pDC)
{
	Eevorg* pDoc = GetDocument();
	char textScore[10];
	_itoa(pDoc->getScore(), textScore, 10);
    pDC->TextOut(5, 5, textScore);
    
    CRect wndRect;
    GetClientRect(&wndRect);
    
    DrawScale(pDC, pDoc, wndRect.Size().cx/2, 0, wndRect.Size().cy);
}

/////////////////////////////////////////////////////////////////////////////
// CWinorgView printing

BOOL CWinorgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWinorgView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWinorgView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}




/////////////////////////////////////////////////////////////////////////////
// CWinorgView diagnostics

#ifdef _DEBUG
void CWinorgView::AssertValid() const
{
	CView::AssertValid();
}

void CWinorgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Eevorg* CWinorgView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Eevorg)));
	return (Eevorg*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinorgView message handlers

int CWinorgView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CWnd *parent = GetParentFrame();
	CRect parentSize;
	parent->GetWindowRect(&parentSize);
	if ((parentSize.Size().cx > 100) || (parentSize.Size().cy > 100))
		parent->SetWindowPos(&wndTop, 0, 0, 100, 100, SWP_NOMOVE);
	
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

void CWinorgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	Eevorg* pDoc = GetDocument();
	pDoc->incScore();
	
	CView::OnLButtonDown(nFlags, point);
	pDoc->UpdateAllViews((CView *)NULL);
}
