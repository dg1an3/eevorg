#pragma once

#undef max

using namespace std;

/////////////////////////////////////////////////////////////
class Eevorg
{
public:

	typedef valimage<size_t> GeneType;
	typedef valimage<int> GenerationType;
	typedef valimage<int> HistogramType;

	Eevorg(size_t dimension = 1, const GeneType *gene = NULL);
	Eevorg(const Eevorg& parent);
	~Eevorg(void);

	// access to the Eevorg
	size_t dimension() const;

	// access to the rule
	const GeneType& gene() const;

	// color variables
	float hueSlope() const { return _hueSlope; }
	float hueOffset() const { return _hueOffset; }
	float saturationFrequency() const { return _saturationFrequency; }
	void setSaturationFrequency(float freq) 
	{ 
		_saturationFrequency = freq; 
		computeHslSineColorTable();
	}

	// number of potential states, given gene
	const size_t maxState() const;

	// zooom variable
	float zoom() const { return _zoom; }

	// access to generations
	const GenerationType& generation(size_t nGen) const;

	// access to the eevorg score
	const int score() const;
	void setScore(int newScore);

	// forms / returns a histogram to the specified generation
	const HistogramType& histogram(size_t toGeneration) const;
	const HistogramType& cumulativeHistogram(size_t toGeneration) const;
	
	// helper to define type for color tables
	typedef valarray<COLORREF> ColorTableType;

	// returns the eevorg's color table
	const ColorTableType& colorTable(bool bUseHE) const;

	// stores the pick name for the eevorg
	const GLuint pickName() const;

	// accessor to the parent
	const Eevorg *parent() const { return _parent; }

	// to tell if the children have been created
	bool childrenCreated() { return _children.size() > 0; }

	// children of the eevorg
	vector<Eevorg*>::iterator firstChild();
	vector<Eevorg*>::const_iterator firstChild() const;
	vector<Eevorg*>::iterator lastChild();
	vector<Eevorg*>::const_iterator lastChild() const;

	// find a particular child
	Eevorg * findChildByPickName(GLuint pickName);

protected:
	// mutation of the current eevorg
	void mutate();

	// helper to lookup a value using the rul
	const int geneLookup(int nValue) const;

	// helper to compute the color table
	void computeHslSineColorTable();

private:
	// pointer to parent (none if none)
	const Eevorg *_parent;

	// dimension of the Eevorg
	size_t _dimension;

	// the rule for the eevorg
	GeneType _gene;

	// stores the maximum possible cell state, given gene
	size_t _maxState;

	// my generations
	mutable vector<GenerationType> _generations;

	float _hueSlope;
	float _hueOffset;
	float _saturationFrequency;

	float _zoom;

	// my histogram and parameters
	mutable HistogramType _histogram;
	mutable HistogramType _cumulativeHistogram;
	mutable size_t _histogramToGeneration;

	// the input color table
	mutable ColorTableType _colorTable;

	// stores the HE color table
	mutable ColorTableType _colorTableHE;
	mutable bool _bComputeColorTableHE;

	// my score
	int _score;

	// the pick name
	GLuint _pickName;
	static GLuint _nextPickName;

	// kiddos
	mutable vector<Eevorg*> _children;
};

/////////////////////////////////////////////////////////////
inline size_t 
	Eevorg::dimension() const
{
	return _dimension;
}

/////////////////////////////////////////////////////////////
inline const size_t 
	Eevorg::maxState() const
{
	return _maxState;
}

/////////////////////////////////////////////////////////////
inline const Eevorg::GeneType& 
	Eevorg::gene() const
{
	return _gene;
}

/////////////////////////////////////////////////////////////
inline const int 
	Eevorg::score() const
{
	return _score;
}

/////////////////////////////////////////////////////////////
inline void 
	Eevorg::setScore(int newScore)
{
	_score = newScore;
}

/////////////////////////////////////////////////////////////
inline const int 
	Eevorg::geneLookup(int nValue) const
{ 
	return _gene.pixels()[nValue]; 
}
