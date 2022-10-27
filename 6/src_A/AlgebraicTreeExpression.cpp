//
//  Calc6A
//  AlgebraicTreeExpression.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include "AlgebraicTreeExpression.hpp"

AlgebraicTreeExpression::~AlgebraicTreeExpression() {
  if(_root!=(AlgebraicTreeNode*)0) delete _root;
}

AlgebraicTreeExpression::AlgebraicTreeExpression():
  _root(new AlgebraicTreeNode()) {
}

double AlgebraicTreeExpression::evaluate() const {
  return _root->evaluate();
}

std::string AlgebraicTreeExpression::toString() const {
  return _root->toString();
}

// parser constructor
AlgebraicTreeExpression::AlgebraicTreeExpression(const char* expression):
  _root(new AlgebraicTreeNode(expression)) {
}

// copy constructor
AlgebraicTreeExpression::AlgebraicTreeExpression(AlgebraicTreeExpression& src) :
  _root(new AlgebraicTreeNode(*src._root)) {
}

// assignment operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::operator=(AlgebraicTreeExpression& src) {
    AlgebraicTreeExpression* temp = new AlgebraicTreeExpression(src);
    this->swap(*temp);
    delete temp;
    return *this;
}

// swap operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::swap(AlgebraicTreeExpression& src) {
    AlgebraicTreeNode* temp = src._root;
    src._root = this->_root;
    this->_root = temp;
    delete temp;
    return *this;
}

// increment operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::operator+=(AlgebraicTreeExpression& src) {
    this->_operatorInPlace(ADD, src);
    return *this;
}

// decrement operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::operator-=(AlgebraicTreeExpression& src) {
    this->_operatorInPlace(SUBTRACT, src);
    return *this;
}

// multiplication in place operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::operator*=(AlgebraicTreeExpression& src) {
    this->_operatorInPlace(MULTIPLY, src);
    return *this;
}

// division in place operator
AlgebraicTreeExpression&
AlgebraicTreeExpression::operator/=(AlgebraicTreeExpression& src) {
    this->_operatorInPlace(DIVIDE, src);
    return *this;
}

AlgebraicTreeExpression&
AlgebraicTreeExpression::_operatorInPlace(AlgebraicTreeNodeType opr, AlgebraicTreeExpression& src) {
    if (src._root->isInvalid() || this->_root->isInvalid()) {
        delete this->_root;
        this->_root = new AlgebraicTreeNode();
    }
    else {
        AlgebraicTreeNode* new_root = new AlgebraicTreeNode(opr);
        AlgebraicTreeNode* copy_src = new AlgebraicTreeNode(*src._root);
        new_root->setChildLeft(this->_root);
        new_root->setChildRight(copy_src);
        this->_root = new_root;
    }
    return *this;
}