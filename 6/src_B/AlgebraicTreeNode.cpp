//
//  Calc6B
//  AlgebraicTreeNode.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include "AlgebraicTreeNode.hpp"



AlgebraicTreeNode::AlgebraicTreeNode() {
  // TBD

}

AlgebraicTreeNode::~AlgebraicTreeNode()
{

   
}

const bool AlgebraicTreeNode::isRoot() const {
        return false;
  
}

const bool AlgebraicTreeNode::isInvalid() const {
    if (!_invalid)
        return true;
    else
        return false;

}

const bool AlgebraicTreeNode::isNumber() const {
    return false;
}

const bool AlgebraicTreeNode::isOperation() const {
        return false;
}

void AlgebraicTreeNode::setInvalid(bool value) {
  // TBD
    _invalid = value;
}

void AlgebraicTreeNode::setParent(AlgebraicTreeNode* parent) {
    _parent = parent;
}

