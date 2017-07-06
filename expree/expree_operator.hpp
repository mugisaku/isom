#ifndef expree_Operator_HPP_INCLUDED
#define expree_Operator_HPP_INCLUDED


#include<cstdint>
#include<cstdio>


namespace expree{


struct
Operator
{
  char  codes[4];


  constexpr Operator(char  c0=0, char  c1=0, char  c2=0):
  codes{c0,c1,c2,0}{}


  constexpr uint32_t operator*() const
  {
    return((codes[0]<<24)|
           (codes[1]<<16)|
           (codes[2]<< 8)|
           (codes[3]    ));
  }

  void  print() const
  {
    printf("%s",codes);
  }

};


struct
PrefixUnaryOperator: Operator
{
  constexpr PrefixUnaryOperator(char  c0=0, char  c1=0, char  c2=0):
  Operator(c0,c1,c2){}

};


struct
SuffixUnaryOperator: Operator
{
  constexpr SuffixUnaryOperator(char  c0=0, char  c1=0, char  c2=0):
  Operator(c0,c1,c2){}

};


struct
BinaryOperator: Operator
{
  constexpr BinaryOperator(char  c0=0, char  c1=0, char  c2=0):
  Operator(c0,c1,c2){}

};


}


#endif




