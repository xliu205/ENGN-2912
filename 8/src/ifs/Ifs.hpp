//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 22:58:42 taubin>
//------------------------------------------------------------------------
//
// Ifs.hpp
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

#ifndef _IFS_HPP_
#define _IFS_HPP_

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

#include <vector>
#include <algorithm>

using namespace std;


class Ifs
{

public:
  enum Binding
  {
    PB_NONE = 0,
    PB_PER_VERTEX,
    PB_PER_FACE,
    PB_PER_FACE_INDEXED,
    PB_PER_CORNER
  };

  Ifs();
  ~Ifs();

  void clear();
  bool &getCcw();
  bool &getConvex();
  float &getCreaseangle();
  bool &getSolid();
  bool &getNormalPerVertex();
  bool &getColorPerVertex();
  vector<float> &getCoord();
  vector<int> &getCoordIndex();
  vector<float> &getNormal();
  vector<int> &getNormalIndex();
  vector<float> &getColor();
  vector<int> &getColorIndex();
  vector<float> &getTexCoord();
  vector<int> &getTexCoordIndex();
  int getNumberOfFaces();
  int getNumberOfCorners();
  int getNumberOfCoord();
  int getNumberOfNormal();
  int getNumberOfColor();
  int getNumberOfTexCoord();
  void setNormalPerVertex(bool value);
  void setColorPerVertex(bool value);
  void setCcw(bool value);
  void setConvex(bool value);
  void setCreaseangle(float value);
  void setSolid(bool value);
  void setCoord(float coords);
  void setCoordIndex(int indexes);
  void setNormal(float normals);
  void setNormalIndex(int indexs);
  void setColor(float colors);
  void setColorIndex(int indexes);
  void setTexCoord(float texCoords);
  void setTexCoordIndex(int indexes);
  void setNumberOfFaces(int num);
  void setNumberOfCorners(int num);
  void setNumberOfCoord(int num);
  void setNumberOfNormal(int num);
  void setNumberOfColor(int num);
  void setNumberOfTexCoord(int num);
  void clearNormal() { _normal.clear(); }
  void clearNormalIndex() { _normalIndex.clear(); }
  void clearCoord() { _coord.clear(); }
  void clearCoordIndex() { _coordIndex.clear(); }
  void clearColor() { _color.clear(); }
  void clearColorIndex() { _colorIndex.clear(); }
  void clearTexCoord() { _texCoord.clear(); }
  void clearTexCoordIndex() { _texCoordIndex.clear(); }

  Binding getNormalBinding();
  Binding getColorBinding();
  Binding getTexCoordBinding();

private:
  bool _ccw;
  bool _convex;
  float _creaseAngle;
  bool _solid;
  bool _normalPerVertex;
  bool _colorPerVertex;
  vector<float> _coord;
  vector<int> _coordIndex;
  vector<float> _normal;
  vector<int> _normalIndex;
  vector<float> _color;
  vector<int> _colorIndex;
  vector<float> _texCoord;
  vector<int> _texCoordIndex;
  int _numberOfFaces = 0;
  int _numberOfCorners = 0;
  int _numberOfCoord = 0;
  int _numberOfNormal = 0;
  int _numberOfColor = 0;
  int _numberOfTextCoord = 0;
  Binding _normalBinding;
  Binding _colorBinding;
  Binding _textCoordBinding;
  // TODO
  // class variables
};

#endif // _IFS_HPP_

