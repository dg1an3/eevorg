
// EevorgDoc.h : interface of the CEevorgDoc class
//


#pragma once


class CEevorgDoc : public CDocument
{
protected: // create from serialization only
	CEevorgDoc();
	DECLARE_DYNCREATE(CEevorgDoc)

// Attributes
public:
	Eevorg * _pRootEevorg;
	Eevorg * _pCurrentEevorg;

// Operations
public:
	Eevorg * FindEevorgByPickName(GLuint pickName);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CEevorgDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


