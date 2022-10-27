//
//  Calc6A
//  AlgebraicTreeExpression.hpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#ifndef _AlgebraicTreeExpression_hpp_
#define _AlgebraicTreeExpression_hpp_

#include "AlgebraicTreeNode.hpp"

class AlgebraicTreeExpression {

public:

  ~AlgebraicTreeExpression();

  // constructors
  AlgebraicTreeExpression();
  AlgebraicTreeExpression(const char* expression);
  AlgebraicTreeExpression(AlgebraicTreeExpression& src);

  AlgebraicTreeExpression& swap(AlgebraicTreeExpression& src);

  AlgebraicTreeExpression& operator=  (AlgebraicTreeExpression& src);
  AlgebraicTreeExpression& operator+= (AlgebraicTreeExpression& src);
  AlgebraicTreeExpression& operator-= (AlgebraicTreeExpression& src);
  AlgebraicTreeExpression& operator*= (AlgebraicTreeExpression& src);
  AlgebraicTreeExpression& operator/= (AlgebraicTreeExpression& src);

  double      evaluate() const;
  std::string toString() const;

private:

  AlgebraicTreeExpression& _operatorInPlace(AlgebraicTreeNodeType opr, AlgebraicTreeExpression& src);

protected:

  AlgebraicTreeNode* _root;

};

#endif // _AlgebraicTreeExpression_hpp_
