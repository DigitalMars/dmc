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
 *  stroke.c 
 *  This program demonstrates some characters of a 
 *  stroke (vector) font.  The characters are represented
 *  by display lists, which are given numbers which 
 *  correspond to the ASCII values of the characters.
 *  Use of glCallLists() is demonstrated.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

typedef struct charpoint {
    GLfloat   x, y;
    int    type;
} CP;

void myinit(void);
void drawLetter(CP *l);
void printStrokedString(char *s);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

#define PT 1
#define STROKE 2
#define END 3


CP Adata[] = {
    { 0, 0, PT}, {0, 9, PT}, {1, 10, PT}, {4, 10, PT}, 
    {5, 9, PT}, {5, 0, STROKE}, {0, 5, PT}, {5, 5, END}
};

CP Edata[] = {
    {5, 0, PT}, {0, 0, PT}, {0, 10, PT}, {5, 10, STROKE},
    {0, 5, PT}, {4, 5, END}
};

CP Pdata[] = {
    {0, 0, PT}, {0, 10, PT},  {4, 10, PT}, {5, 9, PT}, {5, 6, PT}, 
    {4, 5, PT}, {0, 5, END}
};

CP Rdata[] = {
    {0, 0, PT}, {0, 10, PT},  {4, 10, PT}, {5, 9, PT}, {5, 6, PT}, 
    {4, 5, PT}, {0, 5, STROKE}, {3, 5, PT}, {5, 0, END}
};

CP Sdata[] = {
    {0, 1, PT}, {1, 0, PT}, {4, 0, PT}, {5, 1, PT}, {5, 4, PT}, 
    {4, 5, PT}, {1, 5, PT}, {0, 6, PT}, {0, 9, PT}, {1, 10, PT}, 
    {4, 10, PT}, {5, 9, END}
};

/*  drawLetter() interprets the instructions from the array
 *  for that letter and renders the letter with line segments.
 */
void drawLetter(CP *l)
{
    glBegin(GL_LINE_STRIP);
    while (1) {
    switch (l->type) {
        case PT:
        glVertex2fv(&l->x);
        break;
        case STROKE:
        glVertex2fv(&l->x);
        glEnd();
        glBegin(GL_LINE_STRIP);
        break;
        case END:
        glVertex2fv(&l->x);
        glEnd();
        glTranslatef(8.0, 0.0, 0.0);
        return;
    }
    l++;
    }
}

/*  Create a display list for each of 6 characters  */
void myinit (void)
{
    GLuint base;

    glShadeModel (GL_FLAT);

    base = glGenLists (128);
    glListBase(base);
    glNewList(base+'A', GL_COMPILE); drawLetter(Adata); glEndList();
    glNewList(base+'E', GL_COMPILE); drawLetter(Edata); glEndList();
    glNewList(base+'P', GL_COMPILE); drawLetter(Pdata); glEndList();
    glNewList(base+'R', GL_COMPILE); drawLetter(Rdata); glEndList();
    glNewList(base+'S', GL_COMPILE); drawLetter(Sdata); glEndList();
    glNewList(base+' ', GL_COMPILE); glTranslatef(8.0, 0.0, 0.0); glEndList();
}

char *test1 = "A SPARE SERAPE APPEARS AS";
char *test2 = "APES PREPARE RARE PEPPERS";

void printStrokedString(char *s)
{
    GLsizei len = strlen(s);
    glCallLists(len, GL_BYTE, (GLbyte *)s);
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 30.0, 0.0);
    printStrokedString(test1);
    glPopMatrix();
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 13.0, 0.0);
    printStrokedString(test2);
    glPopMatrix();
    glFlush();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 440, 120);
    auxInitWindow ("Stroke Font");
    myinit ();
    auxMainLoop(display);
    return(0);
}
