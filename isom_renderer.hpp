#ifndef ISOM_RENDERER_HPP_INCLUDED
#define ISOM_RENDERER_HPP_INCLUDED


#include<cstdint>
#include<vector>
#include<string>
#include"isom_point.hpp"
#include"isom_vector.hpp"
#include"isom_image.hpp"
#include"isom_LineContext.hpp"
#include"isom_font.hpp"
#include"isom_LightSet.hpp"




struct Dot;


struct
Cell: public Color
{
   int32_t   z;
  uint32_t  id;

};


class
Renderer
{
  std::vector<Cell>  table;

  int  x_base;
  int  y_base;

  int  x_width;
  int  y_width;

  int    left;
  int   right;
  int     top;
  int  bottom;

  std::vector<Dot>  dot_buffer;

  bool  buffering;

public:
  static LightSet  default_lightset;

  const LightSet*  lightset;

  Renderer(int  x=0, int  y=0, int  w=0, int  h=0, const LightSet*  ls=&default_lightset, bool  buffering_=false);

  void  clear();
  void  flush();

  int  get_x_base() const{return x_base;}
  int  get_y_base() const{return y_base;}

  void  change_base_point(int  x, int  y);

  int  get_x_width() const{return x_width;}
  int  get_y_width() const{return y_width;}

  bool  test_2d(int  x, int  y) const;
  bool  test_3d(int  x, int  y) const;

        Cell&  get_cell(int  x, int  y)      ;
  const Cell&  get_cell(int  x, int  y) const;

  void  put(const Color&  color, int  x, int  y                         );
  void  put(const Color&  color, int  x, int  y, int  z, uint32_t  id_=0);

  void  draw_image(const Image&  src, const Rect*  src_rect, int  dst_x, int  dst_y);

  void  draw_line(const Color&  color, int  x0, int  y0, int  x1, int  y1, int  interval=0);
  void  draw_rect(const Rect&  rect, const Color&  color);
  void  fill_rect(const Rect&  rect, const Color&  color);
  void  fill_rect(const Rect&  rect, uint32_t  id);

  void  draw_glyph(const Glyph*  gl,    const Color&  color, int  x, int  y);
  void  draw_string(const char*  s,     const Color&  color, int  x, int  y);
  void  draw_string(const char16_t*  s, const Color&  color, int  x, int  y);

  void  draw_ascii_glyph(const AsciiGlyph*  gl, const Color&  color, int  x, int  y);
  void  draw_ascii(      const char*         s, const Color&  color, int  x, int  y);

};



#endif




