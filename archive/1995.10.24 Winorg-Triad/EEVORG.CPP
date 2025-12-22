// eevorg.cpp : implementation of the Eevorg class
//

#include "stdafx.h"
#include "winorg.h"

#include "eevorg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define random(max) (int)((long)rand()*((long)(max))/(long)RAND_MAX)

/////////////////////////////////////////////////////////////////////////////
// Eevorg

IMPLEMENT_SERIAL(Eevorg, CObject, 0)

/////////////////////////////////////////////////////////////////////////////
// Eevorg construction/destruction

Eevorg::Eevorg()
{	// default constructor
	rule.Add(0); rule.Add(2); rule.Add(0); rule.Add(1); rule.Add(1);
	rule.Add(3); rule.Add(2); rule.Add(1); rule.Add(1); rule.Add(0);
	maxRule = rule.GetSize()-1;
	maxVal = 3;
	score = 0;
	
	CByteArray* firstGen = new CByteArray;
	ASSERT(firstGen != NULL);
	firstGen->Add(1);
	gen.Add(firstGen);
	
	parent = (Eevorg *)NULL;
	ASSERT(maxRule >= 0);
}	// Eevorg::Eevorg

Eevorg::Eevorg(Eevorg *fromParent, int mutFlag)
{	// copy constructor
	ASSERT(fromParent->maxRule >= 0);
	parent = fromParent;
	
	for (int count = 0; count < fromParent->rule.GetSize(); count++)
		rule.Add(fromParent->rule.GetAt(count));
	maxRule = fromParent->maxRule;
	maxVal = fromParent->maxVal;
	if (mutFlag)
	{	// generate a single mutation
		int ruleNum = random(maxRule+1);
		ASSERT((ruleNum >= 0) && (ruleNum <= maxRule));
		int ruleVal;
		if (ruleNum > maxRule)
		{
			ruleVal = 0;
			maxRule = ruleNum;
		} else
		{
			ASSERT(ruleNum < rule.GetSize());
			ruleVal = rule.GetAt(ruleNum);
		}	// if 
			
		ruleVal++; // += ((int)((long)rand()*(long)3/(long)RAND_MAX)-1);
		ruleVal = (ruleVal > 0) ? ruleVal : 0;
		while (ruleVal > maxVal)
		{	// create three new rules
			rule.Add(0);
			rule.Add(0);
			rule.Add(0);
			maxVal++;
		}	// while         
		rule.SetAt(ruleNum, ruleVal);
		ASSERT(ruleNum >= 0 && ruleNum < rule.GetSize());
	}	// if
	
	CByteArray* firstGen = new CByteArray;
	ASSERT(firstGen != NULL);
	firstGen->Add(1);
	gen.Add(firstGen);
	
	score = 0;
	ASSERT(maxRule >= 0);
}	// Eevorg::Eevorg

Eevorg::~Eevorg()
{
	for (int count = 0; count < gen.GetSize(); count++)
		((CByteArray *)gen.GetAt(count))->RemoveAll();
	gen.RemoveAll();
	rule.RemoveAll();
}	// Eevorg::~Eevorg


int Eevorg::nextGen()
{	// compute next generation and return
	CByteArray *next = new CByteArray; 
	ASSERT(next != NULL);
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
	}	// if
	
	return nextNdx;
}	// Eevorg::nextGen

void Eevorg::drawAt(CDC* pDC, int atX, int atY, int hgt)
{   
	if (hgt < 2)
		return;

	CByteArray *curGen;
	int c, at;
	for (int n = 0; n < hgt;)
	{ 
		ASSERT(n < gen.GetSize());
		curGen = getGen(n);
		for (int m = curGen->GetSize()-1; m > 0; m--)
		{                           
			c = curGen->GetAt(m)*64/maxVal;
			at = (int)((float)m*(1.0-(float)n*(float)n*(float)n/
			     ((float)hgt*(float)hgt*(float)hgt)));
			pDC->SetPixel(atX+at, atY+n, RGB(c*4,c,c*2));
			pDC->SetPixel(atX-at, atY+n, RGB(c*4,c,c*2));
		}	// for
		
		pDC->SetPixel(atX, atY+n, RGB(c*4,c,c*2));
		
		if (++n >= numOfGen())
			n = nextGen();
	}	// for
}	// Eevorg::drawAt


/////////////////////////////////////////////////////////////////////////////
// Eevorg serialization

void Eevorg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// write member variables to archive
		ar << (WORD)maxVal;
		ar << (WORD)maxRule;
		ar << (WORD)score;
		rule.Serialize(ar);
	} else
	{	// read member variables from archive
		ar >> (WORD&)maxVal;
		ar >> (WORD&)maxRule;
		ar >> (WORD&)score;
		rule.Serialize(ar);
	}	// if
}	// Eevorg::Serialize

/////////////////////////////////////////////////////////////////////////////
// Eevorg diagnostics

#ifdef _DEBUG
void Eevorg::AssertValid() const
{   
	ASSERT((maxRule > 0) && (maxRule < rule.GetSize()));
	CObject::AssertValid();
}   // Eevorg::AssertValid

void Eevorg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Eevorg commands
