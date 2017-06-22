#ifndef ISOM_Color_HPP_INCLUDED
#define ISOM_Color_HPP_INCLUDED


#include<cstdint>


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




#endif




