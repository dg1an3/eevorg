#include "StdAfx.h"
#include "EevorgRenderer.h"

namespace Eevorg
{

//////////////////////////////////////////////////////////////////////////////
void 
	EevorgRenderer::DrawScale(EevorgOb ^eev, Graphics ^grf, PointF orig, float height)
{
	// get the region
	System::Drawing::Region ^region;
	if (!m_mapRegions.TryGetValue(eev, region))
	{
		region = gcnew System::Drawing::Region();
		m_mapRegions.Add(eev, region);
	}

	// clear the region
	region->MakeEmpty();

	// adjust origin and height
	orig.Y += 10;
	float actHgt = height * 0.80f;

	// compute the cell radius
	float radius = Math::Floor(Math::Log(height))-2;

	// compute color scale (also triggers calculation of colormap
	float colorScale = (GetColormap()->Length-1) / eev->MaxVal;

	// iterate over cells
	Point atCel;
	for (; atCel.Y < actHgt; atCel.Y++)
	{
		array<int> ^curGen = eev->GetGen(atCel.Y);
		for (atCel.X = curGen->Length-1; atCel.X >= 0; atCel.X--)
		{                           
			Color clr = m_colormap[curGen[atCel.X] * colorScale];

			PointF pos = CelPos(atCel, actHgt, radius);

			int nRadius = Math::Max(2.0, Math::Ceiling(radius+1));
			DrawCel(grf, orig.X + pos.X, orig.Y + pos.Y, nRadius, clr, region);
			DrawCel(grf, orig.X - pos.X, orig.Y + pos.Y, nRadius, clr, region);
		}
	}

	// adjust and draw children now
	height *= 0.5;
	if (height > 20)
	{
		for (int nChild = 0; nChild < 3; nChild++)
		{
			DrawScale(eev->GetChild(nChild), grf, orig + ChildOrig(nChild, actHgt, radius), height);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
PointF 
	EevorgRenderer::CelPos(Point atCel, float actHeight, float scale)
{
	PointF pos = atCel;
	pos.X *= scale * (1.0 - Math::Pow(pos.Y / actHeight, 4.0f)); 
	pos.Y *= scale;
	return pos;
}

//////////////////////////////////////////////////////////////////////////////
void 
	EevorgRenderer::DrawCel(Graphics ^grf, int atX, int atY, int nRadius, Color clr, 
			System::Drawing::Region ^reg)
{
	Brush ^brush = gcnew SolidBrush(clr);
	grf->FillEllipse(brush, atX-nRadius, atY-nRadius, nRadius*2, nRadius*2);
	if (clr.A > 0)
	{
		reg->Union(Rectangle(atX-nRadius*2, atY-nRadius*2, nRadius*4, nRadius*4));
	}
} 

//////////////////////////////////////////////////////////////////////////////
SizeF 
	EevorgRenderer::ChildOrig(int nChild, float actHeight, float radius)
{
	// multiply to add a buffer to the height
	actHeight *= 1.10;

	// compute as offset from parent origin
	SizeF childOrig;
	if (nChild == 1)
	{
		childOrig.Height = Math::Round(radius * actHeight);
	}
	else
	{
		float childStartY = Math::Pow(Math::Pow(actHeight, 3)/4.0, 1.0/3.0);
		float xDir = ((nChild == 0) ? -1 : 1);
		childOrig.Width = Math::Round(xDir * radius * childStartY * (1.0 - Math::Pow(childStartY/actHeight, 4)));
		childOrig.Height = Math::Round(radius * (childStartY * 1.3));
	}

	return childOrig;
} 

//////////////////////////////////////////////////////////////////////////////
array<Color> ^
	EevorgRenderer::GetColormap()
{
	if (m_colormap == nullptr)
	{
		// load the colormap
		System::Resources::ResXResourceSet ^resSet
			= gcnew System::Resources::ResXResourceSet(L"EevorgRenderer.resx");
		Bitmap ^colormapBitmap = (Bitmap ^) resSet->GetObject("spectrum");

		// allocate the color map and populate it
		m_colormap = gcnew array<Color>(colormapBitmap->Width);
		for (int n = 0; n < colormapBitmap->Width; n++)
		{
			int alpha = 128;
			if (n < colormapBitmap->Width / 2)
			{
				alpha = 128 * n / (colormapBitmap->Width / 2);
			}
			m_colormap[n] = Color::FromArgb(alpha, colormapBitmap->GetPixel(n, 0));
		}
	}

	return m_colormap;
}

}