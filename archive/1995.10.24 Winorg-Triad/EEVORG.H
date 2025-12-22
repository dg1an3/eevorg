// eevorg.h : interface of the Eevorg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __EEVORG_H__
#define __EEVORG_H__

#define WITH_MUTATION 1

class Eevorg : public CObject
{
public:
	Eevorg(Eevorg *fromParent, int mutFlag = !WITH_MUTATION);
	Eevorg();	// create from serialization only
	DECLARE_SERIAL(Eevorg)
	
// Attributes
	Eevorg *parent;
	CByteArray rule;
	int maxRule;
	CObArray gen;
	int maxVal;
	int score;
	
// Operations
	int numOfGen()  { return gen.GetSize(); }
    int nextGen();	// compute next generation and return
    CByteArray *getGen(int forGen) { return (CByteArray *)gen.GetAt(forGen); }
    // void mutate();

	void drawAt(CDC *pDC, int atX, int atY, int hgt);

    int getScore() { return score; }
    void incScore() { score++; }
    	
// Implementation
	virtual ~Eevorg();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
};	// class Eevorg

#endif
/////////////////////////////////////////////////////////////////////////////
