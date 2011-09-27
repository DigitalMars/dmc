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

#define CI_OFFSET 16

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_W(void );
static void CALLBACK Key_w(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, windType;

GLenum mode1, mode2;
GLint size;
float pntA[3] = {
    -160.0, 0.0, 0.0
};
float pntB[3] = {
    -130.0, 0.0, 0.0
};
float pntC[3] = {
    -40.0, -50.0, 0.0
};
float pntD[3] = {
    30.0, 60.0, 0.0
};


static void Init(void)
{
    GLint i;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glLineStipple(1, 0xF0E0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    if (!rgb) {
    for (i = 0; i < 16; i++) {
        auxSetOneColor(i+CI_OFFSET, i/15.0, i/15.0, 0.0);
    }
    }

    mode1 = GL_FALSE;
    mode2 = GL_FALSE;
    size = 1;
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-175, 175, -175, 175);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_1(void)
{
    mode1 = !mode1;
}

static void CALLBACK Key_2(void)
{
    mode2 = !mode2;
}

static void CALLBACK Key_W(void)
{
    size++;
}

static void CALLBACK Key_w(void)
{
    size--;
}

static void CALLBACK Draw(void)
{
    GLint ci, i;

    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(size);

    if (mode1) {
    glEnable(GL_LINE_STIPPLE);
    } else {
    glDisable(GL_LINE_STIPPLE);
    }
    
    if (mode2) {
    ci = CI_OFFSET;
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    } else {
    ci = AUX_YELLOW;
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    }

    glPushMatrix();

    for (i = 0; i < 360; i += 5) {
    glRotatef(5.0, 0,0,1);

    (rgb) ? glColor3f(1.0, 1.0, 0.0) : glIndexi(ci);
    glBegin(GL_LINE_STRIP);
        glVertex3fv(pntA);
        glVertex3fv(pntB);
    glEnd();

    glPointSize(1);

    AUX_SETCOLOR(windType, AUX_GREEN);
    glBegin(GL_POINTS);
        glVertex3fv(pntA);
        glVertex3fv(pntB);
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

    auxInitPosition(0, 0, 300, 300);

    windType |= (rgb) ? AUX_RGB : AUX_INDEX;
    windType |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(windType);

    if (auxInitWindow("Line Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_W, Key_W);
    auxKeyFunc(AUX_w, Key_w);
    auxMainLoop(Draw);
}
