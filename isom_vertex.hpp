#ifndef ISOM_Vertex_HPP_INCLUDED
#define ISOM_Vertex_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_color.hpp"
#include<vector>




struct
Vertex: public Point
{
  int  r;//テクスチャ座標のUとしても使用
  int  g;//テクスチャ座標のVとしても使用
  int  b;

  constexpr Vertex(int  x_=0, int  y_=0, int  z_=0, int  r_=0, int  g_=0, int  b_=0):
  Point(x_,y_,z_), r(r_), g(g_), b(b_){}

  constexpr Vertex(const Point&  pt, const Color&  color):
  Point(pt), r(color.r), g(color.g), b(color.b){}

  constexpr int  get_u() const{return r;}
  constexpr int  get_v() const{return g;}

  void  set_color(const Color&  color)
  {
    r = color.r;
    g = color.g;
    b = color.b;
  }

  void  set_uv(int  u_, int  v_)
  {
    r = u_;
    g = v_;
  }

};


using VertexString = std::vector<Vertex>;



#endif




