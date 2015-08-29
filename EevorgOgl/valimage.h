#pragma once

using namespace std;

/////////////////////////////////////////////////////////////
template<typename PixelType>
class valimage
{
public:
	valimage(void);
	valimage(size_t rows, size_t columns);
	valimage(const valimage<PixelType>& fromImage);
	~valimage(void);

	// the image rows and columns
	size_t rows() const { return _rows; }
	size_t columns() const { return _columns; }

	// resize by adding rows and columns; preserve content if flagged
	void resize(size_t rows, size_t columns, bool bPreserve);

	// fills the pixels with a given value
	void fill(const PixelType& value);

	// helper typedef for pixel array
	typedef valarray<PixelType> PixelArrayType;

	// accessor to pixel valarray
	PixelArrayType& pixels() { return _pixels; }
	const PixelArrayType& pixels() const {	return _pixels; }

	// helper typedef for vector type
	typedef valarray<float> vec;

	// accessor to pixels
	PixelType& operator()(size_t r, size_t c);
	PixelType& operator()(const vec& vAt);
	const PixelType& operator()(size_t r, size_t c) const;
	const PixelType& operator()(const vec& vAt) const;

	// a simple box filter (filter_rows and filter_columns should be odd)
	valimage<PixelType> applyBoxFilter(size_t filter_rows, size_t filter_columns) const;

private:
	size_t _rows;
	size_t _columns;
	PixelArrayType _pixels;
};

/////////////////////////////////////////////////////////////
template<typename PixelType>
valimage<PixelType>::valimage(void)
	: _rows(0)
	, _columns(0)
{
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
valimage<PixelType>::valimage(size_t init_rows, size_t init_columns)
	: _rows(0)
	, _columns(0)
{
	resize(init_rows, init_columns, false);
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
valimage<PixelType>::valimage(const valimage<PixelType>& fromImage)
	: _rows(0)
	, _columns(0)
{
	resize(fromImage.rows(), fromImage.columns(), false);

	_pixels = fromImage.pixels();

	//// work-around for const pixel pointer access
	//valarray<PixelType>& from_pixels =
	//	const_cast<valimage<PixelType>&>(fromImage).pixels();

	//memcpy(&(pixels()[0]), from_pixels.size()
	//copy(&(from_pixels[0]), 
	//	&(from_pixels[0]) + , 
	//	);
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
valimage<PixelType>::~valimage(void)
{
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
void valimage<PixelType>::resize(size_t new_rows, size_t new_columns, bool bPreserve)
{
	if (rows() == 0 
		|| columns() == 0
		|| !bPreserve)
	{
		_rows = new_rows;
		_columns = new_columns;
		_pixels.resize(_rows * _columns);
		return;
	}

	valimage<PixelType> copyImage(new_rows, new_columns);
	for (size_t row = 0; row < min(rows(), copyImage.rows()); row++)
	{
		memcpy(&copyImage.pixels()[0] + row * copyImage.columns(), 
			&pixels()[0] + row * columns(), 
			min(columns(), copyImage.columns()) * sizeof(PixelType));
	}
	_rows = new_rows;
	_columns = new_columns;
	_pixels = copyImage.pixels();
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
void valimage<PixelType>::fill(const PixelType& value)
{
	std::fill(&pixels()[0], &pixels()[0] + rows() * columns(), value);
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
PixelType& 
	valimage<PixelType>::operator()(size_t r, size_t c) 
{  
	return _pixels[r * _columns + c];  
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
PixelType& 
	valimage<PixelType>::operator()(const vec& vAt)
{
	return (*this)((size_t) vAt[1], (size_t) vAt[0]);
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
const PixelType& 
	valimage<PixelType>::operator()(size_t r, size_t c) const
{  
	return _pixels[r * _columns + c];  
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
const PixelType& 
	valimage<PixelType>::operator()(const vec& vAt) const
{
	return (*this)((size_t) vAt[1], (size_t) vAt[0]);
}

/////////////////////////////////////////////////////////////
template<typename PixelType>
valimage<PixelType> 
	valimage<PixelType>::applyBoxFilter(size_t filter_rows, size_t filter_columns) const
{
	valimage<PixelType> copy(*this);
	copy.resize(copy.rows()+(filter_rows-1), 
		copy.columns()+(filter_columns-1), true);

	// sum the neighborhood in the copy
	valimage<PixelType> result(copy.rows(), copy.columns());
	for (int row = 0; row < (int) filter_rows; row++)
	{
		for (int column = 0; column < (int) filter_columns; column++)
		{
			result.pixels() += copy.pixels().shift(-row * copy.columns() - column);
		}
	}

	return result;
}
