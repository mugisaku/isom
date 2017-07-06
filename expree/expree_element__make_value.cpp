#include"expree_element.hpp"
#include"expree_scope.hpp"
#include"expree_primitive.hpp"
#include"expree_MemorySpace.hpp"




namespace expree{


namespace{
Object
compound_assign(const Object&  lv, const Object&  rv, const Operator&  op, Scope&  scope, bool  create_when_found_not)
{
  Object  r;

    switch(*op)
    {
      case(*Operator('<','<','=')): r = Object::bit_shl(lv,rv);break;
      case(*Operator('>','>','=')): r = Object::bit_shr(lv,rv);break;
      case(*Operator(    '+','=')): r = Object::ari_add(lv,rv);break;
      case(*Operator(    '-','=')): r = Object::ari_sub(lv,rv);break;
      case(*Operator(    '*','=')): r = Object::ari_mul(lv,rv);break;
      case(*Operator(    '/','=')): r = Object::ari_div(lv,rv);break;
      case(*Operator(    '%','=')): r = Object::ari_rem(lv,rv);break;
      case(*Operator(    '&','=')): r = Object::bit_and(lv,rv);break;
      case(*Operator(    '|','=')): r = Object::bit_or( lv,rv);break;
      case(*Operator(    '^','=')): r = Object::bit_xor(lv,rv);break;
      case(*Operator(        '=')): r =                     rv;break;
    }


  return Object::st(lv,r);
}
}




Object
Element::
make_value(Scope*  scope, bool  create_when_found_not) const
{
    switch(get_kind())
    {
      case(ElementKind::null):
        throw Exception("空の要素です");
        break;
      case(ElementKind::operand):
        {
          auto&  op = data.operand;

            switch(op.get_kind())
            {
              case(OperandKind::identifier):
                  if(!scope)
                  {
                    throw Exception("無記憶の計算で、識別子が使用されました");
                  }


                return Object(scope->get_reference(op->string,create_when_found_not));
                break;
              case(OperandKind::element):
                return op->element->make_value(scope,create_when_found_not);
                break;
              case(OperandKind::integer_literal):
                return Object(Integer(op->integer));
                break;
              case(OperandKind::string_literal):
                break;
            }
        }
        break;
      case(ElementKind::prefix_unary_operator):
        {
          auto  v = left->make_value(scope,create_when_found_not);

            switch(*data.operator_)
            {
              case(*Operator('!')): return Object::log_not(v);break;
              case(*Operator('~')): return Object::bit_not(v);break;
              case(*Operator('-')): return Object::neg(v);break;
              case(*Operator('*')):
                  if(!scope)
                  {
                    throw Exception("無記憶の計算で、ポインタをデリファレンスしようとしました");
                  }

                return Object::ld(*scope,v);
              case(*Operator('&')):
                  if(!scope)
                  {
                    throw Exception("無記憶の計算で、オブジェクトのアドレスを取得しようとしました");
                  }


                return Object::get_address(*scope,v);
/*
              case(*Operator('+','+')): return ;break;
              case(*Operator('-','-')): return ;break;
*/
            }
        }
        break;
      case(ElementKind::suffix_unary_operator):
        {
/*
          auto  v = left->make_value(scope,create_when_found_not);

            switch(*data.operator_)
            {
              case(*Operator('+','+')): v = Value(!v);break;
              case(*Operator('-','-')): v = Value(!v);break;
            }
*/
        }
        break;
      case(ElementKind::binary_operator):
        {
          auto  rv = right->make_value(scope,create_when_found_not);
          auto  lv =  left->make_value(scope,create_when_found_not);

            switch(*data.operator_)
            {
              case(*Operator('*')): return Object::ari_mul(lv,rv);
              case(*Operator('/')): return Object::ari_div(lv,rv);
              case(*Operator('%')): return Object::ari_rem(lv,rv);

              case(*Operator('+')): return Object::ari_add(lv,rv);
              case(*Operator('-')): return Object::ari_sub(lv,rv);

              case(*Operator('<','<')): return Object::bit_shl(lv,rv);
              case(*Operator('>','>')): return Object::bit_shr(lv,rv);

              case(*Operator('<')    ): return Object::cmp_lt(lv,rv);
              case(*Operator('<','=')): return Object::cmp_lteq(lv,rv);
              case(*Operator('>')    ): return Object::cmp_gt(lv,rv);
              case(*Operator('>','=')): return Object::cmp_gteq(lv,rv);

              case(*Operator('=','=')): return Object::cmp_eq( lv,rv);
              case(*Operator('!','=')): return Object::cmp_neq(lv,rv);


              case(*Operator('&')): return Object::bit_and(lv,rv);
              case(*Operator('|')): return Object::bit_or( lv,rv);
              case(*Operator('^')): return Object::bit_xor(lv,rv);

              case(*Operator('&','&')): return Object::log_and(lv,rv);
              case(*Operator('|','|')): return Object::log_or( lv,rv);

              case(*Operator('<','<','=')):
              case(*Operator('>','>','=')):
              case(*Operator(    '+','=')):
              case(*Operator(    '-','=')):
              case(*Operator(    '*','=')):
              case(*Operator(    '/','=')):
              case(*Operator(    '%','=')):
              case(*Operator(    '&','=')):
              case(*Operator(    '|','=')):
              case(*Operator(    '^','=')):
              case(*Operator(        '=')):
                  if(!scope)
                  {
                    throw Exception("無記憶の計算で、代入操作をしようとしました");
                  }


                return compound_assign(lv,rv,data.operator_,*scope,create_when_found_not);
              case(*Operator(',')):
                return rv;
            }
        }
        break;
    }


  return Object();
}


}




