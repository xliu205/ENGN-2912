

#include <iostream>
#include <math.h>
#include <algorithm>
#include "Transform.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "IndexedLineSet.hpp"
#include <SceneGraphTraversal.cpp>

Group::Group() : _bboxCenter(0.0f, 0.0f, 0.0f),
                 _bboxSize(-1.0f, -1.0f, -1.0f)
{
}

Group::~Group()
{

  _children.clear();
 // _bboxCenter = Vec3f(0.0f, 0.0f, 0.0f);
 // _bboxSize = Vec3f(-1.0f, -1.0f, -1.0f);
}

vector<pNode> &Group::getChildren()
{
  return _children;
}

Node *Group::getChild(const string &name) const
{
  Node *node = (Node *)0;

  
  int n = _children.size();
  for (int i = 0; i < n; i++)
  {
    if (_children[i]->getName() == name)
    {
      node = _children[i];
      break;
    }
  }

  return node;
}

int Group::getNumberOfChildren() const
{

  //  
  return _children.size();
}

pNode Group::operator[](const int i)
{
  pNode node = (pNode)0;

  
  if (i >= 0 && i < this->getNumberOfChildren())
  {
    node = _children[i];
  }

  return node;
}

Vec3f &Group::getBBoxCenter()
{

  //  
  return _bboxCenter;
}

Vec3f &Group::getBBoxSize()
{

  //  
  return _bboxSize;
}

float Group::getBBoxDiameter()
{
  float diam2 = 0.0f;

  diam2 = _bboxSize.x * _bboxSize.x + _bboxSize.y * _bboxSize.y +
          _bboxSize.z * _bboxSize.z;
  return (diam2 > 0.0f) ? (float)sqrt(diam2) : 0.0f;
}

void Group::addChild(const pNode child)
{

  int n = this->getNumberOfChildren();
  for (int i = 0; i < n; i++)
  {
    if (this->getChildren()[i]->getName() == child->getName())
    {
      return;
    }
  }
  _children.push_back(child);
  child->setParent(this);
}

void Group::removeChild(const pNode child)
{


  int n = this->getNumberOfChildren();
  for (int i = 0; i < n; i++)
  {
    if (this->getChildren()[i] == child)
    {
      delete this->getChildren()[i];
      this->getChildren().erase(this->getChildren().begin() + i);
    }
  }
}

void Group::setBBoxCenter(Vec3f &value)
{

  //  
  _bboxCenter = value;
}
void Group::setBBoxSize(Vec3f &value)
{

  //  
  _bboxSize = value;
}

void Group::clearBBox()
{

  //  
    Vec3f vec(0.0f,0.0f,0.0f);
    _bboxCenter = vec;
    Vec3f vec2(-1.0f,-1.0f,-1.0f);
    _bboxSize= vec2;
  //_bboxCenter = Vec3f(0.0f, 0.0f, 0.0f);
  //_bboxSize = Vec3f(-1.0f, -1.0f, -1.0f);
}
bool Group::hasEmptyBBox() const
{

  //  
  if (_bboxSize.x < 0 || _bboxSize.y < 0 || _bboxSize.z < 0)
    return true;
  return false;
}

void Group::appendBBoxCoord(vector<float> &coord)
{


  if (!this->hasEmptyBBox())
  {
    float xMax = 0, yMax = 0, zMax = 0, xMin = 0, yMin = 0, zMin = 0, i = 0;
    while (i < coord.size())
    {
      xMax = coord[i] > xMax ? coord[i] : xMax;
      xMin = coord[i] < xMin ? coord[i] : xMin;
      i++;
      yMax = coord[i] > yMax ? coord[i] : yMax;
      yMin = coord[i] < yMin ? coord[i] : yMin;
      i++;
      zMax = coord[i] > zMax ? coord[i] : zMax;
      zMin = coord[i] < zMin ? coord[i] : zMin;
      i++;
    }
    coord.push_back(xMax);
    coord.push_back(yMax);
    coord.push_back(zMax);
    coord.push_back(xMax);
    coord.push_back(yMax);
    coord.push_back(zMin);
    coord.push_back(xMax);
    coord.push_back(yMin);
    coord.push_back(zMax);
    coord.push_back(xMax);
    coord.push_back(yMin);
    coord.push_back(zMin);

    coord.push_back(xMin);
    coord.push_back(yMax);
    coord.push_back(zMax);
    coord.push_back(xMin);
    coord.push_back(yMax);
    coord.push_back(zMin);
    coord.push_back(xMin);
    coord.push_back(yMin);
    coord.push_back(zMax);
    coord.push_back(xMin);
    coord.push_back(yMin);
    coord.push_back(zMin);
  }
}

void Group::updateBBox(vector<float> &coord)
{


}

void Group::updateBBox()
{

  SceneGraph wrl;
  wrl.addChild(this);
  
  Eigen::Matrix4f T = Eigen::Matrix4f::Identity(4, 4);
  stack<Eigen::Matrix4f> stack;
  Node *node = &wrl;
  stack.push(T); 
  Group::dfs(stack, node, T);
}

void Group::dfs(stack<Eigen::Matrix4f> stack, Node *node, Eigen::Matrix4f T)
{
  if (node == (Node *)0)
    return;

  if (node->isShape())
  {
    Shape *shape = (Shape *)node;
    
    if (shape->getName() == "BOUNDING-BOX")
    {
      return;
    }
    IndexedFaceSet *geometry = (IndexedFaceSet *)shape->getGeometry();

    vector<float> &coord = geometry->getCoord();
    Vec3f center = _bboxCenter;
    Vec3f size = _bboxSize;
    float xMax = center.x + size.x / 2, yMax = center.y + size.y / 2, zMax = center.z + size.z / 2,
          xMin = center.x - size.x / 2, yMin = center.y - size.y / 2, zMin = center.z - size.z / 2;
    for (int i = 0; i < geometry->getNumberOfCoord(); i++)
    {
      Eigen::Vector4f currCoord;
      int count = 0;
      for (int j = i * 3; j < i * 3 + 3; j++)
      {
        currCoord(count++) = coord[j];
      }
      currCoord(3) = 1.0f;
      Eigen::Vector4f tempCoord = T * currCoord;
      xMax = tempCoord(0) > xMax ? tempCoord(0) : xMax;
      yMax = tempCoord(1) > yMax ? tempCoord(1) : yMax;
      zMax = tempCoord(2) > zMax ? tempCoord(2) : zMax;
      xMin = tempCoord(0) < xMin ? tempCoord(0) : xMin;
      yMin = tempCoord(1) < yMin ? tempCoord(1) : yMin;
      zMin = tempCoord(2) < zMin ? tempCoord(2) : zMin;
    }

    Vec3f vec3(xMax - xMin, yMax - yMin, zMax - zMin);
    Vec3f vec4((xMax + xMin) / 2, (yMax + yMin) / 2, (zMax + zMin) / 2);
    _bboxSize = vec3;
    _bboxCenter = vec4;
  }
  else if (node->isTransform())
  {
    stack.push(T);
    Transform *transform = (Transform *)node;
    Eigen::Matrix4f currT = Eigen::Matrix4f::Identity(4, 4);
    transform->getMatrix(currT);
    T = currT * T;

    for (int i = 0; i < transform->getNumberOfChildren(); i++)
    {
      dfs(stack, transform->getChildren()[i], T);
    }
    T = stack.top();
    stack.pop();
  }
  else if (node->isGroup())
  {
    stack.push(T);
    Group *group = (Group *)node;
    for (int i = 0; i < group->getNumberOfChildren(); i++)
    {
      dfs(stack, group->getChildren()[i], T);
    }
    T = stack.top();
    stack.pop();
  }
}
