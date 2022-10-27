//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:04:03 taubin>
//------------------------------------------------------------------------
//
// IfsGLBuffer.cpp
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

#include <math.h>
#include "IfsGLBuffer.hpp"

//////////////////////////////////////////////////////////////////////
IfsGLBuffer::IfsGLBuffer():
  QOpenGLBuffer(),
  _type(MATERIAL),
  _nVertices(0),
  _nNormals(0),
  _nColors(0),
  _hasFaces(false) {
}

//////////////////////////////////////////////////////////////////////
IfsGLBuffer::IfsGLBuffer(Ifs* pIfs, QColor& materialColor):
  QOpenGLBuffer(),
  _nVertices(0),
  _nNormals(0),
  _nColors(0) {

  QVector<QVector3D> m_vertices;
  QVector<QVector3D> m_normals;
  QVector<QVector3D> m_colors;

  if(pIfs==(Ifs*)0) return;

  vector<float>& coord       = pIfs->getCoord();
  vector<int>&   coordIndex  = pIfs->getCoordIndex();
  vector<float>& color       = pIfs->getColor();
  vector<int>&   colorIndex  = pIfs->getColorIndex();
  Ifs::Binding   cBinding    = pIfs->getColorBinding();
  vector<float>& normal      = pIfs->getNormal();
  vector<int>&   normalIndex = pIfs->getNormalIndex();
  Ifs::Binding   nBinding    = pIfs->getNormalBinding();
  // int         nV          = pIfs->getNumberOfCoord();
  int            nF          = pIfs->getNumberOfFaces();

  // material color values in [0.0:1.0] range
  qreal matR,matG,matB,matA;
  materialColor.getRgbF(&matR,&matG,&matB,&matA);
  QVector3D matColor(matR,matG,matB);

  _hasFaces  = (nF>0);
  _hasNormal = (nBinding!=Ifs::Binding::PB_NONE);
  _hasColor  = (cBinding!=Ifs::Binding::PB_NONE);

  _type =
    (_hasColor)?
    ((_hasNormal)?COLOR_NORMAL:COLOR):((_hasNormal)?MATERIAL_NORMAL:MATERIAL);

  m_vertices.clear();
  m_normals.clear();
  m_colors.clear();

  if(_hasFaces) {
    // polygon mesh

    float x[3][3];
    float n[3][3];
    float c[3][3];
    int j[3];

    int iN,iC,iV,k,h,i0,i1,iF,nTrianglesFace,nTrianglesMesh;
    for(nTrianglesMesh=iF=i0=i1=0;i1<(int)coordIndex.size();i1++) {
      if(coordIndex[i1]<0) {
        // number of triangles in this face
        nTrianglesFace = i1-i0-2;
        nTrianglesMesh += nTrianglesFace;

        // get normal per face
        iN = -1;
        if(nBinding==Ifs::Binding::PB_PER_FACE)
          iN=iF;
        else if(nBinding==Ifs::Binding::PB_PER_FACE_INDEXED)
          iN=normalIndex[iF];
        if(iN>=0)
          for(h=0;h<3;h++)
            n[0][h] = n[1][h] = n[2][h] = normal[3*iN+h];
        // get color per face
        iC = -1;
        if(cBinding==Ifs::Binding::PB_PER_FACE)
          iC = iF;
        else if(cBinding==Ifs::Binding::PB_PER_FACE_INDEXED)
          iC = colorIndex[iF];
        if(iC>=0)
          for(h=0;h<3;h++)
            c[0][h] = c[1][h] = c[2][h] = color[3*iC+h];
        // triangulate face [i0:i1) on the fly and add triangles to current mesh
        for(j[0]=i0,j[1]=i0+1,j[2]=i0+2;j[2]<i1;j[1]=j[2]++) {
          // triangle [j0,j1,j2]
          for(k=0;k<3;k++) {
            // get vertex coordinates
            iV = coordIndex[j[k]];
            for(h=0;h<3;h++)
              x[k][h] = coord[3*iV+h];
            // get normal per vertex or per corner
            iN = -1;
            if(nBinding==Ifs::Binding::PB_PER_VERTEX)
              iN = iV;
            else if (nBinding==Ifs::Binding::PB_PER_CORNER)
              iV = normalIndex[j[k]];
            if(iN>=0)
              for(h=0;h<3;h++)
                n[k][h] = normal[3*iN+h];
            // get color per vertex or per corner
            iC = -1;
            if(cBinding==Ifs::Binding::PB_PER_VERTEX)
              iC = iV;
            else if(cBinding==Ifs::Binding::PB_PER_CORNER)
              iC = colorIndex[j[k]];
            if(iC>=0)
              for(h=0;h<3;h++)
                c[k][h] = color[3*iC+h];
          }

          // if(computeFaceNormal) {
          //   float v1[3],v2[3],v3[3];
          //   for(h=0;h<3;h++) {
          //     v1[h] = x[1][h] - x[0][h];
          //     v2[h] = x[2][h] - x[0][h];
          //   }
          //   v3[0] = v1[2]*v2[1]-v1[1]*v2[2];
          //   v3[1] = v1[0]*v2[2]-v1[2]*v2[0];
          //   v3[2] = v1[1]*v2[0]-v1[0]*v2[1];
          //   float vv = v3[0]*v3[0]+v3[1]*v3[1]+v3[2]*v3[2];
          //   if(vv>0.0f) {
          //     vv = sqrt(vv);
          //     v3[0] /= vv; v3[1] /= vv; v3[2] /= vv;
          //   }
          //   for(h=0;h<3;h++)
          //     n[2][h] = n[1][h] = n[0][h] = v3[h];
          // }

          // push values into buffers
          for(k=2;k>=0;k--) {
            m_vertices.append(QVector3D(x[k][0],x[k][1],x[k][2]));
            if(_hasNormal /* || computeFaceNormal */)
              m_normals.append(QVector3D(n[k][0],n[k][1],n[k][2]));
            if(_hasColor)
              m_colors.append(QVector3D(c[k][0],c[k][1],c[k][2]));
          }
        }

        // advance to next face
        i0 = i1+1; iF++;
      }
    }

    // _hasNormal |= computeFaceNormal;

  } else /*if(!_hasFaces)*/ {
    // point cloud

    unsigned iV,nVertices;
    nVertices = pIfs->getNumberOfCoord();
    vector<float>::iterator x = coord.begin();
    vector<float>::iterator n = normal.begin();
    vector<float>::iterator c = color.begin();
    for(iV=0;iV<nVertices;iV++) {
      m_vertices.append(QVector3D(x[0],x[1],x[2])); x+=3;
      if(_hasNormal) {
        m_normals.append(QVector3D(n[0],n[1],n[2])); n+=3;
      }
      if(_hasColor) {
        m_colors.append(QVector3D(c[0],c[1],c[2]));
      }
    }
  }

  _nVertices = m_vertices.count();
  _nNormals  = m_normals.count();
  _nColors   = m_colors.count();

  // Use a vertex buffer object.

  this->create();
  this->bind();
  QVector<GLfloat> buf;
  buf.resize(3*_nVertices+3*_nNormals+3*_nColors);

  GLfloat *p = buf.data();
  for (unsigned i = 0; i < _nVertices; ++i) {
    // vertex coordinates
    *p++ = m_vertices[i].x();
    *p++ = m_vertices[i].y();
    *p++ = m_vertices[i].z();
    // vertex normals
    if(_nNormals>0) {
      *p++ = m_normals[i].x();
      *p++ = m_normals[i].y();
      *p++ = m_normals[i].z();
    }
    // vertex colors
    if(_nColors>0) {
      *p++ = m_colors[i].x();
      *p++ = m_colors[i].y();
      *p++ = m_colors[i].z();
    }
  }
  this->allocate(buf.constData(), buf.count() * sizeof(GLfloat));
  this->release();
}
