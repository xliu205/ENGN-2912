

#ifndef _SCENE_GRAPH_HPP_
#define _SCENE_GRAPH_HPP_

#include "Group.hpp"
#include "Transform.hpp"

using namespace std;

class SceneGraph : public Group
{

private:
  string _url;

public:
  SceneGraph();
  virtual ~SceneGraph();

  void clear();

  string &getUrl();
  void setUrl(const string &url);

  Node *find(const string &name);

  virtual bool isSceneGraph() const { return true; }
  virtual string getType() const { return "SceneGraph"; }

  typedef bool (*Property)(SceneGraph &sceneGraph);
  typedef void (*Operator)(SceneGraph &sceneGraph);
};

#endif // _SCENE_GRAPH_HPP_
