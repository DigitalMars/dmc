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
 *  dofnot.c
 *  This program demonstrates the same scene as dof.c, but
 *  without use of the accumulation buffer, so everything
 *  is in focus.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <GL/glaux.h>

void myinit(void);
void renderTeapot (GLfloat x, GLfloat y, GLfloat z,
    GLfloat ambr, GLfloat ambg, GLfloat ambb, 
    GLfloat difr, GLfloat difg, GLfloat difb, 
    GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

void myinit(void)
{
    GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 0.0, 3.0, 3.0, 0.0 };
    
    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat local_view[] = { 0.0 };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace (GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void renderTeapot (GLfloat x, GLfloat y, GLfloat z, 
    GLfloat ambr, GLfloat ambg, GLfloat ambb, 
    GLfloat difr, GLfloat difg, GLfloat difb, 
    GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine)
{
    float mat[3];

    glPushMatrix();
    glTranslatef (x, y, z);
    mat[0] = ambr; mat[1] = ambg; mat[2] = ambb;    
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
    mat[0] = difr; mat[1] = difg; mat[2] = difb;    
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
    mat[0] = specr; mat[1] = specg; mat[2] = specb;
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
    glMaterialf (GL_FRONT, GL_SHININESS, shine*128.0);
    auxSolidTeapot(0.5);
    glPopMatrix();
}

/*  display() draws 5 teapots into the accumulation buffer 
 *  several times; each time with a jittered perspective.
 *  The focal point is at z = 5.0, so the gold teapot will 
 *  stay in focus.  The amount of jitter is adjusted by the
 *  magnitude of the accPerspective() jitter; in this example, 0.33.
 *  In this example, the teapots are drawn 8 times.  See jitter.h
 */
void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix ();
/*  ruby, gold, silver, emerald, and cyan teapots   */
    renderTeapot (-1.1, -0.5, -4.5, 0.1745, 0.01175, 0.01175,
        0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
    renderTeapot (-0.5, -0.5, -5.0, 0.24725, 0.1995, 0.0745,
        0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
    renderTeapot (0.2, -0.5, -5.5, 0.19225, 0.19225, 0.19225,
        0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
    renderTeapot (1.0, -0.5, -6.0, 0.0215, 0.1745, 0.0215, 
        0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
    renderTeapot (1.8, -0.5, -6.5, 0.0, 0.1, 0.06, 0.0, 0.50980392, 
        0.50980392, 0.50196078, 0.50196078, 0.50196078, .25);

    glPopMatrix ();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (45.0, (GLfloat) w/(GLfloat) h, 
        1.0, 15.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, depth buffer, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 400, 400);
    auxInitWindow ("Depth-of-Field");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
