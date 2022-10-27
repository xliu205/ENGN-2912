

#include <iostream>
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"


SceneGraphTraversal::SceneGraphTraversal(SceneGraph &wrl) : _wrl(wrl)
{
}

void SceneGraphTraversal::start()
{
  _node.clear();
  int n = _wrl.getNumberOfChildren();
  while ((--n) >= 0)
    _node.push_back(_wrl[n]);
}

Node *SceneGraphTraversal::next()
{
  Node *next = (_node.size() > 0) ? _node.back() : (Node *)0;
 
  if (_node.size() > 0)
  {
    Node *node = _node.back();
    _node.pop_back();
    if (node->isGroup())
    {
      Group *group = (Group *)node;
      int n = group->getNumberOfChildren();
      while ((--n) >= 0)
        _node.push_back((*group)[n]);
    }
  }
  return next;
}

int SceneGraphTraversal::depth()
{
  int d = 0;
  if (_node.size() > 0)
    d = (_node.back())->getDepth();
  return d;
}