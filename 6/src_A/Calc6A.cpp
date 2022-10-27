//
//  Calc6A.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include <iostream>
#include "AlgebraicTreeNode.hpp"

// second part : uncomment after the first part works properly
// 
// #include "AlgebraicTreeExpression.h"

using namespace std;

// use these strings to debug your code
// write other expressions and test your code on them as well
const char* strA = "(8*5)";
const char* strB = "(4*((8*5)+7))";
const char* strC = "(((3*5)+(4*(7+(8*5))))*(7-(9*3)))";
const char* strD = "(4.32*((-8.5*5.0e-2)+ -7.3))";

int main(int argc, const char * argv[]) {

  // if(argc<2) {
  //
  // // print usage message and exit returning -1
  //
  // } else if(argc==2) {

  // AlgebraicTreeNode* root = new AlgebraicTreeNode(argv[1]);
  AlgebraicTreeNode* root = new AlgebraicTreeNode(strB);

  cout << "input  = \"" << strD << "\"" << endl;

  if(root->isInvalid()) {
    cout << "\"" << "expression \""<< strD << "\" is invalid" << endl;
    return -2;
  }

  string str   = root->toString();
  double value = root->evaluate();

  if(str=="") {
    cout << "error parsing expression" << endl;
  } else {
    cout << "parsed = \"" << str << "\" = " << value << endl;
  }

  delete root;

  // } else if(argc==3) {

  // second part : uncomment after the first part works properly

  // use fixed strings to debug; switch to reading the expressions
  // from the command line once everything works
  //
  // AlgebraicTreeExpression expr1("(7-(3+4))");
  // AlgebraicTreeExpression expr2("((5*7)+(100/3))");
  // AlgebraicTreeExpression expr3(expr1);
  // AlgebraicTreeExpression expr4 = expr2;
  // expr4 += expr3;
  // cout << "expr1 = \"" << expr1.toString() << "\"" << endl;
  // cout << "expr2 = \"" << expr2.toString() << "\"" << endl;
  // cout << "expr3 = \"" << expr3.toString() << "\"" << endl;
  // cout << "expr4 = \"" << expr4.toString() << "\"" << endl;
  
  // AlgebraicTreeExpression expr1(argc[1]);
  // AlgebraicTreeExpression expr2(argc[2]);
  // AlgebraicTreeExpression expr3(expr1);
  // AlgebraicTreeExpression expr4 = expr2;
  // expr4 += expr3;
  // cout << "expr1 = \"" << expr1.toString() << "\"" << endl;
  // cout << "expr2 = \"" << expr2.toString() << "\"" << endl;
  // cout << "expr3 = \"" << expr3.toString() << "\"" << endl;
  // cout << "expr4 = \"" << expr4.toString() << "\"" << endl;

  // } else {
  //
  // // too many arguments : print an error message and/or usage
  // // message and exit returning -2
  //
  // }

  return 0;
}
