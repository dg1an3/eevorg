// winorvw.h : interface of the CWinorgView class
//
/////////////////////////////////////////////////////////////////////////////

class CWinorgView : public CView
{
protected: // create from serialization only
	CWinorgView();
	DECLARE_DYNCREATE(CWinorgView)

// Attributes
public:
	Eevorg* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CWinorgView();
	void DrawScale(CDC* pDC, Eevorg *Eev, 
	    int atX, int atY, int hgt); 
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CWinorgView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in winorvw.cpp
inline Eevorg* CWinorgView::GetDocument()
   { return (Eevorg*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
