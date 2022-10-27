//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 23:02:49 taubin>
//------------------------------------------------------------------------
//
// Tokenizer.hpp
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

#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
/*
class Tokenizer : public string {

protected:

  FILE* _fp;

public:

  Tokenizer(FILE* fp):_fp(fp) { }

  int  operator==(const char* str)
  { return !strcmp(&(*(begin())),str); }

  int  length()
  { return (int)((size()>0)?strlen(&(*(begin()))):0); }

  bool get(int skipComments=1);

  static void setDebug(bool value)
  { _debug = value; }
  bool expecting(const string &str);

private:

  static bool _debug;

};

#endif // _TOKENIZER_HPP_
*/

class Tokenizer : public string
{
protected:
  ifstream &_ifstr;

public:
  Tokenizer(ifstream &ifstr) : _ifstr(ifstr) {}
  bool get();
  bool expecting(const string &str);
  unsigned getLineNum();
};

#endif /* Tokenizer_h */
