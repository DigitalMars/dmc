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
 *  feedback.c
 *  This program demonstrates use of OpenGL feedback.  First,
 *  a lighting environment is set up and a few lines are drawn.
 *  Then feedback mode is entered, and the same lines are 
 *  drawn.  The results in the feedback buffer are printed.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

void myinit(void);
void drawGeometry (GLenum mode);
void print3DcolorVertex (GLint size, GLint *count, GLfloat *buffer);
void printBuffer(GLint size, GLfloat *buffer);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

/*  Initialize lighting.
 */
void myinit(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

/*  Draw a few lines and two points, one of which will 
 *  be clipped.  If in feedback mode, a passthrough token 
 *  is issued between the each primitive.
 */
void drawGeometry (GLenum mode)
{
    glBegin (GL_LINE_STRIP);
    glNormal3f (0.0, 0.0, 1.0);
    glVertex3f (30.0, 30.0, 0.0);
    glVertex3f (50.0, 60.0, 0.0);
    glVertex3f (70.0, 40.0, 0.0);
    glEnd ();
    if (mode == GL_FEEDBACK)
    glPassThrough (1.0);
    glBegin (GL_POINTS);
    glVertex3f (-100.0, -100.0, -100.0);    /*  will be clipped  */
    glEnd ();
    if (mode == GL_FEEDBACK)
    glPassThrough (2.0);
    glBegin (GL_POINTS);
    glNormal3f (0.0, 0.0, 1.0);
    glVertex3f (50.0, 50.0, 0.0);
    glEnd ();
}

/*  Write contents of one vertex to stdout. */
void print3DcolorVertex (GLint size, 
    GLint *count, GLfloat *buffer)
{
    int i;

    //printf ("  ");
    for (i = 0; i < 7; i++) {
    //printf ("%4.2f ", buffer[size-(*count)]);
    *count = *count - 1;
    }
    //printf ("\n");
}

/*  Write contents of entire buffer.  (Parse tokens!)   */
void printBuffer(GLint size, GLfloat *buffer)
{
    GLint count;
    GLfloat token;

    count = size;
    while (count) {
    token = buffer[size-count]; count--;
    if (token == GL_PASS_THROUGH_TOKEN) {
        //printf ("GL_PASS_THROUGH_TOKEN\n");
        //printf ("  %4.2f\n", buffer[size-count]);
        count--;
    }
    else if (token == GL_POINT_TOKEN) {
        //printf ("GL_POINT_TOKEN\n");
        print3DcolorVertex (size, &count, buffer);
    }
    else if (token == GL_LINE_TOKEN) {
        //printf ("GL_LINE_TOKEN\n");
        print3DcolorVertex (size, &count, buffer);
        print3DcolorVertex (size, &count, buffer);
    }
    else if (token == GL_LINE_RESET_TOKEN) {
        //printf ("GL_LINE_RESET_TOKEN\n");
        print3DcolorVertex (size, &count, buffer);
        print3DcolorVertex (size, &count, buffer);
    }
    }
}

void CALLBACK display(void)
{
    GLfloat feedBuffer[1024];
    GLint size;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0.0, 100.0, 0.0, 100.0, 0.0, 1.0);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawGeometry (GL_RENDER);

    glFeedbackBuffer (1024, GL_3D_COLOR, feedBuffer);
    (void) glRenderMode (GL_FEEDBACK);
    drawGeometry (GL_FEEDBACK);

    size = glRenderMode (GL_RENDER);
    printBuffer (size, feedBuffer);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Feedback");
    myinit ();
    auxMainLoop(display);
    return(0);
}
