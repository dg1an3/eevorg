
// EevorgDoc.cpp : implementation of the CEevorgDoc class
//

#include "stdafx.h"
#include "EevorgOglApp.h"

#include "Eevorg.h"
#include "EevorgDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEevorgDoc

IMPLEMENT_DYNCREATE(CEevorgDoc, CDocument)

BEGIN_MESSAGE_MAP(CEevorgDoc, CDocument)
END_MESSAGE_MAP()


// CEevorgDoc construction/destruction

CEevorgDoc::CEevorgDoc()
	: _pRootEevorg(NULL)
	, _pCurrentEevorg(NULL)
{
	// Seed the random-number generator with the current time so that
	// the numbers will be different every time we run.
	srand( (unsigned)time( NULL ) );

	_pRootEevorg = new Eevorg();
	_pCurrentEevorg = _pRootEevorg;
}

CEevorgDoc::~CEevorgDoc()
{
	delete _pRootEevorg;
}

BOOL CEevorgDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


Eevorg * 
	CEevorgDoc::FindEevorgByPickName(GLuint pickName)
{
	return _pRootEevorg->findChildByPickName(pickName);
}

// CEevorgDoc serialization

void CEevorgDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CArray<int, int> arrGene;
		arrGene.SetSize(_pCurrentEevorg->gene().columns());
		for (size_t n = 0; n < (size_t) arrGene.GetSize(); n++)
			arrGene[n] = _pCurrentEevorg->gene()(0, n);
		arrGene.Serialize(ar);
	}
	else
	{
		if (_pRootEevorg != NULL)
			delete _pRootEevorg;

		CArray<int, int> arrGene;
		arrGene.Serialize(ar);
		Eevorg::GeneType gene(1, arrGene.GetSize());
		for (size_t n = 0; n < (size_t) arrGene.GetSize(); n++)
			gene(0, n) = arrGene[n];

		_pRootEevorg = new Eevorg(1, &gene);
		_pCurrentEevorg = _pRootEevorg;
	}
}


// CEevorgDoc diagnostics

#ifdef _DEBUG
void CEevorgDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEevorgDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEevorgDoc commands
