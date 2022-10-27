//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:04:51 taubin>
//------------------------------------------------------------------------
//
// IfsGLShader.hpp
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

#ifndef IFS_GLSHADER_HPP
#define IFS_GLSHADER_HPP

#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "IfsGLBuffer.hpp"

class IfsGLShader {

private:

  static const char *s_vsMaterial;
  static const char *s_vsMaterialNormal;
  static const char *s_vsColor;
  static const char *s_vsColorNormal;
  static const char *s_fsColor;

public:

  IfsGLShader(QColor& materialColor, QVector3D& lightSource);
  ~IfsGLShader();

  void           setPointSize(float pointSize);
  void           setVertexBuffer(IfsGLBuffer* vb);
  IfsGLBuffer*   getVertexBuffer() const;
  int            getNumberOfVertices();
  QMatrix4x4&    getMVPMatrix();
  void           paint(QOpenGLFunctions& f);

private:

  QOpenGLShader        *_vshader;
  QOpenGLShader        *_fshader;
  QOpenGLShaderProgram *_program;

  int                   _vertexAttr;
  int                   _normalAttr;
  int                   _colorAttr;
  int                   _mvpMatrixAttr ;
  int                   _materialAttr;
  int                   _lightSourceAttr;

  IfsGLBuffer          *_vertexBuffer;
  QColor&               _materialColor;
  QVector3D&            _lightSource;
  QMatrix4x4            _mvpMatrix; // viewport * projection * modelView
  float                 _pointSize;

};

#endif // IFS_GLSHADER_HPP
