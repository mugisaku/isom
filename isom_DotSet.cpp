#include"isom_DotSet.hpp"




void
DotSet::
relativize(const Point&  base)
{
//  std::stable_sort(dot_list.begin(),dot_list.end());

    for(auto&  dot: dot_list)
    {
      static_cast<Point&>(dot) -= base;
    }
}


void
DotSet::
transform(const Transformer&  tr)
{
    for(auto&  dot: dot_list)
    {
      dot.transform(tr);
    }
}


void
DotSet::
render(Renderer&  dst) const
{
    for(auto&  dot: dot_list)
    {
      dot.render(dst,0);
    }
}




