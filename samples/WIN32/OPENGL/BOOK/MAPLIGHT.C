/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

/*
 *  maplight.c
 *  This program demonstrates the use of the GL lighting model.
 *  A sphere is drawn using a magenta diffuse reflective and
 *  white specular material property.
 *  A single light source illuminates the object.  This program
 *  illustrates lighting in color map mode.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

/*  Initialize material property, light source, and lighting model.
 */
void myinit(void)
{
    GLint i;

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat mat_colormap[] = { 16.0, 48.0, 79.0 };
    GLfloat mat_shininess[] = { 10.0 };
    
    glMaterialfv(GL_FRONT, GL_COLOR_INDEXES, mat_colormap);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    for (i = 0; i < 32; i++) {
    auxSetOneColor (16 + i, 1.0 * (i/32.0), 0.0, 1.0 * (i/32.0));
    auxSetOneColor (48 + i, 1.0, 1.0 * (i/32.0), 1.0);
    }
    glClearIndex(0);
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auxSolidSphere(1.0);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
    glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w, 
        1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else 
    glOrtho (-1.5*(GLfloat)w/(GLfloat)h, 
        1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, color 
 *  index display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_INDEX | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Lighting in Color Map Mode");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
