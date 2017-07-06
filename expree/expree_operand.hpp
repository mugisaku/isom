#ifndef expree_OPERAND_HPP_INCLUDED
#define expree_OPERAND_HPP_INCLUDED


#include<string>
#include<vector>




namespace expree{


enum class
OperandKind
{
  null,
  identifier,
  integer_literal,
  string_literal,
  element,

};


struct Element;
struct Operand;


struct
Identifier
{
  std::string  string;

  Identifier(                ){}
  Identifier(std::string&&  s): string(std::move(s)){}

};


union
OperandData
{
  unsigned int  integer;

  std::string  string;

  std::vector<Operand>  list;

  Element*  element;

   OperandData(){}
  ~OperandData(){}

};


class
Operand
{
  OperandKind  kind;
  OperandData  data;

  const char*  opening_string=nullptr;
  const char*  closing_string=nullptr;

public:
  Operand();
  Operand(std::string&&  s);
  Operand(Identifier&&  id);
  Operand(unsigned int   i);
  Operand(Element*       e, const char*  op_s=nullptr, const char*  cl_s=nullptr);
  Operand(const Operand&   rhs) noexcept;
  Operand(      Operand&&  rhs) noexcept;
 ~Operand();


  Operand&  operator=(const Operand&   rhs) noexcept;
  Operand&  operator=(      Operand&&  rhs) noexcept;

  const OperandData*  operator->() const{return &data;}

  bool  operator==(OperandKind  k) const{return(kind == k);}
  bool  operator!=(OperandKind  k) const{return(kind != k);}

  OperandKind  get_kind() const{return kind;}

  void  set_bracket(const char*  op_s=nullptr, const char*  cl_s=nullptr);

  void  clear();

  void  print() const;


};


}


#endif




