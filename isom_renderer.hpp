#ifndef ISOM_RENDERER_HPP_INCLUDED
#define ISOM_RENDERER_HPP_INCLUDED


#include<cstdint>
#include<vector>
#include"isom_point.hpp"
#include"isom_image.hpp"
#include"isom_LineContext.hpp"




struct Plane;


struct
Cell: public Color
{
  int32_t  z;

  const Plane*  plane;

};


struct
TextureVertex: public  Point
{
  int  u;
  int  v;

  constexpr TextureVertex(const Point&  pt, int  u_=0, int  v_=0):
  Point(pt), u(u_), v(v_){}

};


class
Renderer
{
  std::vector<Cell>  table;

  int  width;
  int  height;

public:
  Point  offset;

  Renderer(int  w=0, int  h=0);

  void  clear();

  void  resize(int  w, int  h);

  int   get_width() const;
  int   get_height() const;

  bool  test(int  x, int  y) const
  {
    return((x >=     0) &&
           (y >=     0) &&
           (x <  width) &&
           (y < height));
  }

  void   set_cell(const Cell&  src, int  x, int  y);

        Cell&  get_cell(int  x, int  y)      ;
  const Cell&  get_cell(int  x, int  y) const;

  void  render_line(const Point&  a, const Point&  b, const Color&  color);
  void  render_polygon_line(LineContext&  longer, LineContext&  shorter, const Color&  color);
  void  render_polygon(const Point&  a, const Point&  b, const Point&  c, const Color&  color);

  void  render_texture_line(const Image&  img, const Point&  a, const Point&  b, const Point&  va, const Point&  vb);
  void  render_texture_line(const Image&  img, LineContext&  longer    , LineContext&  shorter,
                                               LineContext&  longer_vtx, LineContext&  shorter_vtx);
  void  render_texture(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c);

};




#endif




