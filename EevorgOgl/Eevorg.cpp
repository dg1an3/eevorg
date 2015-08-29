#include "StdAfx.h"

#include "Eevorg.h"

#undef max
#undef min

using namespace std;

size_t initRule[] = {0, 2, 0, 1, 1, 3, 2, 1, 1, 0};
const size_t CHILD_COUNT = 3;
const int HISTOGRAM_GENERATIONS_HE = 25;
const float HUE_SLOPE_VARIANCE = 36.0f;
const float HUE_OFFSET_VARIANCE = 72.0f;
const float SATURATION_FREQUENCY_VARIANCE = 1.0f;

GLuint Eevorg::_nextPickName = 7;

/////////////////////////////////////////////////////////////
Eevorg::Eevorg(size_t dimension, const GeneType* gene)
	: _dimension(dimension)
	, _parent(NULL)
	, _score(0)
	, _zoom(4.0f)
	, _histogramToGeneration(0)
	, _bComputeColorTableHE(true)
{
	if (gene == NULL)
	{
		_gene.resize(1, sizeof(initRule)/sizeof(size_t), false);
		memcpy(&_gene.pixels()[0], &initRule[0], sizeof(initRule));
	}
	else
	{
		_gene = (*gene);
	}

	// cache the max state
	_maxState = _gene.pixels().max();

	GenerationType firstGen(1, 1);
	firstGen(0, 0) = 1;
	_generations.push_back(firstGen);

	_hueSlope = 180.0f * (float)rand()/(float)(RAND_MAX+1);
	_hueOffset = 220.0f * (float)rand()/(float)(RAND_MAX+1);
	_saturationFrequency = 4.0f * (float)rand()/(float)(RAND_MAX+1);
	computeHslSineColorTable();

	_pickName = _nextPickName;
	_nextPickName++;
}

/////////////////////////////////////////////////////////////
Eevorg::Eevorg(const Eevorg& parent)
	: _dimension(parent._dimension)
	, _parent(&parent)
	, _gene(parent.gene())
	, _hueOffset(parent.hueOffset())
	, _hueSlope(parent.hueSlope())
	, _saturationFrequency(parent.saturationFrequency())
	, _score(0)
	, _histogramToGeneration(0)
{
	// make a hybrid
	mutate();

	// and add g0
	GenerationType firstGen(1, 1);
	firstGen(0, 0) = 1;
	_generations.push_back(firstGen);

	_pickName = _nextPickName;
	_nextPickName++;
}

/////////////////////////////////////////////////////////////
Eevorg::~Eevorg(void)
{
	vector<Eevorg*>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++)
		delete (*iter);
}

/////////////////////////////////////////////////////////////
const Eevorg::GenerationType& 
	Eevorg::generation(size_t nGeneration) const
{
	if (_generations.size() <= nGeneration)
	{
		const GenerationType& prev = generation(nGeneration-1);

		size_t filter_rows = (dimension() > 1) ? 3 : 1;
		size_t filter_columns = 3;
		GenerationType next = prev.applyBoxFilter(filter_rows, filter_columns);

		// and transform by the rule lookup
		GenerationType::PixelArrayType::value_type *pixelIter;
		for (pixelIter = begin(next.pixels()); pixelIter != end(next.pixels()); pixelIter++)
			(*pixelIter) = geneLookup((*pixelIter));

		_generations.push_back(next);
	}

	return _generations[nGeneration];
}

/////////////////////////////////////////////////////////////
const Eevorg::HistogramType& 
	Eevorg::histogram(size_t toGeneration) const
{
	if (_histogramToGeneration != toGeneration)
	{
		_histogram.resize(1, maxState()+1, false);
		for (size_t n = 0; n <= toGeneration; n++)
		{
			const GenerationType& gen = generation(n);
			for (size_t c = 0; c < gen.columns(); c++)
				_histogram(0, gen(0, c))++;
		}
		_histogramToGeneration = toGeneration;
	}

	return _histogram;
}

/////////////////////////////////////////////////////////////
const Eevorg::HistogramType& 
	Eevorg::cumulativeHistogram(size_t toGeneration) const
{
	if (_histogramToGeneration != toGeneration)
	{
		const Eevorg::HistogramType& histo = histogram(toGeneration);
		_cumulativeHistogram.resize(1, histo.columns(), false);
		int nCum = 0;
		for (size_t n = 0; n < histo.columns(); n++)
		{
			nCum += histo(0, n);
			_cumulativeHistogram(0, n) = nCum;
		}
	}

	return _cumulativeHistogram;
}

/////////////////////////////////////////////////////////////
vector<Eevorg*>::iterator 
	Eevorg::firstChild()
{
	// construct the children if not already
	if (_children.size() == 0)
		for (int n = 0; n < CHILD_COUNT; n++)
			_children.push_back(new Eevorg(*this));

	// get begin child
	return _children.begin();
}

/////////////////////////////////////////////////////////////
vector<Eevorg*>::const_iterator 
	Eevorg::firstChild() const
{
	// construct the children if not already
	if (_children.size() == 0)
		for (int n = 0; n < CHILD_COUNT; n++)
			_children.push_back(new Eevorg(*this));

	// get begin child
	return _children.begin();
}

/////////////////////////////////////////////////////////////
vector<Eevorg*>::iterator 
	Eevorg::lastChild()
{
	// construct the children if not already
	if (_children.size() == 0)
		for (int n = 0; n < CHILD_COUNT; n++)
			_children.push_back(new Eevorg(*this));

	// get the end child
	return _children.end();
}

/////////////////////////////////////////////////////////////
vector<Eevorg*>::const_iterator 
	Eevorg::lastChild() const
{
	// construct the children if not already
	if (_children.size() == 0)
		for (int n = 0; n < CHILD_COUNT; n++)
			_children.push_back(new Eevorg(*this));

	// get the end child
	return _children.end();
}

//////////////////////////////////////////////////////////////
Eevorg * 
	Eevorg::findChildByPickName(GLuint pickName)
{
	if (this->pickName() == pickName)
		return this;

	vector<Eevorg*>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++)
	{
		Eevorg *pMatch = (*iter)->findChildByPickName(pickName);
		if (pMatch != NULL)
			return pMatch;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////
void Eevorg::mutate()
{ 
	size_t geneToModify = rand() * (gene().columns()-1) / RAND_MAX;
#ifdef _DEBUG
	assert(geneToModify < gene().columns());
#endif
	_gene(0, geneToModify)++;

	_maxState = gene().pixels().max();
	size_t neighborhood_size = 3;
	if (dimension() > 1)
		neighborhood_size *= 3;
	if (gene().columns() < _maxState * neighborhood_size + 1)
	{
		_gene.resize(1, _maxState * neighborhood_size + 1, true);
	}

	_hueSlope += (HUE_SLOPE_VARIANCE * (-1.0f + 2.0f * (float)rand()/(float)(RAND_MAX+1))); 
	_hueSlope = __max(_hueSlope, 20.0f);
	_hueOffset += (HUE_OFFSET_VARIANCE * (-1.0f + 2.0f * (float)rand()/(float)(RAND_MAX+1)));
	while (0.0f > _hueOffset) _hueOffset += 360.0f;
	while (_hueOffset >= 360.0f) _hueOffset -= 360.0f;
	_saturationFrequency += (SATURATION_FREQUENCY_VARIANCE * (-1.0f + 2.0f * (float)rand()/(float)(RAND_MAX+1))); 
	_saturationFrequency = __max(_saturationFrequency, 1.0f);
	computeHslSineColorTable();
}

/////////////////////////////////////////////////////////////
const Eevorg::ColorTableType& 
	Eevorg::colorTable(bool bUseHE) const
{
	if (!bUseHE)
	{
		return _colorTable;
	}

	if (_bComputeColorTableHE)
	{
		_colorTableHE.resize(_colorTable.size());

		// set up the cumulative histogram
		const Eevorg::HistogramType& cumulativeHisto = cumulativeHistogram(HISTOGRAM_GENERATIONS_HE);
		const int cumulativeMax = cumulativeHisto(0, cumulativeHisto.columns()-1);
		for (size_t n = 0; n < _colorTableHE.size(); n++)
		{
			size_t cumulativeHistoBin = n * (cumulativeHisto.columns()-1) / _colorTableHE.size();
			cumulativeHistoBin = min(cumulativeHisto.columns()-1, cumulativeHistoBin);
			const int cumulativeHistoValue = cumulativeHisto(0, cumulativeHistoBin);

			size_t atBin = cumulativeHistoValue * (_colorTable.size()-1) / cumulativeMax;
			atBin = min(_colorTable.size()-1, atBin);
			_colorTableHE[n] = _colorTable[atBin];
		}

		// store the input for caching
		_bComputeColorTableHE = false;
	}

	return _colorTableHE;
}


/////////////////////////////////////////////////////////////
void 
	rgbToHsl(COLORREF color, float& hue, float& sat, float& lum)
{
	float r = (float) GetRValue(color) / 255.0f;
	float g = (float) GetGValue(color) / 255.0f;
	float b = (float) GetBValue(color) / 255.0f;

	float cmax = __max(__max(r, g), b);
	float cmin = __min(__min(r, g), b);

	// compute hue
	hue = 0.0f;
	if (cmax > cmin)
	{
		float crange = cmax - cmin;
		if (cmax == r)
		{
			if (g >= b)
			{
				hue = 60.0f * (g-b)/crange + 0.0f;
			}
			else
			{
				hue = 60.0f * (g-b)/crange + 360.0f;
			}
		}
		else if (cmax == g)
		{
			hue = 60.0f * (b-r)/crange + 120.0f;
		}
		else if (cmax == b)
		{
			hue = 60.0f * (r-g)/crange + 240.0f;
		}
	}

	// compute luminance
	lum = 0.5f * (cmax + cmin);

	// compute saturation
	sat = 0.0f;
	if (cmax > cmin)
	{
		if (lum <= 0.5f)
		{
			sat = (cmax-cmin)/(2.0f*lum);
		}
		else
		{
			sat = (cmax-cmin)/(2.0f - 2.0f*lum);
		}
	}
}

////////////////////////////////////////////////////////////
float 
	baryc(float t, float p, float q)
{
	if (t < 0.0f) 
		t += 1.0f;

	if (t > 1.0f) 
		t -= 1.0f;

	float c = 0.0f;
	if (t < 1.0f / 6.0f)
	{
		c = p + ((q-p) * 6.0f * t);
	}
	else if (t < 0.5f)
	{
		c = q;
	}
	else if (t < 2.0f / 3.0f)
	{
		c = p + ((q-p) * 6.0f * (2.0f / 3.0f - t));
	}
	else
	{
		c = p;
	}

	return c;
}

////////////////////////////////////////////////////////////
COLORREF 
	hslToRgb(float hue, float sat, float lum)
{
	float q = 0.0f;
	if (lum < 0.5f)
	{
		q = lum + (lum*sat);
	}
	else
	{
		q = lum + sat - (lum*sat);
	}

	float p = 2.0f * lum - q;

	float hue_k = hue / 360.0f;

	float r = baryc(hue_k + 1.0f / 3.0f, p, q);
	float g = baryc(hue_k, p, q);
	float b = baryc(hue_k - 1.0f / 3.0f, p, q);

	return (DWORD)(RGB(r * 255.0f, g * 255.0f, b * 255.0f)) | (DWORD)(__min(1.0, 1.7*sqrt(lum)) * 255.0f) << 24;
}

/////////////////////////////////////////////////////////////
void 
	Eevorg::computeHslSineColorTable()
{
	_colorTable.resize(256);
	_colorTable[0] = 0x20000000;

	const float PI = (float) (2.0 * asin(1.0));
	float luminance = 0.0f;
	for (size_t n = 1; n < _colorTable.size(); n++)
	{
		float hue = _hueOffset + luminance * _hueSlope; 
		while (0.0f > hue) hue += 360.0f;
		while (hue >= 360.0f) hue -= 360.0f;
		float saturation = 0.5f - (float) cos(luminance * 2.0 * PI * _saturationFrequency) / 2.0f;
		_colorTable[n] = hslToRgb(hue, saturation, luminance);

		luminance += 1.0f / (float) _colorTable.size();
	}

	_bComputeColorTableHE = true;
}

/////////////////////////////////////////////////////////////
const GLuint 
	Eevorg::pickName() const
{
	return _pickName;
}