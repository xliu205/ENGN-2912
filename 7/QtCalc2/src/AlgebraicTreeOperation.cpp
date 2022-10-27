#include "AlgebraicTreeOperation.hpp"

 AlgebraicTreeOperation::~AlgebraicTreeOperation() {
   // TBD
     if(this->isOperation()){
         delete this->getChildLeft();
         delete this->getChildRight();
     }
}

AlgebraicTreeOperation::AlgebraicTreeOperation(AlgebraicTreeOperationType type) {
  // TBD
    _type = type;
    _childLeft = (AlgebraicTreeNode*)0;
    _childRight = (AlgebraicTreeNode*)0;
}


const bool AlgebraicTreeOperation::isNumber() const {
  return false;
}

bool AlgebraicTreeOperation::operator==(AlgebraicTreeNode *src){
    AlgebraicTreeOperation* temp = dynamic_cast<AlgebraicTreeOperation*>(src);
    if(this->_type==temp->_type){
        return this->getChildLeft()->operator==(temp->getChildLeft()) && this->getChildRight()->operator==(temp->getChildRight());
    }else{
        return false;
    }
}

const bool AlgebraicTreeOperation::isOperation() const {
  return true;
}

double AlgebraicTreeOperation::evaluate() const {
  double value = 0.0f;
  // TBD
    if(_type==ADD)
        value += _childLeft->evaluate() + _childRight->evaluate();
    if(_type==SUBTRACT)
        value += _childLeft->evaluate() - _childRight->evaluate();
    if(_type==MULTIPLY)
        value += _childLeft->evaluate() * _childRight->evaluate();
    if(this->_type==DIVIDE)
        value += _childLeft->evaluate() / _childRight->evaluate();
  return value;
}

unsigned AlgebraicTreeOperation::toStringLength() const {
  unsigned length = 0;
  // TBD
    length+=3;
    length += this->_childLeft->toStringLength();
    length += this->_childRight->toStringLength();
  return length;
}

unsigned AlgebraicTreeOperation::toString(char& str) const {
  char* strPtr = &str;
  // TBD
    *strPtr = '(';
    strPtr++;
    strPtr += _childLeft->toString(*strPtr);
    if(_type==ADD) *(strPtr) = '+';
    if(_type==SUBTRACT) *(strPtr) = '-';
    if(_type==MULTIPLY) *(strPtr) = '*';
    if(_type==DIVIDE) *(strPtr) = '/';
    strPtr++;
    strPtr += _childRight->toString(*strPtr);
    *strPtr = ')';
    strPtr++;
  return strPtr-&str;
}

void AlgebraicTreeOperation::setType(AlgebraicTreeOperationType type) {
  // TBD
    _type = type;
}

void AlgebraicTreeOperation::setChildLeft(AlgebraicTreeNode* childLeft) {
  // TBD
    _childLeft = childLeft;
}

void AlgebraicTreeOperation::setChildRight(AlgebraicTreeNode* childRight) {
  // TBD
    _childRight = childRight;
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
