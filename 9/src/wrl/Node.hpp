

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>

using namespace std;

class Color {
public:
  float r,g,b;
public:
  Color(float R=0.0f, float G=0.0f, float B=0.0f):r(R),g(G),b(B) {}
  Color& operator=(const Color& src) {
    r=src.r; g=src.g; b=src.b;
    return *this;
  }
};

class Vec2f {
public:
  float x,y;
public:
  Vec2f(float X=0.0f, float Y=0.0f):x(X),y(Y) {}
  Vec2f& operator=(Vec2f& src) {
    x=src.x; y=src.y;
    return *this;
  }
};

class Vec3f {
public:
  float x,y,z;
public:
  Vec3f(float X=0.0f, float Y=0.0f, float Z=0.0f):x(X),y(Y),z(Z) {}
  Vec3f& operator=(Vec3f& src) {
    x=src.x; y=src.y; z=src.z;
    return *this;
  }
};

class Vec4f {
public:
  float x,y,z,w;
public:
  Vec4f(float X=0.0f, float Y=0.0f, float Z=0.0f, float W=0.0f):
    x(X),y(Y),z(Z),w(W) {}
  Vec4f& operator=(Vec4f& src) {
    x=src.x; y=src.y; z=src.z; w=src.w;
    return *this;
  }
};

class Node {

protected:

  string _name;
  const Node *_parent;
  bool _show;

public:
  
  Node();
  virtual ~Node();

  const string& getName() const;
  void          setName(const string& name);
  bool nameEquals(const string &name);
  const Node *getParent() const;
  void setParent(const Node *node);
  bool getShow() const;
  void setShow(bool value);
  int getDepth() const;

  virtual bool  isAppearance() const;
  virtual bool  isGroup() const;
  virtual bool  isImageTexture() const;
  virtual bool  isIndexedFaceSet() const;
  virtual bool  isMaterial() const;
  virtual bool  isPixelTexture() const;
  virtual bool  isSceneGraph() const;
  virtual bool  isShape() const;
  virtual bool  isTransform() const;
  virtual string getType() const;

  typedef bool (*Property)(Node &node);
  typedef void (*Operator)(Node &node);
};

typedef Node *pNode;

#endif // _NODE_HPP_
