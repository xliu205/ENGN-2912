//
//  Calc6B
//  AlgebraicTreeExpression.hpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#ifndef _AlgebraicTreeExpression_hpp_
#define _AlgebraicTreeExpression_hpp_

#include "AlgebraicTreeNode.hpp"
#include "AlgebraicTreeNumber.hpp"
#include "AlgebraicTreeOperation.hpp"

class AlgebraicTreeExpression {

public:

  ~AlgebraicTreeExpression();

  AlgebraicTreeExpression();
  AlgebraicTreeExpression(const char* expression);

  bool        isEmpty() const;
  double      evaluate() const;
  std::string toString() const;

  void        setRoot(AlgebraicTreeNode* root);

protected:

   AlgebraicTreeNode* _root;

  unsigned _parse(const char* expression, AlgebraicTreeNode** parentPtr);

};

#endif // _AlgebraicTreeExpression_hpp_
