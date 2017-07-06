#include"expree_element.hpp"
#include<new>




namespace expree{




Element::Element(                              ): kind(ElementKind::null                 ){}
Element::Element(Operand&&                    o): kind(ElementKind::operand              ){new(&data) Operand(std::move(o));}
Element::Element(const             Operator&  o): kind(ElementKind::operator_            ){new(&data) Operator(o);}
Element::Element(const  PrefixUnaryOperator&  o): kind(ElementKind::prefix_unary_operator){new(&data) Operator(o);}
Element::Element(const  SuffixUnaryOperator&  o): kind(ElementKind::suffix_unary_operator){new(&data) Operator(o);}
Element::Element(const BinaryOperator&        o): kind(ElementKind::binary_operator      ){new(&data) Operator(o);}

Element::Element(const Element&   rhs) noexcept: kind(ElementKind::null){*this = rhs;}
Element::Element(      Element&&  rhs) noexcept: kind(ElementKind::null){*this = std::move(rhs);}


Element::
~Element()
{
  clear();
}




Element&
Element::
operator=(const Element&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

   left = rhs.left ? new Element(*rhs.left ):nullptr;
  right = rhs.right? new Element(*rhs.right):nullptr;

    if(kind == ElementKind::operand)
    {
      new(&data.operand) Operand(rhs.data.operand);
    }

  else
    {
      data.operator_ = rhs.data.operator_;
    }
}


Element&
Element::
operator=(Element&&  rhs) noexcept
{
  clear();

  kind = rhs.kind                    ;
         rhs.kind = ElementKind::null;

  std::swap( left,rhs.left );
  std::swap(right,rhs.right);

    if(kind == ElementKind::operand)
    {
      new(&data.operand) Operand(std::move(rhs.data.operand));
    }

  else
    {
      data.operator_ = rhs.data.operator_;
    }
}


void
Element::
clear()
{
    switch(kind)
    {
      case(ElementKind::operand):
        data.operand.~Operand();
        break;
    }


  delete left          ;
         left = nullptr;

  delete right          ;
         right = nullptr;

  kind = ElementKind::null;
}




void
Element::
insert_to_left(Element*  e)
{
  delete left    ;
         left = e;
}


void
Element::
insert_to_right(Element*  e)
{
  delete right    ;
         right = e;
}




void
Element::
print() const
{
    switch(kind)
    {
      case(ElementKind::null):
        printf(" NULL_ELEMENT ");
        break;
      case(ElementKind::operand):
        data.operand.print();
        break;
      case(ElementKind::operator_):
        data.operator_.print();
        break;
      case(ElementKind::prefix_unary_operator):
        printf("(");
        data.operator_.print();

          if(left)
          {
            left->print();
          }

        else
          {
            throw Exception("要素がありません");
          }

        printf(")");
        break;
      case(ElementKind::suffix_unary_operator):
        printf("(");

          if(left)
          {
            left->print();
          }

        else
          {
            throw Exception("要素がありません");
          }


        data.operator_.print();
        printf(")");
        break;
      case(ElementKind::binary_operator):
        printf("(");

          if(left)
          {
            left->print();
          }

        else
          {
            throw Exception("左の要素がありません");
          }

        data.operator_.print();

          if(right)
          {
            right->print();
          }

        else
          {
            throw Exception("右の要素がありません");
          }


        printf(")");
        break;
    }
}


}




