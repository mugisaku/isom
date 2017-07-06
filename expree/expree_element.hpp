#ifndef expree_ELEMENT_HPP_INCLUDED
#define expree_ELEMENT_HPP_INCLUDED


#include"expree_operator.hpp"
#include"expree_operand.hpp"
#include"expree_object.hpp"


#ifndef report
#define report  printf("[report in %s] %d %s\n",__FILE__,__LINE__,__func__);
#endif


namespace expree{


struct Scope;



enum class
ElementKind
{
  null,
  operand,
  operator_,
  prefix_unary_operator,
  suffix_unary_operator,
  binary_operator,

};


union
ElementData
{
  Operator  operator_;
  Operand   operand;

   ElementData(){}
  ~ElementData(){}

};


class
Element
{
  ElementKind  kind;
  ElementData  data;

  Element*   left=nullptr;
  Element*  right=nullptr;

public:
  Element();
  Element(Operand&&                   o);
  Element(const            Operator&  o);
  Element(const PrefixUnaryOperator&  o);
  Element(const SuffixUnaryOperator&  o);
  Element(const BinaryOperator&       o);
  Element(const Element&   rhs) noexcept;
  Element(      Element&&  rhs) noexcept;
  ~Element();


  Element&  operator=(const Element&   rhs) noexcept;
  Element&  operator=(      Element&&  rhs) noexcept;

  const ElementData*  operator->() const{return &data;}

  bool  operator==(ElementKind  k) const{return(kind == k);}
  bool  operator!=(ElementKind  k) const{return(kind != k);}

  ElementKind  get_kind() const{return kind;}

  Operand&  as_operand(){return data.operand;}

  void  clear();

  void  insert_to_left( Element*  e);
  void  insert_to_right(Element*  e);

  const Element*  get_left()  const{return  left;}
  const Element*  get_right() const{return right;}

  Object  make_value(Scope*  scope=nullptr, bool  create_when_found_not=false) const;

  void  print() const;

};


}


#endif




