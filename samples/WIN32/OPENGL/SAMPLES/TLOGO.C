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


#define PI 3.141592654

#define BLACK 0
#define GRAY 128
#define WHITE 255
#define BL 0x00
#define WH 0xFF
#define RD 0xA40000FF
#define WT 0xFFFFFFFF

#define CHECKIMAGEWIDTH 8
#define CHECKIMAGEHEIGHT 8
#define BRICKIMAGEWIDTH 16
#define BRICKIMAGEHEIGHT 16

static void SetUpAntiAliasedGrayScale(void );
static void BendForward(void );
static void BendLeft(void );
static void BendRight(void );
static void BuildSingleCylinder(void );
static void BuildDoubleCylinder(void );
static void BuildElbow(void );
static void BuildLogo(void );
static void BuildLists(void );
static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_UP(void );
static void CALLBACK Key_DOWN(void );
static void CALLBACK Key_Z(void );
static void CALLBACK Key_z(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );
static void CALLBACK Key_p(void );
static void CALLBACK Key_4(void );
static void CALLBACK Key_5(void );
static void CALLBACK Key_6(void );
static void CALLBACK Key_8(void );
static void CALLBACK Key_9(void );
static void CALLBACK Key_0(void );
static void CALLBACK Key_q(void );
static void CALLBACK Key_w(void );
static void CALLBACK Key_e(void );
static void CALLBACK Key_r(void );
static void CALLBACK Key_t(void );
static void CALLBACK Key_y(void );
static void CALLBACK Key_u(void );
static void CALLBACK Key_a(void );
static void CALLBACK Key_s(void );
static void CALLBACK Key_d(void );
static void CALLBACK Key_f(void );
static void CALLBACK Key_g(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer, directRender;

float black[3] = {0.0, 0.0, 0.0};
float white[3] = {1.0, 1.0, 1.0};
float gray[3] = {0.5, 0.5, 0.5};
float blue[3] = {0.0, 0.0, 1.0};
GLint colorIndexes[3] = {0, 200, 255};

GLenum polyMode;
GLenum dithering;
GLenum shade;
GLenum doStipple;

double plane[4] = {1.0, 0.0, -1.0, 0.0};
float xRotation = 30.0, yRotation = 30.0;
float zTranslation = -15.0;

GLint singleCylinder;
GLint doubleCylinder;
GLint elbow, logo;

GLubyte checkImage[3*CHECKIMAGEWIDTH*CHECKIMAGEHEIGHT] = {
    BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH,
    WH, WH, BL, BL, BL, WH, WH, WH, WH, WH, WH, BL, BL, BL, WH, WH,
    WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL,
    BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH,
    WH, WH, BL, BL, BL, WH, WH, WH, WH, WH, WH, BL, BL, BL, WH, WH,
    WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL,
    BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH,
    WH, WH, BL, BL, BL, WH, WH, WH, WH, WH, WH, BL, BL, BL, WH, WH,
    WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL,
    BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH,
    WH, WH, BL, BL, BL, WH, WH, WH, WH, WH, WH, BL, BL, BL, WH, WH,
    WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, WH, WH, WH, BL, BL, BL, 
};
GLuint brickImage[BRICKIMAGEWIDTH*BRICKIMAGEHEIGHT] = {
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD
};

GLubyte *image = checkImage;
GLint imageHeight = CHECKIMAGEHEIGHT;
GLint imageWidth = CHECKIMAGEWIDTH;

float decal[] = {
    GL_DECAL,
};
float modulate[] = {
    GL_MODULATE,
};
float repeat[] = {
    GL_REPEAT,
};
float nearest[] = {
    GL_NEAREST,
};

GLubyte stipple[4*32] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,

    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,
    0x00, 0x0F, 0xF0, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

float tscp[18][2] = {
    {
    0.0, 0.0
    },
    {
    1.0, 0.0
    },
    {
    0.0, 0.125
    },
    {
    1.0, 0.125
    },
    {
    0.0, 0.250
    },
    {
    1.0, 0.25
    },
    {
    0.0, 0.375
    },
    {
    1.0, 0.375
    },
    {
    0.0, 0.50
    },
    {
    1.0, 0.50
    },
    {
    0.0, 0.625
    },
    {
    1.0, 0.625
    },
    {
    0.0, 0.75
    },
    {
    1.0, 0.75
    },
    {
    0.0, 0.875
    },
    {
    1.0, 0.875
    },
    {
    0.0, 1.0
    },
    {
    1.0, 1.0
    }
};
float scp[18][3] = {
    {
    1.000000, 0.000000, 0.000000
    },
    {
    1.000000, 0.000000, 5.000000
    },
    {
    0.707107, 0.707107, 0.000000
    },
    {
    0.707107, 0.707107, 5.000000
    },
    {
    0.000000, 1.000000, 0.000000
    },
    {
    0.000000, 1.000000, 5.000000
    },
    {
    -0.707107, 0.707107, 0.000000
    },
    {
    -0.707107, 0.707107, 5.000000
    },
    {
    -1.000000, 0.000000, 0.000000
    },
    {
    -1.000000, 0.000000, 5.000000
    },
    {
    -0.707107, -0.707107, 0.000000
    },
    {
    -0.707107, -0.707107, 5.000000
    },
    {
    0.000000, -1.000000, 0.000000
    },
    {
    0.000000, -1.000000, 5.000000
    },
    {
    0.707107, -0.707107, 0.000000
    },
    {
    0.707107, -0.707107, 5.000000
    },
    {
    1.000000, 0.000000, 0.000000
    },
    {
    1.000000, 0.000000, 5.000000
    }
};
float dcp[18][3] = {
    {
    1.000000, 0.000000, 0.000000
    },
    {
    1.000000, 0.000000, 7.000000
    },
    {
    0.707107, 0.707107, 0.000000
    },
    {
    0.707107, 0.707107, 7.000000
    },
    {
    0.000000, 1.000000, 0.000000
    },
    {
    0.000000, 1.000000, 7.000000
    },
    {
    -0.707107, 0.707107, 0.000000
    },
    {
    -0.707107, 0.707107, 7.000000
    },
    {
    -1.000000, 0.000000, 0.000000
    },
    {
    -1.000000, 0.000000, 7.000000
    },
    {
    -0.707107, -0.707107, 0.000000
    },
    {
    -0.707107, -0.707107, 7.000000
    },
    {
    0.000000, -1.000000, 0.000000
    },
    {
    0.000000, -1.000000, 7.000000
    },
    {
    0.707107, -0.707107, 0.000000
    },
    {
    0.707107, -0.707107, 7.000000
    },
    {
    1.000000, 0.000000, 0.000000
    },
    {
    1.000000, 0.000000, 7.000000
    }
};
float ep[7][9][3] = {
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.707107, 0.000000
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.707107, 0.000000
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.707107, 0.000000
    },
    {
        0.000000, -1.000000, 0.000000
    },
    {
        0.707107, -0.707107, 0.000000
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.034074, 0.258819
    },
    {
        0.707107, 0.717087, 0.075806
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.717087, 0.075806
    },
    {
        -1.000000, 0.034074, 0.258819
    },
    {
        -0.707107, -0.648939, 0.441832
    },
    {
        0.000000, -0.931852, 0.517638
    },
    {
        0.707107, -0.648939, 0.441832
    },
    {
        1.000000, 0.034074, 0.258819
    }
    },
    {
    {
        1.000000, 0.133975, 0.500000
    },
    {
        0.707107, 0.746347, 0.146447
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.746347, 0.146447
    },
    {
        -1.000000, 0.133975, 0.500000
    },
    {
        -0.707107, -0.478398, 0.853553
    },
    {
        0.000000, -0.732051, 1.000000
    },
    {
        0.707107, -0.478398, 0.853553
    },
    {
        1.000000, 0.133975, 0.500000
    }
    },
    {
    {
        1.000000, 0.292893, 0.707107
    },
    {
        0.707107, 0.792893, 0.207107
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.792893, 0.207107
    },
    {
        -1.000000, 0.292893, 0.707107
    },
    {
        -0.707107, -0.207107, 1.207107
    },
    {
        0.000000, -0.414214, 1.414214
    },
    {
        0.707107, -0.207107, 1.207107
    },
    {
        1.000000, 0.292893, 0.707107
    }
    },
    {
    {
        1.000000, 0.500000, 0.866025
    },
    {
        0.707107, 0.853553, 0.253653
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.853553, 0.253653
    },
    {
        -1.000000, 0.500000, 0.866025
    },
    {
        -0.707107, 0.146447, 1.478398
    },
    {
        0.000000, 0.000000, 1.732051
    },
    {
        0.707107, 0.146447, 1.478398
    },
    {
        1.000000, 0.500000, 0.866025
    }
    },
    {
    {
        1.000000, 0.741181, 0.965926
    },
    {
        0.707107, 0.924194, 0.282913
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.924194, 0.282913
    },
    {
        -1.000000, 0.741181, 0.965926
    },
    {
        -0.707107, 0.558168, 1.648939
    },
    {
        0.000000, 0.482362, 1.931852
    },
    {
        0.707107, 0.558168, 1.648939
    },
    {
        1.000000, 0.741181, 0.965926
    }
    },
    {
    {
        1.000000, 1.000000, 1.000000
    },
    {
        0.707107, 1.000000, 0.292893
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 1.000000, 0.292893
    },
    {
        -1.000000, 1.000000, 1.000000
    },
    {
        -0.707107, 1.000000, 1.707107
    },
    {
        0.000000, 1.000000, 2.000000
    },
    {
        0.707107, 1.000000, 1.707107
    },
    {
        1.000000, 1.000000, 1.000000
    }
    }
};
float en[7][9][3] = {
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.707107, 0.000000
    },
    {
        0.000000, 1.000000, 0.000000
    },
    {
        -0.707107, 0.707107, 0.000000
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.707107, 0.000000
    },
    {
        0.000000, -1.000000, 0.000000
    },
    {
        0.707107, -0.707107, 0.000000
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.683013, -0.183013
    },
    {
        0.000000, 0.965926, -0.258819
    },
    {
        -0.707107, 0.683013, -0.183013
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.683013, 0.183013
    },
    {
        0.000000, -0.965926, 0.258819
    },
    {
        0.707107, -0.683013, 0.183013
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.612372, -0.353553
    },
    {
        0.000000, 0.866025, -0.500000
    },
    {
        -0.707107, 0.612372, -0.353553
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.612372, 0.353553
    },
    {
        0.000000, -0.866025, 0.500000
    },
    {
        0.707107, -0.612372, 0.353553
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.500000, -0.500000
    },
    {
        0.000000, 0.707107, -0.707107
    },
    {
        -0.707107, 0.500000, -0.500000
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.500000, 0.500000
    },
    {
        0.000000, -0.707107, 0.707107
    },
    {
        0.707107, -0.500000, 0.500000
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.353553, -0.612372
    },
    {
        0.000000, 0.500000, -0.866025
    },
    {
        -0.707107, 0.353553, -0.612372
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.353553, 0.612372
    },
    {
        0.000000, -0.500000, 0.866025
    },
    {
        0.707107, -0.353553, 0.612372
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.183013, -0.683013
    },
    {
        0.000000, 0.258819, -0.965926
    },
    {
        -0.707107, 0.183013, -0.683013
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, -0.183013, 0.683013
    },
    {
        0.000000, -0.258819, 0.965926
    },
    {
        0.707107, -0.183013, 0.683013
    },
    {
        1.000000, 0.000000, 0.000000
    }
    },
    {
    {
        1.000000, 0.000000, 0.000000
    },
    {
        0.707107, 0.000000, -0.707107
    },
    {
        0.000000, 0.000000, -1.000000
    },
    {
        -0.707107, 0.000000, -0.707107
    },
    {
        -1.000000, 0.000000, 0.000000
    },
    {
        -0.707107, 0.000000, 0.707107
    },
    {
        0.000000, 0.000000, 1.000000
    },
    {
        0.707107, 0.000000, 0.707107
    },
    {
        1.000000, 0.000000, 0.000000
    }
    }
};
float tep[7][9][2] = {
    {
    {
        0,     0.0
    },
    {
        0.125, 0.0
    },
    {
        0.25,  0.0
    },
    {
        0.375, 0.0
    },
    {
        0.5,   0.0
    },
    {
        0.625, 0.0
    },
    {
        0.75,  0.0
    },
    {
        0.875, 0.0
    },
    {
        1.0,   0.0
    }
    },
    {
    {
        0,     0.16667
    },
    {
        0.125, 0.16667
    },
    {
        0.25,  0.16667
    },
    {
        0.375, 0.16667
    },
    {
        0.5,   0.16667
    },
    {
        0.625, 0.16667
    },
    {
        0.75,  0.16667
    },
    {
        0.875, 0.16667
    },
    {
        1.0,   0.16667
    }
    },
    {
    {
        0,     0.33333
    },
    {
        0.125, 0.33333
    },
    {
        0.25,  0.33333
    },
    {
        0.375, 0.33333
    },
    {
        0.5,   0.33333
    },
    {
        0.625, 0.33333
    },
    {
        0.75,  0.33333
    },
    {
        0.875, 0.33333
    },
    {
        1.0,   0.33333
    }
    },
    {
    {
        0,     0.5
    },
    {
        0.125, 0.5
    },
    {
        0.25,  0.5
    },
    {
        0.375, 0.5
    },
    {
        0.5,   0.5
    },
    {
        0.625, 0.5
    },
    {
        0.75,  0.5
    },
    {
        0.875, 0.5
    },
    {
        1.0,   0.5
    }
    },
    {
    {
        0,     0.6667
    },
    {
        0.125, 0.6667
    },
    {
        0.25,  0.6667
    },
    {
        0.375, 0.6667
    },
    {
        0.5,   0.6667
    },
    {
        0.625, 0.6667
    },
    {
        0.75,  0.6667
    },
    {
        0.875, 0.6667
    },
    {
        1.0,   0.6667
    }
    },
    {
    {
        0,     0.83333
    },
    {
        0.125, 0.83333
    },
    {
        0.25,  0.83333
    },
    {
        0.375, 0.83333
    },
    {
        0.5,   0.83333
    },
    {
        0.625, 0.83333
    },
    {
        0.75,  0.83333
    },
    {
        0.875, 0.83333
    },
    {
        1.0,   0.83333
    }
    },
    {
    {
        0,     1.0
    },
    {
        0.125, 1.0
    },
    {
        0.25,  1.0
    },
    {
        0.375, 1.0
    },
    {
        0.5,   1.0
    },
    {
        0.625, 1.0
    },
    {
        0.75,  1.0
    },
    {
        0.875, 1.0
    },
    {
        1.0,   1.0
    }
    }
};


static void SetUpAntiAliasedGrayScale(void)
{
    float color;
    GLint i, j;

    for (i = 0; i < 16; i++) {
    color = (2 * i + 1) / 32.0;
    for (j = 0; j < 16; j++) {
        auxSetOneColor(i*16+j, color*j/15.0, color*j/15.0, color*j/15.0);
    }
    }
}

static void BendForward(void)
{

    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(90.0, 1, 0, 0);
    glTranslatef(0.0, -1.0, 0.0);
}

static void BendLeft(void)
{

    glRotatef(-90.0, 0, 0, 1);
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(90.0, 1, 0, 0);
    glTranslatef(0.0, -1.0, 0.0);
}

static void BendRight(void)
{

    glRotatef(90.0, 0, 0, 1);
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(90.0, 1, 0, 0);
    glTranslatef(0.0, -1.0, 0.0);
}

static void BuildSingleCylinder(void)
{

    glNewList(singleCylinder, GL_COMPILE);

    glBegin(GL_TRIANGLE_STRIP);
       glNormal3fv(scp[0]); glTexCoord2fv(tscp[0]); glVertex3fv(scp[0]);
       glNormal3fv(scp[0]); glTexCoord2fv(tscp[1]); glVertex3fv(scp[1]);
       glNormal3fv(scp[2]); glTexCoord2fv(tscp[2]); glVertex3fv(scp[2]);
       glNormal3fv(scp[2]); glTexCoord2fv(tscp[3]); glVertex3fv(scp[3]);
       glNormal3fv(scp[4]); glTexCoord2fv(tscp[4]); glVertex3fv(scp[4]);
       glNormal3fv(scp[4]); glTexCoord2fv(tscp[5]); glVertex3fv(scp[5]);
       glNormal3fv(scp[6]); glTexCoord2fv(tscp[6]); glVertex3fv(scp[6]);
       glNormal3fv(scp[6]); glTexCoord2fv(tscp[7]); glVertex3fv(scp[7]);
       glNormal3fv(scp[8]); glTexCoord2fv(tscp[8]); glVertex3fv(scp[8]);
       glNormal3fv(scp[8]); glTexCoord2fv(tscp[9]); glVertex3fv(scp[9]);
       glNormal3fv(scp[10]); glTexCoord2fv(tscp[10]); glVertex3fv(scp[10]);
       glNormal3fv(scp[10]); glTexCoord2fv(tscp[11]); glVertex3fv(scp[11]);
       glNormal3fv(scp[12]); glTexCoord2fv(tscp[12]); glVertex3fv(scp[12]);
       glNormal3fv(scp[12]); glTexCoord2fv(tscp[13]); glVertex3fv(scp[13]);
       glNormal3fv(scp[14]); glTexCoord2fv(tscp[14]); glVertex3fv(scp[14]);
       glNormal3fv(scp[14]); glTexCoord2fv(tscp[15]); glVertex3fv(scp[15]);
       glNormal3fv(scp[16]); glTexCoord2fv(tscp[16]); glVertex3fv(scp[16]);
       glNormal3fv(scp[16]); glTexCoord2fv(tscp[17]); glVertex3fv(scp[17]);
    glEnd();

    glEndList();
}

static void BuildDoubleCylinder(void)
{

    glNewList(doubleCylinder, GL_COMPILE);

    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(dcp[0]); glTexCoord2fv(tscp[0]); glVertex3fv(dcp[0]);
    glNormal3fv(dcp[0]); glTexCoord2fv(tscp[1]); glVertex3fv(dcp[1]);
    glNormal3fv(dcp[2]); glTexCoord2fv(tscp[2]); glVertex3fv(dcp[2]);
    glNormal3fv(dcp[2]); glTexCoord2fv(tscp[3]); glVertex3fv(dcp[3]);
    glNormal3fv(dcp[4]); glTexCoord2fv(tscp[4]); glVertex3fv(dcp[4]);
    glNormal3fv(dcp[4]); glTexCoord2fv(tscp[5]); glVertex3fv(dcp[5]);
    glNormal3fv(dcp[6]); glTexCoord2fv(tscp[6]); glVertex3fv(dcp[6]);
    glNormal3fv(dcp[6]); glTexCoord2fv(tscp[7]); glVertex3fv(dcp[7]);
    glNormal3fv(dcp[8]); glTexCoord2fv(tscp[8]); glVertex3fv(dcp[8]);
    glNormal3fv(dcp[8]); glTexCoord2fv(tscp[9]); glVertex3fv(dcp[9]);
    glNormal3fv(dcp[10]); glTexCoord2fv(tscp[10]); glVertex3fv(dcp[10]);
    glNormal3fv(dcp[10]); glTexCoord2fv(tscp[11]); glVertex3fv(dcp[11]);
    glNormal3fv(dcp[12]); glTexCoord2fv(tscp[12]); glVertex3fv(dcp[12]);
    glNormal3fv(dcp[12]); glTexCoord2fv(tscp[13]); glVertex3fv(dcp[13]);
    glNormal3fv(dcp[14]); glTexCoord2fv(tscp[14]); glVertex3fv(dcp[14]);
    glNormal3fv(dcp[14]); glTexCoord2fv(tscp[15]); glVertex3fv(dcp[15]);
    glNormal3fv(dcp[16]); glTexCoord2fv(tscp[16]); glVertex3fv(dcp[16]);
    glNormal3fv(dcp[16]); glTexCoord2fv(tscp[17]); glVertex3fv(dcp[17]);
    glEnd();

    glEndList();
}

static void BuildElbow(void)
{

    glNewList(elbow, GL_COMPILE);

    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[0][0]); glTexCoord2fv(tep[0][0]); glVertex3fv(ep[0][0]);
    glNormal3fv(en[1][0]); glTexCoord2fv(tep[1][0]); glVertex3fv(ep[1][0]);
    glNormal3fv(en[0][1]); glTexCoord2fv(tep[0][1]); glVertex3fv(ep[0][1]);
    glNormal3fv(en[1][1]); glTexCoord2fv(tep[1][1]); glVertex3fv(ep[1][1]);
    glNormal3fv(en[0][2]); glTexCoord2fv(tep[0][2]); glVertex3fv(ep[0][2]);
    glNormal3fv(en[1][2]); glTexCoord2fv(tep[1][2]); glVertex3fv(ep[1][2]);
    glNormal3fv(en[0][3]); glTexCoord2fv(tep[0][3]); glVertex3fv(ep[0][3]);
    glNormal3fv(en[1][3]); glTexCoord2fv(tep[1][3]); glVertex3fv(ep[1][3]);
    glNormal3fv(en[0][4]); glTexCoord2fv(tep[0][4]); glVertex3fv(ep[0][4]);
    glNormal3fv(en[1][4]); glTexCoord2fv(tep[1][4]); glVertex3fv(ep[1][4]);
    glNormal3fv(en[0][5]); glTexCoord2fv(tep[0][5]); glVertex3fv(ep[0][5]);
    glNormal3fv(en[1][5]); glTexCoord2fv(tep[1][5]); glVertex3fv(ep[1][5]);
    glNormal3fv(en[0][6]); glTexCoord2fv(tep[0][6]); glVertex3fv(ep[0][6]);
    glNormal3fv(en[1][6]); glTexCoord2fv(tep[1][6]); glVertex3fv(ep[1][6]);
    glNormal3fv(en[0][7]); glTexCoord2fv(tep[0][7]); glVertex3fv(ep[0][7]);
    glNormal3fv(en[1][7]); glTexCoord2fv(tep[1][7]); glVertex3fv(ep[1][7]);
    glNormal3fv(en[0][8]); glTexCoord2fv(tep[0][8]); glVertex3fv(ep[0][8]);
    glNormal3fv(en[1][8]); glTexCoord2fv(tep[1][8]); glVertex3fv(ep[1][8]);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[1][0]); glTexCoord2fv(tep[1][0]); glVertex3fv(ep[1][0]);
    glNormal3fv(en[2][0]); glTexCoord2fv(tep[2][0]); glVertex3fv(ep[2][0]);
    glNormal3fv(en[1][1]); glTexCoord2fv(tep[1][1]); glVertex3fv(ep[1][1]);
    glNormal3fv(en[2][1]); glTexCoord2fv(tep[2][1]); glVertex3fv(ep[2][1]);
    glNormal3fv(en[1][2]); glTexCoord2fv(tep[1][2]); glVertex3fv(ep[1][2]);
    glNormal3fv(en[2][2]); glTexCoord2fv(tep[2][2]); glVertex3fv(ep[2][2]);
    glNormal3fv(en[1][3]); glTexCoord2fv(tep[1][3]); glVertex3fv(ep[1][3]);
    glNormal3fv(en[2][3]); glTexCoord2fv(tep[2][3]); glVertex3fv(ep[2][3]);
    glNormal3fv(en[1][4]); glTexCoord2fv(tep[1][4]); glVertex3fv(ep[1][4]);
    glNormal3fv(en[2][4]); glTexCoord2fv(tep[2][4]); glVertex3fv(ep[2][4]);
    glNormal3fv(en[1][5]); glTexCoord2fv(tep[1][5]); glVertex3fv(ep[1][5]);
    glNormal3fv(en[2][5]); glTexCoord2fv(tep[2][5]); glVertex3fv(ep[2][5]);
    glNormal3fv(en[1][6]); glTexCoord2fv(tep[1][6]); glVertex3fv(ep[1][6]);
    glNormal3fv(en[2][6]); glTexCoord2fv(tep[2][6]); glVertex3fv(ep[2][6]);
    glNormal3fv(en[1][7]); glTexCoord2fv(tep[1][7]); glVertex3fv(ep[1][7]);
    glNormal3fv(en[2][7]); glTexCoord2fv(tep[2][7]); glVertex3fv(ep[2][7]);
    glNormal3fv(en[1][8]); glTexCoord2fv(tep[1][8]); glVertex3fv(ep[1][8]);
    glNormal3fv(en[2][8]); glTexCoord2fv(tep[2][8]); glVertex3fv(ep[2][8]);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[2][0]); glTexCoord2fv(tep[2][0]); glVertex3fv(ep[2][0]);
    glNormal3fv(en[3][0]); glTexCoord2fv(tep[3][0]); glVertex3fv(ep[3][0]);
    glNormal3fv(en[2][1]); glTexCoord2fv(tep[2][1]); glVertex3fv(ep[2][1]);
    glNormal3fv(en[3][1]); glTexCoord2fv(tep[3][1]); glVertex3fv(ep[3][1]);
    glNormal3fv(en[2][2]); glTexCoord2fv(tep[2][2]); glVertex3fv(ep[2][2]);
    glNormal3fv(en[3][2]); glTexCoord2fv(tep[3][2]); glVertex3fv(ep[3][2]);
    glNormal3fv(en[2][3]); glTexCoord2fv(tep[2][3]); glVertex3fv(ep[2][3]);
    glNormal3fv(en[3][3]); glTexCoord2fv(tep[3][3]); glVertex3fv(ep[3][3]);
    glNormal3fv(en[2][4]); glTexCoord2fv(tep[2][4]); glVertex3fv(ep[2][4]);
    glNormal3fv(en[3][4]); glTexCoord2fv(tep[3][4]); glVertex3fv(ep[3][4]);
    glNormal3fv(en[2][5]); glTexCoord2fv(tep[2][5]); glVertex3fv(ep[2][5]);
    glNormal3fv(en[3][5]); glTexCoord2fv(tep[3][5]); glVertex3fv(ep[3][5]);
    glNormal3fv(en[2][6]); glTexCoord2fv(tep[2][6]); glVertex3fv(ep[2][6]);
    glNormal3fv(en[3][6]); glTexCoord2fv(tep[3][6]); glVertex3fv(ep[3][6]);
    glNormal3fv(en[2][7]); glTexCoord2fv(tep[2][7]); glVertex3fv(ep[2][7]);
    glNormal3fv(en[3][7]); glTexCoord2fv(tep[3][7]); glVertex3fv(ep[3][7]);
    glNormal3fv(en[2][8]); glTexCoord2fv(tep[2][8]); glVertex3fv(ep[2][8]);
    glNormal3fv(en[3][8]); glTexCoord2fv(tep[3][8]); glVertex3fv(ep[3][8]);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[3][0]); glTexCoord2fv(tep[3][0]); glVertex3fv(ep[3][0]);
    glNormal3fv(en[4][0]); glTexCoord2fv(tep[4][0]); glVertex3fv(ep[4][0]);
    glNormal3fv(en[3][1]); glTexCoord2fv(tep[3][1]); glVertex3fv(ep[3][1]);
    glNormal3fv(en[4][1]); glTexCoord2fv(tep[4][1]); glVertex3fv(ep[4][1]);
    glNormal3fv(en[3][2]); glTexCoord2fv(tep[3][2]); glVertex3fv(ep[3][2]);
    glNormal3fv(en[4][2]); glTexCoord2fv(tep[4][2]); glVertex3fv(ep[4][2]);
    glNormal3fv(en[3][3]); glTexCoord2fv(tep[3][3]); glVertex3fv(ep[3][3]);
    glNormal3fv(en[4][3]); glTexCoord2fv(tep[4][3]); glVertex3fv(ep[4][3]);
    glNormal3fv(en[3][4]); glTexCoord2fv(tep[3][4]); glVertex3fv(ep[3][4]);
    glNormal3fv(en[4][4]); glTexCoord2fv(tep[4][4]); glVertex3fv(ep[4][4]);
    glNormal3fv(en[3][5]); glTexCoord2fv(tep[3][5]); glVertex3fv(ep[3][5]);
    glNormal3fv(en[4][5]); glTexCoord2fv(tep[4][5]); glVertex3fv(ep[4][5]);
    glNormal3fv(en[3][6]); glTexCoord2fv(tep[3][6]); glVertex3fv(ep[3][6]);
    glNormal3fv(en[4][6]); glTexCoord2fv(tep[4][6]); glVertex3fv(ep[4][6]);
    glNormal3fv(en[3][7]); glTexCoord2fv(tep[3][7]); glVertex3fv(ep[3][7]);
    glNormal3fv(en[4][7]); glTexCoord2fv(tep[4][7]); glVertex3fv(ep[4][7]);
    glNormal3fv(en[3][8]); glTexCoord2fv(tep[3][8]); glVertex3fv(ep[3][8]);
    glNormal3fv(en[4][8]); glTexCoord2fv(tep[4][8]); glVertex3fv(ep[4][8]);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[4][0]); glTexCoord2fv(tep[4][0]); glVertex3fv(ep[4][0]);
    glNormal3fv(en[5][0]); glTexCoord2fv(tep[5][0]); glVertex3fv(ep[5][0]);
    glNormal3fv(en[4][1]); glTexCoord2fv(tep[4][1]); glVertex3fv(ep[4][1]);
    glNormal3fv(en[5][1]); glTexCoord2fv(tep[5][1]); glVertex3fv(ep[5][1]);
    glNormal3fv(en[4][2]); glTexCoord2fv(tep[4][2]); glVertex3fv(ep[4][2]);
    glNormal3fv(en[5][2]); glTexCoord2fv(tep[5][2]); glVertex3fv(ep[5][2]);
    glNormal3fv(en[4][3]); glTexCoord2fv(tep[4][3]); glVertex3fv(ep[4][3]);
    glNormal3fv(en[5][3]); glTexCoord2fv(tep[5][3]); glVertex3fv(ep[5][3]);
    glNormal3fv(en[4][4]); glTexCoord2fv(tep[4][4]); glVertex3fv(ep[4][4]);
    glNormal3fv(en[5][4]); glTexCoord2fv(tep[5][4]); glVertex3fv(ep[5][4]);
    glNormal3fv(en[4][5]); glTexCoord2fv(tep[4][5]); glVertex3fv(ep[4][5]);
    glNormal3fv(en[5][5]); glTexCoord2fv(tep[5][5]); glVertex3fv(ep[5][5]);
    glNormal3fv(en[4][6]); glTexCoord2fv(tep[4][6]); glVertex3fv(ep[4][6]);
    glNormal3fv(en[5][6]); glTexCoord2fv(tep[5][6]); glVertex3fv(ep[5][6]);
    glNormal3fv(en[4][7]); glTexCoord2fv(tep[4][7]); glVertex3fv(ep[4][7]);
    glNormal3fv(en[5][7]); glTexCoord2fv(tep[5][7]); glVertex3fv(ep[5][7]);
    glNormal3fv(en[4][8]); glTexCoord2fv(tep[4][8]); glVertex3fv(ep[4][8]);
    glNormal3fv(en[5][8]); glTexCoord2fv(tep[5][8]); glVertex3fv(ep[5][8]);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(en[5][0]); glTexCoord2fv(tep[5][0]); glVertex3fv(ep[5][0]);
    glNormal3fv(en[6][0]); glTexCoord2fv(tep[6][0]); glVertex3fv(ep[6][0]);
    glNormal3fv(en[5][1]); glTexCoord2fv(tep[5][1]); glVertex3fv(ep[5][1]);
    glNormal3fv(en[6][1]); glTexCoord2fv(tep[6][1]); glVertex3fv(ep[6][1]);
    glNormal3fv(en[5][2]); glTexCoord2fv(tep[5][2]); glVertex3fv(ep[5][2]);
    glNormal3fv(en[6][2]); glTexCoord2fv(tep[6][2]); glVertex3fv(ep[6][2]);
    glNormal3fv(en[5][3]); glTexCoord2fv(tep[5][3]); glVertex3fv(ep[5][3]);
    glNormal3fv(en[6][3]); glTexCoord2fv(tep[6][3]); glVertex3fv(ep[6][3]);
    glNormal3fv(en[5][4]); glTexCoord2fv(tep[5][4]); glVertex3fv(ep[5][4]);
    glNormal3fv(en[6][4]); glTexCoord2fv(tep[6][4]); glVertex3fv(ep[6][4]);
    glNormal3fv(en[5][5]); glTexCoord2fv(tep[5][5]); glVertex3fv(ep[5][5]);
    glNormal3fv(en[6][5]); glTexCoord2fv(tep[6][5]); glVertex3fv(ep[6][5]);
    glNormal3fv(en[5][6]); glTexCoord2fv(tep[5][6]); glVertex3fv(ep[5][6]);
    glNormal3fv(en[6][6]); glTexCoord2fv(tep[6][6]); glVertex3fv(ep[6][6]);
    glNormal3fv(en[5][7]); glTexCoord2fv(tep[5][7]); glVertex3fv(ep[5][7]);
    glNormal3fv(en[6][7]); glTexCoord2fv(tep[6][7]); glVertex3fv(ep[6][7]);
    glNormal3fv(en[5][8]); glTexCoord2fv(tep[5][8]); glVertex3fv(ep[5][8]);
    glNormal3fv(en[6][8]); glTexCoord2fv(tep[6][8]); glVertex3fv(ep[6][8]);
    glEnd();

    glEndList();
}

static void BuildLogo(void)
{

    glNewList(logo, GL_COMPILE);

    glTranslatef(5.5, -3.5, 4.5);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendRight();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendLeft();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendRight();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendLeft();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendRight();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -7.0);
    glCallList(doubleCylinder);
    BendForward();
    glCallList(elbow);
    glTranslatef(0.0, 0.0, -5.0);
    glCallList(singleCylinder);
    BendLeft();
    glCallList(elbow);

    glEndList();
}

static void BuildLists(void)
{

    singleCylinder = glGenLists(1);
    doubleCylinder = glGenLists(1);
    elbow = glGenLists(1);
    logo = glGenLists(1);

    BuildSingleCylinder();
    BuildDoubleCylinder();
    BuildElbow();
    BuildLogo();
}

static void Init(void)
{
    static float ambient[] = {0.1, 0.1, 0.1, 1.0};
    static float diffuse[] = {0.5, 1.0, 1.0, 1.0};
    static float position[] = {90.0, 90.0, 150.0, 0.0};
    static float front_mat_shininess[] = {30.0};
    static float front_mat_specular[] = {0.2, 0.2, 0.2, 1.0};
    static float front_mat_diffuse[] = {0.5, 0.28, 0.38, 1.0};
    static float back_mat_shininess[] = {50.0};
    static float back_mat_specular[] = {0.5, 0.5, 0.2, 1.0};
    static float back_mat_diffuse[] = {1.0, 1.0, 0.2, 1.0};
    static float lmodel_ambient[] = {1.0, 1.0, 1.0, 1.0};
    static float lmodel_twoside[] = {GL_TRUE};

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glFrontFace(GL_CW);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);

    glEnable(GL_CLIP_PLANE0);

    if (rgb) {
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, CHECKIMAGEWIDTH, CHECKIMAGEHEIGHT, 0,
             GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)checkImage);
    glEnable(GL_TEXTURE_2D);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    } else {
    auxSetGreyRamp();
    /*
    if (doubleBuffer) {
        colorIndexes[1] = 10;
        colorIndexes[2] = 15;
    } */

    glMaterialiv(GL_FRONT_AND_BACK, GL_COLOR_INDEXES, colorIndexes);
    }

    BuildLists();

    dithering = GL_TRUE;
    shade = GL_TRUE;
    doStipple = GL_FALSE;
    polyMode = GL_BACK;
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_LEFT(void)
{
    yRotation += 0.5;
}

static void CALLBACK Key_RIGHT(void)
{
    yRotation -= 0.5;
}

static void CALLBACK Key_UP(void)
{
    plane[3] += 2.0;
}

static void CALLBACK Key_DOWN(void)
{
    plane[3] -= 2.0;
}

static void CALLBACK Key_Z(void)
{
    zTranslation -= 1.0;
}

static void CALLBACK Key_z(void)
{
    zTranslation += 1.0;
}

static void CALLBACK Key_1(void)
{
    glPolygonMode(polyMode, GL_POINT);
}

static void CALLBACK Key_2(void)
{
    glPolygonMode(polyMode, GL_LINE);
}

static void CALLBACK Key_3(void)
{
    glPolygonMode(polyMode, GL_FILL);
}

static void CALLBACK Key_p(void)
{
    switch (polyMode) {
      case GL_BACK:
        polyMode = GL_FRONT;
        break;
      case GL_FRONT:
        polyMode = GL_FRONT_AND_BACK;
        break;
      case GL_FRONT_AND_BACK:
        polyMode = GL_BACK;
        break;
    }
}

static void CALLBACK Key_4(void)
{
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

static void CALLBACK Key_5(void)
{
    glEnable(GL_POLYGON_SMOOTH);
    if (rgb) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    } else {
        SetUpAntiAliasedGrayScale();
    }
}

static void CALLBACK Key_6(void)
{
    glDisable(GL_POLYGON_SMOOTH);
    if (rgb) {
        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    } else {
        auxSetGreyRamp();
    }
}

static void CALLBACK Key_8(void)
{
    dithering = !dithering;
    (dithering) ? glEnable(GL_DITHER) : glDisable(GL_DITHER);
}

static void CALLBACK Key_9(void)
{
    doStipple = !doStipple;
    if (doStipple) {
        glPolygonStipple(stipple);
        glEnable(GL_POLYGON_STIPPLE);
    } else {
        glDisable(GL_POLYGON_STIPPLE);
    }
}

static void CALLBACK Key_0(void)
{
    shade = !shade;
    (shade) ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);
}

static void CALLBACK Key_q(void)
{
    glDisable(GL_CULL_FACE);
}

static void CALLBACK Key_w(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

static void CALLBACK Key_e(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

static void CALLBACK Key_r(void)
{
    glFrontFace(GL_CW);
}

static void CALLBACK Key_t(void)
{
    glFrontFace(GL_CCW);
}

static void CALLBACK Key_y(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_LSB_FIRST, 0);
    glPolygonStipple(stipple);
}

static void CALLBACK Key_u(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_LSB_FIRST, 1);
    glPolygonStipple(stipple);
}

static void CALLBACK Key_a(void)
{
    glEnable(GL_TEXTURE_2D);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, BRICKIMAGEWIDTH,
             BRICKIMAGEHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE,
             (GLvoid *)brickImage);
}

static void CALLBACK Key_s(void)
{
    glEnable(GL_TEXTURE_2D);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, CHECKIMAGEWIDTH,
             CHECKIMAGEHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
             (GLvoid *)checkImage);
}

static void CALLBACK Key_d(void)
{
    glDisable(GL_TEXTURE_2D);
}

static void CALLBACK Key_f(void)
{
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
}

static void CALLBACK Key_g(void)
{
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modulate);
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(0, 0, zTranslation);
    glRotatef(30.0, 1, 0, 0);
    glRotatef(yRotation, 0, 1, 0);
    glClipPlane(GL_CLIP_PLANE0, plane);
    glCallList(logo);

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
    directRender = GL_FALSE;

    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-ci") == 0) {
        rgb = GL_FALSE;
    } else if (strcmp(argv[i], "-rgb") == 0) {
        rgb = GL_TRUE;
    } else if (strcmp(argv[i], "-sb") == 0) {
        doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
        doubleBuffer = GL_TRUE;
    } else if (strcmp(argv[i], "-dr") == 0) {
        directRender = GL_TRUE;
    } else if (strcmp(argv[i], "-ir") == 0) {
        directRender = GL_FALSE;
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

    auxInitPosition(0, 0, 300, 300);

    type = AUX_DEPTH16;
    type |= (rgb) ? AUX_RGB : AUX_INDEX;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;
    auxInitDisplayMode(type);

    if (auxInitWindow("Logo Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxKeyFunc(AUX_Z, Key_Z);
    auxKeyFunc(AUX_z, Key_z);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_3, Key_3);
    auxKeyFunc(AUX_p, Key_p);
    auxKeyFunc(AUX_4, Key_4);
    auxKeyFunc(AUX_5, Key_5);
    auxKeyFunc(AUX_6, Key_6);
    auxKeyFunc(AUX_8, Key_8);
    auxKeyFunc(AUX_9, Key_9);
    auxKeyFunc(AUX_0, Key_0);
    auxKeyFunc(AUX_q, Key_q);
    auxKeyFunc(AUX_w, Key_w);
    auxKeyFunc(AUX_e, Key_e);
    auxKeyFunc(AUX_r, Key_r);
    auxKeyFunc(AUX_t, Key_t);
    auxKeyFunc(AUX_y, Key_y);
    auxKeyFunc(AUX_u, Key_u);
    auxKeyFunc(AUX_a, Key_a);
    auxKeyFunc(AUX_s, Key_s);
    auxKeyFunc(AUX_d, Key_d);
    auxKeyFunc(AUX_f, Key_f);
    auxKeyFunc(AUX_g, Key_g);
    auxMainLoop(Draw);
}
