
// EevorgViewGl.cpp : implementation of the CEevorgViewGl class
//

#include "stdafx.h"
#include "EevorgOglApp.h"

#include "Eevorg.h"
#include "EevorgDoc.h"

#include "EevorgRenderer.h"
#include "EevorgViewGl.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEevorgViewGl

IMPLEMENT_DYNCREATE(CEevorgViewGl, CView)

BEGIN_MESSAGE_MAP(CEevorgViewGl, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CEevorgView construction/destruction

CEevorgViewGl::CEevorgViewGl()
{
}

CEevorgViewGl::~CEevorgViewGl()
{
}

BOOL CEevorgViewGl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// GL helper functions

#define checkImageSize 128

void CEevorgViewGl::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	// glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
}

BOOL CEevorgViewGl::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}
#pragma warning(default : 4244)

static float PI = 3.1415926535897932384626433832795f;

void RenderWedge(float angleStart, float angleEnd, GLuint textureId, bool bInverse, float scale)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureId);

	float sinStart = sin(angleStart * PI / 180.0f);
	float cosStart = cos(angleStart * PI / 180.0f);

	float sinEnd = sin(angleEnd * PI / 180.0f);
	float cosEnd = cos(angleEnd  * PI / 180.0f);

	glBegin(GL_QUAD_STRIP);

	for (float y = 0.0f; y <= scale + 0.005f; y += 0.01f)
	{
		float y_scale = y / ((scale >= 1.0f) ? scale : 1.0f);

		float bulbWidth = 1.0f - pow(y/scale, 3.0f);
		float x = y * bulbWidth;
		double dx_dy = (-3.0f / scale * pow(y/scale, 2.0f)) * x;

		glTexCoord2f(bInverse ? y_scale : 0, y_scale);
		glVertex3f(x * cosStart, -y, x * sinStart);
		glNormal3f((GLfloat)(dx_dy * cosStart), -1.0f, (GLfloat)(dx_dy * sinStart));

		glTexCoord2f(bInverse ? 0 : y_scale, y_scale);
		glVertex3f(x * cosEnd, -y, x * sinEnd);
		glNormal3f((GLfloat)(dx_dy * cosEnd), -1.0f, (GLfloat)(dx_dy * sinEnd));
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

GLuint CreateEevorgTexture(const valimage<COLORREF>& buffer, GLuint size)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size, size,
		GL_RGBA, GL_UNSIGNED_BYTE, &buffer.pixels()[0]);

	return textureId;
}

void 
	CEevorgViewGl::DrawEevorg(const Eevorg *eevorg, float x, float y, float rotateX, float rotateY, float rotateZ, float scale, bool bPickMode)
{
	GLuint textureId = 0;
	std::map<const Eevorg*, GLuint>::const_iterator iter = m_mapTextures.find(eevorg);
	if (iter != m_mapTextures.end())
	{
		textureId = iter->second;
	}
	else
	{
		// resize pixel buffer if needed
		_renderer.ResetBuffers(checkImageSize, checkImageSize);
		_renderer.DrawStraight(*eevorg, 2.0f);
		
		textureId = CreateEevorgTexture(_renderer.pixelBuffer(), checkImageSize);
		m_mapTextures.insert( std::pair<const Eevorg*,GLuint>(eevorg, textureId) );
	}

	if (bPickMode) glPushName(eevorg->pickName());

	glPushMatrix();

		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glTranslatef(x, y, -m_fRadius);
		glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
		glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

		// first rendering back faces with no depth test
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		for (float wedgeStart = 0.0f; wedgeStart < 360.0f; wedgeStart += 120.0f)
		{
			RenderWedge( wedgeStart +  0.0f, wedgeStart +  60.0f, textureId, true, scale);
			RenderWedge( wedgeStart + 60.0f, wedgeStart + 120.0f, textureId, false, scale);
		}

		// now render front faces with depth test
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		for (float wedgeStart = 0.0f; wedgeStart < 360.0f; wedgeStart += 120.0f)
		{
			RenderWedge( wedgeStart +  0.0f, wedgeStart +  60.0f, textureId, true, scale);
			RenderWedge( wedgeStart + 60.0f, wedgeStart + 120.0f, textureId, false, scale);
		}

	glPopMatrix();

	if (bPickMode) glPopName();
}

// the pick buffer
#define BUFSIZE 512
int nHits;
GLuint selectBuf[BUFSIZE];

void CEevorgViewGl::DrawScene(bool bPickMode, LPRECT pickRect)
{
	static BOOL     bBusy = FALSE;
	static GLfloat  wAngleX = 20.0f;
	static GLfloat  wAngleY = 0.0f;
	static GLfloat  wAngleZ = 0.0f;

	if(bBusy)
		return;
	bBusy = TRUE;

	if (bPickMode)
	{
		GLint viewport[4];

		glSelectBuffer(BUFSIZE,selectBuf);
		glRenderMode(GL_SELECT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glGetIntegerv(GL_VIEWPORT, viewport);
		gluPickMatrix(pickRect->left, viewport[3]-pickRect->bottom, 
			5, 5, viewport);

		CRect rectClient;
		GetClientRect(&rectClient);

		double ratio = (double) rectClient.Width()
			/ (double) rectClient.Height();
		gluPerspective(45, ratio, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
		glInitNames();
	}

	const Eevorg * eevorg = currentEevorg();

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	// 0	1	1
	DrawEevorg(eevorg, 0.0f, 1.0f, wAngleX, wAngleY, wAngleZ, 1.3f, bPickMode);

	vector<Eevorg*>::const_iterator iterChild = eevorg->firstChild();
	// -0.75 -0.05 0.5
	DrawEevorg(*iterChild, -0.75f, -0.05f, wAngleX, wAngleY, wAngleZ, 0.6f, bPickMode);

		vector<Eevorg*>::const_iterator iterGrandchild = (*iterChild)->firstChild();
		// -1.125 -0.575 0.25
		DrawEevorg(*iterGrandchild, -1.0f, -0.575f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// -0.75 -0.675 0.25
		DrawEevorg(*iterGrandchild, -0.75f, -0.675f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// -0.375 -0.575 0.25
		DrawEevorg(*iterGrandchild, -0.5f, -0.575f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

	iterChild++;
	// 0	-0.25	0.5
	DrawEevorg(*iterChild, 0.0f, -0.25f, wAngleX, wAngleY, wAngleZ, 0.6f, bPickMode);

		iterGrandchild = (*iterChild)->firstChild();
		// -0.375 -0.775 0.25
		DrawEevorg(*iterGrandchild, -0.25f, -0.775f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// 0 -0.875 0.25
		DrawEevorg(*iterGrandchild, 0.0f, -0.875f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// 0.375	-0.775	0.25
		DrawEevorg(*iterGrandchild,  0.25f, -0.775f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

	iterChild++;
	// 0.75 -0.05 0.5
	DrawEevorg(*iterChild, 0.75f, -0.05f, wAngleX, wAngleY, wAngleZ, 0.6f, bPickMode);

		iterGrandchild = (*iterChild)->firstChild();
		// 0.375 -0.575 0.25
		DrawEevorg(*iterGrandchild, 0.5f, -0.575f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// 0.75 -0.675 0.25
		DrawEevorg(*iterGrandchild, 0.75f, -0.675f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

		iterGrandchild++;
		// 1.125 -0.575 0.25
		DrawEevorg(*iterGrandchild, 1.0f, -0.575f, wAngleX, wAngleY, wAngleZ, 0.25f, bPickMode);

	wAngleY += 1.0f;
	if (wAngleY >= 360.0f)
		wAngleY -= 360.0f;

	if (bPickMode)
	{
		// restoring the original projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glFlush();
	
		// returning to normal rendering mode
		nHits = glRenderMode(GL_RENDER);
	}
	else
	{
		glFinish();
		SwapBuffers(wglGetCurrentDC());
	}

	bBusy = FALSE;
}

void ProcessHits(int nHits, GLuint *pSelectBuf)
{
}

// CEevorgView drawing


// CEevorgView printing

BOOL CEevorgViewGl::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEevorgViewGl::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEevorgViewGl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CEevorgView diagnostics

#ifdef _DEBUG
void CEevorgViewGl::AssertValid() const
{
	CView::AssertValid();
}

void CEevorgViewGl::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEevorgDoc* CEevorgViewGl::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEevorgDoc)));
	return (CEevorgDoc*)m_pDocument;
}
#endif //_DEBUG

const Eevorg *
	CEevorgViewGl::currentEevorg()
{
	CEevorgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		return pDoc->_pCurrentEevorg;

	return NULL;
}

// CEevorgView message handlers

void CEevorgViewGl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEevorgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	const Eevorg *pNewEevorg = NULL;
#ifdef OLD_SELECTION
	pNewEevorg = _renderer.hitTest(point.y, point.x);
#else
	CRect wndRect;
	GetClientRect(&wndRect);

	nHits = 0;
	CRect rectPoint(point.x-5, point.y-5, point.x+5, point.y+5);
	DrawScene(true, &rectPoint);

	GLuint *pCurrentHit = selectBuf;
	GLuint minDepth = numeric_limits<GLuint>::max();
	for (int i = 0; i < nHits; i++)
	{
		GLuint currentNameCount = pCurrentHit[0];
		GLuint currentMinDepth = pCurrentHit[1];
		GLuint currentMaxDepth = pCurrentHit[2];

		if (currentMinDepth < minDepth)
		{
			for (int name = 0; name < currentNameCount; name++)
			{
				Eevorg *pMatch = pDoc->FindEevorgByPickName(pCurrentHit[3 + name]);
				if (pMatch != NULL)
					pNewEevorg = pMatch;
			}
		}
		pCurrentHit += (2 + currentNameCount);
	}
#endif

	if (pNewEevorg != NULL)
	{
		pDoc->_pCurrentEevorg = const_cast<Eevorg*>(pNewEevorg);

		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE);
	}

	CView::OnLButtonDown(nFlags, point);
}



BOOL CEevorgViewGl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE; // CView::OnEraseBkgnd(pDC);
}


int CEevorgViewGl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init(); // initialize OpenGL

	SetTimer(1, 30, NULL);

	return 0;
}


void CEevorgViewGl::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(cy > 0)
	{
		glViewport(0, 0, cx, cy);

		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx/cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}


void CEevorgViewGl::OnTimer(UINT_PTR nIDEvent)
{
	DrawScene(false, NULL);

	CView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}
