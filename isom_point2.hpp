#ifndef ISOM_POINT2_HPP_INCLUDED
#define ISOM_POINT2_HPP_INCLUDED




struct
Point2
{
  int  x;
  int  y;

  constexpr Point2(int  x_=0, int  y_=0):
  x(x_), y(y_){}


  Point2  operator+(const Point2&  rhs) const;
  Point2  operator-(const Point2&  rhs) const;
  Point2  operator*(double  rate) const;

  Point2&  operator+=(const Point2&  rhs);
  Point2&  operator-=(const Point2&  rhs);
  Point2&  operator*=(double  rate);

  void  assign(int  x_, int  y_);

  void  print(const char*  s="") const;

};


struct
Rect
{
  int  x;
  int  y;
  int  w;
  int  h;

  constexpr Rect(int  x_=0, int  y_=0, int  w_=0, int  h_=0):
  x(x_),
  y(y_),
  w(w_),
  h(h_)
  {}

};


#endif




