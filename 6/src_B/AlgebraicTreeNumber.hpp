//
//  Calc6B
//  AlgebraicTreeNumber.hpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#ifndef _AlgebraicTreeNumber_hpp_
#define _AlgebraicTreeNumber_hpp_

#include <string>

#include "AlgebraicTreeNode.hpp"

class AlgebraicTreeNumber : public AlgebraicTreeNode {

public:

  virtual ~AlgebraicTreeNumber();

  AlgebraicTreeNumber(const double value=0.0);

  virtual const bool isNumber() const;
  virtual const bool isOperation() const;

  virtual double     evaluate() const;
  virtual unsigned   toStringLength() const;
  virtual unsigned   toString(std::string& str) const;

  void setValue(double value);

private:

  double _value;

};

#define BUFF_LENGTH 256
#define NUMBER_FORMAT "%.4f"

#endif // _AlgebraicTreeNumber_hpp_
