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


#define CI_RED AUX_RED
#define CI_ANTI_ALIAS_GREEN 16
#define CI_ANTI_ALIAS_YELLOW 32
#define CI_ANTI_ALIAS_RED 48

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_1(void );
static void CALLBACK Key_W(void );
static void CALLBACK Key_w(void );
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_UP(void );
static void CALLBACK Key_DOWN(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, windType;
GLint windW, windH;

GLenum mode;
GLint size;
float point[3] = {
    1.0, 1.0, 0.0
};


static void Init(void)
{
    GLint i;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    if (!rgb) {
    for (i = 0; i < 16; i++) {
        auxSetOneColor(i+CI_ANTI_ALIAS_RED, i/15.0, 0.0, 0.0);
        auxSetOneColor(i+CI_ANTI_ALIAS_YELLOW, i/15.0, i/15.0, 0.0);
        auxSetOneColor(i+CI_ANTI_ALIAS_GREEN, 0.0, i/15.0, 0.0);
    }
    }

    mode = GL_FALSE;
    size = 1;
}

static void CALLBACK Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windW/2, windW/2, -windH/2, windH/2);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_1(void)
{
    mode = !mode;
}

static void CALLBACK Key_W(void)
{
    size++;
}

static void CALLBACK Key_w(void)
{
    size--;
    if (size < 1) {
        size = 1;
    }
}

static void CALLBACK Key_LEFT(void)
{
    point[0] -= 0.25;
}

static void CALLBACK Key_RIGHT(void)
{
    point[0] += 0.25;
}

static void CALLBACK Key_UP(void)
{
    point[1] += 0.25;
}

static void CALLBACK Key_DOWN(void)
{
    point[1] -= 0.25;
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    AUX_SETCOLOR(windType, AUX_YELLOW);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-windW/2, 0);
    glVertex2f(windW/2, 0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, -windH/2);
    glVertex2f(0, windH/2);
    glEnd();

    if (mode) {
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    } else {
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    }

    glPointSize(size);
    if (mode) {
    (rgb) ? glColor3f(1.0, 0.0, 0.0) : glIndexf(CI_ANTI_ALIAS_RED);
    } else {
    (rgb) ? glColor3f(1.0, 0.0, 0.0) : glIndexf(CI_RED);
    }
    glBegin(GL_POINTS);
    glVertex3fv(point);
    glEnd();

    glDisable(GL_POINT_SMOOTH);

    glPointSize(1);
    AUX_SETCOLOR(windType, AUX_GREEN);
    glBegin(GL_POINTS);
    glVertex3fv(point);
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

    windW = 300;
    windH = 300;
    auxInitPosition(0, 0, windW, windH);

    windType |= (rgb) ? AUX_RGB : AUX_INDEX;
    windType |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(windType);

    if (auxInitWindow("Point Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_W, Key_W);
    auxKeyFunc(AUX_w, Key_w);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxMainLoop(Draw);
}
