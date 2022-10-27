//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:00:24 taubin>
//------------------------------------------------------------------------
//
// IfsSaverFactory.cpp
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

#include "IfsSaverFactory.hpp"

bool IfsSaverFactory::save(const char* filename, Ifs& ifs) {
  bool success = false;
  // TODO
    if(filename!=(const char*)0) {
      // extract ext from filename
      int n = (int)strlen(filename);
      int i;
      for(i=n-1;i>=0;i--)
        if(filename[i]=='.')
          break;
      if(i>=0) {
        string ext(filename+i+1);
        // look for a proper loader
        IfsSaver* saver = _registry[ext];
        // if loader found load filename
        if(saver!=(IfsSaver*)0)
          success = saver->save(filename,ifs);
      }
    }
    return success;
}

bool IfsSaverFactory::add(IfsSaver* saver) {
  // TODO
    if(saver == nullptr) return false;
    string ext(saver -> ext());
    if(_registry[ext] != nullptr) return false;
    pair<string, IfsSaver*> ext_saver(ext, saver);
    _registry.insert(ext_saver);
    return true; 
}
