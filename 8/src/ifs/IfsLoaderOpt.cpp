//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 22:59:34 taubin>
//------------------------------------------------------------------------
//
// IfsOptLoader.cpp
//
// Software developed for the Fall 2015-2021 Brown University course
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

#include <cstdio>
#include <util/Tokenizer.hpp>
#include "IfsLoaderOpt.hpp"

using namespace std;

const char* IfsLoaderOpt::_ext = "opt";

bool IfsLoaderOpt::load(const char* filename, Ifs& ifs) {
  
  bool success = false;
  FILE* fp = nullptr;

  try {
    
    if(filename==(char*)0) throw 1;

    fp = fopen(filename,"r");
    if(fp==(FILE*)0) throw 2;
      
    ifs.clear();
    // bool&          ccw             = ifs.getCcw();
    // bool&          convex          = ifs.getConvex();
    // float&         creaseAngle     = ifs.getCreaseangle();
    // bool&          solid           = ifs.getSolid();
    bool&             normalPerVertex = ifs.getNormalPerVertex();
    // bool&          colorPerVertex  = ifs.getColorPerVertex();
    vector<float>&    coord           = ifs.getCoord();
    // vector<int>&   coordIndex      = ifs.getCoordIndex();
    vector<float>&    normal          = ifs.getNormal();
    // vector<int>&   normalIndex     = ifs.getNormalIndex();
    // vector<float>& color           = ifs.getColor();
    // vector<int>&   colorIndex      = ifs.getColorIndex();
    // vector<float>& texCoord        = ifs.getTexCoord();
    // vector<int>&   texCoordIndex   = ifs.getTexCoordIndex();
    normalPerVertex = true;
    
    // first line should be OPT_HEADER
    const char* OPT_HEADER = "#OPT V1.0";
    const int N = strlen(OPT_HEADER)+1; // computed at compile time
    char header[N]; // c-string
    memset(header,'\0',N); // initialize to 0x0 ... 0x0
    char fmt[8];
    sprintf(fmt,"%%%1dc",N-1);
    fscanf(fp,fmt,header); 
    if(strcmp(header,OPT_HEADER)) throw 3;
    std::ifstream ifs(filename, std::ifstream::in);
    Tokenizer tkn(ifs);

    // then file may contain an arbitrary number of additional lines
    // each line should contain 6 floating point numbers
    // the x,y,z coordinates of a point, followed by
    // the nx,ny,nz coodinates of the corresponding normal vector
    
    // get 6 tokens until the end of the file
    // quit if you cannot read or parse any one of the 6 coordinates
    while(true) {
      double x,y,z,nx,ny,nz;

      if(tkn.get()==false)
          break; // end of file
        
      if(sscanf(tkn.c_str(),"%lf", &x)==0 ||
         tkn.get()==false || sscanf(tkn.c_str(),"%lf", &y)==0 ||
         tkn.get()==false || sscanf(tkn.c_str(),"%lf", &z)==0 ||
         tkn.get()==false || sscanf(tkn.c_str(),"%lf",&nx)==0 ||
         tkn.get()==false || sscanf(tkn.c_str(),"%lf",&ny)==0 ||
         tkn.get()==false || sscanf(tkn.c_str(),"%lf",&nz)==0) throw 4;
      
      coord.push_back(x);
      coord.push_back(y);
      coord.push_back(z);
      normal.push_back(nx);
      normal.push_back(ny);
      normal.push_back(nz);
    }
  
    fclose(fp);
    success = true;

  } catch(int e) { // ERROR
    if(fp!=nullptr) fclose(fp);
    ifs.clear();
  }

  return success;
}

