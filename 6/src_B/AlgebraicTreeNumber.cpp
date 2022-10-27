//
//  Calc6B
//  AlgebraicTreeNumber.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include <stdio.h>
#include <string>
#include "AlgebraicTreeNumber.hpp"

// remember that
// class AlgebraicTreeNumber : public AlgebraicTreeNode {

AlgebraicTreeNumber::~AlgebraicTreeNumber() {
  // TBD
}

AlgebraicTreeNumber::AlgebraicTreeNumber(const double value) {
  // TBD
}

const bool AlgebraicTreeNumber::isNumber() const {
  return true; // TBD
}

const bool AlgebraicTreeNumber::isOperation() const {
  return false; // TBD
}

double AlgebraicTreeNumber::evaluate() const {
  return _value; // TBD
}

unsigned AlgebraicTreeNumber::toStringLength() const {
  static char buff[BUFF_LENGTH];
  // TBD
  // 1) fill the buffer with the string representation of the node
  // 2) return the length of the resulting string
  return 0;
}

unsigned AlgebraicTreeNumber::toString(std::string& str) const {
  // TBD
  // 1) fill the str argument with the string representation of the node
  // 2) return the length of the resulting string
  str = std::to_string(_value);

  return str.length();
}

void AlgebraicTreeNumber::setValue(double value) {
  // TBD
  _value = value;
}
