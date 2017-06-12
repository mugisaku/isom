#ifndef ISOM_Box_HPP_INCLUDED
#define ISOM_Box_HPP_INCLUDED


#include"isom_plane.hpp"



struct
Box
{
  Plane  planes[6];


  void  build(const Point&  base, int  x_width, int  y_width, int  z_width);

  Plane&     get_top(){return planes[0];}
  Plane&  get_bottom(){return planes[1];}
  Plane&    get_left(){return planes[2];}
  Plane&   get_right(){return planes[3];}
  Plane&   get_front(){return planes[4];}
  Plane&    get_back(){return planes[5];}

  void  transform(const Transformer&  tr);

  void  update();

  void  render(Renderer&  dst) const;
  void  render_wire(Renderer&  dst) const;

};




#endif




