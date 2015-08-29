#pragma once

/////////////////////////////////////////////////////////////
class EevorgRenderer
{
public:
	EevorgRenderer(void);
	~EevorgRenderer(void);

	// checks and sets the buffer size to the passed rectangle
	void ResetBuffers(int height, int width);

	// accessors for rendered maps
	const valimage<COLORREF>& pixelBuffer();

	// tests for hit on a recently rendered Eevorg
	const Eevorg* hitTest(size_t row, size_t column);

	// helper vector class
	typedef valimage<int>::vec vec;

	// helper to recursively draw the eevorg with its children
	void DrawEevorgAndChildren(const Eevorg& eevorg, const vec& origin, float height, float zoom, bool bHistory = true);

	// draws a single Eevorg, at the given origin and with height and zoom factors
	//   returns actual drawing height
	float DrawScale(const Eevorg& eevorg, const vec& origin, float height, float zoom);

	// draws the eevorg straight (with center aligned at x = 0)
	void DrawStraight(const Eevorg& eevorg, float zoom);

protected:
	// transforming using the 'bulb' mapping
	vec transformBulb(const vec& vDisplay, float height, float zoom);
	vec inverseTransformBulb(const vec& vMapped, float height, float zoom);

	// bilinear interpolation based on 'bulb' mapping
	COLORREF interpEevorgPixelColor(const Eevorg& eevorg, 
		const Eevorg::ColorTableType& colorTable, const vec& vMapped);
	float interpGenerationGrayscale(const Eevorg::GenerationType& generation, float x);
	COLORREF pixelColor(int cellValue, size_t maxState, 
		const Eevorg::ColorTableType& colorTable);

private:
	// actual pixel buffer for drawing
	valimage<COLORREF> _pixelBuffer;

	// flag to indicate use of HE
	bool _bUseHE;

	// buffer for hit test
	valimage<const Eevorg*> _hitTest;
};

/////////////////////////////////////////////////////////////
inline const valimage<COLORREF>& 
	EevorgRenderer::pixelBuffer() 
{ 
	return _pixelBuffer; 
}

/////////////////////////////////////////////////////////////
inline const Eevorg* 
	EevorgRenderer::hitTest(size_t row, size_t column) 
{ 
	return _hitTest(row, column); 
}