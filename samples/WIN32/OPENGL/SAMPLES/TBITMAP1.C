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

#define OPENGL_WIDTH 24
#define OPENGL_HEIGHT 13

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, windType;

float boxA[3] = {
    0, 0, 0
};
float boxB[3] = {
    -100, 0, 0
};
float boxC[3] = {
    100, 0, 0
};
float boxD[3] = {
    0, 95, 0
};
float boxE[3] = {
    0, -105, 0
};
GLubyte OpenGL_bits1[] = {
   0x00, 0x03, 0x00,
   0x7f, 0xfb, 0xff,
   0x7f, 0xfb, 0xff,
   0x00, 0x03, 0x00,
   0x3e, 0x8f, 0xb7,
   0x63, 0xdb, 0xb0,
   0x63, 0xdb, 0xb7,
   0x63, 0xdb, 0xb6,
   0x63, 0x8f, 0xf3,
   0x63, 0x00, 0x00,
   0x63, 0x00, 0x00,
   0x63, 0x00, 0x00,
   0x3e, 0x00, 0x00,
};
GLubyte OpenGL_bits2[] = {
   0x00, 0x00, 0x00,
   0xff, 0xff, 0x01,
   0xff, 0xff, 0x01, 
   0x00, 0x00, 0x00,
   0xf9, 0xfc, 0x01, 
   0x8d, 0x0d, 0x00,
   0x8d, 0x0d, 0x00, 
   0x8d, 0x0d, 0x00,
   0xcc, 0x0d, 0x00, 
   0x0c, 0x4c, 0x0a,
   0x0c, 0x4c, 0x0e, 
   0x8c, 0xed, 0x0e,
   0xf8, 0x0c, 0x00, 
};
GLubyte logo_bits[] = {
   0x00, 0x66, 0x66, 
   0xff, 0x66, 0x66, 
   0x00, 0x00, 0x00, 
   0xff, 0x3c, 0x3c, 
   0x00, 0x42, 0x40, 
   0xff, 0x42, 0x40, 
   0x00, 0x41, 0x40, 
   0xff, 0x21, 0x20, 
   0x00, 0x2f, 0x20, 
   0xff, 0x20, 0x20, 
   0x00, 0x10, 0x90, 
   0xff, 0x10, 0x90, 
   0x00, 0x0f, 0x10, 
   0xff, 0x00, 0x00, 
   0x00, 0x66, 0x66, 
   0xff, 0x66, 0x66, 
};


static void Init(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearIndex(0.0);
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-175, 175, -175, 175);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Draw(void)
{
    float mapI[2], mapIA[2], mapIR[2];

    glClear(GL_COLOR_BUFFER_BIT);

    mapI[0] = 0.0;
    mapI[1] = 1.0;
    mapIR[0] = 0.0;
    mapIR[1] = 0.0;
    mapIA[0] = 1.0;
    mapIA[1] = 1.0;
    
    glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, mapIR);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, mapI);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, mapI);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, mapIA);
    glPixelTransferi(GL_MAP_COLOR, GL_TRUE);
    
    AUX_SETCOLOR(windType, AUX_WHITE);
    glRasterPos3fv(boxA);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 24);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 8);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 2);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(16, 12, 8.0, 0.0, 0.0, 0.0, logo_bits);
         
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    AUX_SETCOLOR(windType, AUX_WHITE);
    glRasterPos3fv(boxB);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits1);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits2);

    AUX_SETCOLOR(windType, AUX_YELLOW);
    glRasterPos3fv(boxC);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits1);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits2);

    AUX_SETCOLOR(windType, AUX_CYAN);
    glRasterPos3fv(boxD);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits1);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits2);

    AUX_SETCOLOR(windType, AUX_RED);
    glRasterPos3fv(boxE);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits1);
    glBitmap(OPENGL_WIDTH, OPENGL_HEIGHT, OPENGL_WIDTH, 0.0, OPENGL_WIDTH, 0.0,
         OpenGL_bits2);

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

    windType = (rgb) ? AUX_RGB : AUX_INDEX;
    windType |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(windType);

    if (auxInitWindow("Bitmap Test") == GL_FALSE) {
        auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxMainLoop(Draw);
}
