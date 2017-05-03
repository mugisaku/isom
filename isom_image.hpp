#ifndef ISOM_IMAGE_HPP_INCLUDED
#define ISOM_IMAGE_HPP_INCLUDED


#include<cstdint>
#include<vector>


struct
Color
{
  uint8_t  r;
  uint8_t  g;
  uint8_t  b;
  uint8_t  a;

  constexpr Color(uint8_t  r_=0, uint8_t  g_=0, uint8_t  b_=0, uint8_t  a_=255):
  r(r_),
  g(g_),
  b(b_),
  a(a_){}

  void  assign(uint8_t  r_, uint8_t  g_, uint8_t  b_, uint8_t  a_)
  {
    r = r_;
    g = g_;
    b = b_;
    a = a_;
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

  int  get_width() const{return width;}
  int  get_height() const{return height;}

  const Color&  get_color(int  x, int  y) const{return table[width*y+x];}

};




#endif




