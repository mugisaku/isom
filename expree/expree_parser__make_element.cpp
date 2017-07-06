#include"expree_parser.hpp"
#include"expree_exception.hpp"
#include<cctype>
#include<cstring>
#include<vector>




namespace expree{


namespace{


struct
OperatorFile
{
  int  precedence;

  ElementKind  kind;

  Operator  data;

  OperatorFile(ElementKind  k, const Operator&  o):
  kind(k),
  data(o)
  {
      if(k == ElementKind::prefix_unary_operator)
      {
          switch(*data)
          {
        case(*Operator('+')):
        case(*Operator('-')):
        case(*Operator('!')):
        case(*Operator('~')):
        case(*Operator('*')):
        case(*Operator('&')):
        case(*Operator('+','+')):
        case(*Operator('-','-')):
            precedence = 3;
            break;
          }
      }

    else
      if(k == ElementKind::suffix_unary_operator)
      {
          switch(*data)
          {
        case(*Operator('+','+')):
        case(*Operator('-','-')):
            precedence = 2;
            break;
          }
      }

    else
      if(k == ElementKind::binary_operator)
      {
          switch(*data)
          {
        case(*Operator(':',':')):
             precedence = 1;
             break;
        case(*Operator('.')):
        case(*Operator('-','>')):
             precedence = 2;
             break;
        case(*Operator('.','*')):
        case(*Operator('-','>','*')):
             precedence = 4;
             break;
        case(*Operator('*')):
        case(*Operator('/')):
        case(*Operator('%')):
             precedence = 5;
             break;
        case(*Operator('+')):
        case(*Operator('-')):
             precedence = 6;
             break;
        case(*Operator('<','<')):
        case(*Operator('>','>')):
             precedence = 7;
             break;
        case(*Operator('<')    ):
        case(*Operator('<','=')):
        case(*Operator('>')    ):
        case(*Operator('>','=')):
             precedence = 8;
             break;
        case(*Operator('=','=')):
        case(*Operator('!','=')):
             precedence = 9;
             break;
        case(*Operator('&')):
             precedence = 10;
             break;
        case(*Operator('^')):
             precedence = 11;
             break;
        case(*Operator('|')):
             precedence = 12;
             break;
        case(*Operator('&','&')):
             precedence = 13;
             break;
        case(*Operator('|','|')):
             precedence = 14;
             break;
        case(*Operator('=')):
        case(*Operator('+','=')):
        case(*Operator('-','=')):
        case(*Operator('*','=')):
        case(*Operator('/','=')):
        case(*Operator('%','=')):
        case(*Operator('&','=')):
        case(*Operator('|','=')):
        case(*Operator('^','=')):
        case(*Operator('<','<','=')):
        case(*Operator('>','>','=')):
             precedence = 15;
             break;
        case(*Operator(',')):
             precedence = 16;
             break;
          }
      }

    else
      {
        Formatted  f;

        throw Exception(f("%dは無効なエレメント種です",static_cast<int>(k)));
      }


    precedence = 16-precedence;
  }


  bool  operator<(const OperatorFile&  rhs) const
  {
    return(precedence < rhs.precedence);
  }


  Element  to_element() const
  {
    return (kind == ElementKind::prefix_unary_operator)? Element(static_cast<const PrefixUnaryOperator&>(data)):
           (kind == ElementKind::suffix_unary_operator)? Element(static_cast<const SuffixUnaryOperator&>(data)):
           (kind == ElementKind::binary_operator      )? Element(static_cast<const      BinaryOperator&>(data)):Element();
  }

  void  print() const
  {
    printf("%s %2d\n",data.codes,precedence);
  }

};


}




Element
Parser::
make_element(const char*  opening, const char*  closing) const
{
    if(buffer.empty())
    {
      throw Exception("要素が一つもありません");
    }


  std::vector<Element>        element_buffer;
  std::vector<OperatorFile>  operator_buffer;

  ElementKind  last_element_kind = ElementKind::prefix_unary_operator;

  std::vector<Element>  calc;

    if(buffer.size() == 1)
    {
      calc.emplace_back(buffer.front());

      goto FINISH;
    }


    for(auto  e: buffer)
    {
        if(e == ElementKind::operand)
        {
          element_buffer.emplace_back(std::move(e));

          last_element_kind = ElementKind::operand;
        }

      else
        if(e == ElementKind::operator_)
        {
          ElementKind  k;

          auto  o = e->operator_;

            if(last_element_kind == ElementKind::operand)
            {
                switch(*o)
                {
              case(*Operator('+','+')):
              case(*Operator('-','-')):
                  k = ElementKind::suffix_unary_operator;
                  break;
              default:
                  k = ElementKind::binary_operator;
                  break;
                }
            }

          else
            if(last_element_kind == ElementKind::prefix_unary_operator)
            {
              k = ElementKind::prefix_unary_operator;
            }

          else
            if(last_element_kind == ElementKind::suffix_unary_operator)
            {
              k = ElementKind::binary_operator;
            }

          else
            if(last_element_kind == ElementKind::binary_operator)
            {
              k = ElementKind::prefix_unary_operator;
            }


          auto  opf = OperatorFile(k,o);

          last_element_kind = k;

            while(operator_buffer.size())
            {
              auto&  top = operator_buffer.back();

                if(top < opf)
                {
                  break;
                }


              element_buffer.emplace_back(top.to_element());

              operator_buffer.pop_back();
            }


          operator_buffer.emplace_back(opf);
        }


if(0)
{
        for(auto&  e: operator_buffer)
        {
          printf("%s",e.data.codes);
        }

      printf("\n");
}
    }

    while(operator_buffer.size())
    {
      element_buffer.emplace_back(operator_buffer.back().to_element());

      operator_buffer.pop_back();
    }


    for(auto  e: element_buffer)
    {
        if(e == ElementKind::operand)
        {
          calc.emplace_back(std::move(e));
        }

      else
        if((e == ElementKind::prefix_unary_operator) ||
           (e == ElementKind::suffix_unary_operator))
        {
            if(calc.empty())
            {
              throw Exception("単項演算の対象がありません");
            }


          auto  operand_e = std::move(calc.back());

          e.insert_to_left(new Element(std::move(operand_e)));

          calc.back() = Element(Operand(new Element(std::move(e))));
        }

      else
        if(e == ElementKind::binary_operator)
        {
            if(calc.size() < 2)
            {
              throw Exception("二項演算の対象が足りません");
            }


          auto  r = std::move(calc.back());

          calc.pop_back();

          auto  l = std::move(calc.back());

          e.insert_to_left( new Element(std::move(l)));
          e.insert_to_right(new Element(std::move(r)));

          calc.back() = Element(Operand(new Element(std::move(e))));
        }

      else
        {
          throw Exception("スタックの中身が不正です");
        }
    }


FINISH:
    if(calc.size() != 1)
    {
        for(auto&  e: calc)
        {
          e.print();
        }


      throw Exception("計算結果が不正です");
    }


  auto&  result = calc.back();

    if(result != ElementKind::operand)
    {
      throw Exception("計算結果がオペランドではありません");
    }


  result.as_operand().set_bracket(opening,closing);

  return std::move(result);
}



}




