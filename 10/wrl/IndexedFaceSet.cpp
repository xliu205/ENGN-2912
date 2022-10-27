
#include "IndexedFaceSet.hpp"



IndexedFaceSet::IndexedFaceSet() : 
  _ccw(true),
  _convex(true),
  _creaseAngle(0),
  _solid(true),
  _normalPerVertex(true),
  _colorPerVertex(true)
{
}

void IndexedFaceSet::clear()
{

  // 
  _ccw = true;
  _convex = true;
  _creaseAngle = 0.0;
  _solid = true;
  _normalPerVertex = true;
  _colorPerVertex = true;
  _coord.clear();
  _coordIndex.clear();
  _normal.clear();
  _normalIndex.clear();
  _color.clear();
  _colorIndex.clear();
  _texCoord.clear();
  _texCoordIndex.clear();
}

bool& IndexedFaceSet::getCcw(){
  return _ccw;
}

bool& IndexedFaceSet::getConvex(){
  return _convex;
}

float& IndexedFaceSet::getCreaseangle(){
  return _creaseAngle;
}

bool& IndexedFaceSet::getSolid(){
  return _solid;
}

bool& IndexedFaceSet::getNormalPerVertex(){
  return _normalPerVertex;
}

bool& IndexedFaceSet::getColorPerVertex(){
  return _colorPerVertex;
}

vector<float>& IndexedFaceSet::getCoord(){
  return _coord;
}

vector<int>& IndexedFaceSet::getCoordIndex(){
  return _coordIndex;
}

vector<float>& IndexedFaceSet::getNormal(){
  return _normal;
}

vector<int>& IndexedFaceSet::getNormalIndex(){
  return _normalIndex;
}

vector<float>& IndexedFaceSet::getColor(){
  return _color;
}

vector<int>& IndexedFaceSet::getColorIndex(){
  return _colorIndex;
}

vector<float>& IndexedFaceSet::getTexCoord(){
  //   
  return _coord;
}

vector<int>& IndexedFaceSet::getTexCoordIndex(){
  //   
  return _coordIndex;
}

int IndexedFaceSet::getNumberOfFaces(){
  //   
  int nFaces = 0;
  for (int i = 0; i < _coordIndex.size(); i++)
  {
    if (_coordIndex[i] == -1)
      nFaces++;
  }

  return nFaces;
}

int IndexedFaceSet::getNumberOfCorners(){
  //   
  return 0;
}

int IndexedFaceSet::getNumberOfCoord(){
  //   
  return (int)(_coord.size()/3);
}

int IndexedFaceSet::getNumberOfNormal(){
  //   
  return (int)(_normal.size() / 3);
}

int IndexedFaceSet::getNumberOfColor(){
  //   
  return (int)(_color.size() / 3);
}

int IndexedFaceSet::getNumberOfTexCoord(){
  //   
  return (int)(_texCoord.size() / 2);
}

void IndexedFaceSet::setNormalPerVertex(bool value){
  //   
  _normalPerVertex = value;
}

void IndexedFaceSet::setColorPerVertex(bool value){
  //   
  _colorPerVertex = value;
}
  
IndexedFaceSet::Binding IndexedFaceSet::getCoordBinding(){
  //   
  return PB_PER_VERTEX;
}

IndexedFaceSet::Binding IndexedFaceSet::getNormalBinding(){
  //   
  if (_normal.size() == 0)
  {
    //	NO COLORS
    return PB_NONE;
  }
  else if (_normalPerVertex == false)
  {
    if (_normalIndex.size() > 0)
    {
      //	COLOR_PER_FACE_INDEXED;
      return PB_PER_FACE_INDEXED;

    }
    else
    {
      return PB_PER_FACE;
     
    }
  }
  else
  {
    if (_normalIndex.size() > 0)
    {
      return PB_PER_VERTEX;
      // assert(_normalIndex.size() == _coordIndex.size());
    }
    else
    {
      return PB_PER_VERTEX;
      // assert(_normal.size() / 3 == _coord.size() / 3);
    }
  }
  return PB_NONE;

}

IndexedFaceSet::Binding IndexedFaceSet::getColorBinding(){
  //   
  if (_color.size() == 0)
  {
    //	NO COLORS
  }
  else if (_colorPerVertex == false)
  {
    if (_colorIndex.size() > 0)
    {
      //	COLOR_PER_FACE_INDEXED;
      return PB_PER_FACE_INDEXED;
      //	If not equal, program will exit (parse error)
      // assert(_colorIndex.size() == Ifs::getNumberOfFaces());
    }
    else
    {
      //	COLOR_PER_FACE;
      return PB_PER_FACE;
      
    }
  }
  else
  {
    if (_colorIndex.size() > 0)
    {
      //	COLOR_PER_CORNER
      return PB_PER_CORNER;
      // assert(_colorIndex.size() == _coordIndex.size());
    }
    else
    {
      //  COLOR_PER_VERTEX;
      return PB_PER_CORNER;
      // assert(_color.size() / 3 == _coord.size() / 3);
    }
  }
  return PB_NONE;
}

IndexedFaceSet::Binding IndexedFaceSet::getTexCoordBinding(){
  //   
  if (_texCoord.size() == 0)
  {
    //	NO_TEX_COORD
    return PB_NONE;
  }
  else if (_texCoordIndex.size() > 0)
  {
    //	TEX_COORD_PER_CORNER
    return PB_PER_CORNER;
    // assert(_texCoordIndex.size() == _coordIndex.size());
  }
  else
  {
    //	TEX_COORD_PER_VERTEX
    return PB_PER_CORNER;
    // assert(_texCoord.size() / 3 == _coord.size() / 3);
  }
  return PB_NONE;
}

