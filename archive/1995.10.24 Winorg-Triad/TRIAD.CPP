// triad.cpp : implementation file
//

#include "stdafx.h"
#include "winorg.h"
#include "triad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define random(max) (int)((long)rand()*((long)(max))/(long)RAND_MAX)

/////////////////////////////////////////////////////////////////////////////
// Triad

IMPLEMENT_SERIAL(Triad, CDocument, 0 /* schema number*/ )

Triad::Triad()
{
	Eevorg *oldEevorg = new Eevorg();
	ASSERT(oldEevorg != NULL);
	ASSERT(oldEevorg->maxRule > 0);
	for (int count = 0; count < 5; count++)
	{	// generate an initial pool of Eevorg
		myPool.Add(oldEevorg);
		Eevorg *newEevorg = new Eevorg(oldEevorg, WITH_MUTATION);
		ASSERT(newEevorg != NULL);
		ASSERT(newEevorg->maxRule > 0);
		oldEevorg = newEevorg;
	}	// for
	myPool.Add(oldEevorg);
	for (count = 0; count < myPool.GetSize(); count++)
		ASSERT(((Eevorg *)myPool.GetAt(count))->maxRule >= 0);
}	// Triad::Triad

BOOL Triad::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	AfxMessageBox("Ready to begin trial...");
	return TRUE;
}   // Triad::OnNewDocument

Triad::~Triad()
{
	myPool.RemoveAll();
	myStimResp.RemoveAll();
}	// Triad::~Triad

// Operations

int Triad::pClustNAN = 0;

StimResp *Triad::genStimResp()
{	// display 3 Eevorg and get one response 
	for (int count = 0; count < myPool.GetSize(); count++)
		ASSERT(((Eevorg *)myPool.GetAt(count))->maxRule >= 0);
	int pick1 = random(myPool.GetSize()-1);
	int pick2;
	if (pClustNAN < 10)
	{	// generate another Eevorg for the pool 
		ASSERT((pick1 >= 0) && (pick1 < myPool.GetSize()));
		Eevorg *oldEevorg = (Eevorg *)myPool.GetAt(pick1);
		ASSERT(oldEevorg != NULL);
		ASSERT(oldEevorg->maxRule > 0);
		Eevorg *newEevorg = new Eevorg(oldEevorg, WITH_MUTATION);
		ASSERT(newEevorg->maxRule > 0);
		ASSERT(newEevorg != NULL);
		pick2 = myPool.Add(newEevorg);
	} else
	do {  
		pick2 = random(myPool.GetSize()-1);
	} while ( (pick1 == pick2) || 
		(random(100) < (int)(100.0*pCluster(pick1, pick2))) );
	int pick3;
	do {
		pick3 = random(myPool.GetSize()-1);
	} while ( (pick1 == pick3) ||
		(random(100) < (int)(100.0*pCluster(pick1, pick3))) ||
		(pick2 == pick3) || 
		(random(100) < (int)(100.0*pCluster(pick2, pick3))) ); 
	ASSERT(pick1 >= 0 && pick1 < myPool.GetSize());
	ASSERT(pick2 >= 0 && pick2 < myPool.GetSize());
	ASSERT(pick3 >= 0 && pick3 < myPool.GetSize());
	StimResp *sr = new StimResp(pick1, pick2, pick3);
	ASSERT(sr != NULL);
	for (count = 0; count < myPool.GetSize(); count++)
		ASSERT(((Eevorg *)myPool.GetAt(count))->maxRule >= 0);
	return sr;
}	// Triad::Triad

float Triad::pCluster(int eev1, int eev2)
{	// calculate likelihood of clustering between two Eevorg	
	// = #(trials where eev1 and eev2 not selected)/#(trials with both)
	int trialsIn = 0;
	int trialsNeitherSelected = 0;
	for (int count = 0; count < myStimResp.GetSize(); count++)
	{
		StimResp *sr = (StimResp *)myStimResp.GetAt(count);
		int eev1In = 0;
		int eev2In = 0;
		for (int count2 = 0; count2 < 3; count2++)
			if (sr->theEevorg[count2] == eev1)
				eev1In = 1;
			else if (sr->theEevorg[count2] == eev2)
				eev2In = 1;
		if (eev1In && eev2In)
		{
			trialsIn++;
			if ((sr->userResponse != eev1) && (sr->userResponse != eev2))
				trialsNeitherSelected++;
		}	// if
	}	// for
	
	if (trialsIn == 0)
	{
		pClustNAN++;
		return 0;
	} else
	{	
		if (pClustNAN > 0)
			pClustNAN--;
		return (float)trialsNeitherSelected/(float)trialsIn;
	}	// if
}	// Triad::pCluster

BEGIN_MESSAGE_MAP(Triad, CDocument)
	//{{AFX_MSG_MAP(Triad)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Triad serialization

void Triad::Serialize(CArchive& ar)
{
	myPool.Serialize(ar);
	myStimResp.Serialize(ar);
	if (ar.IsStoring())
		ar << (WORD)pClustNAN;
	else
		ar >> (WORD&)pClustNAN;
}	// Triad::Serialize

/////////////////////////////////////////////////////////////////////////////
// Triad commands
                          
                          
/////////////////////////////////////////////////////////////////////////////
// StimResp class implementation

IMPLEMENT_SERIAL(StimResp, CObject, 0 /* Schema number */)

StimResp::StimResp()
{	// default constructor
}	// StimResp::StimResp

StimResp::StimResp(int e0, int e1, int e2)
{
	theEevorg[0] = e0;
	theEevorg[1] = e1;
	theEevorg[2] = e2;
}	// StimResp::StimResp

StimResp::~StimResp()
{
}	// StimResp::~StimResp

void StimResp::Serialize(CArchive& ar)
{	// overridden for document i/o
	if (ar.IsStoring())
	{
		ar << (WORD)theEevorg[0];
		ar << (WORD)theEevorg[1];
		ar << (WORD)theEevorg[2];
		ar << shownWhen;
		ar << shownUntil;
		ar << (WORD)userResponse;
	} else
	{
		ar >> (WORD&)theEevorg[0];
		ar >> (WORD&)theEevorg[1];
		ar >> (WORD&)theEevorg[2];
		ar >> shownWhen;
		ar >> shownUntil;
		ar >> (WORD&)userResponse;
	}	// if
}	// StimResp::Serialize             
