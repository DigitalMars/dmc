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
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <gl\glaux.h>

#define INREAL float

#define S_NUMPOINTS 13
#define S_ORDER     3   
#define S_NUMKNOTS  (S_NUMPOINTS + S_ORDER)
#define T_NUMPOINTS 3
#define T_ORDER     3 
#define T_NUMKNOTS  (T_NUMPOINTS + T_ORDER)
#define SQRT_TWO    1.41421356237309504880

typedef INREAL Point[4];

static void CALLBACK ErrorCallback(GLenum which);
static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_DOWN(void );
static void CALLBACK Key_UP(void );
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum doubleBuffer;

GLenum expectedError;
GLint rotX = 40, rotY = 40;
INREAL sknots[S_NUMKNOTS] = {
    -1.0, -1.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0,
    4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 9.0, 9.0
};
INREAL tknots[T_NUMKNOTS] = {
    1.0, 1.0, 1.0, 2.0, 2.0, 2.0
};
Point ctlpoints[S_NUMPOINTS][T_NUMPOINTS] = {
    {
    {
        4.0, 2.0, 2.0, 1.0
    },
    {
        4.0, 1.6, 2.5, 1.0
    },
    {
        4.0, 2.0, 3.0, 1.0
    }
    },
    {
    {
        5.0, 4.0, 2.0, 1.0
    },
    {
        5.0, 4.0, 2.5, 1.0
    },
    {
        5.0, 4.0, 3.0, 1.0
    }
    },
    {
    {
        6.0, 5.0, 2.0, 1.0
    },
    {
        6.0, 5.0, 2.5, 1.0
    },
    {
        6.0, 5.0, 3.0, 1.0
    }
    },
    {
    {
        SQRT_TWO*6.0, SQRT_TWO*6.0, SQRT_TWO*2.0, SQRT_TWO
    },
    {
        SQRT_TWO*6.0, SQRT_TWO*6.0, SQRT_TWO*2.5, SQRT_TWO
    },
    {
        SQRT_TWO*6.0, SQRT_TWO*6.0, SQRT_TWO*3.0, SQRT_TWO
    }  
    },
    {
    {
        5.2, 6.7, 2.0, 1.0
    },
    {
        5.2, 6.7, 2.5, 1.0
    },
    {
        5.2, 6.7, 3.0, 1.0
    }
    },
    {
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*2.0, SQRT_TWO
    },
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*2.5, SQRT_TWO
    }, 
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*3.0, SQRT_TWO
    }  
    }, 
    {
    {
        4.0, 5.2, 2.0, 1.0
    },
    {
        4.0, 4.6, 2.5, 1.0
    },
    {
        4.0, 5.2, 3.0, 1.0
    }  
    },
    {
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*2.0, SQRT_TWO
    },
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*2.5, SQRT_TWO
    },
    {
        SQRT_TWO*4.0, SQRT_TWO*6.0, SQRT_TWO*3.0, SQRT_TWO
    }  
    },
    {
    {
        2.8, 6.7, 2.0, 1.0
    },
    {
        2.8, 6.7, 2.5, 1.0
    },
    {
        2.8, 6.7, 3.0, 1.0
    }   
    },
    {
    {
        SQRT_TWO*2.0, SQRT_TWO*6.0, SQRT_TWO*2.0, SQRT_TWO
    },
    {
        SQRT_TWO*2.0, SQRT_TWO*6.0, SQRT_TWO*2.5, SQRT_TWO
    },
    {
        SQRT_TWO*2.0, SQRT_TWO*6.0, SQRT_TWO*3.0, SQRT_TWO
    }  
    },
    {
    {
        2.0, 5.0, 2.0, 1.0
    },
    {
        2.0, 5.0, 2.5, 1.0
    },
    {
        2.0, 5.0, 3.0, 1.0
    } 
    },
    {
    {
        3.0, 4.0, 2.0, 1.0
    },
    {
        3.0, 4.0, 2.5, 1.0
    },
    {
        3.0, 4.0, 3.0, 1.0
    } 
    },
    {
    {
        4.0, 2.0, 2.0, 1.0
    },
    {
        4.0, 1.6, 2.5, 1.0
    },
    {
        4.0, 2.0, 3.0, 1.0
    }    
    }
};
GLUnurbsObj *theNurbs;


static void CALLBACK ErrorCallback(GLenum which)
{

    if (which != expectedError) {
    fprintf(stderr, "Unexpected error occured (%d):\n", which);
    fprintf(stderr, "    %s\n", gluErrorString(which));
    }
}

static void Init(void)
{

    theNurbs = gluNewNurbsRenderer();
    //gluNurbsCallback(theNurbs, GLU_ERROR, ErrorCallback);

    gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 15.0);
    gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);

    expectedError = GLU_INVALID_ENUM;
    gluNurbsProperty(theNurbs, (GLenum)~0, 15.0);
    expectedError = GLU_NURBS_ERROR13;
    gluEndSurface(theNurbs);
    expectedError = 0;

    glColor3f(1.0, 1.0, 1.0);
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -2.0, 2.0, 0.8, 10.0);
    gluLookAt(7.0, 4.5, 4.0, 4.5, 4.5, 2.5, 6.0, -3.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_DOWN(void)
{
    rotX -= 5;
}

static void CALLBACK Key_UP(void)
{
    rotX += 5;
}

static void CALLBACK Key_LEFT(void)
{
    rotY -= 5;
}

static void CALLBACK Key_RIGHT(void)
{
    rotY += 5;
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(4.0, 4.5, 2.5);
    glRotatef(rotY, 1, 0, 0);
    glRotatef(rotX, 0, 1, 0);
    glTranslatef(-4.0, -4.5, -2.5);

    gluBeginSurface(theNurbs);
    gluNurbsSurface(theNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots,
            4*T_NUMPOINTS, 4, &ctlpoints[0][0][0], S_ORDER,
            T_ORDER, GL_MAP2_VERTEX_4);
    gluEndSurface(theNurbs);

    glPopMatrix();

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
    } else {
        printf("%s (Bad option).\n", argv[i]);
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

    auxInitPosition(0, 0, 300, 300);

    type = AUX_RGB;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(type);

    if (auxInitWindow("NURBS Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxMainLoop(Draw);
}
