//
//  Calc6B
//  AlgebraicTreeOperation.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include "AlgebraicTreeOperation.hpp"

#include <string>

 AlgebraicTreeOperation::~AlgebraicTreeOperation() {
   // TBD
   _type = ZERO;
 }

AlgebraicTreeOperation::AlgebraicTreeOperation(AlgebraicTreeOperationType type) {
  // TBD
  _type = type;
}

const bool AlgebraicTreeOperation::isNumber() const {
  return false;
}

const bool AlgebraicTreeOperation::isOperation() const {
  return true;
}

double AlgebraicTreeOperation::evaluate() const {
  
  // TBD
  switch (_type)
  {
  case ZERO:
    return 0;
    break;
  case ADD:
    return _childLeft->evaluate() + _childRight->evaluate();
    break;
  case SUBTRACT:
    return _childLeft->evaluate() - _childRight->evaluate();
    break;
  case MULTIPLY:
    return _childLeft->evaluate() * _childRight->evaluate();
    break;
  case DIVIDE:
    return _childLeft->evaluate() / _childRight->evaluate();
    break;
  default:
    return 0.0f;
  }

}

unsigned AlgebraicTreeOperation::toStringLength() const {
  unsigned length = 0;
  // TBD
  return length;
}

unsigned AlgebraicTreeOperation::toString(std::string& str) const {
  // TBD
  // 1) fill the str argument with the string representation of the node
  // 2) return the length of the resulting string
 

  std::string l, r;
  _childLeft->toString(l);
  _childRight->toString(r);

  switch (_type)
  {
  case ZERO:
    return 0;
    break;
  case ADD:
    str = "(" + l + "+" + r + ")";
    break;
  case SUBTRACT:
    str = "(" + l + "-" + r + ")";
    break;
  case MULTIPLY:
    str = "(" + l + "*" + r + ")";
    break;
  case DIVIDE:
    str = "(" + l + "/" + r + ")";
    break;
  
  }
  return 0;
}

void AlgebraicTreeOperation::setType(AlgebraicTreeOperationType type) {
  // TBD
  _type = type;
}

void AlgebraicTreeOperation::setChildLeft(AlgebraicTreeNode* childLeft) {
  // TBD
  _childLeft = childLeft;
  childLeft->setParent(this);
}

void AlgebraicTreeOperation::setChildRight(AlgebraicTreeNode* childRight) {
  // TBD
  _childRight = childRight;
  _childRight->setParent(this);
}

AlgebraicTreeNode* AlgebraicTreeOperation::getChildLeft() {
  return _childLeft;
}

AlgebraicTreeNode* AlgebraicTreeOperation::getChildRight() {
  return _childRight;
}

AlgebraicTreeNode** AlgebraicTreeOperation::getChildLeftPtr() {
  return &_childLeft;
}

AlgebraicTreeNode** AlgebraicTreeOperation::getChildRightPtr() {
  return &_childRight;
}
