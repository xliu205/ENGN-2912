#include "AlgebraicTreeNode.hpp"

AlgebraicTreeNode::~AlgebraicTreeNode() {
  // TBD
    if(!this->isInvalid()){
        delete this;
    }
}

AlgebraicTreeNode::AlgebraicTreeNode() {
  // TBD
    _parent=(AlgebraicTreeNode*)0;
    _invalid = false;
}


const bool AlgebraicTreeNode::isRoot() const {
  return false; // TBD
}

const bool AlgebraicTreeNode::isInvalid() const {
    if(this->_invalid==true) return true;
  return false; // TBD
}

const bool AlgebraicTreeNode::isNumber() const {
  return false; // TBD
}

const bool AlgebraicTreeNode::isOperation() const {
  return false; // TBD
}

void AlgebraicTreeNode::setInvalid(bool value) {
  // TBD
    this->_invalid=value;
}

void AlgebraicTreeNode::setParent(AlgebraicTreeNode* parent) {
  // TBD
    this->_parent=parent;
}

