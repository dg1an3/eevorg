
// EevorgView.h : interface of the CEevorgView class
//


#pragma once


class CEevorgView : public CView
{
protected: // create from serialization only
	CEevorgView();
	DECLARE_DYNCREATE(CEevorgView)

// Attributes
public:
	CEevorgDoc* GetDocument() const;

	// returns the currently selected Eevorg
	const Eevorg *currentEevorg();

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC) { } // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CEevorgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// the renderer for performing the drawing
	EevorgRenderer _renderer;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in EevorgView.cpp
inline CEevorgDoc* CEevorgView::GetDocument() const
   { return reinterpret_cast<CEevorgDoc*>(m_pDocument); }
#endif

