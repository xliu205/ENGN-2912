
#include "Transform.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include <algorithm>
#include <math.h>
#include "SceneGraph.hpp"
#include "SceneGraphTraversal.hpp"
#include <iostream>
#include <cmath>

Group::Group()
{
}

Group::~Group()
{
  // delete whole subtree
  for (int i = 0; i < _children.size(); i++)
  {
    delete _children.at(i);
  }
}

vector<pNode> &Group::getChildren()
{
  return _children;
}

unsigned Group::getNumberOfChildren()
{
  return _children.size();
}

pNode Group::operator[](const int i)
{
  // returns i-th child or (Node*)0
  Node *temp;
  try
  {
    temp = _children[i];
  }

  catch (const std::exception &e)
  {
    return (Node *)0;
  }

  return temp;
}

void Group::addChild(pNode child)
{
  _children.push_back(child);
}

void Group::removeChild(pNode child)
{
  vector<Node *>::iterator ite;
  ite = find(_children.begin(), _children.end(), child);
  _children.erase(ite);
}

void Group::clearBBox()
{
  _bboxCenter.x = 0;
  _bboxCenter.y = 0;
  _bboxCenter.z = 0;

  _bboxSize.x = 0;
  _bboxSize.y = 0;
  _bboxSize.z = 0;
}

bool Group::hasEmptyBBox()
{
  return (_bboxSize.x == 0 && _bboxSize.y == 0 && _bboxSize.z == 0);
}

Vec3f &Group::getBBoxCenter()
{
  return _bboxCenter;
}

Vec3f &Group::getBBoxSize()
{
  return _bboxSize;
}

float Group::getBBoxDiameter()
{
  float temp = max(_bboxSize.x, _bboxSize.y);
  float temp2 = max(temp, _bboxSize.z);
  return temp2;
}

void Group::updateBBox()
{
  // clear previous bbox if any
  _bboxCenter.x = 0.0;
  _bboxCenter.y = 0.0;
  _bboxCenter.z = 0.0;
  _bboxSize.x = 0.0;
  _bboxSize.y = 0.0;
  _bboxSize.z = 0.0;

  vector<float *> matrixStack;
  float M[16]; // global transform matrix

  // initialize current identity matrix
  M[0] = 1.0f;
  M[1] = 0.0f;
  M[2] = 0.0f;
  M[3] = 0.0f;
  M[4] = 0.0f;
  M[5] = 1.0f;
  M[6] = 0.0f;
  M[7] = 0.0f;
  M[8] = 0.0f;
  M[9] = 0.0f;
  M[10] = 1.0f;
  M[11] = 0.0f;
  M[12] = 0.0f;
  M[13] = 0.0f;
  M[14] = 0.0f;
  M[15] = 1.0f;

  SceneGraph *wrl = (SceneGraph *)this;

  // load wrl from file or create
  SceneGraphTraversal *t = new SceneGraphTraversal(*wrl);
  Node *child;
  t->start();

  while ((child = t->next()) != NULL)
  {

    if (child->isTransform())
    {
      Transform *temp = (Transform *)child;
      temp->getMatrix(M); // now M has transform matrix
      matrixStack.push_back(M);
    }

    if (child->isShape())
    {
      Shape *shape = (Shape *)child;
      child = shape->getGeometry();

      if (child != (Node *)0 && child->isIndexedFaceSet())
      {
        IndexedFaceSet *tifs = (IndexedFaceSet *)child;
        vector<float> &coords = tifs->getCoord();

     
        float minx = 10000000.0;
        float maxx = -10000000.0;

        float miny = 10000000.0;
        float maxy = -10000000.0;

        float minz = 10000000.0;
        float maxz = -10000000.0;

        for (int i = 0; i < coords.size(); i++)
        {
          int num = i % 3; 
         
          if (num == 0)
          {
            if (coords[i] < minx)
            {
              minx = coords[i];
            }
            if (coords[i] > maxx)
            {
              maxx = coords[i];
            }
          }

    
          if (num == 1)
          {
            if (coords[i] < miny)
            {
              miny = coords[i];
            }
            if (coords[i] > maxy)
            {
              maxy = coords[i];
            }
          }

       
          if (num == 2)
          {
            if (coords[i] < minz)
            {
              minz = coords[i];
            }
            if (coords[i] > maxz)
            {
              maxz = coords[i];
            }
          }
        }

        float xSize = (maxx - minx);
        float ySize = (maxy - miny);
        float zSize = (maxz - minz);

        this->_bboxSize.x = xSize;
        this->_bboxSize.y = ySize;
        this->_bboxSize.z = zSize;

        this->_bboxCenter.x = (maxx + minx) * (0.5f);
        this->_bboxCenter.y = (maxy + miny) * (0.5f);
        this->_bboxCenter.z = (maxz + minz) * (0.5f);

  
        std::vector<Vec4f> corners(8);
        std::vector<Vec4f> updated_corners;

     
        for (int i = 0; i < corners.size(); i++)
        {
          corners.at(i).w = 1;
        }


        corners.at(0).x = _bboxCenter.x - xSize / 2.0;
        corners.at(0).y = _bboxCenter.y - ySize / 2.0;
        corners.at(0).z = _bboxCenter.z - zSize / 2.0;

        corners.at(1).x = _bboxCenter.x - xSize / 2.0;
        corners.at(1).y = _bboxCenter.y - ySize / 2.0;
        corners.at(1).z = _bboxCenter.z + zSize / 2.0;

        corners.at(2).x = _bboxCenter.x - xSize / 2.0;
        corners.at(2).y = _bboxCenter.y + ySize / 2.0;
        corners.at(2).z = _bboxCenter.z - zSize / 2.0;

        corners.at(3).x = _bboxCenter.x - xSize / 2.0;
        corners.at(3).y = _bboxCenter.y + ySize / 2.0;
        corners.at(3).z = _bboxCenter.z + zSize / 2.0;

        corners.at(4).x = _bboxCenter.x + xSize / 2.0;
        corners.at(4).y = _bboxCenter.y - ySize / 2.0;
        corners.at(4).z = _bboxCenter.z - zSize / 2.0;

        corners.at(5).x = _bboxCenter.x + xSize / 2.0;
        corners.at(5).y = _bboxCenter.y - ySize / 2.0;
        corners.at(5).z = _bboxCenter.z + zSize / 2.0;

        corners.at(6).x = _bboxCenter.x + xSize / 2.0;
        corners.at(6).y = _bboxCenter.y + ySize / 2.0;
        corners.at(6).z = _bboxCenter.z - zSize / 2.0;

        corners.at(7).x = _bboxCenter.x + xSize / 2.0;
        corners.at(7).y = _bboxCenter.y + ySize / 2.0;
        corners.at(7).z = _bboxCenter.z + zSize / 2.0;

        
        for (int i = 0; i < corners.size(); i++)
        {
          Vec4f oldCorner = corners.at(i);
          Vec4f newCorner;

          newCorner.x = (M[0] * oldCorner.x + M[1] * oldCorner.y + M[2] * oldCorner.z + M[3] * oldCorner.w);

          newCorner.y = (M[4] * oldCorner.x + M[5] * oldCorner.y + M[6] * oldCorner.z + M[7] * oldCorner.w);

          newCorner.z = (M[8] * oldCorner.x + M[9] * oldCorner.y + M[10] * oldCorner.z + M[11] * oldCorner.w);

          newCorner.w = (M[12] * oldCorner.x + M[13] * oldCorner.y + M[14] * oldCorner.z + M[15] * oldCorner.w);

          updated_corners.push_back(newCorner);
        }

        float newMinx = 10000000;
        float newMaxx = -10000000;

        float newMiny = 10000000;
        float newMaxy = -10000000;

        float newMinz = 10000000;
        float newMaxz = -10000000;

        for (int i = 0; i < updated_corners.size(); i++)
        {
          if (updated_corners.at(i).x < newMinx)
          {
            newMinx = updated_corners.at(i).x;
          }

          if (updated_corners.at(i).x > newMaxx)
          {
            newMaxx = updated_corners.at(i).x;
          }

          if (updated_corners.at(i).y < newMiny)
          {
            newMiny = updated_corners.at(i).y;
          }

          if (updated_corners.at(i).y > newMaxy)
          {
            newMaxy = updated_corners.at(i).y;
          }

          if (updated_corners.at(i).z < newMinz)
          {
            newMinz = updated_corners.at(i).z;
          }

          if (updated_corners.at(i).z > newMaxz)
          {
            newMaxz = updated_corners.at(i).z;
          }
        }

        float newXlen = std::abs(newMaxx - newMinx);
        float newYlen = std::abs(newMaxy - newMiny);
        float newZlen = std::abs(newMaxz - newMinz);

        this->_bboxSize.x = newXlen * (1.00f);
        this->_bboxSize.y = newYlen * (1.00f);
        this->_bboxSize.z = newZlen * (1.00f);

        this->_bboxCenter.x = (newMaxx + newMinx) * (0.5f);
        this->_bboxCenter.y = (newMaxy + newMiny) * (0.5f);
        this->_bboxCenter.z = (newMaxz + newMinz) * (0.5f);
      }
    }
  } 
}
