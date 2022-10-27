

#include <iostream>
#include "Node.hpp"

Node::Node() : 
  _name(""),
  _parent((Node *)0),
  _show(true){
}
Node::~Node()
  {
  }

const string &Node::getName() const
  {
    return _name;
  }

void Node::setName(const string &name)
  {
    _name = name;
  }


bool Node::nameEquals(const string &name)
  {
    return (_name == name);
  }

const Node *Node::getParent() const
  {
    return _parent;
  }
void Node::setParent(const Node *node)
  {
    _parent = node;
  }

bool Node::getShow() const
  {
    return _show;
  }

void Node::setShow(bool value)
  {
    _show = value;
  }

int Node::getDepth() const
  {
    int d = 0;

    Node *node = (Node *)this;
    while (!node->isSceneGraph())
    {
      node = (Node *)this->getParent();
      d++;
    }

    return d;
  }
bool Node::isAppearance() const { return false; }
bool Node::isGroup() const { return false; }
bool Node::isImageTexture() const { return false; }
bool Node::isIndexedFaceSet() const { return false; }
bool Node::isMaterial() const { return false; }
bool Node::isPixelTexture() const { return false; }
bool Node::isSceneGraph() const { return false; }
bool Node::isShape() const { return false; }
bool Node::isTransform() const { return false; }

string Node::getType() const { return "Node"; }

