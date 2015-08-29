TEPlot::TEPlot(int X1, int Y1, int X2, int Y2)
{	// constructor
	m_x1 = X1;
	m_y1 = Y1;
	m_x2 = X2;
	m_y2 = Y2;
	m_regions = new struct region[1];
	m_numOfRegions = 1;
	m_sortVelocity = 1.0;
	m_forAt = 0;
	m_backAt = 0;
};	// TEPlot::TEPlot

TEPlot::~TEPlot()
{	// destructor
	delete [] m_regions;
};	// TEPlot::~TEPlot

int TEPlot::doPlot()
{       // call to do one iteration of the plot
	// do the forward plot
	m_regions[m_forAt].
	if (m_forAt != m_backAt)
	{	// do backward plot
		m_regions[m_backAt].
	};	// if
	m_forAt = (m_forAt+1) % m_numOfRegions;
	m_backAt = (m_backAt+m_numOfRegions-1) % m_numOfRegions;
	// do the sort/split/merge
	// ...
	int newAttr = 0;
	int oldAttr = 0;
	if (newAttr > oldAttr)
	{	// switch the current region with the next
	} else if (m_regions[m_sortAt].TE > m_avgTE*4)
	{	// a split is pending
	} else if (m_regions[m_sortAt].TE < m_avgTE/4)
	{	// a merge is pending
	};	// if
};	// TEPlot::doPlot

