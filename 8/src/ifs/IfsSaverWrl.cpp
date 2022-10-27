//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:00:46 taubin>
//------------------------------------------------------------------------
//
// IfsSaverWrl.cpp
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

#include "IfsSaverWrl.hpp"

const char* IfsSaverWrl::_ext = "wrl";

bool IfsSaverWrl::save(const char* filename, Ifs& ifs) {
  bool success = false;
  if(filename!=(char*)0) {
    // TODO : check that filename extension == ext()
    FILE* fp = fopen(filename,"w");
    if(fp!=(FILE*)0) {

      bool&          _ccw             = ifs.getCcw();
      bool&          _convex          = ifs.getConvex();
      float&         _creaseAngle     = ifs.getCreaseangle();
      bool&          _solid           = ifs.getSolid();
      bool&          _normalPerVertex = ifs.getNormalPerVertex();
      bool&          _colorPerVertex  = ifs.getColorPerVertex();
      vector<float>& _coord           = ifs.getCoord();
      vector<int>&   _coordIndex      = ifs.getCoordIndex();
      vector<float>& _normal          = ifs.getNormal();
      vector<int>&   _normalIndex     = ifs.getNormalIndex();
      vector<float>& _color           = ifs.getColor();
      vector<int>&   _colorIndex      = ifs.getColorIndex();
      vector<float>& _texCoord        = ifs.getTexCoord();
      vector<int>&   _texCoordIndex   = ifs.getTexCoordIndex();

      fprintf(fp,"#VRML V2.0 utf8\n");
      fprintf(fp,"Shape {\n");
      fprintf(fp," geometry IndexedFaceSet {\n");

      // default ccw TRUE
      if(_ccw   ==false)   fprintf(fp,"  ccw FALSE\n");
      // default convex TRUE
      if(_convex==false)   fprintf(fp,"  convex FALSE\n");
      // default solid TRUE
      if(_solid ==false)   fprintf(fp,"  solid FALSE\n");
      // default creaseAngle 0.0
      if(_creaseAngle>0.0) fprintf(fp,"  creaseAngle %8.4f\n",_creaseAngle);

      if(_coordIndex.size()>0) {
        fprintf(fp,"  coordIndex [\n");
        for(unsigned i=0;i<_coordIndex.size();i++) {
          fprintf(fp,"%d ",_coordIndex[i]);
          if(_coordIndex[i]<0) fprintf(fp,"\n");
        }
        fprintf(fp,"  ]\n");
      }

      // COORD_PER_VERTEX
      if(_coord.size()>0) {
        fprintf(fp,"  coord Coordinate {\n");
        fprintf(fp,"   point [\n");
        for(unsigned i=0;i<_coord.size();i++) {
          fprintf(fp,"%8.4f ",_coord[i]);
          if(i%3==2) fprintf(fp,"\n");
        }
        fprintf(fp,"   ]\n");
        fprintf(fp,"  }\n");
      }

      // if(normal.size()==0)
      //   NO_NORMALS
      // else if(normalPerVertex==FALSE)
      //   if(normalIndex.size()>0)
      //     NORMAL_PER_FACE_INDEXED;
      //     normalIndex.size()==getNumberOfFaces()
      //   else
      //     NORMAL_PER_FACE;
      //     normal.size()/3==getNumberOfFaces()
      // else // if(normalPerVertex==TRUE)
      //   if(normalIndex.size()>0)
      //     NORMAL_PER_CORNER_INDEXED;
      //     normalIndex.size()==coordIndex.size()
      //   else
      //     NORMAL_PER_VERTEX;
      //     normal.size()/3==coord.size()/3

      if(_normal.size()>0) {
        unsigned i;
        fprintf(fp,"  normalPerVertex %s\n",
                (_normalPerVertex==true)?"TRUE":"FALSE");
      
        fprintf(fp,"  normal Normal {\n");
        fprintf(fp,"   vector [\n");
        for(i=0;i<_normal.size();i++) {
          fprintf(fp,"%8.4f ",_normal[i]);
          if(i%3==2) fprintf(fp,"\n");
        }
        fprintf(fp,"   ]\n");
        fprintf(fp,"  }\n");
      
        if(_normalIndex.size()>0) {
          fprintf(fp,"  normalIndex [\n");
          for(i=0;i<_normalIndex.size();i++) {
            fprintf(fp,"%d ",_normalIndex[i]);
            if(_normalIndex[i]<0) fprintf(fp,"\n");
          }
          fprintf(fp,"  ]\n");
        }
      }
    
      // if(color.size()==0)
      //   NO_COLORS
      // else if(colorPerVertex==FALSE)
      //   if(colorIndex.size()>0)
      //     COLOR_PER_FACE_INDEXED;
      //     colorIndex.size()==getNumberOfFaces()
      //   else
      //     COLOR_PER_FACE;
      //     color.size()/3==getNumberOfFaces()
      // else // if(colorPerVertex==TRUE)
      //   if(colorIndex.size()>0)
      //     COLOR_PER_CORNER_INDEXED;
      //     colorIndex.size()==coordIndex.size()
      //   else
      //     COLOR_PER_VERTEX;
      //     color.size()/3==coord.size()/3

      if(_color.size()>0) {
        unsigned i;
        fprintf(fp,"  colorPerVertex %s\n",
                (_colorPerVertex==true)?"TRUE":"FALSE");

        fprintf(fp,"  color Color {\n");
        fprintf(fp,"   color [\n");
        for(i=0;i<_color.size();i++) {
          fprintf(fp,"%8.4f ",_color[i]);
          if(i%3==2) fprintf(fp,"\n");
        }
        fprintf(fp,"   ]\n");
        fprintf(fp,"  }\n");
      
        if(_colorIndex.size()>0) {
          fprintf(fp,"  colorIndex [\n");
          for(i=0;i<_colorIndex.size();i++) {
            fprintf(fp,"%d ",_colorIndex[i]);
            if(_colorIndex[i]<0) fprintf(fp,"\n");
          }
          fprintf(fp,"  ]\n");
        }
      }
    
      // if(texCoord.size()==0)
      //   NO_TEX_COORD
      // else if(texCoordIndex.size()>0)
      //   TEX_COORD_PER_CORNER_INDEXED;
      //   texCoordIndex.size()==coordIndex.size()
      // else
      //   TEX_COORD_PER_VERTEX;
      //   texCoord.size()/2==coord.size()/3

      if(_texCoord.size()>0) {
        unsigned i;
      
        fprintf(fp,"  texCoord TextureCoordinate {\n");
        fprintf(fp,"   point [\n");
        for(i=0;i<_texCoord.size();i++) {
          fprintf(fp,"%8.4f ",_texCoord[i]);
          if(i%2==1) fprintf(fp,"\n");
        }
        fprintf(fp,"   ]\n");
        fprintf(fp,"  }\n");
      
        if(_texCoordIndex.size()>0) {
          fprintf(fp,"  texCoordIndex [\n");
          for(i=0;i<_texCoordIndex.size();i++) {
            fprintf(fp,"%d ",_texCoordIndex[i]);
            if(_texCoordIndex[i]<0) fprintf(fp,"\n");
          }
          fprintf(fp,"  ]\n");
        }
      }
    
      fprintf(fp," }\n"); // IndexedFaceSet
      fprintf(fp,"}\n");  // Shape
      fclose(fp);
      success = true;
    }
  }
  return success;
}

