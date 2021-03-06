#ifndef ISOM_FONT_HPP_INCLUDED
#define ISOM_FONT_HPP_INCLUDED


#include<cstdint>
#include<string>


struct
AsciiGlyph
{
  uint8_t  data[8];

};


struct
Glyph
{
  static constexpr int  size = 14;

  uint16_t  unicode;

  uint16_t  data[size];

  int  count;

};




const Glyph*  get_glyph(char16_t  c);

void  print_required_glyphes();


#endif




