

#ifndef _INDEXED_FACE_SET_HPP_
#define _INDEXED_FACE_SET_HPP_


#include "Node.hpp"
#include <vector>

class IndexedFaceSet : public Node {

private:

  bool          _ccw;
  bool          _convex;
  float         _creaseAngle;
  bool          _solid;
  bool          _normalPerVertex;
  bool          _colorPerVertex;
  vector<float> _coord;
  vector<int>   _coordIndex;
  vector<float> _normal;
  vector<int>   _normalIndex;
  vector<float> _color;
  vector<int>   _colorIndex;
  vector<float> _texCoord;
  vector<int>   _texCoordIndex;

public:
  
  IndexedFaceSet();

  void           clear();
  bool&          getCcw();
  bool&          getConvex();
  float&         getCreaseangle();
  bool&          getSolid();
  bool&          getNormalPerVertex();
  bool&          getColorPerVertex();
  vector<float>& getCoord();
  vector<int>&   getCoordIndex();
  vector<float>& getNormal();
  vector<int>&   getNormalIndex();
  vector<float>& getColor();
  vector<int>&   getColorIndex();
  vector<float>& getTexCoord();
  vector<int>&   getTexCoordIndex();

  int            getNumberOfFaces();
  int            getNumberOfCorners();
  int            getNumberOfCoord();
  int            getNumberOfNormal();
  int            getNumberOfColor();
  int            getNumberOfTexCoord();

  void           setNormalPerVertex(bool value);
  void           setColorPerVertex(bool value);

  enum Binding {
    PB_NONE = 0,
    PB_PER_VERTEX,
    PB_PER_FACE,
    PB_PER_FACE_INDEXED,
    PB_PER_CORNER
  };
  static const string stringBinding(Binding b)
  {
    return 
    (b == PB_PER_VERTEX) ? "PER_VERTEX" : 
    (b == PB_PER_FACE)       ? "PER_FACE": 
    (b == PB_PER_FACE_INDEXED) ? "PER_FACE_INDEXED": 
    (b == PB_PER_CORNER)       ? "PER_CORNER"
                                                                          : "NONE";
  }

  Binding        getCoordBinding();
  Binding        getNormalBinding();
  Binding        getColorBinding();
  Binding        getTexCoordBinding();

  virtual bool   isIndexedFaceSet() const { return true; }
  
  virtual string getType() const { return "IndexedFaceSet"; }

  typedef bool (*Property)(IndexedFaceSet &ifs);
  typedef void (*Operator)(IndexedFaceSet &ifs);

};

#endif // _INDEXED_FACE_SET_HPP_
