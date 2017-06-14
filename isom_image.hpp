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


constexpr Color   white(0xFF,0xFF,0xFF,0xFF);
constexpr Color    gray(0x7F,0x7F,0x7F,0xFF);
constexpr Color   black(0x00,0x00,0x00,0xFF);
constexpr Color     red(0xFF,0x00,0x00,0xFF);
constexpr Color   green(0x00,0xFF,0x00,0xFF);
constexpr Color    blue(0x00,0x00,0xFF,0xFF);
constexpr Color  yellow(0xFF,0xFF,0x00,0xFF);


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




