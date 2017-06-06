#include"isom_DotSet.hpp"




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
      dot.render(dst);
    }
}





