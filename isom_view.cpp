#include"isom_view.hpp"




void
View::
update()
{
  angle.x = to_degree(std::atan2(src_point.y-dst_point.y,src_point.z-dst_point.z));
  angle.y = to_degree(std::atan2(src_point.x-dst_point.x,src_point.z-dst_point.z));
  angle.z = to_degree(std::atan2(src_point.y-dst_point.y,src_point.x-dst_point.x));
}


Transformer
View::
make_transformer() const
{
  Transformer  tr;

  tr.change_angle(angle);
  tr.change_center(src_point);

  tr.set_rotation_flag();

  return tr;
}




