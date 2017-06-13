#ifndef ISOM_TexturedPolygon_HPP_INCLUDED
#define ISOM_TexturedPolygon_HPP_INCLUDED


#include"isom_TextureRenderingContext.hpp"
#include"isom_FaceRenderingContext.hpp"
#include"isom_transformer.hpp"
#include"isom_renderer.hpp"




struct
TexturedPolygon
{
  const Image*  image;

  TextureVertex  a;
  TextureVertex  b;
  TextureVertex  c;

  Vector  normal_vector;

  TexturedPolygon(): image(nullptr){}
  TexturedPolygon(const Image*  img, const TextureVertex&  a_,
                                     const TextureVertex&  b_,
                                     const TextureVertex&  c_):
  image(img), a(a_), b(b_), c(c_){}


  TextureRenderingContext  make_rendering_context() const;
  FaceRenderingContext     make_face_rendering_context() const;

  void  update();

  void  transform(const Transformer&  tr);
  void  render(Renderer&  dst) const;

};




#endif




