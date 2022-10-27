//
//  Calc6A
//  AlgebraicTreeNode.hpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#ifndef _AlgebraicTreeNode_hpp_
#define _AlgebraicTreeNode_hpp_

#include <string>
#include <string.h>
#include <math.h>

enum AlgebraicTreeNodeType {
  INVALID,NUMBER,ADD,SUBTRACT,MULTIPLY,DIVIDE
};

class AlgebraicTreeNode {

public:

  // destructor
  ~AlgebraicTreeNode();

  // empty constructor
  AlgebraicTreeNode();

  // construct a NUMBER node
  AlgebraicTreeNode(const double value);

  // construct a ADD,SUBTRACT,MULTIPLY, or DIVIDE node
  AlgebraicTreeNode(const AlgebraicTreeNodeType type);

  // copy constructor
  AlgebraicTreeNode(AlgebraicTreeNode& src);

  // parser constructor
  AlgebraicTreeNode(const char* expression);

  // setter
  void       setParent(AlgebraicTreeNode* parent);
  void       setChildLeft(AlgebraicTreeNode* childLeft);
  void       setChildRight(AlgebraicTreeNode* childRight);

  double     evaluate() const;

  std::string     toString() const;
                    
  const bool isInvalid() const;
  const bool isNumber() const;
  const bool isOperation() const;

  const bool isRoot() const;
  const bool isLeftChild() const;
  const bool isRightChild() const;

private:

  AlgebraicTreeNodeType _type;
  double                _value;
  AlgebraicTreeNode*    _parent;
  AlgebraicTreeNode*    _childLeft;
  AlgebraicTreeNode*    _childRight;

  unsigned _parseExpression(const char* expression);
  unsigned _parseNumber(const char* expression);

  // HINT:
  // implement these private methods and use them in the
  // implementation of some of the public methods defined above
  // 
  // It will be OK to design your implementation withou implementing
  // these methods
  // 
  // unsigned _toStringLength() const;
  // unsigned _toString(char* str) const;

};

// HINT:
// implement these global method and use it in the implementation of
// some of the public methods defined above
// 
// #define FTOA_BUFFER_LENGTH 1024
// #define FTOA_FORMAT "%.4f"
// unsigned _ftoa(const double value, char* str=(char*)0);

#endif // _AlgebraicTreeNode_hpp_
