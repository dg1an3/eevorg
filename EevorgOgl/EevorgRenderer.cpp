#include "StdAfx.h"
#include "Eevorg.h"
#include "EevorgRenderer.h"

const float ACTUAL_HEIGHT_DRAW_FACTOR = 2.0f / 3.0f;
const float ACTUAL_HEIGHT_CHILD_POSITION_FACTOR = 33.0f / 48.0f;
const float CHILD_HEIGHT_RATIO = 1.0f / 3.0f;
const float CHILD_ZOOM_RATIO = 1.0f / 2.0f; // 2.0f / 3.0f;
const float SIDE_CHILD_YSTART_RATIO = 1.2f;

/////////////////////////////////////////////////////////////
EevorgRenderer::EevorgRenderer(void)
{
	_bUseHE = false;
}

/////////////////////////////////////////////////////////////
EevorgRenderer::~EevorgRenderer(void)
{
}

/////////////////////////////////////////////////////////////
void EevorgRenderer::ResetBuffers(int height, int width)
{
	//int height = rect.bottom - rect.top;
	//int width = rect.right - rect.left;
	if (_pixelBuffer.rows() != height
		|| _pixelBuffer.columns() != width)
	{
		_pixelBuffer.resize(height, width, false);
		_hitTest.resize(height, width, false);
	}

	_pixelBuffer.fill(RGB(64,64,64));
}

/////////////////////////////////////////////////////////////
static float bulbWidth(float y, float height)
{
	return 1.0f - pow(y, 3) / pow(height, 3);
}

/////////////////////////////////////////////////////////////
EevorgRenderer::vec 
	EevorgRenderer::transformBulb(const vec& vDisplay, float height, float zoom)
{
	valarray<float> vMapped(2);
	vMapped[0] = vDisplay[0] / bulbWidth(vDisplay[1], height) / zoom;
	vMapped[1] = vDisplay[1] / zoom;
	return vMapped;
}

/////////////////////////////////////////////////////////////
EevorgRenderer::vec 
	EevorgRenderer::inverseTransformBulb(const vec& vMapped, float height, float zoom)
{
	valarray<float> vDisplay(2);
	vDisplay[1] = vMapped[1] * zoom;
	vDisplay[0] = vMapped[0] * bulbWidth(vDisplay[1], height) * zoom;
	return vDisplay;
}

/////////////////////////////////////////////////////////////
COLORREF 
	EevorgRenderer::pixelColor(int cellValue, size_t maxState, const Eevorg::ColorTableType& colorTable)
{
	int nIndex = cellValue * colorTable.size() / (maxState+1);
	return colorTable[nIndex];
}

/////////////////////////////////////////////////////////////
COLORREF 
	interpRGB(COLORREF color0, float frac0, COLORREF color1, float frac1)
{
	// NOTE: inverts RGB to actually make BGR for rendering
	return RGB(
		(float) GetBValue(color0) * frac0 + (float) GetBValue(color1) * frac1,
		(float) GetGValue(color0) * frac0 + (float) GetGValue(color1) * frac1,
		(float) GetRValue(color0) * frac0 + (float) GetRValue(color1) * frac1);
}

/////////////////////////////////////////////////////////////
template<typename T>
int Round(const T& value)
{
	return (int) floor(value + T(0.5));
}

/////////////////////////////////////////////////////////////
float
	EevorgRenderer::interpGenerationGrayscale(const Eevorg::GenerationType& generation, float x)
{
	const float middle = (float) (generation.columns()-1) / 2;
	const float actualX = middle + x;

	float gray0 = 0.0f;
	const float cell0 = floor(actualX);
	if (cell0 >= 0 && cell0 < generation.columns())
		gray0 = (float) generation(0, (size_t) cell0);

	float gray1 = 0.0f;
	const float cell1 = cell0 + 1.0f;
	if (cell1 >= 0 && cell1 < generation.columns())
		gray1 = (float) generation(0, (size_t) cell1);

	float frac0 = (cell1 - actualX) / (cell1 - cell0);
	float frac1 = 1.0f - frac0;
	
	return gray0 * frac0 + gray1 * frac1;
}

/////////////////////////////////////////////////////////////
COLORREF 
	EevorgRenderer::interpEevorgPixelColor(const Eevorg& eevorg, 
	const Eevorg::ColorTableType& colorTable, const vec& vMapped)
{
	const float prevGenerationNumber = floor(vMapped[1]);
	const Eevorg::GenerationType& prevGeneration = eevorg.generation((size_t) prevGenerationNumber);
	float prevGray = interpGenerationGrayscale(prevGeneration, vMapped[0]);

	const float nextGenerationNumber = prevGenerationNumber + 1.0f;
	const Eevorg::GenerationType& nextGeneration = eevorg.generation((size_t) nextGenerationNumber);
	float nextGray = interpGenerationGrayscale(nextGeneration, vMapped[0]);

	float fracPrev = (nextGenerationNumber - vMapped[1]) / (nextGenerationNumber - prevGenerationNumber);
	float fracNext = (vMapped[1] - prevGenerationNumber) / (nextGenerationNumber - prevGenerationNumber);

	// float quantize = 0.5f;
	return pixelColor(Round((fracPrev * prevGray + fracNext * nextGray)*2.0f), 
		eevorg.maxState()*2, colorTable);
}

/////////////////////////////////////////////////////////////
float 
	EevorgRenderer::DrawScale(const Eevorg& eevorg, const vec& origin, float height, float zoom)
{
	float actualHeight = ACTUAL_HEIGHT_DRAW_FACTOR * height;

	const Eevorg::ColorTableType& colorTable = eevorg.colorTable(_bUseHE); 

	vec vDisplay(2);
	vec vWindow(2);
	for (vDisplay[1] = 0; vDisplay[1] < actualHeight; vDisplay[1]++)
	{
		// determine start / end for display
		vDisplay[0] = 0;
		vec vMapped = transformBulb(vDisplay, actualHeight, zoom);
		vMapped[0] = vMapped[1];	// bounds are along y = x line (in mapped space)
		vec vDisplayBounds = inverseTransformBulb(vMapped, actualHeight, zoom);

		vDisplay[0] = -vDisplayBounds[0];
		for (; vDisplay[0] <= vDisplayBounds[0]; vDisplay[0]++)
		{
			vWindow = origin + vDisplay;

			if ((int) vWindow[1] < (int) _pixelBuffer.rows()-1)
			{
				vec vMapped = transformBulb(vDisplay, actualHeight, zoom);
				_pixelBuffer(vWindow) = interpEevorgPixelColor(eevorg, colorTable, vMapped);
				_hitTest(vWindow) = &eevorg;
			}
		}
	}

	return actualHeight;
}

/////////////////////////////////////////////////////////////
void 
	EevorgRenderer::DrawStraight(const Eevorg& eevorg, float zoom)
{
	const Eevorg::ColorTableType& colorTable = eevorg.colorTable(_bUseHE); 

	vec vDisplay(2);
	for (int y = 0; y < _pixelBuffer.rows(); y++)
	{
		vDisplay[1] = (float) y / zoom;
		for (int x = 0; x < _pixelBuffer.columns(); x++)
		{
			vDisplay[0] = (float) x / zoom;
			_pixelBuffer(y, x) = interpEevorgPixelColor(eevorg, colorTable, vDisplay);
		}
	}
}

/////////////////////////////////////////////////////////////
static float sideChildYStart(float actualHeight)
{
	return pow(pow(actualHeight, 3) / 4.0f, 1.0f / 3.0f);
}

/////////////////////////////////////////////////////////////
void 
	EevorgRenderer::DrawEevorgAndChildren(const Eevorg& eevorg, const vec& origin, float height, float zoom, bool bHistory)
{
	if (height < 4)
		return;

	DrawScale(eevorg, origin, height, zoom);

	// adjust actual height to make it slightly larger
	float actualHeight = ACTUAL_HEIGHT_CHILD_POSITION_FACTOR * height;

	// calculate the starting height for child
	float yStartSideChild = sideChildYStart(actualHeight);

	std::vector<Eevorg*>::const_iterator iter = eevorg.firstChild();

	vec originLeft = origin;
	originLeft[0] -= yStartSideChild * bulbWidth(yStartSideChild, actualHeight);
	originLeft[1] += yStartSideChild * SIDE_CHILD_YSTART_RATIO;
	DrawEevorgAndChildren(*(*iter), originLeft, height * CHILD_HEIGHT_RATIO, zoom * CHILD_ZOOM_RATIO, false);

	vec originMid = origin;
	originMid[1] += actualHeight;
	DrawEevorgAndChildren(*(*(iter+1)), originMid, height * CHILD_HEIGHT_RATIO, zoom * CHILD_ZOOM_RATIO, false);

	vec originRight = origin;
	originRight[0] += yStartSideChild * bulbWidth(yStartSideChild, actualHeight);
	originRight[1] += yStartSideChild * SIDE_CHILD_YSTART_RATIO;
	DrawEevorgAndChildren(*(*(iter+2)), originRight, height * CHILD_HEIGHT_RATIO, zoom * CHILD_ZOOM_RATIO, false);

	if (bHistory 
		&& eevorg.parent() != NULL)
	{
		// calculate maximum extent of bulb width
		float maxExtent = 0.0f;
		vec vDisplay(2);
		for (vDisplay[1] = 0; vDisplay[1] < ACTUAL_HEIGHT_DRAW_FACTOR * height; vDisplay[1]++)
		{
			// determine start / end for display
			vDisplay[0] = 0;
			vec vMapped = transformBulb(vDisplay, ACTUAL_HEIGHT_DRAW_FACTOR * height, zoom);
			vMapped[0] = vMapped[1];	// bounds are along y = x line (in mapped space)
			vec vDisplayBounds = inverseTransformBulb(vMapped, ACTUAL_HEIGHT_DRAW_FACTOR * height, zoom);
			maxExtent = __max(vDisplayBounds[0], maxExtent);
		}

		vec originHistory = origin;
		originHistory[0] += maxExtent * 1.5f + 20.0f;
		originHistory[1] += 40;
		DrawScale(*eevorg.parent(), originHistory, height * CHILD_HEIGHT_RATIO, zoom * CHILD_ZOOM_RATIO);
	}
}
