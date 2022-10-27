

#include <iostream>
#include "Shape.hpp"
#include "Appearance.hpp"

Shape::Shape() : 
        _appearance((Node *)0),
        _geometry((Node *)0){
    //   
  }

Shape::~Shape()
  {
    //   
    _appearance = NULL;
    _geometry = NULL;
  }

Node *Shape::getAppearance()
  {
    return _appearance;
  }

Node *Shape::getGeometry()
  {
    return _geometry;
  }

  bool Shape::hasAppearanceNone()
  {

    //   
    Node *node = this->getAppearance();
    if (node == (Node *)0)
      return true;
    return false;
  }

  bool Shape::hasAppearanceMaterial()
  {

    //   
    if (this->getAppearance() == (Node *)0)
      return false;
    Appearance *appearance = (Appearance *)this->getAppearance();
    Node *node = appearance->getMaterial();
    if (node != (Node *)0)
      return true;
    return false;
  }

  bool Shape::hasAppearanceImageTexture()
  {

    //   
    if (this->getAppearance() == (Node *)0)
      return false;
    Appearance *appearance = (Appearance *)this->getAppearance();
    Node *node = appearance->getTexture();
    if (node != (Node *)0 && node->isImageTexture())
      return true;
    return false;
  }

  bool Shape::hasAppearanceUnsupported()
  {

    //  
    if (!_appearance->isImageTexture() && !_appearance->isMaterial())
      return true;
    return false;
  }

  bool Shape::hasGeometryNull()
  {

    //   
    Node *node = this->getGeometry();
    if (node == (Node *)0)
      return true;
    return false;
  }

  bool Shape::hasGeometryIndexedFaceSet()
  {

    //   
    Node *node = this->getGeometry();
    if (node != (Node *)0 && node->isIndexedFaceSet())
      return true;
    return false;
  }

bool Shape::hasGeometryIndexedLineSet()
  {

    //  
    if (_geometry->isIndexedLineSet())
      return true;
    return false;
  }

  bool Shape::hasGeometryUnsupported()
  {

    //  
    if (!_geometry->isIndexedLineSet() && !_geometry->isIndexedFaceSet() && _geometry != (Node *)0)
      return true;
    return false;
  }

void Shape::setAppearance(Node *node)
  {
    _appearance = node;
  }

void Shape::setGeometry(Node *node)
  {
    _geometry = node;
  }
