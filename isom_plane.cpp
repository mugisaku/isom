#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"




void
Plane::
update()
{
  Rotater  r(angle);

    for(int  i = 0;  i < 4;  ++i)
    {
      auto&  pt = transformed_points[i];

      pt = points[i];

      pt = r(pt,Point(64,64,0));

      pt += offset;
    }
}




FaceRenderingContext
Plane::
make_face_rendering_context(int  i, const Color&  color) const
{
  auto&  pts = transformed_points;

  using T = FaceRenderingContext;

    if(i == 0){return T(color,pts[0],pts[1],pts[2]);}
  else        {return T(color,pts[0],pts[2],pts[3]);}
}


TextureRenderingContext
Plane::
make_texture_rendering_context(int  i, const Image&  image, const Rect&  rect) const
{
  auto&  pts = transformed_points;

  using CTX = TextureRenderingContext;
  using   T = TextureVertex;

  int     top = rect.y         ;
  int  bottom = rect.y+rect.h-1;
  int    left = rect.x         ;
  int   right = rect.x+rect.w-1;

    if(i == 0){return CTX(image,T(pts[0],left,top),T(pts[1],right,   top),T(pts[2],right,bottom));}
  else        {return CTX(image,T(pts[0],left,top),T(pts[2],right,bottom),T(pts[3], left,bottom));}
}


void
Plane::
render_face(Renderer&  dst, const Color&  color) const
{
  auto&  pts = transformed_points;


  FaceRenderingContext  frctx(color,pts[0],pts[1],pts[2]);

  dst.render_face(frctx);

  frctx.reset(color,pts[0],pts[2],pts[3]);

  dst.render_face(frctx);
}


void
Plane::
render_wire(Renderer&  dst) const
{
  constexpr Color  wire_color(0xFF,0xFF,0xFF);

  auto&  pts = transformed_points;

  dst.render_line(pts[0],pts[1],wire_color);
  dst.render_line(pts[1],pts[2],wire_color);
  dst.render_line(pts[2],pts[3],wire_color);
  dst.render_line(pts[3],pts[0],wire_color);
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




