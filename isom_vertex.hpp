#ifndef ISOM_Vertex_HPP_INCLUDED
#define ISOM_Vertex_HPP_INCLUDED


#include"isom_point.hpp"




struct
Vertex: public Point
{
  int  u;
  int  v;

  constexpr Vertex(int  x_=0, int  y_=0, int  z_=0, int  u_=0, int  v_=0):
  Point(x_,y_,z_), u(u_), v(v_){}

  constexpr Vertex(const Point&  pt, int  u_, int  v_):
  Point(pt), u(u_), v(v_){}

  void  assign(int  x_, int  y_, int  z_, int  u_, int  v_)
  {
    x = x_;
    y = y_;
    z = z_;
    u = u_;
    v = v_;
  }

  void  assign(const Point&  pt, int  u_, int  v_)
  {
    static_cast<Point&>(*this) = pt;

    u = u_;
    v = v_;
  }

  void  set_uv(int  u_, int  v_)
  {
    u = u_;
    v = v_;
  }

};


using VertexString = std::vector<Vertex>;



#endif




