//
//  Calc6B
//  AlgebraicTreeExpression.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "AlgebraicTreeExpression.hpp"

AlgebraicTreeExpression::~AlgebraicTreeExpression() {
  // TBD
    if(_root!=(AlgebraicTreeNode*)0) delete _root;
}

AlgebraicTreeExpression::AlgebraicTreeExpression() {
  // TBD
    // _root(new AlgebraicTreeNode()) {
}

bool AlgebraicTreeExpression::isEmpty() const {
  return false; // TBD
}

double AlgebraicTreeExpression::evaluate() const {
    return _root->evaluate();
}

std::string AlgebraicTreeExpression::toString() const {
  std::string str;
  _root->toString(str);
  return str;
}

void AlgebraicTreeExpression::setRoot(AlgebraicTreeNode* root) {
  // TBD
  _root = root;
}

AlgebraicTreeExpression::AlgebraicTreeExpression(const char* expression) {
  // TBD
  
  _parse(expression, &_root);
}

unsigned AlgebraicTreeExpression::_parse(const char *expression, AlgebraicTreeNode **nodePtr)
{
  unsigned length = 0;

  while (isspace(expression[length]))
  {
    length++;
  }

  if (expression[length] == '(')
  {
    AlgebraicTreeNode *left_node = new AlgebraicTreeOperation(ZERO);
    unsigned left_len = _parse(expression + length + 1, &left_node);

    AlgebraicTreeOperation *op = new AlgebraicTreeOperation(ZERO);
    switch (*(expression + length + 1 + left_len))
    {
    case '+':
      op->setType(ADD);
      break;
    case '-':
      op->setType(SUBTRACT);
      break;
    case '*':
      op->setType(MULTIPLY);
      break;
    case '/':
      op->setType(DIVIDE);
      break;
    }

    AlgebraicTreeNode *right_node = new AlgebraicTreeOperation(ZERO);
    unsigned right_len = _parse(expression + length + 1 + left_len + 1, &right_node);

    op->setChildLeft(left_node);
    op->setChildRight(right_node);
    *nodePtr = op;

    length += left_len + right_len + 3;
  }
  else
  {
    //In this case, the current node should be set to type NUMBER,
    AlgebraicTreeNumber *num = new AlgebraicTreeNumber();
    //and the _parseNumber() function should be called to set its fields.
    char *pEnd;
    double d1;
    d1 = strtod(expression + length, &pEnd);
    //If the string does not start with a number, it should set the variable this->_value to 0.0.
    if (d1 == 0)
    { //???
      num->setValue(0.0);
      return 0;
    }
    num->setValue(d1);
    *nodePtr = num;
    length += pEnd - (expression + length);
  }

  while (isspace(expression[length]))
  {
    length++;
  }

  return length;
}
