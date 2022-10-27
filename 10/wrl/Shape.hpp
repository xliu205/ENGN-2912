

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

  void setAppearance(Node *node);
  Node *getAppearance();
  bool hasAppearanceNone();
  bool hasAppearanceMaterial();
  bool hasAppearanceImageTexture();
  bool hasAppearanceUnsupported();

  void setGeometry(Node *node);
  Node *getGeometry();
  bool hasGeometryNull();
  bool hasGeometryIndexedFaceSet();
  bool hasGeometryIndexedLineSet();
  bool hasGeometryUnsupported();

  virtual bool isShape() const { return true; }
  virtual string getType() const { return "Shape"; }

  typedef bool (*Property)(Shape &shape);
  typedef void (*Operator)(Shape &shape);
};

#endif // _SHAPE_HPP_
