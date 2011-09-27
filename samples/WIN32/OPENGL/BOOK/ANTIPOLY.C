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
 *  antipoly.c
 *  This program draws filled polygons with antialiased
 *  edges.  The special GL_SRC_ALPHA_SATURATE blending 
 *  function is used.
 *  Pressing the left mouse button turns the antialiasing
 *  on and off.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK toggleSmooth (AUX_EVENTREC *event);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLboolean polySmooth;

void myinit(void)
{
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.00 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.00 };
    GLfloat mat_shininess[] = { 15.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_BLEND);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glEnable (GL_POLYGON_SMOOTH);
    polySmooth = GL_TRUE;

    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void CALLBACK toggleSmooth (AUX_EVENTREC *event)
{
    if (polySmooth) {
    polySmooth = GL_FALSE;
    glDisable (GL_BLEND);
    glDisable (GL_POLYGON_SMOOTH);
    glEnable (GL_DEPTH_TEST);
    }
    else {
    polySmooth = GL_TRUE;
    glEnable (GL_BLEND);
    glEnable (GL_POLYGON_SMOOTH);
    glDisable (GL_DEPTH_TEST);
    }
}

/*  Note:  polygons must be drawn from front to back
 *  for proper blending.
 */
void CALLBACK display(void)
{
    GLfloat position[] = { 0.0, 0.0, 1.0, 0.0 };
    GLfloat mat_cube1[] = { 0.75, 0.75, 0.0, 1.0 };
    GLfloat mat_cube2[] = { 0.0, 0.75, 0.75, 1.0 };

    if (polySmooth)
    glClear (GL_COLOR_BUFFER_BIT);
    else 
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix ();
    glTranslatef (0.0, 0.0, -8.0);    
    glLightfv (GL_LIGHT0, GL_POSITION, position);

    glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);

    glPushMatrix ();
    glRotatef (30.0, 1.0, 0.0, 0.0);
    glRotatef (60.0, 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cube1);
    auxSolidCube (1.0);
    glPopMatrix ();

    glTranslatef (0.0, 0.0, -2.0);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cube2);
    glRotatef (30.0, 0.0, 1.0, 0.0);
    glRotatef (60.0, 1.0, 0.0, 0.0);
    auxSolidCube (1.0);

    glPopMatrix ();

    glFlush ();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    h = (h == 0) ? 1 : h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Main Loop
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_ALPHA | AUX_DEPTH16);
    auxInitPosition (0, 0, 200, 200);
    auxInitWindow ("Antialiased Edges");
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, toggleSmooth);
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
