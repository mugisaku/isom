#include"isom_image.hpp"
#include<SDL.h>
#include<SDL_image.h>




namespace{
void
get(uint32_t  colorcode, const SDL_Surface*  bmp, Color&  color)
{
  SDL_GetRGBA(colorcode,bmp->format,&color.r,&color.g,&color.b,&color.a);
}
}




Image::
Image(int  w, int  h):
width(w),
height(h),
table(w*h)
{
}




void
Image::
resize(int  w, int  h)
{
  width  = w;
  height = h;

  table.resize(width*height);
}


void
Image::
open(const char*  path)
{
  auto  bmp = IMG_Load(path);

  resize(bmp->w,bmp->h);


  auto  dst = table.begin();

  auto  p = static_cast<const uint8_t*>(bmp->pixels);

  auto  bpp = bmp->format->BytesPerPixel;

    for(int  y = 0;  y < height;  ++y)
    {
      auto  src = p              ;
                  p += bmp->pitch;

        for(int  x = 0;  x < width;  ++x)
        {
          Color  color;

            switch(bpp)
            {
          case(1): get(*reinterpret_cast<const uint8_t* >(src),bmp,color);break;
          case(2): get(*reinterpret_cast<const uint16_t*>(src),bmp,color);break;
          case(3):
            color.r = src[0];
            color.g = src[1];
            color.b = src[2];
            color.a = 255;
            break;
          case(4): get(*reinterpret_cast<const uint32_t*>(src),bmp,color);break;
            }


          src += bpp;

          *dst++ = color;
        }
    }


  SDL_FreeSurface(bmp);
}


void
Image::
set_colorkey(uint8_t  r, uint8_t  g, uint8_t  b)
{
  int  n = width*height;

  auto  p = table.data();

    while(n--)
    {
      auto&  color = *p++;

      color.a = ((color.r == r) &&
                 (color.g == g) &&
                 (color.b == b))? 0:255;
    }
}




