// eevorg.cpp : implementation of the Eevorg class
//

#include "stdafx.h"
#include "winorg.h"

#include "eevorg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eevorg

IMPLEMENT_DYNCREATE(Eevorg, CDocument)

BEGIN_MESSAGE_MAP(Eevorg, CDocument)
	//{{AFX_MSG_MAP(Eevorg)
	ON_COMMAND(ID_FILE_MUTATE, OnFileMutate)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Eevorg construction/destruction

Eevorg::Eevorg()
{
	rule.Add(0); rule.Add(2); rule.Add(0); rule.Add(1); rule.Add(1);
	rule.Add(3); rule.Add(2); rule.Add(1); rule.Add(1); rule.Add(0);
	maxRule = rule.GetSize()-1;
	maxVal = 3;
	score = 0;
	CByteArray* firstGen = new CByteArray;
	firstGen->Add(1);
	
	gen.Add(firstGen);
	leftChild = (Eevorg *)NULL;
	midChild = (Eevorg *)NULL;
    rightChild = (Eevorg *)NULL;
}

Eevorg::~Eevorg()
{
	for (int n = 0; n < gen.GetSize(); n++)
		((CByteArray *)gen.GetAt(n))->RemoveAll();
	gen.RemoveAll();
	rule.RemoveAll();
}
 
int Eevorg::nextGen()
{	// compute next generation and return
	CByteArray *next = new CByteArray;
	int nextNdx = gen.Add(next);
	CByteArray *prev = (CByteArray *)gen.GetAt(nextNdx-1);
	
	if (prev->GetSize() == 1)
	{
		next->Add(rule.GetAt(prev->GetAt(0)));
		next->Add(rule.GetAt(prev->GetAt(0)));
	} else
	{
		next->Add(rule.GetAt(prev->GetAt(0)+2*prev->GetAt(1)));
		for (int n = 1; n < prev->GetSize()-1; n++)
			next->Add(rule.GetAt(prev->GetAt(n-1)+prev->GetAt(n)+
				prev->GetAt(n+1)));
		next->Add(rule.GetAt(prev->GetAt(n-1)+prev->GetAt(n)));
		next->Add(rule.GetAt(prev->GetAt(n)));
	}
	
	return nextNdx;
}
            
void Eevorg::Mutate()
{
	int ruleNum = (int)((long)rand()*((long)(maxRule+1))
					/(long)RAND_MAX);
	 
	int ruleVal;                                 
	if (ruleNum > maxRule)
	{
		ruleVal = 0;
		maxRule = ruleNum;
	} else
	{
		ASSERT(ruleNum < rule.GetSize());
		ruleVal = rule.GetAt(ruleNum);
	}
		
	ruleVal++; // += ((int)((long)rand()*(long)3/(long)RAND_MAX)-1);
	ruleVal = (ruleVal > 0) ? ruleVal : 0;
	while (ruleVal > maxVal)
	{	// create three new rules
		rule.Add(0);
		rule.Add(0);
		rule.Add(0);
		maxVal++;
	}
	rule.SetAt(ruleNum, ruleVal);
	                                                          
	for (int n = 0; n < gen.GetSize(); n++)
		((CByteArray *)gen.GetAt(n))->RemoveAll();
	gen.RemoveAll();	// causes recalculation of generations
	
	CByteArray* firstGen = new CByteArray;
	firstGen->Add(1);
	gen.Add(firstGen);

	UpdateAllViews((CView *)NULL);
}

Eevorg *Eevorg::getLeftChild()
{
	if (leftChild == NULL)
	{
		leftChild = new Eevorg();
		leftChild->Mutate();
	}
	return leftChild;
}

Eevorg *Eevorg::getMidChild()
{
	if (midChild == NULL)
	{
		midChild = new Eevorg();
		midChild->Mutate();
	}
	return midChild;
}

Eevorg *Eevorg::getRightChild()
{
	if (rightChild == NULL)
	{
		rightChild = new Eevorg();
		rightChild->Mutate();
	}
	return rightChild;
}

BOOL Eevorg::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Eevorg serialization

void Eevorg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (WORD)maxVal;
		ar << (WORD)maxRule;
		ar << (WORD)score;
		rule.Serialize(ar);
	}
	else
	{
		ar >> (WORD&)maxVal;
		ar >> (WORD&)maxRule;
		ar >> (WORD&)score;
		rule.Serialize(ar);
	}
}

void Eevorg::LoadOldFormat(const char * fileName)
{	// loads from an old file
	CFile dataFile(fileName, CFile::modeRead || CFile::typeBinary);
	char data[64];
	    
	maxVal = 0;
	maxRule = dataFile.Read(data, 64) - 1;
	for (int n = 0; n <= maxRule; n++)
	{	rule.Add(data[n]);
		maxVal = (data[n] > maxVal) ? data[n] : maxVal;
	}

	for (n = 0; n < gen.GetSize(); n++)
		((CByteArray *)gen.GetAt(n))->RemoveAll();
	gen.RemoveAll();	// causes recalculation of generations
	
	CByteArray* firstGen = new CByteArray;
	firstGen->Add(1);
	gen.Add(firstGen);

	UpdateAllViews((CView *)NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Eevorg diagnostics

#ifdef _DEBUG
void Eevorg::AssertValid() const
{
	CDocument::AssertValid();
}

void Eevorg::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Eevorg commands

void Eevorg::OnFileMutate()
{
	Mutate();
}

void Eevorg::OnFileClose()
{
	for (int n = 0; n < gen.GetSize(); n++)
		((CByteArray *)gen.GetAt(n))->RemoveAll();
	gen.RemoveAll();	// causes recalculation of generations
		
	CByteArray* firstGen = new CByteArray;
	firstGen->Add(1);
	gen.Add(firstGen);
	
	UpdateAllViews((CView *)NULL);
}
