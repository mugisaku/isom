#ifndef ISOM_RENDERER_HPP_INCLUDED
#define ISOM_RENDERER_HPP_INCLUDED


#include<cstdint>
#include<vector>
#include"isom_point3.hpp"
#include"isom_image.hpp"




struct Plane;


struct
Cell: public Color
{
  int32_t  w_value;

  const Plane*  plane;

};


class
Renderer
{
  std::vector<Cell>  table;

  int  width;
  int  height;

public:
  Point3  offset;

  int  w_min=0;
  int  w_max=65535;

  Renderer(int  w=0, int  h=0);

  void  clear();

  void  resize(int  w, int  h);

  int   get_width() const;
  int   get_height() const;

  void   set_cell(const Cell&  src, int  x, int  y);

  const Cell&  get_cell(int  x, int  y) const;

};




#endif




