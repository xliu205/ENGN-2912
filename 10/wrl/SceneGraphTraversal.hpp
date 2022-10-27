

#ifndef _SCENE_GRAPH_TRAVERSAL_HPP_
#define _SCENE_GRAPH_TRAVERSAL_HPP_

#include "SceneGraph.hpp"

class SceneGraphTraversal
{

private:
  SceneGraph &_wrl;
  vector<Node *> _node;

public:
  SceneGraphTraversal(SceneGraph &wrl);

  void start();
  Node *next();
  int depth();
};

#endif // _SCENE_GRAPH_TRAVERSAL_HPP_
