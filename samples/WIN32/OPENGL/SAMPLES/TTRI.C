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


#define SOLID 1
#define LINE 2
#define POINT 3

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_Z(void );
static void CALLBACK Key_z(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );
static void CALLBACK Key_4(void );
static void CALLBACK Key_5(void );
static void CALLBACK Key_6(void );
static void CALLBACK Key_7(void );
static void CALLBACK Key_8(void );
static void CALLBACK Key_9(void );
static void CALLBACK Key_v(void );
static void CALLBACK Key_s(void );
static void CALLBACK Key_h(void );
static void CALLBACK Key_o(void );
static void CALLBACK Key_m(void );
static void CALLBACK Key_0(void );
static void BeginPrim(void );
static void EndPrim(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, windType;
GLint windW, windH;

GLenum dithering = GL_TRUE;
GLenum showVerticies = GL_TRUE;
GLenum hideBottomTriangle = GL_FALSE;
GLenum outline = GL_TRUE;
GLenum culling = GL_FALSE;
GLenum winding = GL_FALSE;
GLenum face = GL_FALSE;
GLenum state = SOLID;
GLenum aaMode = GL_FALSE;
GLenum shade = GL_TRUE;

GLint color1, color2, color3;

float zRotation = 90.0;
float zoom = 1.0;

float boxA[3] = {-100, -100, 0};
float boxB[3] = { 100, -100, 0};
float boxC[3] = { 100,  100, 0};
float boxD[3] = {-100,  100, 0};

float p0[3] = {-125,-80, 0};
float p1[3] = {-125, 80, 0};
float p2[3] = { 172,  0, 0};


static void Init(void)
{
    float r, g, b;
    float percent1, percent2;
    GLint i, j;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glLineStipple(1, 0xF0F0);

    glEnable(GL_SCISSOR_TEST);

    if (!rgb) {
    for (j = 0; j <= 12; j++) {
        if (j <= 6) {
        percent1 = j / 6.0;
        r = 1.0 - 0.8 * percent1;
        g = 0.2 + 0.8 * percent1;
        b = 0.2;
        } else {
        percent1 = (j - 6) / 6.0;
        r = 0.2;
        g = 1.0 - 0.8 * percent1;
        b = 0.2 + 0.8 * percent1;
        }
        auxSetOneColor(j+18, r, g, b);
        for (i = 0; i < 16; i++) {
        percent2 = i / 15.0;
        auxSetOneColor(j*16+1+32, r*percent2, g*percent2, b*percent2);
        }
    }
    color1 = 18;
    color2 = 24;
    color3 = 30;
    }
}

static void CALLBACK Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;
}

static void CALLBACK Key_LEFT(void)
{
    zRotation += 0.5;
}

static void CALLBACK Key_RIGHT(void)
{
    zRotation -= 0.5;
}

static void CALLBACK Key_Z(void)
{
    zoom *= 0.75;
}

static void CALLBACK Key_z(void)
{
    zoom /= 0.75;
    if (zoom > 10) {
        zoom = 10;
    }
}

static void CALLBACK Key_1(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

static void CALLBACK Key_2(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void CALLBACK Key_3(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void CALLBACK Key_4(void)
{
    state = POINT;
}

static void CALLBACK Key_5(void)
{
    state = LINE;
}

static void CALLBACK Key_6(void)
{
    state = SOLID;
}

static void CALLBACK Key_7(void)
{
    culling = !culling;
}

static void CALLBACK Key_8(void)
{
    winding = !winding;
}

static void CALLBACK Key_9(void)
{
    face = !face;
}

static void CALLBACK Key_v(void)
{
    showVerticies = !showVerticies;
}

static void CALLBACK Key_s(void)
{
    shade = !shade;
    (shade) ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);
}

static void CALLBACK Key_h(void)
{
    hideBottomTriangle = !hideBottomTriangle;
}

static void CALLBACK Key_o(void)
{
    outline = !outline;
}

static void CALLBACK Key_m(void)
{
    dithering = !dithering;
}

static void CALLBACK Key_0(void)
{
    aaMode = !aaMode;
    if (aaMode) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        if (!rgb) {
        color1 = 32;
        color2 = 128;
        color3 = 224;
        }
    } else {
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_BLEND);
        if (!rgb) {
        color1 = 18;
        color2 = 24;
        color3 = 30;
        }
    }
}

static void BeginPrim(void)
{

    switch (state) {
      case SOLID:
    glBegin(GL_POLYGON);
    break;
      case LINE:
    glBegin(GL_LINE_LOOP);
    break;
      case POINT:
    glBegin(GL_POINTS);
    break;
    }
}

static void EndPrim(void)
{

    glEnd();
}

static void CALLBACK Draw(void)
{
    float scaleX, scaleY;

    glViewport(0, 0, windW, windH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-175, 175, -175, 175);
    glMatrixMode(GL_MODELVIEW);

    glScissor(0, 0, windW, windH);

    (culling) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    (winding) ? glFrontFace(GL_CCW) : glFrontFace(GL_CW);
    (face) ? glCullFace(GL_FRONT) : glCullFace(GL_BACK);

    (dithering) ? glEnable(GL_DITHER) : glDisable(GL_DITHER);

    glClear(GL_COLOR_BUFFER_BIT);

    AUX_SETCOLOR(windType, AUX_GREEN);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(boxA);
    glVertex3fv(boxB);
    glVertex3fv(boxC);
    glVertex3fv(boxD);
    glEnd();

    if (!hideBottomTriangle) {
    glPushMatrix();

    glScalef(zoom, zoom, zoom);
    glRotatef(zRotation, 0, 0, 1);

    AUX_SETCOLOR(windType, AUX_BLUE);
    BeginPrim();
        glVertex3fv(p0);
        glVertex3fv(p1);
        glVertex3fv(p2);
    EndPrim();

    if (showVerticies) {
        (rgb) ? glColor3fv(auxRGBMap[AUX_RED]) : glIndexf(color1);
        glRectf(p0[0]-2, p0[1]-2, p0[0]+2, p0[1]+2);
        (rgb) ? glColor3fv(auxRGBMap[AUX_GREEN]) : glIndexf(color2);
        glRectf(p1[0]-2, p1[1]-2, p1[0]+2, p1[1]+2);
        (rgb) ? glColor3fv(auxRGBMap[AUX_BLUE]) : glIndexf(color3);
        glRectf(p2[0]-2, p2[1]-2, p2[0]+2, p2[1]+2);
    }

    glPopMatrix();
    }

    scaleX = (float)(windW - 20) / 2 / 175 * (175 - 100) + 10;
    scaleY = (float)(windH - 20) / 2 / 175 * (175 - 100) + 10;

    glViewport(scaleX, scaleY, windW-2*scaleX, windH-2*scaleY);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);

    glScissor(scaleX, scaleY, windW-2*scaleX, windH-2*scaleY);

    glPushMatrix();

    glScalef(zoom, zoom, zoom);
    glRotatef(zRotation, 0,0,1);

    glPointSize(10);
    glLineWidth(5);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    AUX_SETCOLOR(windType, AUX_RED);
    BeginPrim();
    (rgb) ? glColor3fv(auxRGBMap[AUX_RED]) : glIndexf(color1);
    glVertex3fv(p0);
    (rgb) ? glColor3fv(auxRGBMap[AUX_GREEN]) : glIndexf(color2);
    glVertex3fv(p1);
    (rgb) ? glColor3fv(auxRGBMap[AUX_BLUE]) : glIndexf(color3);
    glVertex3fv(p2);
    EndPrim();

    glPointSize(1);
    glLineWidth(1);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glBlendFunc(GL_ONE, GL_ZERO);

    if (outline) {
    AUX_SETCOLOR(windType, AUX_WHITE);
    glBegin(GL_LINE_LOOP);
        glVertex3fv(p0);
        glVertex3fv(p1);
        glVertex3fv(p2);
    glEnd();
    }

    glPopMatrix();

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

    if (auxInitWindow("Triangle Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_Z, Key_Z);
    auxKeyFunc(AUX_z, Key_z);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_3, Key_3);
    auxKeyFunc(AUX_4, Key_4);
    auxKeyFunc(AUX_5, Key_5);
    auxKeyFunc(AUX_6, Key_6);
    auxKeyFunc(AUX_7, Key_7);
    auxKeyFunc(AUX_8, Key_8);
    auxKeyFunc(AUX_9, Key_9);
    auxKeyFunc(AUX_v, Key_v);
    auxKeyFunc(AUX_s, Key_s);
    auxKeyFunc(AUX_h, Key_h);
    auxKeyFunc(AUX_o, Key_o);
    auxKeyFunc(AUX_m, Key_m);
    auxKeyFunc(AUX_0, Key_0);
    auxMainLoop(Draw);
}
