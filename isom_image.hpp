#ifndef ISOM_IMAGE_HPP_INCLUDED
#define ISOM_IMAGE_HPP_INCLUDED


#include"isom_color.hpp"
#include<vector>


struct
Rect
{
  int  x;
  int  y;
  int  w;
  int  h;

  constexpr Rect(int  x_=0, int  y_=0, int  w_=0, int  h_=0):
  x(x_), y(y_), w(w_), h(h_){}


  constexpr bool  test(int  x_, int  y_) const
  {
    return((x_ >= (x  )) &&
           (y_ >= (y  )) &&
           (x_ <  (x+w)) &&
           (y_ <  (y+h)));
  }

};


class
Image
{
  int  width;
  int  height;

  std::vector<Color>  table;

public:
  Image(int  w=0, int  h=0);


  void  resize(int  w, int  h);

  void  open(const char*  path);

  void  set_colorkey(uint8_t  r, uint8_t  g, uint8_t  b);

  int  get_width()  const{return  width;}
  int  get_height() const{return height;}

  const Color&  get_color(int  x, int  y) const;

};




#endif




