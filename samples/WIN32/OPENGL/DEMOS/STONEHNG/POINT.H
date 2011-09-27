#ifndef POINT_H
#define POINT_H

#ifndef POINT_EXTERN
#define POINT_EXTERN extern
#endif 

#include <math.h>

const float point_fudge = .000001;

class Point {
 public:
  inline Point operator=(Point a);
  inline Point operator=(GLfloat *a);
  inline Point operator+(Point a);
  inline Point operator+=(Point a);
  inline Point operator-(Point a);
  // This takes a cross-product
  inline Point operator*(Point b);
  inline Point operator*(GLfloat b);
  inline Point operator*=(GLfloat b);
  inline Point operator/(GLfloat b);
  inline Point operator/=(GLfloat b);
  inline GLfloat& operator[](int index);
  
  inline void set(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0);
  
  inline GLfloat dist(Point b);
  inline GLfloat dot(Point b);
  inline GLfloat dot(GLfloat x, GLfloat y, GLfloat z);
  inline GLfloat mag();
  inline GLfloat magsquared();
  inline Point unit();
  inline void unitize();
  
  inline Point scale(Point p);
  
  // Angle is in RADIANS
  inline Point rotate(Point axis, GLfloat angle);
  inline Point rotate(Point axis, GLfloat c, GLfloat s);
  inline void rotate_self(Point axis, GLfloat c, GLfloat s);
  Point rotate_abouty(GLfloat c, GLfloat s);
  Point rotate_aboutz(GLfloat c, GLfloat s);
  
  // Returns point projected through proj_pt into XY plane
  // Does nothing if proj_pt - *this is parallel to XY plane
  inline Point project(Point proj_pt);
  inline void project_self(Point proj_pt);
  inline void Point::project_self(GLfloat px, GLfloat py, GLfloat pz);
  inline Point project_direction(Point direction);
  inline Point Point::project_direction(GLfloat x, GLfloat y, GLfloat z);
  // This projects (px, py, pz) into this in direction (dx, dy, dz)
  inline void Point::compute_projected(GLfloat px, GLfloat py, GLfloat pz,
					 GLfloat x, GLfloat y, GLfloat z);

  // Returns point projected through light and refracted into XY
  // plane.  
  // N is normal at point (ie normal at *this)
  // I is the index of refraction
  inline Point refract(Point light, Point N, GLfloat I);
  void refract_self(Point light, Point N, GLfloat I);
  Point refract_direction(Point light, Point N, GLfloat I);
  
  inline void glvertex();
  inline void glnormal();
  
  void print();
  void print(const char *format);
  
  GLfloat pt[4];
 private:
};

POINT_EXTERN Point val;

#define DOT(a, b) (a.pt[0]*b.pt[0] + a.pt[1]*b.pt[1] + a.pt[2]*b.pt[2])
#define THIS_DOT(b) (pt[0]*b.pt[0] + pt[1]*b.pt[1] + pt[2]*b.pt[2])

inline Point Point::operator=(Point a)
{
  pt[0] = a.pt[0];
  pt[1] = a.pt[1];
  pt[2] = a.pt[2];
  pt[3] = a.pt[3];
  return *this;
}

inline Point Point::operator=(GLfloat *a)
{
  pt[0] = a[0]; 
  pt[1] = a[1]; 
  pt[2] = a[2]; 
  pt[3] = 1;
  return *this;
}

inline Point Point::operator+(Point a)
{  
  val.pt[0] = pt[0] + a.pt[0];
  val.pt[1] = pt[1] + a.pt[1];
  val.pt[2] = pt[2] + a.pt[2]; 
  return val; 
}

inline Point Point::operator+=(Point a)
{
  pt[0] += a.pt[0];
  pt[1] += a.pt[1];
  pt[2] += a.pt[2];
  return *this;
}

inline Point Point::operator-(Point a)
{
  val.pt[0] = pt[0] - a.pt[0];
  val.pt[1] = pt[1] - a.pt[1];
  val.pt[2] = pt[2] - a.pt[2];
  return val;
}
  
inline Point Point::operator*(Point b)
{
  val.pt[0] = pt[1]*b.pt[2] - b.pt[1]*pt[2];
  val.pt[1] = pt[2]*b.pt[0] - b.pt[2]*pt[0];
  val.pt[2] = pt[0]*b.pt[1] - pt[1]*b.pt[0];
  return val;
}

inline Point Point::operator*(GLfloat b)
{
  val.pt[0] = pt[0] * b;
  val.pt[1] = pt[1] * b;
  val.pt[2] = pt[2] * b;
  return val;
}

inline Point Point::operator*=(GLfloat b)
{
  pt[0] *= b;
  pt[1] *= b;
  pt[2] *= b;
  return *this;
}

inline Point Point::operator/(GLfloat b)
{
  val.pt[0] = pt[0] / b;
  val.pt[1] = pt[1] / b;
  val.pt[2] = pt[2] / b;
  return val;
}

inline Point Point::operator/=(GLfloat b)
{
  pt[0] /= b;
  pt[1] /= b;
  pt[2] /= b;
  return *this;
}

inline GLfloat& Point::operator[](int index)
{
  return pt[index];
}

inline void Point::set(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  pt[0] = x;
  pt[1] = y;
  pt[2] = z;
  pt[3] = w;
}

inline GLfloat Point::dist(Point b)
{
  return (*this - b).mag();
}

inline GLfloat Point::dot(Point b)
{
  return pt[0]*b.pt[0] + pt[1]*b.pt[1] + pt[2]*b.pt[2];
}

inline GLfloat Point::dot(GLfloat x, GLfloat y, GLfloat z)
{ 
  return pt[0]*x + pt[1]*y + pt[2]*z;
}

inline GLfloat Point::mag()
{
  return sqrt(pt[0]*pt[0] + pt[1]*pt[1] + 
              pt[2]*pt[2]);
}

inline GLfloat Point::magsquared()
{
  return pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2];
}

inline Point Point::unit()
{
  GLfloat m = sqrt(pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2]);
  val.pt[0] = pt[0] / m;
  val.pt[1] = pt[1] / m;
  val.pt[2] = pt[2] / m;
  return val;
}

inline void Point::unitize()
{
  GLfloat m = sqrt(pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2]);
  pt[0] /= m;
  pt[1] /= m;
  pt[2] /= m;
}

inline Point Point::scale(Point p) 
{
  val.pt[0] = pt[0] * p.pt[0];
  val.pt[1] = pt[1] * p.pt[1];
  val.pt[2] = pt[2] * p.pt[2];
  return val;
}

inline Point Point::rotate(Point axis, GLfloat angle)
{
  return rotate(axis, cos(angle), sin(angle));
}

inline Point Point::rotate(Point axis, GLfloat c, GLfloat s)
{
  float x = axis.pt[0], y = axis.pt[1], z = axis.pt[2], t = 1.0 - c;
  float tx, ty;
  
  tx = t*x;
  /* Taking advantage of inside info that this is a common case */
  if (y == 0.0) {
    val.pt[0] = pt[0]*(tx*x + c) + pt[1]*(-s*z) + pt[2]*(tx*z);
    val.pt[1] = pt[0]*(s*z) + pt[1]*c + pt[2]*(-s*x);
    val.pt[2] = pt[0]*(tx*z) + pt[1]*s*x + pt[2]*(t*z*z + c);
  } else {
    ty = t*y;
    val.pt[0] = pt[0]*(tx*x + c) + pt[1]*(tx*y - s*z) +
      pt[2]*(tx*z + s*y);
    val.pt[1] = pt[0]*(tx*y + s*z) + pt[1]*(ty*y + c) +
      pt[2]*(ty*z - s*x);
    val.pt[2] = pt[0]*(tx*z - s*y) + pt[1]*(ty*z + s*x) +
      pt[2]*(t*z*z + c);
  }
  return val;
}

inline void Point::rotate_self(Point axis, GLfloat c, GLfloat s)
{
  float Px, Py, Pz;
  float x = axis.pt[0], y = axis.pt[1], z = axis.pt[2], t = 1.0 - c;
  float tx, ty;
  
  tx = t*x;
  Px = pt[0];
  Py = pt[1];
  Pz = pt[2];
  /* Taking advantage of inside info that this is a common case */
  if (!y) {
    pt[0] = Px*(tx*x + c) +	Py*(-s*z) + 	Pz*(tx*z);
    pt[1] = Px*(s*z) + 		Py*c + 		Pz*(-s*x);
    pt[2] = Px*(tx*z) + 	Py*s*x + 	Pz*(t*z*z + c);
  } else {
    ty = t*y;
    pt[0] = Px*(tx*x + c) + 	Py*(tx*y - s*z) +
      Pz*(tx*z + s*y);
    pt[1] = Px*(tx*y + s*z) + 	Py*(ty*y + c) +
      Pz*(ty*z - s*x);
    pt[2] = Px*(tx*z - s*y) + 	Py*(ty*z + s*x) +
      Pz*(t*z*z + c);
  }
}  

inline void Point::glvertex() 
{
  glVertex3fv(pt);
}

inline void Point::glnormal()
{
  glNormal3fv(pt);
}

inline Point Point::project(Point proj_pt)
{
  GLfloat dirx = pt[0] - proj_pt.pt[0], 
      diry = pt[1] - proj_pt.pt[1],
      dirz = pt[2] - proj_pt.pt[2];
  GLfloat t;

  if (fabs(dirz) < point_fudge) val = *this;
  else {
    t = -proj_pt.pt[2] / dirz;
    val.pt[0] = proj_pt.pt[0] + dirx*t;
    val.pt[1] = proj_pt.pt[1] + diry*t;
    val.pt[2] = 0.0;
  }
  return val;
}

// This naively assumes that proj_pt[z] != this->pt[z]
inline void Point::project_self(Point proj_pt)
{
  GLfloat dirx = pt[0] - proj_pt.pt[0], 
      diry = pt[1] - proj_pt.pt[1],
      dirz = pt[2] - proj_pt.pt[2];
  GLfloat t;

  t = -proj_pt.pt[2] / dirz;
  pt[0] = proj_pt.pt[0] + dirx*t;
  pt[1] = proj_pt.pt[1] + diry*t;
  pt[2] = 0.0;
}

inline void Point::project_self(GLfloat px, GLfloat py, GLfloat pz)
{
  GLfloat dirx = pt[0] - px, 
  diry = pt[1] - py,
  dirz = pt[2] - pz, t;

  t = -pz / dirz;
  pt[0] = px + dirx*t;
  pt[1] = py + diry*t;
  pt[2] = 0.0;
}

inline Point Point::project_direction(Point direction) {
  GLfloat t;

  t = -pt[2] / direction.pt[2];
  val.pt[0] = pt[0] + direction.pt[0]*t;
  val.pt[1] = pt[1] + direction.pt[1]*t;
  val.pt[2] = 0;
  return val;
}

inline Point Point::project_direction(GLfloat x, GLfloat y, GLfloat z) 
{
  GLfloat t;

  t = -pt[2] / z;
  val.pt[0] = pt[0] + x*t;
  val.pt[1] = pt[1] + y*t;
  val.pt[2] = 0;
  return val;
}

inline void Point::compute_projected(GLfloat px, GLfloat py, GLfloat pz,
				     GLfloat dx, GLfloat dy, GLfloat dz)
{
  GLfloat t = -pz / dz;
  pt[0] = px + dx*t;
  pt[1] = py + dy*t;
  pt[2] = 0;
}


#undef POINT_EXTERN

#endif
