/******************************Module*Header*******************************\
* Module Name: toffset.c
*
* Program demonstrating the Polygon Offset call. 
*
* Draws a quadric, first normally and then with polygon offset.
*
* Copyright (c) 1994-1996 Microsoft Corporation
*
\**************************************************************************/

/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993, 1994 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

#include "glos.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gl\glaux.h>
#include "trackbal.h"


#define PI 3.141592654
#define BLACK 0
#define GRAY 128
#define WHITE 255
#define RD 0xA40000FF
#define WT 0xFFFFFFFF
#define brickImageWidth 16
#define brickImageHeight 16

//static void CALLBACK ErrorHandler(unsigned long which);
static void Init(void );

static void CALLBACK Key_Left(void );
static void CALLBACK Key_Right(void );
static void CALLBACK Key_Up(void );
static void CALLBACK Key_Down(void );
static void CALLBACK Key_a(void );
static void CALLBACK Key_A(void );
static void CALLBACK Key_b(void );
static void CALLBACK Key_c(void );
static void CALLBACK Key_f(void );
static void CALLBACK Key_g(void );
static void CALLBACK Key_G(void );
static void CALLBACK Key_h(void );
static void CALLBACK Key_H(void );
static void CALLBACK Key_i(void );
static void CALLBACK Key_j(void );
static void CALLBACK Key_J(void );
static void CALLBACK Key_l(void );
static void CALLBACK Key_L(void );
static void CALLBACK Key_k(void );
static void CALLBACK Key_K(void );
static void CALLBACK Key_o(void );
static void CALLBACK Key_p(void );
static void CALLBACK Key_s(void );
static void CALLBACK Key_S(void );
static void CALLBACK Key_t(void );
static void CALLBACK Key_z(void );
static void CALLBACK Key_Z(void );
static void CALLBACK Key_0(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );

static void CALLBACK Reshape(int width,int height);
static void CALLBACK Draw( void ); 
static unsigned long Args(int argc,char **argv );

GLenum rgb, doubleBuffer;

GLint wWidth = 300, wHeight = 300;

GLenum doDither = GL_TRUE;
GLenum shade = GL_TRUE;
GLenum texture = GL_TRUE;

BOOL bPolyOffset = TRUE;


float factor=-1.0f, units=0.0f;

float inc = 1.0f;
int polygonMode = GL_LINE;
GLenum polyFace = GL_FRONT;
float zTrans = -2.7;
BOOL bCullFace = FALSE;
BOOL bHiddenLine;

GLint radius1, radius2;
GLdouble angle1, angle2;
GLint slices, stacks;
GLint height;
GLint whichQuadric;
GLUquadricObj *quadObj;

GLubyte brickImage[brickImageWidth*brickImageHeight] = {
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD
};
char *texFileName = 0;


static void UpdateInfo()
{
    HWND hwnd = auxGetHWND();
    char buf[100];

    sprintf( buf, "Factor = %4.1f, Units = %4.1f", factor, units );
    SendMessage( hwnd, WM_SETTEXT, 0, (LPARAM)buf );
}

static void SetMaterial( bBlack )
{
    static float front_mat_shininess[] = {30.0};
    static float front_mat_specular[] = {0.2, 0.2, 0.2, 1.0};
    static float front_mat_diffuse[] = {0.5, 0.28, 0.38, 1.0};
    static float back_mat_shininess[] = {50.0};
    static float back_mat_specular[] = {0.5, 0.5, 0.2, 1.0};
    static float back_mat_diffuse[] = {1.0, 1.0, 0.2, 1.0};
    static float black_mat_shininess[] = {0.0};
    static float black_mat_specular[] = {0.0, 0.0, 0.0, 0.0};
    static float black_mat_diffuse[] = {0.0, 0.0, 0.0, 0.0};
    static float ambient[] = {0.1, 0.1, 0.1, 1.0};
    static float no_ambient[] = {0.0, 0.0, 0.0, 0.0};
    static float lmodel_ambient[] = {1.0, 1.0, 1.0, 1.0};
    static float lmodel_no_ambient[] = {0.0, 0.0, 0.0, 0.0};

    if( !bBlack ) {
        glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
        glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
        glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
        glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
        glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    } else {
        glMaterialfv(GL_FRONT, GL_SHININESS, black_mat_shininess);
        glMaterialfv(GL_FRONT, GL_SPECULAR, black_mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, black_mat_diffuse);
        glMaterialfv(GL_BACK, GL_SHININESS, black_mat_shininess);
        glMaterialfv(GL_BACK, GL_SPECULAR, black_mat_specular);
        glMaterialfv(GL_BACK, GL_DIFFUSE, black_mat_diffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, no_ambient);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_no_ambient);
    }
}

static void Init(void)
{
    static GLint colorIndexes[3] = {0, 200, 255};
    static float ambient[] = {0.1, 0.1, 0.1, 1.0};
    static float diffuse[] = {0.5, 1.0, 1.0, 1.0};
    static float position[] = {90.0, 90.0, 150.0, 0.0};
    static float lmodel_ambient[] = {1.0, 1.0, 1.0, 1.0};
    static float lmodel_twoside[] = {GL_TRUE};
    static float decal[] = {GL_DECAL};
    static float modulate[] = {GL_MODULATE};
    static float repeat[] = {GL_REPEAT};
    static float nearest[] = {GL_NEAREST};
    AUX_RGBImageRec *image;

    if (!rgb) {
        auxSetGreyRamp();
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    bHiddenLine = FALSE;
    SetMaterial( bHiddenLine );

    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest);
    if (texFileName) {
        image = auxRGBImageLoad(texFileName);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
              GL_RGB, GL_UNSIGNED_BYTE, image->data);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, brickImageWidth, brickImageHeight,
             0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)brickImage);
    }

    quadObj = gluNewQuadric();

    radius1 = 10;
    radius2 = 5;
    angle1 = 90;
    angle2 = 180;
    slices = 16;
    stacks = 10;
    height = 20;

    glCullFace( GL_BACK );

    UpdateInfo();
}


static void SetDistance( void )
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);
    // This defines how far away we're looking from
    glTranslated( 0, 0, zTrans );
}

static void CALLBACK Reshape(int width, int height)
{
    trackball_Resize( width, height );

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);
    // This defines how far away we're looking from
    glTranslated( 0, 0, zTrans );
}


static void CALLBACK Key_Left (void)
{
    units -= inc;
}


static void CALLBACK Key_Right (void)
{
    units += inc;
}


static void CALLBACK Key_Up (void)
{
    factor += inc;
}


static void CALLBACK Key_Down (void)
{
    factor -= inc;
}


static void CALLBACK Key_a (void)
{
    if (stacks > 1)
        stacks--;
}


static void CALLBACK Key_A (void)
{
    stacks++;
}


static void CALLBACK Key_b (void)
{
    bHiddenLine = !bHiddenLine;
    SetMaterial( bHiddenLine );
}


static void CALLBACK Key_c (void)
{
    bCullFace = !bCullFace;
    if( bCullFace )
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );
}


static void CALLBACK Key_f (void)
{
	whichQuadric = whichQuadric >= 3 ? 0 : whichQuadric + 1;
}


static void CALLBACK Key_g (void)
{
    if (radius1 > 0)
        radius1 -= 1;
}


static void CALLBACK Key_G (void)
{
    radius1 += 1;
}


static void CALLBACK Key_h (void)
{
    if (height > 0)
        height -= 2;
}


static void CALLBACK Key_H (void)
{
    height += 2;
}


static void CALLBACK Key_i (void)
{
    factor = -1.0f;
    units = 0.0f;
}


static void CALLBACK Key_j (void)
{
    if (radius2 > 0)
        radius2 -= 1;
}


static void CALLBACK Key_J (void)
{
    radius2 += 1;
}


static void CALLBACK Key_k (void)
{
	angle1 -= 5;
}


static void CALLBACK Key_K (void)
{
	angle1 += 5;
}


static void CALLBACK Key_l (void)
{
	angle2 -= 5;
}


static void CALLBACK Key_L (void)
{
	angle2 += 5;
}


static void CALLBACK Key_o (void)
{
    bPolyOffset = !bPolyOffset;
}


static void CALLBACK Key_p (void)
{
	switch (polyFace) {
	  case GL_BACK:
	    polyFace = GL_FRONT;
	    break;
	  case GL_FRONT:
	    polyFace = GL_FRONT_AND_BACK;
	    break;
	  case GL_FRONT_AND_BACK:
	    polyFace = GL_BACK;
	    break;
	}
}


static void CALLBACK Key_s (void)
{
    if (slices > 3)
        slices--;
}


static void CALLBACK Key_S (void)
{
    slices++;
}


static void CALLBACK Key_t (void)
{
    texture = !texture;
    if (texture) {
        gluQuadricTexture(quadObj, GL_TRUE);
        glEnable(GL_TEXTURE_2D);
    } else {
        gluQuadricTexture(quadObj, GL_FALSE);
        glDisable(GL_TEXTURE_2D);
    }
}


static void CALLBACK Key_z (void)
{
    zTrans += 0.1f;
    SetDistance();
}


static void CALLBACK Key_Z (void)
{
    zTrans -= 0.1f;
    SetDistance();
}


static void CALLBACK Key_0 (void)
{
    shade = !shade;
    if (shade) {
        glShadeModel(GL_SMOOTH);
        gluQuadricNormals(quadObj, GLU_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
        gluQuadricNormals(quadObj, GLU_FLAT);
    }
}


static void CALLBACK Key_1 (void)
{
    polygonMode = GL_FILL;
}


static void CALLBACK Key_2 (void)
{
    polygonMode = GL_LINE;
}


static void CALLBACK Key_3 (void)
{
    polygonMode = GL_POINT;
}


static void DrawObject( void ) 
{
    switch (whichQuadric) {
      case 0:
	gluCylinder(quadObj, radius1/10.0, radius2/10.0, height/10.0, 
		    slices, stacks);
	break;
      case 1:
	gluSphere(quadObj, radius1/10.0, slices, stacks);
	break;
      case 2:
	gluPartialDisk(quadObj, radius2/10.0, radius1/10.0, slices, 
		       stacks, angle1, angle2);
	break;
      case 3:
	gluDisk(quadObj, radius2/10.0, radius1/10.0, slices, stacks);
	break;
    }
}

static void CALLBACK Draw( void ) 
{
    float matRot[4][4];

    UpdateInfo();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    trackball_CalcRotMatrix( matRot );
    glMultMatrixf( &(matRot[0][0]) );

    if( whichQuadric == 0 ) // cylinder
	glTranslatef(0, 0, -height/20.0);

    // Draw object normally

    DrawObject();

    // Draw object again with polygon offset

    // Set polygon mode for offset faces
    glPolygonMode( GL_FRONT_AND_BACK, polygonMode );

    if( bPolyOffset ) {
        switch( polygonMode ) {
            case GL_FILL:
                glEnable( GL_POLYGON_OFFSET_FILL );
                break;
            case GL_LINE:
                glEnable( GL_POLYGON_OFFSET_LINE );
                break;
            case GL_POINT:
                glEnable( GL_POLYGON_OFFSET_POINT );
                break;
        }
    }

    glPolygonOffset( factor, units );
    glColor3f(1.0, 0.0, 0.0);
    glDisable( GL_LIGHTING );
    DrawObject();
    glEnable( GL_LIGHTING );
    // restore modes
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    if( bPolyOffset ) {
        glDisable( GL_POLYGON_OFFSET_FILL );
        glDisable( GL_POLYGON_OFFSET_LINE );
        glDisable( GL_POLYGON_OFFSET_POINT );
    }

    glFlush();

    if (doubleBuffer) {
	auxSwapBuffers();
    }
}

static unsigned long Args(int argc, char **argv)
{
    GLint i;

    rgb = GL_TRUE;
    doubleBuffer = GL_TRUE;


    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ci") == 0) {
            rgb = GL_FALSE;
        } else if (strcmp(argv[i], "-rgb") == 0) {
            rgb = GL_TRUE;
        } else if (strcmp(argv[i], "-sb") == 0) {
            doubleBuffer = GL_FALSE;
        } else if (strcmp(argv[i], "-db") == 0) {
            doubleBuffer = GL_TRUE;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i+1 >= argc || argv[i+1][0] == '-') {
                //printf("-f (No file name).\n");
                return GL_FALSE;
            } else {
                texFileName = argv[++i];
            }
        } else {
            //printf("%s (Bad option).\n", argv[i]);
            return GL_FALSE;
        }
    }
    return GL_TRUE;
}

void  _CRTAPI1 main(int argc, char **argv)
{
    GLenum type;

    if (Args(argc, argv) == GL_FALSE) {
       auxQuit();
    }

    auxInitPosition(0, 0, wWidth, wHeight);

    type = AUX_DEPTH16;
    type |= (rgb) ? AUX_RGB : AUX_INDEX;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(type);

    if (auxInitWindow("Polygon Offset Demo") == GL_FALSE) {
        auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);

    auxKeyFunc( AUX_LEFT, Key_Left );
    auxKeyFunc( AUX_RIGHT, Key_Right );
    auxKeyFunc( AUX_UP, Key_Up );
    auxKeyFunc( AUX_DOWN, Key_Down );
    auxKeyFunc( AUX_a, Key_a );
    auxKeyFunc( AUX_A, Key_A );
    auxKeyFunc( AUX_b, Key_b );
    auxKeyFunc( AUX_c, Key_c );
    auxKeyFunc( AUX_f, Key_f );
    auxKeyFunc( AUX_g, Key_g );
    auxKeyFunc( AUX_G, Key_G );
    auxKeyFunc( AUX_h, Key_h );
    auxKeyFunc( AUX_H, Key_H );
    auxKeyFunc( AUX_i, Key_i );
    auxKeyFunc( AUX_j, Key_j );
    auxKeyFunc( AUX_J, Key_J );
    auxKeyFunc( AUX_k, Key_k );
    auxKeyFunc( AUX_K, Key_K );
    auxKeyFunc( AUX_l, Key_l );
    auxKeyFunc( AUX_L, Key_L );
    auxKeyFunc( AUX_o, Key_o );
    auxKeyFunc( AUX_p, Key_p );
    auxKeyFunc( AUX_s, Key_s );
    auxKeyFunc( AUX_S, Key_S );
    auxKeyFunc( AUX_t, Key_t );
    auxKeyFunc( AUX_z, Key_z );
    auxKeyFunc( AUX_Z, Key_Z );
    auxKeyFunc( AUX_0, Key_0 );
    auxKeyFunc( AUX_1, Key_1 );
    auxKeyFunc( AUX_2, Key_2 );
    auxKeyFunc( AUX_3, Key_3 );

    auxMouseFunc( AUX_LEFTBUTTON, AUX_MOUSEDOWN, trackball_MouseDown );
    auxMouseFunc( AUX_LEFTBUTTON, AUX_MOUSEUP, trackball_MouseUp );
    
    trackball_Init( wWidth, wHeight );

    auxIdleFunc( Draw );
    auxMainLoop( Draw );
}
