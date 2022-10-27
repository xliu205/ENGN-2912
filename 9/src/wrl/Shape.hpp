

#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "Node.hpp"
using namespace std;

class Shape : public Node {

private:

  Node* _appearance;
  Node* _geometry;

public:
  
  Shape();
  virtual ~Shape();

  Node* getAppearance();
  Node* getGeometry();

  void setAppearance(Node* node);
  void setGeometry(Node* node);

  bool hasAppearanceNone();
  bool hasAppearanceMaterial();
  bool hasAppearanceImageTexture();
  bool hasGeometryNull();
  bool hasGeometryIndexedFaceSet();


  virtual bool isShape() const { return true; }
  virtual string getType() const { return "Shape"; }

  typedef bool (*Property)(Shape &shape);
  typedef void (*Operator)(Shape &shape);
};

#endif // _SHAPE_HPP_
