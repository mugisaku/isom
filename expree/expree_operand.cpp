#include"expree_operand.hpp"
#include"expree_element.hpp"
#include<cstring>
#include<string>
#include<vector>
#include<new>




namespace expree{


Operand::Operand(                ): kind(OperandKind::null           ){}
Operand::Operand(std::string&&  s): kind(OperandKind::string_literal ){new(&data) std::string(std::move(s));}
Operand::Operand(Identifier&&  id): kind(OperandKind::identifier     ){new(&data) std::string(std::move(id.string));}
Operand::Operand(unsigned int   i): kind(OperandKind::integer_literal){new(&data) unsigned int(i);}

Operand::Operand(Element*  e, const char*  op_s, const char*  cl_s):
kind(OperandKind::element),
opening_string(op_s),
closing_string(cl_s)
{
  data.element = e;
}


Operand::Operand(const Operand&  rhs) noexcept:kind(OperandKind::null){*this = rhs;}
Operand::Operand(     Operand&&  rhs) noexcept:kind(OperandKind::null){*this = std::move(rhs);}


Operand::
~Operand()
{
  clear();
}




Operand&
Operand::
operator=(const Operand&  rhs) noexcept
{
  clear();

  kind = rhs.kind;
  opening_string = rhs.opening_string;
  closing_string = rhs.closing_string;

    switch(kind)
    {
      case(OperandKind::identifier):
      case(OperandKind::string_literal):
        new(&data.string) std::string(rhs.data.string);
        break;
      case(OperandKind::integer_literal):
        data.integer = rhs.data.integer;
        break;
      case(OperandKind::element):
        data.element = new Element(*rhs.data.element);
        break;
    }


  return *this;
}


Operand&
Operand::
operator=(Operand&&  rhs) noexcept
{
  clear();

  kind = rhs.kind                    ;
         rhs.kind = OperandKind::null;

  opening_string = rhs.opening_string;
  closing_string = rhs.closing_string;

    switch(kind)
    {
      case(OperandKind::string_literal):
      case(OperandKind::identifier):
        new(&data.string) std::string(std::move(rhs.data.string));
        break;
      case(OperandKind::integer_literal):
        data.integer = rhs.data.integer;
        break;
      case(OperandKind::element):
        data.element = rhs.data.element;
        break;
    }


  return *this;
}


void
Operand::
set_bracket(const char*  op_s, const char*  cl_s)
{
  opening_string = op_s;
  closing_string = cl_s;
}


void
Operand::
clear()
{
    switch(kind)
    {
      case(OperandKind::identifier):
      case(OperandKind::string_literal):
        data.string.~basic_string();
        break;
      case(OperandKind::integer_literal):
        break;
      case(OperandKind::element):
        delete data.element;
        break;
    }


  kind = OperandKind::null;

  opening_string = nullptr;
  closing_string = nullptr;
}


void
Operand::
print() const
{
    if(opening_string)
    {
      printf("%s",opening_string);
    }


    switch(kind)
    {
      case(OperandKind::identifier):
        printf(" %s ",data.string.data());
        break;
      case(OperandKind::string_literal):
        printf(" \"%s\" ",data.string.data());
        break;
      case(OperandKind::integer_literal):
        printf(" %u ",data.integer);
        break;
      case(OperandKind::element):
        data.element->print();
        break;
    }


    if(closing_string)
    {
      printf("%s",closing_string);
    }
}


}


