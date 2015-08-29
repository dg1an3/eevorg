
// EevorgView.cpp : implementation of the CEevorgView class
//

#include "stdafx.h"
#include "EevorgOglApp.h"

#include "Eevorg.h"
#include "EevorgDoc.h"

#include "EevorgRenderer.h"
#include "EevorgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEevorgView

IMPLEMENT_DYNCREATE(CEevorgView, CView)

BEGIN_MESSAGE_MAP(CEevorgView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CEevorgView construction/destruction

CEevorgView::CEevorgView()
{
}

CEevorgView::~CEevorgView()
{
}

BOOL CEevorgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEevorgView drawing

void CEevorgView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectClient;
	GetClientRect(&rectClient);

	// render the eevorg family

	// resize pixel buffer if needed
	_renderer.ResetBuffers(rectClient.Height(), rectClient.Width());

	valarray<float> origin(2);
	origin[0] = (float) rectClient.Size().cx/2;
	origin[1] = 10.0f;
	_renderer.DrawEevorgAndChildren(*currentEevorg(), origin, (float) rectClient.Size().cy - 40, 8.0);

	// create and populate backbuffer bitmap
	CBitmap backBuffer;
	backBuffer.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());

	// need to cast here because only non-const valarray allows dereference of pixels
	valarray<COLORREF>& bufferPixels = const_cast< valarray<COLORREF>& >(_renderer.pixelBuffer().pixels());
	backBuffer.SetBitmapBits(rectClient.Width() * rectClient.Height() * sizeof(COLORREF),
		(void *) &bufferPixels[0]);

	// now select the DIB
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	HBITMAP hOldBitmap = (HBITMAP) dcMem.SelectObject(backBuffer);

	CString strInfo;
	strInfo.Format(_T("States: %i"), currentEevorg()->maxState());
	dcMem.TextOutW(10, 10, strInfo);
	strInfo.Format(_T("Color table: %4.1f, %4.1f, %4.1f"), 
		currentEevorg()->hueSlope(),
		currentEevorg()->hueOffset(),
		currentEevorg()->saturationFrequency());
	dcMem.TextOutW(10, 30, strInfo);

	// and blt
	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &dcMem, 0, 0, SRCCOPY);

	// select the old bitmap and destroy the backbuffer
	dcMem.SelectObject(hOldBitmap);
	backBuffer.DeleteObject();
}


// CEevorgView printing

BOOL CEevorgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEevorgView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEevorgView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CEevorgView diagnostics

#ifdef _DEBUG
void CEevorgView::AssertValid() const
{
	CView::AssertValid();
}

void CEevorgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEevorgDoc* CEevorgView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEevorgDoc)));
	return (CEevorgDoc*)m_pDocument;
}
#endif //_DEBUG

const Eevorg *
	CEevorgView::currentEevorg()
{
	CEevorgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		return pDoc->_pCurrentEevorg;

	return NULL;
}

// CEevorgView message handlers

void CEevorgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect wndRect;
	GetClientRect(&wndRect);

	CRect rectPoint(point.x-5, point.y-5, point.x+5, point.y+5);
	const Eevorg *pNewEevorg = _renderer.hitTest(point.y, point.x);
	if (pNewEevorg != NULL)
	{
		CEevorgDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->_pCurrentEevorg = const_cast<Eevorg*>(pNewEevorg);

		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE);
	}

	CView::OnLButtonDown(nFlags, point);
}



BOOL CEevorgView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE; // CView::OnEraseBkgnd(pDC);
}
