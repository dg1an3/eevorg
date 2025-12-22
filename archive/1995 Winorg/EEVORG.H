// eevorg.h : interface of the Eevorg class
//
/////////////////////////////////////////////////////////////////////////////

class Eevorg : public CDocument
{
protected: // create from serialization only
	Eevorg();
	DECLARE_DYNCREATE(Eevorg)

// Attributes
public:
protected:
	CObArray gen;
	int maxVal;
	CByteArray rule;
	int maxRule;
	int score;
	Eevorg *leftChild, *midChild, *rightChild;
	
// Operations
public:
	int numOfGen()	// returns number of generations
		{ return gen.GetSize(); }
    int nextGen();	// compute next generation and return
    CByteArray *getGen(int forGen)	// retrieve a generation
		{ return (CByteArray *)gen.GetAt(forGen); }    
    void Mutate();
    int getScore()
    	{ return score; }
    void incScore()
    	{ score++; }
    Eevorg *getLeftChild();
    Eevorg *getMidChild();
    Eevorg *getRightChild();
    
// Implementation
public:
	virtual ~Eevorg();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	void LoadOldFormat(const char * fileName);	// loads from an old file
	
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual	BOOL	OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(Eevorg)
	afx_msg void OnFileMutate();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
