//
//  Calc6B.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include <iostream>

#include "AlgebraicTreeExpression.hpp"

using namespace std;

const char* expr1str = "(8*5)";

// HINT:
//
// AlgebraicTreeExpression* newExpr1() {
//   AlgebraicTreeExpression* expr = new AlgebraicTreeExpression();
//   AlgebraicTreeOperation* root =
//     new AlgebraicTreeOperation(MULTIPLY);
//   expr->setRoot(root);
//   {
//     root->setChildLeft(new AlgebraicTreeNumber(8.0));
//     root->setChildRight(new AlgebraicTreeNumber(5.0));
//   }
//   return expr;
// }

const char* expr2str = "(7+(8*5))";

// HINT:
//
// AlgebraicTreeExpression* newExpr2() {
//   AlgebraicTreeExpression* expr = new AlgebraicTreeExpression();
//   AlgebraicTreeOperation* node0 =
//     new AlgebraicTreeOperation(ADD);
//   expr->setRoot(node0);
//   {
//     node0->setChildLeft(new AlgebraicTreeNumber(7.0));
//     AlgebraicTreeOperation* node1 =
//       new AlgebraicTreeOperation(MULTIPLY);
//     node0->setChildRight(node1);
//     {
//       node1->setChildLeft(new AlgebraicTreeNumber(8.0));
//       node1->setChildRight(new AlgebraicTreeNumber(5.0));
//     }
//   }
//   return expr;
// }

const char* expr3str = "(((3*5)+(4*(7+(8*5))))*(7-(9*3)))";

// AlgebraicTreeExpression* newExpr3() {
//   AlgebraicTreeExpression* expr = new AlgebraicTreeExpression();
//   // ...
//     return expr;
// }

const char* expr4str =
  "(((3.1*-5.35)+(4.1e-2*(-7.0e+1+(0.08*5.0e-3))))*(7-(9.0*-0.23)))";

// AlgebraicTreeExpression* newExpr4() {
//   AlgebraicTreeExpression* expr = new AlgebraicTreeExpression();
//   // ...
//     return expr;
// }

//////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]) {

  // create these functions to help debug your parser
  //
  // AlgebraicTreeExpression* expr = newExpr1();
  // AlgebraicTreeExpression* expr = newExpr2();
  // AlgebraicTreeExpression* expr = newExpr3();
  // AlgebraicTreeExpression* expr = newExpr4();

  // const char* exprStr = expr1str;
  // const char* exprStr = expr2str;
  // const char* exprStr = expr3str;
  const char* exprStr = expr1str;

  AlgebraicTreeExpression* expr = new AlgebraicTreeExpression(exprStr);

  cout << "input  = \"" << exprStr << "\"" << endl;

  if(expr->isEmpty()) {
    cout << "\"" << "expression is empty" << endl;
    return -2;
  }

  string outStr = expr->toString();
  double value = expr->evaluate();

  cout << "parsed = \"" << outStr  << "\" = " << value << endl;

  delete expr;

  return 0;
}
