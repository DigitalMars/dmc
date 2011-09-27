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

#include <GL/gl.h>

#include <math.h>
#include <stdio.h>

#define POINT_EXTERN
#include "point.hxx"

// Change this back to inline
Point Point::rotate_abouty(GLfloat c, GLfloat s)
{
  val.pt[0] = c*pt[0] + s*pt[2];
  val.pt[1] = pt[1];
  val.pt[2] = -s*pt[0] + c*pt[2];
  return val;
}



void Point::refract_self(Point light, Point N, GLfloat I)
{
  GLfloat t;
  Point dlight;

  dlight = refract_direction(light, N, I);
  t = -pt[2] / dlight.pt[2]; 
  pt[0] = pt[0] + dlight.pt[0]*t;
  pt[1] = pt[1] + dlight.pt[1]*t;
  pt[2] = 0;
}

Point Point::refract_direction(Point light, Point N, GLfloat I) 
{
  GLfloat cos1, sin1, cos2, sin2, m;
  GLfloat dlight[3], dN[3], axis[3];
  
  /* dlight = (light - *this).unit() * -1.0; */ 
  dlight[0] = pt[0] - light.pt[0]; 
  dlight[1] = pt[1] - light.pt[1]; 
  dlight[2] = pt[2] - light.pt[2]; 
  m = (GLfloat)sqrt((double)(dlight[0]*dlight[0] + dlight[1]*dlight[1] + dlight[2]*dlight[2]));
  dlight[0] /= m;
  dlight[1] /= m;
  dlight[2] /= m;
  
  // dN = N * -1.0; 
  dN[0] = -N.pt[0]; 
  dN[1] = -N.pt[1]; 
  dN[2] = -N.pt[2]; 
  
  // cos1 = dN.dot(dlight); 
  cos1 = dN[0]*dlight[0] + dN[1]*dlight[1] + dN[2]*dlight[2];
  
  if (1.0 - cos1*cos1 < point_fudge) {
    val = dN;
    return val;
  }
  
  // axis = ((dN * dlight) * dN).unit(); 
  val.pt[0] = dN[1]*dlight[2] - dlight[1]*dN[2];
  val.pt[1] = dN[2]*dlight[0] - dlight[2]*dN[0];
  val.pt[2] = dN[0]*dlight[1] - dN[1]*dlight[0];
  axis[0] = val.pt[1]*dN[2] - dN[1]*val.pt[2];
  axis[1] = val.pt[2]*dN[0] - dN[2]*val.pt[0];
  axis[2] = val.pt[0]*dN[1] - val.pt[1]*dN[0];
  m = (GLfloat)sqrt((double)(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]));
  axis[0] /= m;
  axis[1] /= m;
  axis[2] /= m;
  
  if (axis[0]*axis[0] > point_fudge)  
    sin1 = (dlight[0] - dN[0] * cos1) / axis[0]; 
  else if (axis[1]*axis[1] > point_fudge)  
    sin1 = (dlight[1] - dN[1] * cos1) / axis[1]; 
  else sin1 = dlight[2] - dN[2] * cos1; 
  
  sin2 = sin1 / I; 
  cos2 = (sin1*sin1 < 1.0) ? (GLfloat)sqrt((double)(1.0 - sin2*sin2)) : (GLfloat)0;
  
  dlight[0] = dN[0]*cos2 + axis[0]*sin2;
  dlight[1] = dN[1]*cos2 + axis[1]*sin2;
  dlight[2] = dN[2]*cos2 + axis[2]*sin2;
  
  /* I'm not sure this is quite legal */ 
  if (dlight[2] > 0.0) dlight[2] = -dlight[2];   
  
  val = dlight;

  return val;
}

void Point::print()
{
  print("%f %f %f\n");
}

void Point::print(const char *format)
{
  //printf(format, this->pt[0], this->pt[1], this->pt[2], 1.0);
}		    
