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

static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_Z(void );
static void CALLBACK Key_z(void );
static GLenum CALLBACK Mouse_left(struct _AUX_EVENTREC *event);
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum doubleBuffer;
GLint windW, windH;

char *fileName = 0;
AUX_RGBImageRec *xximage;
float point[3];
float zoom;
GLint x, y;


static void Init(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);

    x = 0;
    y = windH;
    zoom = 1.8;
}

static void CALLBACK Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;

    glViewport(0, 0, windW, windH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windW, 0, windH);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_Z(void)
{
    zoom += 0.2;
}

static void CALLBACK Key_z(void)
{
    zoom -= 0.2;
    if (zoom < 0.2) {
        zoom = 0.2;
    }
}

static GLenum CALLBACK Mouse_left(AUX_EVENTREC *event)
{

    x = event->data[AUX_MOUSEX];
    y = event->data[AUX_MOUSEY];
    return GL_TRUE;
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    point[0] = (windW / 2) - (xximage->sizeX / 2);
    point[1] = (windH / 2) - (xximage->sizeY / 2);
    point[2] = 0;
    glRasterPos3fv(point);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelZoom(1.0, 1.0);
    glDrawPixels(xximage->sizeX, xximage->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
         xximage->data);

    point[0] = (float)x;
    point[1] = windH - (float)y;
    point[2] = 0.0;
    glRasterPos3fv(point);

    glPixelZoom(zoom, zoom);
    glCopyPixels((windW/2)-(xximage->sizeX/2),
         (windH/2)-(xximage->sizeY/2),
         xximage->sizeX, xximage->sizeY, GL_COLOR);

    glFlush();

    if (doubleBuffer) {
    auxSwapBuffers();
    }
}

static GLenum Args(int argc, char **argv)
{
    GLint i;

    doubleBuffer = GL_FALSE;


    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-sb") == 0) {
        doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
        doubleBuffer = GL_TRUE;
    } else if (strcmp(argv[i], "-f") == 0) {
        if (i+1 >= argc || argv[i+1][0] == '-') {
        //printf("-f (No file name).\n");
        return GL_FALSE;
        } else {
        fileName = argv[++i];
        }
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

    if (fileName == 0) {
    //printf("No image file.\n");
    auxQuit();
    }

    xximage = auxRGBImageLoad(fileName);

    windW = 300;
    windH = 300;
    auxInitPosition(0, 0, windW, windH);

    type = AUX_RGB;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(type);

    if (auxInitWindow("Copy Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_Z, Key_Z);
    auxKeyFunc(AUX_z, Key_z);
    auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, Mouse_left);
    auxMainLoop(Draw);
}
