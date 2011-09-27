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


#define CI_OFFSET_1 32
#define CI_OFFSET_2 48

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, directRender;

GLenum antiAlias, stipple;
GLubyte stippleBits[32*4] = {
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
};


static void Init(void)
{
    GLint i;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearIndex(0.0);

    if (!rgb) {
    for (i = 0; i < 16; i++) {
        auxSetOneColor(i+CI_OFFSET_1, 0.0, 0.0, i/15.0);
        auxSetOneColor(i+CI_OFFSET_2, 0.0, i/15.0, 0.0);
    }
    }

    antiAlias = GL_FALSE;
    stipple = GL_FALSE;
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -0.5, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_1(void)
{
    antiAlias = !antiAlias;
}

static void CALLBACK Key_2(void)
{
    stipple = !stipple;
}

static void CALLBACK Draw(void)
{
    GLint ci1, ci2;
    float blue[3] = { 0.0, 0.0, 1.0 };
    float green[3] = { 0.0, 1.0, 0.0 };

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (antiAlias) {
    ci1 = CI_OFFSET_1;
    ci2 = CI_OFFSET_2;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    } else {
    ci1 = AUX_BLUE;
    ci2 = AUX_GREEN;
    glDisable(GL_BLEND);
    glDisable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    }

    if (stipple) {
    glPolygonStipple(stippleBits);
    glEnable(GL_POLYGON_STIPPLE);
    } else {
    glDisable(GL_POLYGON_STIPPLE);
    }

    glBegin(GL_TRIANGLES);
    (rgb) ? glColor3fv(blue) : glIndexi(ci1);
    glVertex3f( 0.9, -0.9, -30.0);
    glVertex3f( 0.9,  0.9, -30.0);
    glVertex3f(-0.9,  0.0, -30.0);
    (rgb) ? glColor3fv(green) : glIndexi(ci2);
    glVertex3f(-0.9, -0.9, -40.0);
    glVertex3f(-0.9,  0.9, -40.0);
    glVertex3f( 0.9,  0.0, -25.0);
    glEnd();

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
    directRender = GL_FALSE;

    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-ci") == 0) {
        rgb = GL_FALSE;
    } else if (strcmp(argv[i], "-rgb") == 0) {
        rgb = GL_TRUE;
    } else if (strcmp(argv[i], "-sb") == 0) {
        doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
        doubleBuffer = GL_TRUE;
    } else if (strcmp(argv[i], "-dr") == 0) {
        directRender = GL_TRUE;
    } else if (strcmp(argv[i], "-ir") == 0) {
        directRender = GL_FALSE;
    } else {
        //printf("%s (Bad option).\n", argv[i]);
        return GL_FALSE;
    }
    }
    return GL_TRUE;
}

void main(int argc, char **argv)
{
    GLenum type;

    if (Args(argc, argv) == GL_FALSE) {
    auxQuit();
    }

    auxInitPosition(0, 0, 300, 300);

    type = AUX_DEPTH16;
    type |= (rgb) ? AUX_RGB : AUX_INDEX;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;
    auxInitDisplayMode(type);

    if (auxInitWindow("Depth Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxMainLoop(Draw);
}
