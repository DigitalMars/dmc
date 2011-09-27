/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
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
#include <gl\glaux.h>


#define PIXEL_CENTER(x) ((long)(x) + 0.5)

#define GAP 10
#define ROWS 3
#define COLS 4

#define OPENGL_WIDTH 48
#define OPENGL_HEIGHT 13

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void RotateColorMask(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );
static void Viewport(GLint row, GLint column);
static void Point(void );
static void Lines(void );
static void LineStrip(void );
static void LineLoop(void );
static void Bitmap(void );
static void Triangles(void );
static void TriangleStrip(void );
static void TriangleFan(void );
static void Rect(void );
static void xPolygon(void );
static void Quads(void );
static void QuadStrip(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, windType;
GLint windW, windH;

GLenum mode1, mode2;
GLint boxW, boxH;
GLubyte OpenGL_bits[] = {
   0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
   0x7f, 0xfb, 0xff, 0xff, 0xff, 0x01,
   0x7f, 0xfb, 0xff, 0xff, 0xff, 0x01, 
   0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
   0x3e, 0x8f, 0xb7, 0xf9, 0xfc, 0x01, 
   0x63, 0xdb, 0xb0, 0x8d, 0x0d, 0x00,
   0x63, 0xdb, 0xb7, 0x8d, 0x0d, 0x00, 
   0x63, 0xdb, 0xb6, 0x8d, 0x0d, 0x00,
   0x63, 0x8f, 0xf3, 0xcc, 0x0d, 0x00, 
   0x63, 0x00, 0x00, 0x0c, 0x4c, 0x0a,
   0x63, 0x00, 0x00, 0x0c, 0x4c, 0x0e, 
   0x63, 0x00, 0x00, 0x8c, 0xed, 0x0e,
   0x3e, 0x00, 0x00, 0xf8, 0x0c, 0x00, 
};


static void Init(void)
{

    mode1 = GL_TRUE;
    mode2 = GL_TRUE;
}

static void CALLBACK Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;
}

static void RotateColorMask(void)
{
    static GLint rotation = 0;
    
    rotation = (rotation + 1) & 0x3;
    switch (rotation) {
      case 0:
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glIndexMask( 0xff );
    break;
      case 1:
    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    glIndexMask(0xFE);
    break;
      case 2:
    glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
    glIndexMask(0xFD);
    break;
      case 3:
    glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE);
    glIndexMask(0xFB);
    break;
    }
}

static void CALLBACK Key_1(void)
{
    mode1 = !mode1;
}

static void CALLBACK Key_2(void)
{
    mode2 = !mode2;
}

static void CALLBACK Key_3(void)
{
    RotateColorMask();
}

static void Viewport(GLint row, GLint column)
{
    GLint x, y;

    boxW = (windW - (COLS + 1) * GAP) / COLS;
    boxH = (windH - (ROWS + 1) * GAP) / ROWS;

    x = GAP + column * (boxW + GAP);
    y = GAP + row * (boxH + GAP);

    glViewport(x, y, boxW, boxH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-boxW/2, boxW/2, -boxH/2, boxH/2, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, boxW, boxH);
}

static void Point(void)
{
    GLint i;

    glBegin(GL_POINTS);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(0, 0);
    for (i = 1; i < 8; i++) {
        GLint j = i * 2;
        AUX_SETCOLOR(windType, i + AUX_RED - 1);
        glVertex2i(-j, -j);
        glVertex2i(-j, 0);
        glVertex2i(-j, j);
        glVertex2i(0, j);
        glVertex2i(j, j);
        glVertex2i(j, 0);
        glVertex2i(j, -j);
        glVertex2i(0, -j);
    }
    glEnd();
}

static void Lines(void)
{
    GLint i;

    glPushMatrix();

    glTranslatef(-12, 0, 0);
    for (i = 1; i < 8; i++) {
    AUX_SETCOLOR(windType, i + AUX_RED - 1);
    glBegin(GL_LINES);
        glVertex2i(-boxW/4, -boxH/4);
        glVertex2i(boxW/4, boxH/4);
    glEnd();
    glTranslatef(4, 0, 0);
    }

    glPopMatrix();

    glBegin(GL_LINES);
    glVertex2i(0, 0);
    glEnd();
}

static void LineStrip(void)
{

    glBegin(GL_LINE_STRIP);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2f(PIXEL_CENTER(-boxW/4), PIXEL_CENTER(-boxH/4));
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2f(PIXEL_CENTER(-boxW/4), PIXEL_CENTER(boxH/4));
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2f(PIXEL_CENTER(boxW/4), PIXEL_CENTER(boxH/4));
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2f(PIXEL_CENTER(boxW/4), PIXEL_CENTER(-boxH/4));
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 0);
    glEnd();
}

static void LineLoop(void)
{

    glBegin(GL_LINE_LOOP);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2f(PIXEL_CENTER(-boxW/4), PIXEL_CENTER(-boxH/4));
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2f(PIXEL_CENTER(-boxW/4), PIXEL_CENTER(boxH/4));
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2f(PIXEL_CENTER(boxW/4), PIXEL_CENTER(boxH/4));
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2f(PIXEL_CENTER(boxW/4), PIXEL_CENTER(-boxH/4));
    glEnd();

    glEnable(GL_LOGIC_OP);
    glLogicOp(GL_XOR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    AUX_SETCOLOR(windType, AUX_MAGENTA);
    glBegin(GL_LINE_LOOP);
    glVertex2f(PIXEL_CENTER(-boxW/8), PIXEL_CENTER(-boxH/8));
    glVertex2f(PIXEL_CENTER(-boxW/8), PIXEL_CENTER(boxH/8));
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(PIXEL_CENTER(-boxW/8), PIXEL_CENTER(boxH/8+5));
    glVertex2f(PIXEL_CENTER(boxW/8), PIXEL_CENTER(boxH/8+5));
    glEnd();
    glDisable(GL_LOGIC_OP);
    glDisable(GL_BLEND);

    AUX_SETCOLOR(windType, AUX_GREEN);
    glBegin(GL_POINTS);
    glVertex2i(0, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glEnd();
}

static void Bitmap(void)
{

    glBegin(GL_LINES);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/2, 0);
    glVertex2i(boxW/2, 0);
    glVertex2i(0, -boxH/2);
    glVertex2i(0, boxH/2);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(0, -3);
    glVertex2i(0, -3+OPENGL_HEIGHT);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(0, -3);
    glVertex2i(OPENGL_WIDTH, -3);
    glEnd();

    AUX_SETCOLOR(windType, AUX_GREEN);

    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glRasterPos2i(0, 0);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, 0, 3, 0.0, 0.0, OpenGL_bits);
}

static void Triangles(void)
{

    glBegin(GL_TRIANGLES);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/8, -boxH/16);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(boxW/8, -boxH/16);

    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/8, boxH/16);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(boxW/8, boxH/16);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2i(0, 0);
    glVertex2i(-100, 100);
    glEnd();
}

static void TriangleStrip(void)
{

    glBegin(GL_TRIANGLE_STRIP);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/4, boxH/4);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(0, -boxH/4);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(0, boxH/4);
    AUX_SETCOLOR(windType, AUX_CYAN);
    glVertex2i(boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_YELLOW);
    glVertex2i(boxW/4, boxH/4);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(-100, 100);
    glEnd();
}

static void TriangleFan(void)
{
    GLint vx[8][2];
    GLint x0, y0, x1, y1, x2, y2, x3, y3;
    GLint i;

    y0 = -boxH/4;
    y1 = y0 + boxH/2/3;
    y2 = y1 + boxH/2/3;
    y3 = boxH/4;
    x0 = -boxW/4;
    x1 = x0 + boxW/2/3;
    x2 = x1 + boxW/2/3;
    x3 = boxW/4;

    vx[0][0] = x0; vx[0][1] = y1;
    vx[1][0] = x0; vx[1][1] = y2;
    vx[2][0] = x1; vx[2][1] = y3;
    vx[3][0] = x2; vx[3][1] = y3;
    vx[4][0] = x3; vx[4][1] = y2;
    vx[5][0] = x3; vx[5][1] = y1;
    vx[6][0] = x2; vx[6][1] = y0;
    vx[7][0] = x1; vx[7][1] = y0;

    glBegin(GL_TRIANGLE_FAN);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(0, 0);
    for (i = 0; i < 8; i++) {
        AUX_SETCOLOR(windType, AUX_WHITE-i);
        glVertex2iv(vx[i]);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(0, 0);
    glVertex2i(-100, 100);
    glEnd();
}

static void Rect(void)
{

    AUX_SETCOLOR(windType, AUX_GREEN);
    glRecti(-boxW/4, -boxH/4, boxW/4, boxH/4);
}

static void xPolygon(void)
{
    GLint vx[8][2];
    GLint x0, y0, x1, y1, x2, y2, x3, y3;
    GLint i;

    y0 = -boxH/4;
    y1 = y0 + boxH/2/3;
    y2 = y1 + boxH/2/3;
    y3 = boxH/4;
    x0 = -boxW/4;
    x1 = x0 + boxW/2/3;
    x2 = x1 + boxW/2/3;
    x3 = boxW/4;

    vx[0][0] = x0; vx[0][1] = y1;
    vx[1][0] = x0; vx[1][1] = y2;
    vx[2][0] = x1; vx[2][1] = y3;
    vx[3][0] = x2; vx[3][1] = y3;
    vx[4][0] = x3; vx[4][1] = y2;
    vx[5][0] = x3; vx[5][1] = y1;
    vx[6][0] = x2; vx[6][1] = y0;
    vx[7][0] = x1; vx[7][1] = y0;

    glBegin(GL_POLYGON);
    for (i = 0; i < 8; i++) {
        AUX_SETCOLOR(windType, AUX_WHITE-i);
        glVertex2iv(vx[i]);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(100, 100);
    glEnd();
}

static void Quads(void)
{

    glBegin(GL_QUADS);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/8, -boxH/16);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(boxW/8, -boxH/16);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(boxW/4, -boxH/4);

    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/8, boxH/16);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(boxW/8, boxH/16);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(boxW/4, boxH/4);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(100, 100);
    glVertex2i(-100, 100);
    glEnd();
}

static void QuadStrip(void)
{

    glBegin(GL_QUAD_STRIP);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glVertex2i(-boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_RED);
    glVertex2i(-boxW/4, boxH/4);
    AUX_SETCOLOR(windType, AUX_BLUE);
    glVertex2i(0, -boxH/4);
    AUX_SETCOLOR(windType, AUX_WHITE);
    glVertex2i(0, boxH/4);
    AUX_SETCOLOR(windType, AUX_CYAN);
    glVertex2i(boxW/4, -boxH/4);
    AUX_SETCOLOR(windType, AUX_YELLOW);
    glVertex2i(boxW/4, boxH/4);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex2i(0, 0);
    glVertex2i(100, 100);
    glVertex2i(-100, 100);
    glEnd();
}

static void CALLBACK Draw(void)
{

    glViewport(0, 0, windW, windH);
    glDisable(GL_SCISSOR_TEST);

    glPushAttrib(GL_COLOR_BUFFER_BIT);

    glColorMask(1, 1, 1, 1);
    glIndexMask((GLuint)~0);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPopAttrib();

    if (mode1) {
    glShadeModel(GL_SMOOTH);
    } else {
    glShadeModel(GL_FLAT);
    }

    if (mode2) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    Viewport(0, 0); Point();
    Viewport(0, 1); Lines();
    Viewport(0, 2); LineStrip();
    Viewport(0, 3); LineLoop();

    Viewport(1, 0); Bitmap();

    Viewport(1, 1); TriangleFan();
    Viewport(1, 2); Triangles();
    Viewport(1, 3); TriangleStrip();

    Viewport(2, 0); Rect();
    Viewport(2, 1); xPolygon();
    Viewport(2, 2); Quads();
    Viewport(2, 3); QuadStrip();

    glFlush();

    if (doubleBuffer) {
    auxSwapBuffers();
    }
}

static GLenum Args(int argc, char **argv)
{
    GLint i;

    rgb = GL_TRUE;
    doubleBuffer = GL_FALSE;


    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-ci") == 0) {
        rgb = GL_FALSE;
    } else if (strcmp(argv[i], "-rgb") == 0) {
        rgb = GL_TRUE;
    } else if (strcmp(argv[i], "-sb") == 0) {
        doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
        doubleBuffer = GL_TRUE;
    } else {
        //printf("%s (Bad option).\n", argv[i]);
        return GL_FALSE;
    }
    }
    return GL_TRUE;
}

void main(int argc, char **argv)
{

    if (Args(argc, argv) == GL_FALSE) {
    auxQuit();
    }

    windW = 600;
    windH = 300;
    auxInitPosition(0, 0, windW, windH);

    windType |= (rgb) ? AUX_RGB : AUX_INDEX;
    windType |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(windType);

    if (auxInitWindow("Primitive Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_3, Key_3);
    auxMainLoop(Draw);
}
