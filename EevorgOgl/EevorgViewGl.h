
// EevorgViewGl.h : interface of the CEevorgViewGl class
//


#pragma once


class CEevorgViewGl : public CView
{
protected: // create from serialization only
	CEevorgViewGl();
	DECLARE_DYNCREATE(CEevorgViewGl)

// Attributes
public:
	CEevorgDoc* GetDocument() const;

	// returns the currently selected Eevorg
	const Eevorg *currentEevorg();

	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;
	std::map<const Eevorg*,GLuint> m_mapTextures;

// Operations
public:
	void Init();
	BOOL bSetupPixelFormat(void);
	void DrawScene(bool bPickMode, LPRECT pickRect);
	void DrawEevorg(const Eevorg *eevorg, float x, float y, float rotateX, float rotateY, float rotateZ, float scale, bool bPickMode);
	void ProcessHits(int nHits, GLuint *pSelectBuf);

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
	virtual ~CEevorgViewGl();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in EevorgView.cpp
inline CEevorgDoc* CEevorgViewGl::GetDocument() const
   { return reinterpret_cast<CEevorgDoc*>(m_pDocument); }
#endif

