#include"isom_view.hpp"




Transformer
View::
make_transformer() const
{
  Transformer  tr;

  Point  tmpt;

//    tr.change_offset(dst_point);
  tr.change_angle(angle);
  tr.change_center(dst_point);

  tr.set_rotation_flag();

  tr(tmpt);

  tr.change_center(tmpt);

  return tr;
}




