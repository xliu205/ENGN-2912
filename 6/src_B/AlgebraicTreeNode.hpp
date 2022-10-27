//
//  Calc6B
//  AlgebraicTreeNode.hpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#ifndef _AlgebraicTreeNode_hpp_
#define _AlgebraicTreeNode_hpp_

#include <string>

class AlgebraicTreeNode {

public:

  virtual ~AlgebraicTreeNode();

  AlgebraicTreeNode();

  const bool isRoot() const;
  const bool isInvalid() const;

  virtual const bool isNumber() const;
  virtual const bool isOperation() const;

  virtual double   evaluate() const =0;
  virtual unsigned toStringLength() const =0;
  virtual unsigned toString(std::string& str) const =0;

  void setInvalid(bool value);
  void setParent(AlgebraicTreeNode* parent);

protected:

  bool               _invalid;
  AlgebraicTreeNode* _parent;

};

#endif // _AlgebraicTreeNode_hpp_
