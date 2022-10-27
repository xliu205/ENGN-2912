//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 22:58:27 taubin>
//------------------------------------------------------------------------
//
// Ifs.cpp
//
// Software developed for the Fall 2021 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2015-2021, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Ifs.hpp"
#include <iostream>
#include <assert.h>

// VRML'97
//
// IndexedFaceSet {
//   eventIn       MFInt32 set_colorIndex
//   eventIn       MFInt32 set_coordIndex
//   eventIn       MFInt32 set_normalIndex
//   eventIn       MFInt32 set_texCoordIndex
//   exposedField  SFNode  color             NULL
//   exposedField  SFNode  coord             NULL
//   exposedField  SFNode  normal            NULL
//   exposedField  SFNode  texCoord          NULL
//   field         SFBool  ccw               TRUE
//   field         MFInt32 colorIndex        []        # [-1,)
//   field         SFBool  colorPerVertex    TRUE
//   field         SFBool  convex            TRUE
//   field         MFInt32 coordIndex        []        # [-1,)
//   field         SFFloat creaseAngle       0         # [ 0,)
//   field         MFInt32 normalIndex       []        # [-1,)
//   field         SFBool  normalPerVertex   TRUE
//   field         SFBool  solid             TRUE
//   field         MFInt32 texCoordIndex     []        # [-1,)
// }

Ifs::Ifs() {
/* remember to properly initialize all the class variables which do
   not have a default constructor */
  _ccw = true;
  _colorPerVertex = true;
  _convex = true;
  _creaseAngle = 0.0f;
  _normalPerVertex = true;
  _solid = true;
}

Ifs::~Ifs() {
  // TBD
  clear();
}
void Ifs::clear()
{
  _coord.clear();
  _coordIndex.clear();
  _normal.clear();
  _normalIndex.clear();
  _color.clear();
  _colorIndex.clear();
  _texCoord.clear();
  _texCoordIndex.clear();
}

bool &Ifs::getCcw() { return _ccw; }
bool &Ifs::getConvex() { return _convex; }
float &Ifs::getCreaseangle() { return _creaseAngle; }
bool &Ifs::getSolid() { return _solid; }
bool &Ifs::getNormalPerVertex() { return _normalPerVertex; }
bool &Ifs::getColorPerVertex() { return _colorPerVertex; }
vector<float> &Ifs::getCoord() { return _coord; }
vector<int> &Ifs::getCoordIndex() { return _coordIndex; }
vector<float> &Ifs::getNormal() { return _normal; }
vector<int> &Ifs::getNormalIndex() { return _normalIndex; }
vector<float> &Ifs::getColor() { return _color; }
vector<int> &Ifs::getColorIndex() { return _colorIndex; }
vector<float> &Ifs::getTexCoord() { return _texCoord; }
vector<int> &Ifs::getTexCoordIndex() { return _texCoordIndex; }
int Ifs::getNumberOfFaces() { return count(_coordIndex.begin(), _coordIndex.end(), -1); }
int Ifs::getNumberOfCorners() { return _coord.size() / 3; }
int Ifs::getNumberOfCoord() { return _coord.size() / 3; }
int Ifs::getNumberOfNormal() { return _normal.size() / 3; }
int Ifs::getNumberOfColor() { return _numberOfColor; }
int Ifs::getNumberOfTexCoord() { return _numberOfTextCoord; }
void Ifs::setNormalPerVertex(bool value) { _normalPerVertex = value; }
void Ifs::setColorPerVertex(bool value) { _colorPerVertex = value; }
void Ifs::setCcw(bool value) { _ccw = value; }
void Ifs::setConvex(bool value) { _convex = value; }
void Ifs::setCreaseangle(float value) { _creaseAngle = value; }
void Ifs::setSolid(bool value) { _solid = value; }
void Ifs::setCoord(float coords) { _coord.push_back(coords); }
void Ifs::setCoordIndex(int indexes) { _coordIndex.push_back(indexes); }
void Ifs::setNormal(float normals) { _normal.push_back(normals); }
void Ifs::setNormalIndex(int indexs) { _normalIndex.push_back(indexs); }
void Ifs::setColor(float colors) { _color.push_back(colors); }
void Ifs::setColorIndex(int indexes) { _colorIndex.push_back(indexes); }
void Ifs::setTexCoord(float texCoords) { _texCoord.push_back(texCoords); }
void Ifs::setTexCoordIndex(int indexes) { _texCoordIndex.push_back(indexes); }
void Ifs::setNumberOfFaces(int num) { _numberOfFaces = num; }
void Ifs::setNumberOfCorners(int num) { _numberOfCorners = num; }
void Ifs::setNumberOfCoord(int num) { _numberOfCoord = num; }
void Ifs::setNumberOfNormal(int num) { _numberOfNormal = num; }
void Ifs::setNumberOfColor(int num) { _numberOfColor = num; }
void Ifs::setNumberOfTexCoord(int num) { _numberOfTextCoord = num; }



// TODO  
Ifs::Binding Ifs::getNormalBinding() {
  // if(normal.size()==0)
  //   NO_NORMALS
  // else if(normalPerVertex==FALSE)
  //   if(normalIndex.size()>0)
  //     NORMAL_PER_FACE_INDEXED;
  //     assert(normalIndex.size()==getNumberOfFaces())
  //   else
  //     NORMAL_PER_FACE;
  //     assert(normal.size()/3==getNumberOfFaces())
  // else // if(normalPerVertex==TRUE)
  //   if(normalIndex.size()>0)
  //     NORMAL_PER_CORNER;
  //     assert(normalIndex.size()==coordIndex.size())
  //   else
  //     NORMAL_PER_VERTEX;
  //     assert(normal.size()/3==coord.size()/3)
  if (_normal.size() == 0)
    _normalBinding = PB_NONE;
  else if (_normalPerVertex == false)
  {
    if (_normalIndex.size() > 0)
    {
      //           if(_normalIndex.size()==getNumberOfFaces())
      _normalBinding = PB_PER_FACE_INDEXED;
      //           else _normalBinding=PB_NONE;
      assert(_normalIndex.size() == getNumberOfFaces());
    }
    else
    {
      //         if(_normal.size()/3==getNumberOfFaces())
      _normalBinding = PB_PER_FACE;
      //         else _normalBinding = PB_NONE;
      assert(_normal.size() / 3 == getNumberOfFaces());
    }
  }
  else
  { // if(normalPerVertex==TRUE)
    if (_normalIndex.size() > 0)
    {
      //           if(_normalIndex.size()==_coordIndex.size())
      _normalBinding = PB_PER_CORNER;
      //           else _normalBinding=PB_NONE;
      assert(_normalIndex.size() == _coordIndex.size());
    }
    else
    {
      //           if(_normal.size()/3==_coord.size()/3)
      _normalBinding = PB_PER_VERTEX;
      //           else _normalBinding=PB_NONE;
      assert(_normal.size() / 3 == _coord.size() / 3);
    }
  }
  //return PB_NONE;
  return _normalBinding;
}

// TODO  
Ifs::Binding Ifs::getColorBinding() {
  // if(color.size()==0)
  //   NO_COLORS
  // else if(colorPerVertex==FALSE)
  //   if(colorIndex.size()>0)
  //     COLOR_PER_FACE_INDEXED;
  //     assert(colorIndex.size()==getNumberOfFaces())
  //   else
  //     COLOR_PER_FACE;
  //     assert(color.size()/3==getNumberOfFaces())
  // else // if(colorPerVertex==TRUE)
  //   if(colorIndex.size()>0)
  //     COLOR_PER_CORNER;
  //     assert(colorIndex.size()==coordIndex.size())
  //   else
  //     COLOR_PER_VERTEX;
  //     assert(color.size()/3==coord.size()/3)
  //return PB_NONE;
  if (_color.size() == 0)
    _colorBinding = PB_NONE;
  else if (_colorPerVertex == false)
  {
    if (_colorIndex.size() > 0)
    {
      //           if(_colorIndex.size()==getNumberOfFaces())
      _colorBinding = PB_PER_FACE_INDEXED;
      //           else _colorBinding=PB_NONE;
      assert(_colorIndex.size() == getNumberOfFaces());
    }
    else
    {
      //           if(_color.size()/3==getNumberOfFaces())
      _colorBinding = PB_PER_FACE;
      //           else _colorBinding=PB_NONE;
      assert(_color.size() / 3 == getNumberOfFaces());
    }
  }
  else
  { // if(colorPerVertex==TRUE)
    if (_colorIndex.size() > 0)
    {
      //           if(_colorIndex.size()==_coordIndex.size())
      _colorBinding = PB_PER_CORNER;
      //           else _colorBinding=PB_NONE;
      assert(_colorIndex.size() == _coordIndex.size());
    }
    else
    {
      //           if(_color.size()/3==_coord.size()/3)
      _colorBinding = PB_PER_VERTEX;
      //           else _colorBinding=PB_NONE;
      assert(_color.size() / 3 == _coord.size() / 3);
    }
  }
  return _colorBinding;
}

// TODO  
Ifs::Binding Ifs::getTexCoordBinding() {
  // if(texCoord.size()==0)
  //   NO_TEX_COORD
  // else if(texCoordIndex.size()>0)
  //   TEX_COORD_PER_CORNER;
  //   assert(texCoordIndex.size()==coordIndex.size())
  // else
  //   TEX_COORD_PER_VERTEX;
  //   assert(texCoord.size()/3==coord.size()/3)
  //return PB_NONE;
  if (_texCoord.size() == 0)
    _textCoordBinding = PB_NONE;
  else if (_texCoordIndex.size() > 0)
  {
    //       if(_texCoordIndex.size()==_coordIndex.size())
    _textCoordBinding = PB_PER_CORNER;
    //       else _textCoordBinding=PB_NONE;
    assert(_texCoordIndex.size() == _coordIndex.size());
  }
  else
  {
    //       if(_texCoord.size()/3==_coord.size()/3)
    _textCoordBinding = PB_PER_VERTEX;
    //       else _textCoordBinding=PB_NONE;
    assert(_texCoord.size() / 3 == _coord.size() / 3);
  }
  return _textCoordBinding;
}

