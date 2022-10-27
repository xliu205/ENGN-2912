//
//  Calc6A
//  AlgebraicTreeNode.cpp
//  Copyright (c) 2021 Gabriel Taubin. All rights reserved.
//

#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AlgebraicTreeNode.hpp"

AlgebraicTreeNode::~AlgebraicTreeNode() {
    if (_childLeft != nullptr) delete _childLeft;
    if (_childRight != nullptr) delete _childRight;
}

AlgebraicTreeNode::AlgebraicTreeNode() {
    _parent = nullptr;
    _type = INVALID;
    _value = 0.0;
    _childLeft = nullptr;
    _childRight = nullptr;
}

AlgebraicTreeNode::AlgebraicTreeNode(const double value) {
    _parent = nullptr;
    _type = NUMBER;
    _value = value;
    _childLeft = nullptr;
    _childRight = nullptr;
}

AlgebraicTreeNode::AlgebraicTreeNode(const AlgebraicTreeNodeType type) {
    _type = type;
}

AlgebraicTreeNode::AlgebraicTreeNode(AlgebraicTreeNode& src) {
    _parent = nullptr;
    _type = src._type;
    _value = src._value;
    if (src._childLeft != nullptr)
        _childLeft = new AlgebraicTreeNode(*src._childLeft);
    else
        _childLeft = nullptr;

    if (src._childRight != nullptr)
        _childRight = new AlgebraicTreeNode(*src._childRight);
    else
        _childRight = nullptr;
}

void AlgebraicTreeNode::setParent(AlgebraicTreeNode* parent) {
    _parent = parent;
}

void AlgebraicTreeNode::setChildLeft(AlgebraicTreeNode* childLeft) {
    _childLeft = childLeft;
    childLeft->setParent(this);
}

void AlgebraicTreeNode::setChildRight(AlgebraicTreeNode* childRight) {
    _childRight = childRight;
    _childRight->setParent(this);
}

double AlgebraicTreeNode::evaluate() const {
    switch (_type) {
    case NUMBER:
        return _value;
    case ADD:
        return _childLeft->evaluate() + _childRight->evaluate();
    case SUBTRACT:
        return _childLeft->evaluate() - _childRight->evaluate();
    case MULTIPLY:
        return _childLeft->evaluate() * _childRight->evaluate();
    case DIVIDE:
        return _childLeft->evaluate() / _childRight->evaluate();
    default:
        return 0.0f;
    }
}

const bool AlgebraicTreeNode::isInvalid() const {
    if (_type == INVALID) 
        return true;
    else 
        return false;
}

const bool AlgebraicTreeNode::isNumber() const {
    if (_type == NUMBER) 
        return true;
    else
        return false;
}

const bool AlgebraicTreeNode::isOperation() const {
    if (_type >= ADD && _type <= DIVIDE) 
        return true;
    else 
        return false;
}

const bool AlgebraicTreeNode::isRoot() const {
    if (isOperation() && _parent == nullptr)
        return true;
    else if (isNumber() && _childLeft == nullptr && _childRight == nullptr && _parent == nullptr)
        return true;
    else
        return false;
}
const bool AlgebraicTreeNode::isLeftChild() const {
    if (_parent != nullptr && this == _parent->_childLeft) 
        return true;
    else 
        return false;
}

const bool AlgebraicTreeNode::isRightChild() const {
    if (_parent != nullptr && this == _parent->_childRight) 
        return true;
    else
        return false;
}

std::string AlgebraicTreeNode::toString() const {
    std::stringstream ss;
    ss << _value;

    switch (_type) {
    case NUMBER:
        return ss.str();
    case ADD:
        return "(" + _childLeft->toString() + "+" + _childRight->toString() + ")";
    case SUBTRACT:
        return "(" + _childLeft->toString() + "-" + _childRight->toString() + ")";
    case MULTIPLY:
        return "(" + _childLeft->toString() + "*" + _childRight->toString() + ")";
    case DIVIDE:
        return "(" + _childLeft->toString() + "/" + _childRight->toString() + ")";
    default:
        return "INVALID";
    }
}

// new parser constructor
AlgebraicTreeNode::AlgebraicTreeNode(const char* expression) {
    _parent = nullptr;
    _type = INVALID;
    _value = 0.0;
    _childLeft = nullptr;
    _childRight = nullptr;

    const char* check_space = expression;
    while (isspace(static_cast<int>(*check_space))) check_space++;

    if (check_space - expression != strlen(expression)) {
        unsigned length = _parseExpression(expression);
        const char* spacePtr = expression + length;
        while (isspace(static_cast<int>(*spacePtr))) {
            spacePtr++;
            length++;
        }
        if (length == strlen(expression))
            _parseExpression(expression);

    }

}

// returns length of parsed substring 
unsigned AlgebraicTreeNode::_parseExpression(const char* expression) {
    unsigned length = 0;
    unsigned str_length = strlen(expression);
    unsigned num_par = 0;
    while (isspace(static_cast<int>(*expression))) {
       expression++;
    }
    if (expression[0] == '(') {
        unsigned i;
        num_par++;

        for (i = 1; i < str_length; i++) {
    
            if (expression[i] == '(')   num_par++;

            else if (expression[i] == ')')   num_par--;

            else if (expression[i] == '+' && num_par == 1 && i > 1 && expression[i - 1] != 'e') {
                this->_type = ADD;
                break;
            }

            else if (expression[i] == '-' && num_par == 1 && i > 1 && expression[i - 1] != 'e') {
                this->_type = SUBTRACT;
                break;
            }

            else if (expression[i] == '*' && num_par == 1 && i > 1) {
                this->_type = MULTIPLY;
                break;
            }

            else if (expression[i] == '/' && num_par == 0 && i > 1) {
                this->_type = DIVIDE;
                break;
            }
        }

        this->setChildLeft(new AlgebraicTreeNode());
        this->setChildRight(new AlgebraicTreeNode());
        this->_childLeft->_parseExpression(expression + 1);
        this->_childRight->_parseExpression(expression + i + 1);
        length = i;
    }
    else {
        this->_type = NUMBER;
        length = this->_parseNumber(expression);
    }

    return length;
}

unsigned AlgebraicTreeNode::_parseNumber(const char* expression) {
    char* endPtr;
    this->_value = strtod(expression, &endPtr);
    unsigned length = endPtr - expression;
    return length;
}
