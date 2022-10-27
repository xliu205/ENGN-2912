

#include <iostream>
#include "SceneGraph.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "Appearance.hpp"
SceneGraph::SceneGraph()
{
  _parent = this; // SceneGraph is always a tree root
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::clear()
{

  getChildren().clear();
}

string &SceneGraph::getUrl()
{
  return _url;
}

void SceneGraph::setUrl(const string &url)
{
  _url = url;
}


Node *SceneGraph::find(const string &name)
{
  Node *node = (Node *)0;


  SceneGraphTraversal *t = new SceneGraphTraversal(*this);
  t->start();
  node = t->next();
  while (node != NULL)
  {
    if (node->isShape())
    {
      Shape *shape = (Shape *)node;
      if (shape->nameEquals(name))
        break;
      if (!shape->hasAppearanceNone())
      {
        if (shape->getAppearance()->nameEquals(name))
        {
          node = shape->getAppearance();
          break;
        }
      }
      if (!shape->hasAppearanceNone() && shape->hasAppearanceMaterial())
      {
        Appearance *appearance = (Appearance *)shape;
        if (appearance->getMaterial()->nameEquals(name))
        {
          node = appearance->getMaterial();
          break;
        }
      }
      if (!shape->hasAppearanceNone() && shape->hasAppearanceImageTexture())
      {
        Appearance *appearance = (Appearance *)shape;
        if (appearance->getTexture()->nameEquals(name))
        {
          node = appearance->getTexture();
          break;
        }
      }
      if (shape->hasGeometryIndexedFaceSet())
      {
        if (shape->getGeometry()->nameEquals(name))
        {
          node = shape->getGeometry();
          break;
        }
      }
    }
    node = t->next();
  }
  return node;
}
