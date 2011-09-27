/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */
#ifndef COLOR_H
#define COLOR_H

class Color {
 public:
  inline Color();
  inline Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0);
  
  inline Color operator=(GLfloat *a);
  inline Color operator=(GLfloat a);
  inline Color operator+(Color a);
  inline Color operator+=(Color a);
  inline Color operator*(Color a);
  inline Color operator*(GLfloat a);
  inline Color operator*=(Color a);
  inline Color operator*=(GLfloat *a);
  inline Color operator*=(GLfloat a);
 
  inline GLfloat& operator[](int index);

  inline Color clamp();
  
  inline void glcolor();

  inline void print();
  inline void print(const char *format);

  GLfloat c[4];
};

inline Color::Color()
{
}

inline Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  c[0] = r;
  c[1] = g;
  c[2] = b;
  c[3] = a;
}

inline Color Color::operator=(GLfloat a)
{
  c[0] = c[1] = c[2] = c[3] = a;
  return *this;
}

inline Color Color::operator=(GLfloat *a)
{
  c[0] = a[0];
  c[1] = a[1];
  c[2] = a[2];
  c[3] = a[3];
  return *this;
}

inline Color Color::operator+(Color a) 
{
  Color val;
  val.c[0] = c[0] + a.c[0];
  val.c[1] = c[1] + a.c[1];
  val.c[2] = c[2] + a.c[2];
  val.c[3] = c[3] + a.c[3];
  return val;
}

inline Color Color::operator+=(Color a)
{
  c[0] += a.c[0];
  c[1] += a.c[1];
  c[2] += a.c[2];
  c[3] += a.c[3];
  return *this;
}

inline Color Color::operator*(Color a)
{
  Color val;
  val.c[0] = c[0] * a.c[0];
  val.c[1] = c[1] * a.c[1];
  val.c[2] = c[2] * a.c[2];
  val.c[3] = c[3] * a.c[3];
  return val;
}

inline Color Color::operator*(GLfloat a)
{
  Color val;
  val.c[0] = c[0] * a;
  val.c[1] = c[1] * a;
  val.c[2] = c[2] * a;
  val.c[3] = c[3] * a;
  return val;
}

inline Color Color::operator*=(Color a)
{
  c[0] *= a.c[0];
  c[1] *= a.c[1];
  c[2] *= a.c[2];
  return *this;
}

inline Color Color::operator*=(GLfloat *a) 
{
  c[0] *= a[0];
  c[1] *= a[1];
  c[2] *= a[2];
  return *this;
}

inline Color Color::operator*=(GLfloat a)
{
  c[0] *= a;
  c[1] *= a;
  c[2] *= a;
  c[3] *= a;
  
  return *this;
}

inline GLfloat& Color::operator[](int index)
{
  return c[index];
}

inline Color Color::clamp()
{
  Color val;
  val.c[0] = (GLfloat) (c[0] < 0.0 ? 0.0 : (c[0] > 1.0 ? 1.0 : c[0]));
  val.c[1] = (GLfloat) (c[1] < 0.0 ? 0.0 : (c[1] > 1.0 ? 1.0 : c[1]));
  val.c[2] = (GLfloat) (c[2] < 0.0 ? 0.0 : (c[2] > 1.0 ? 1.0 : c[2]));
  val.c[3] = (GLfloat) (c[3] < 0.0 ? 0.0 : (c[3] > 1.0 ? 1.0 : c[3]));
  return val;
}

inline void Color::glcolor()
{
  glColor4fv(c);
}

inline void Color::print()
{
  print("%f %f %f %f\n");
}

inline void Color::print(const char *format)
{
  printf(format, c[0], c[1], c[2], c[3]);
}

#endif
