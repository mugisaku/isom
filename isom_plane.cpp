#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"




TextureRenderingContext
Plane::
make_texture_rendering_context(int  i, const Image&  image, const Rect&  rect) const
{
  using CTX = TextureRenderingContext;
  using   T = TextureVertex;

  int     top = rect.y         ;
  int  bottom = rect.y+rect.h-1;
  int    left = rect.x         ;
  int   right = rect.x+rect.w-1;

    if(i == 0){return CTX(image,T(points[0],left,top),T(points[1],right,   top),T(points[2],right,bottom));}
  else        {return CTX(image,T(points[0],left,top),T(points[2],right,bottom),T(points[3], left,bottom));}
}


void
Plane::
transform(const Transformer&  tr)
{
    for(auto&  pt: points)
    {
      pt = tr(pt);
    }
}


void
Plane::
render(Renderer&  dst) const
{
    for(auto&  pt: points)
    {
    }
}


void
Plane::
render_face(Renderer&  dst, const Color&  color) const
{
/*
  FaceRenderingContext  frctx(color,points[0],points[1],points[2]);

  dst.render_face(frctx);

  frctx.reset(color,points[0],points[2],points[3]);

  dst.render_face(frctx);
*/
}


void
Plane::
render_wire(Renderer&  dst) const
{
  constexpr Color  wire_color(0xFF,0xFF,0xFF);

  dst.render_line(points[0],points[1],wire_color);
  dst.render_line(points[1],points[2],wire_color);
  dst.render_line(points[2],points[3],wire_color);
  dst.render_line(points[3],points[0],wire_color);
}


void
Plane::
render_texture(Renderer&  dst, const Image&  img, const Rect&  rect) const
{
  auto  a = make_texture_rendering_context(0,img,rect);
  auto  b = make_texture_rendering_context(1,img,rect);

  dst.render_texture(a);
  dst.render_texture(b);
}




