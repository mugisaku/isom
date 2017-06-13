#ifndef ISOM_RENDERER_HPP_INCLUDED
#define ISOM_RENDERER_HPP_INCLUDED


#include<cstdint>
#include<vector>
#include"isom_point.hpp"
#include"isom_vector.hpp"
#include"isom_image.hpp"
#include"isom_LineContext.hpp"




struct Plane;


struct
Cell: public Color
{
  int32_t  z;

  const Plane*  plane;

};


extern Vector  light;


class
Renderer
{
  std::vector<Cell>  table;

  int  width;
  int  height;

public:
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

  void  put(const Color&  color, int  x, int  y, int  z);
  void  put(const LineContext&  lc, const Color&  color);

  void  render_line(const Point&  p0, const Point&  p1, const Color&  color);
  void  render_image(const Image&  src, const Rect*  src_rect, int  src_z, int  dst_x, int  dst_y);

};



#endif




