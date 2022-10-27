#ifndef _AlgebraicTreeExpression_hpp_
#define _AlgebraicTreeExpression_hpp_

#include "AlgebraicTreeNode.hpp"
#include "AlgebraicTreeNumber.hpp"
#include "AlgebraicTreeOperation.hpp"
#include <string>
using namespace std;
class AlgebraicTreeExpression {

public:

  ~AlgebraicTreeExpression();

  AlgebraicTreeExpression();
  AlgebraicTreeExpression(const char* expression);
  
  bool   isEmpty() const;
  double evaluate() const;
  char*  toString() const;
    int    getLength() const;

  void setRoot(AlgebraicTreeNode* root);
    int getrightParenthese();
protected:

    
   AlgebraicTreeNode* _root;
    int actual_length    =   0;
    int right_parenthese  = 0;
    int left_parenthese = 0;

  unsigned _parse(const char* expression, AlgebraicTreeNode** parentPtr);

};

#endif // _AlgebraicTreeExpression_hpp_
