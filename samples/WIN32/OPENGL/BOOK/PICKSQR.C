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
 *  picksquare.c
 *  Use of multiple names and picking are demonstrated.  
 *  A 3x3 grid of squares is drawn.  When the left mouse 
 *  button is pressed, all squares under the cursor position 
 *  have their color changed.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

void myinit(void);
void processHits (GLint hits, GLuint buffer[]);
void CALLBACK pickSquares(AUX_EVENTREC *event);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

int board[3][3];    /*  amount of color for each square */

/*  Clear color value for every square on the board     */
void myinit(void)
{
    int i, j;
    for (i = 0; i < 3; i++) 
    for (j = 0; j < 3; j ++)
        board[i][j] = 0;
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

/*  The nine squares are drawn.  In selection mode, each 
 *  square is given two names:  one for the row and the 
 *  other for the column on the grid.  The color of each 
 *  square is determined by its position on the grid, and 
 *  the value in the board[][] array.
 */
void drawSquares(GLenum mode)
{
    GLuint i, j;
    for (i = 0; i < 3; i++) {
    if (mode == GL_SELECT)
        glLoadName (i);
    for (j = 0; j < 3; j ++) {
        if (mode == GL_SELECT)
        glPushName (j);
        glColor3f ((GLfloat) i/3.0, (GLfloat) j/3.0, 
            (GLfloat) board[i][j]/3.0);
        glRecti (i, j, i+1, j+1);
        if (mode == GL_SELECT)
        glPopName ();
    }
    }
}

/*  processHits() prints out the contents of the 
 *  selection array.
 */
void processHits (GLint hits, GLuint buffer[])
{
    int i, j;
    GLint ii, jj, names, *ptr;

    //printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) {    /*  for each hit  */
    names = *ptr;
    //printf (" number of names for this hit = %d\n", names);
    ptr++;
    //printf ("  z1 is %u;", *ptr);
    ptr++;
    //printf (" z2 is %u\n", *ptr);
    ptr++;
    //printf ("   names are ");
    for (j = 0; j < names; j++) {   /*  for each name */
        //printf ("%d ", *ptr);
        if (j == 0) /*  set row and column  */
        ii = *ptr;
        else if (j == 1)
        jj = *ptr;
        ptr++;
    }
    //printf ("\n");
    board[ii][jj] = (board[ii][jj] + 1) % 3;
    }
}

/*  pickSquares() sets up selection mode, name stack, 
 *  and projection matrix for picking.  Then the 
 *  objects are drawn.
 */
#define BUFSIZE 512

void CALLBACK pickSquares(AUX_EVENTREC *event)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    int x, y;

    x = event->data[AUX_MOUSEX];
    y = event->data[AUX_MOUSEY];
    glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    glInitNames();
    glPushName((unsigned)-1);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
/*  create 5x5 pixel picking region near cursor location    */
    gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
    5.0, 5.0, viewport);
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    drawSquares (GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
    processHits (hits, selectBuf);
} 

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSquares (GL_RENDER);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Picking Squares");
    myinit ();
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, pickSquares);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
