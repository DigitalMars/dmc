#include "gengl.h"

HPALETTE ghpalOld, ghPalette = (HPALETTE) 0;

BOOL draw_cone = FALSE;
BOOL draw_pyramid = FALSE;

GLvoid drawCone(GLvoid);
GLvoid drawPyramid(GLvoid);

GLfloat radius;
RECT	oldrect;

unsigned char threeto8[8] = {
    0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] = {
    0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] = {
    0, 255
};

static int defaultOverride[13] = {
    0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = {
    { 0,   0,   0,    0 },
    { 0x80,0,   0,    0 },
    { 0,   0x80,0,    0 },
    { 0x80,0x80,0,    0 },
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 },

    { 192, 220, 192,  0 },
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 },

    { 0x80,0x80,0x80, 0 },
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }
};

unsigned char
ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = (unsigned char) (i >> shift);
    switch (nbits) {

    case 1:
        val &= 0x1;
        return oneto8[val];

    case 2:
        val &= 0x3;
        return twoto8[val];

    case 3:
        val &= 0x7;
        return threeto8[val];

    default:
        return 0;
    }
}

void
CreateRGBPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE *pPal;
    int n, i;

    n = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        n = 1 << pfd.cColorBits;
        pPal = (PLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) +
                n * sizeof(PALETTEENTRY));
        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;
        for (i=0; i<n; i++) {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        /* fix up the palette to include the default GDI palette */
        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
           ) {
            for (i = 1 ; i <= 12 ; i++)
                pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
        }

        ghPalette = CreatePalette(pPal);
        LocalFree(pPal);

        ghpalOld = SelectPalette(hDC, ghPalette, FALSE);
        n = RealizePalette(hDC);
    }
}

BOOL bSetupPixelFormat(HDC hDC)
{
    static PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd
	1,				// version number
	PFD_DRAW_TO_WINDOW |		// support window
	  PFD_SUPPORT_OPENGL |		// support OpenGL
	  PFD_DOUBLEBUFFER,		// double buffered
	PFD_TYPE_RGBA,			// RGBA type
	24,				// 24-bit color depth
	0, 0, 0, 0, 0, 0,		// color bits ignored
	0,				// no alpha buffer
	0,				// shift bit ignored
	0,				// no accumulation buffer
	0, 0, 0, 0, 			// accum bits ignored
	32,				// 32-bit z-buffer	
	0,				// no stencil buffer
	0,				// no auxiliary buffer
	PFD_MAIN_PLANE,			// main layer
	0,				// reserved
	0, 0, 0				// layer masks ignored
    };
    int pixelformat;

    if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 )
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    CreateRGBPalette(hDC);

    return TRUE;
}

GLvoid initialize(HWND hWnd)
{
    GLfloat	maxObjectSize, aspect;
    GLdouble	near_plane, far_plane;

    GetClientRect(hWnd, &oldrect);

    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClearDepth( 1.0 );

    glEnable(GL_DEPTH_TEST);

    glMatrixMode( GL_PROJECTION );
    aspect = (GLfloat) oldrect.right / oldrect.bottom;;
    gluPerspective( 45.0, aspect, 3.0, 7.0 );
    glMatrixMode( GL_MODELVIEW );

    near_plane = 3.0;
    far_plane = 7.0;
    maxObjectSize = 3.0;
    radius = near_plane + maxObjectSize/2.0;
}

GLvoid draw_scene(HWND hWnd)
{
    HDC hDC;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    	glTranslatef(0.0, 0.0, -radius);

	if(draw_cone)
	    drawCone();

	if(draw_pyramid)
	    drawPyramid();

    glPopMatrix();
    glFinish();

    hDC = wglGetCurrentDC();
    SwapBuffers(hDC);
}

GLvoid resize(HWND hWnd)
{
    RECT	rect;

    GetClientRect(hWnd, &rect);

    glViewport(0, 0, rect.right, rect.bottom);

    if((oldrect.right > rect.right) || (oldrect.bottom > rect.bottom))
	PostMessage (hWnd, WM_PAINT, 0, 0L);

    oldrect.right = rect.right;
    oldrect.bottom = rect.bottom;
}

GLvoid drawCone(GLvoid)
{
    GLUquadricObj *quadObj;

    glColor3f(0.0, 1.0, 0.0);

    glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
        quadObj = gluNewQuadric();
        gluQuadricDrawStyle(quadObj, GLU_FILL);
        gluQuadricNormals(quadObj, GLU_SMOOTH);
        gluCylinder(quadObj, 1.0, 0.0, 1.0, 20, 10);
        gluDeleteQuadric(quadObj);
    glPopMatrix();

}

GLvoid drawPyramid(GLvoid)
{

    glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	    glColor3f(1.0, 0.0, 0.0);
	    glVertex3f(0.0, 1.0, 0.0);
	    glColor3f(0.0, 1.0, 0.0);
	    glVertex3f(-1.0, 0.0, 0.0);
	    glColor3f(0.0, 0.0, 1.0);
	    glVertex3f(0.0, 0.0, 1.0);
	    glColor3f(0.0, 1.0, 0.0);
	    glVertex3f(1.0, 0.0, 0.0);
	glEnd();
    glPopMatrix();
}
