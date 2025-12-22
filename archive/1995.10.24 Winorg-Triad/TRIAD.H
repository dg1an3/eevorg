// triad.h : header file
//
    
#ifndef __TRIAD_H__
#define __TRIAD_H__

#include "eevorg.h"

class StimResp;

class Triad : public CDocument
{
	DECLARE_SERIAL(Triad)
protected:
	Triad();			// protected constructor used by dynamic creation

// Attributes
public:
	CObArray myPool;	// Pool of Eevorg associated with this group
	CObArray myStimResp;	// Stimulus/responses associated with this group
	static int pClustNAN;	// # of times pCluster statistic is undefined

// Operations
public:
	StimResp *genStimResp();	// set up a single stimulus
	float pCluster(int eev1, int eev2);
		// calculate likelihood of two Eevorg being in the same cluster
		// returns -1 for NAN and increments pClustNAN

// Implementation
protected:
	virtual ~Triad();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(Triad)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};	// class Triad


class StimResp : public CObject
{
	DECLARE_SERIAL(StimResp)
public:
	StimResp();
	StimResp(int e0, int e1, int e2);
	
// Attributes
public:
	int theEevorg[3];	// Eevorg displayed
	
	CTime shownWhen;			// when displayed
	CTimeSpan shownUntil;	// length of time until response
	int userResponse;		// 0, 1, or 2
	
	virtual ~StimResp();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
};	// class StimResp

#endif
