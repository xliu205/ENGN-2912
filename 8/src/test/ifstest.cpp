//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2021-11-07 23:01:46 taubin>
//------------------------------------------------------------------------
//
// ifstest.cpp
//
// Software developed for the Fall 2021 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2021, Gabriel Taubin
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

#include <string>
#include <iostream>

using namespace std;

#include <ifs/Ifs.hpp>
#include <ifs/IfsLoaderFactory.hpp>
#include <ifs/IfsSaverFactory.hpp>
#include <ifs/IfsLoaderOpt.hpp>
#include <ifs/IfsLoaderWrl.hpp>
#include <ifs/IfsSaverWrl.hpp>

class Data {
public:
  bool   _debug;
  bool   _removeNormal;
  bool   _removeColor;
  bool   _removeTexCoord;
  string _inFile;
  string _outFile;
public:
  Data():
    _debug(false),
    _removeNormal(false),
    _removeColor(false),
    _removeTexCoord(false),
    _inFile(""),
    _outFile("")
  { }
};

bool  hasNormal(Ifs& ifs)   { return ifs.getNormalBinding()  !=Ifs::PB_NONE; }
bool  hasColor(Ifs& ifs)    { return ifs.getColorBinding()   !=Ifs::PB_NONE; }
bool  hasTexCoord(Ifs& ifs) { return ifs.getTexCoordBinding()!=Ifs::PB_NONE; }

const char* tv(bool value)        { return (value)?"true":"false";                 }

void options(Data& D) {
  cerr << "   -d|-debug               [" << tv(D._debug)          << "]" << endl;
  cerr << "  -rn|-removeNormal        [" << tv(D._removeNormal)   << "]" << endl;
  cerr << "  -rc|-removeColor         [" << tv(D._removeColor)    << "]" << endl;
  cerr << "  -rt|-removeTexCoord      [" << tv(D._removeTexCoord) << "]" << endl;
}

void usage(Data& D) {
  cerr << "USAGE: ifstest [options] inFile outFile" << endl;
  cerr << "   -h|-help" << endl;
  options(D);
  cerr << endl;
  exit(0);
}

void error(const char *msg) {
  cerr << "ERROR: ifstest | " << ((msg)?msg:"") << endl;
  exit(0);
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {

  Data D;

  if(argc==1) usage(D);

  for(int i=1;i<argc;i++) {
    if(string(argv[i])=="-h" || string(argv[i])=="-help") {
      usage(D);
    } else if(string(argv[i])=="-d" || string(argv[i])=="-debug") {
      D._debug = !D._debug;
    } else if(string(argv[i])=="-rn" || string(argv[i])=="-removeNormal") {
      D._removeNormal = !D._removeNormal;
    } else if(string(argv[i])=="-rc" || string(argv[i])=="-removeColor") {
      D._removeColor = !D._removeColor;
    } else if(string(argv[i])=="-rt" || string(argv[i])=="-removeTexCoord") {
      D._removeTexCoord = !D._removeTexCoord;
    } else if(string(argv[i])[0]=='-') {
      error("unknown option");
    } else if(D._inFile=="") {
      D._inFile = string(argv[i]);
    } else if(D._outFile=="") {
      D._outFile = string(argv[i]);
    }
  }

  if(D._inFile =="") error("no inFile");
  if(D._outFile=="") error("no outFile");

  if(D._debug) {
    cerr << "ifstest {" << endl;
    cerr << endl;
    options(D);
    cerr << endl;
    fflush(stderr);
  }

  bool success;

  //////////////////////////////////////////////////////////////////////
  // create loader and saver and register IfsLoaders and IfsSavers
  IfsLoaderFactory loaderFactory;

  IfsLoaderOpt* optLoader = new IfsLoaderOpt();
  loaderFactory.add(optLoader);

  IfsLoaderWrl* wrlLoader = new IfsLoaderWrl();
  loaderFactory.add(wrlLoader);

  IfsSaverFactory saverFactory;
  IfsSaverWrl* wrlSaver = new IfsSaverWrl();
  saverFactory.add(wrlSaver);

  //////////////////////////////////////////////////////////////////////
  // read Ifs

  Ifs ifs;

  if(D._debug) {
    cerr << "  loading Ifs {" << endl;
    cerr << "    fileName       = \"" << D._inFile << "\"" << endl;
  }

  success = loaderFactory.load(D._inFile.c_str(),ifs);
  int nCoordRead = ifs.getNumberOfCoord();

  if(D._debug) {
    cerr << "    success        = " << tv(success)          << endl;
    cerr << "    nCoordRead     = " << nCoordRead           << endl;
    cerr << "    hasNormal      = " << tv(hasNormal(ifs))   << endl;
    cerr << "    hasColor       = " << tv(hasColor(ifs))    << endl;
    cerr << "    hasTexCoord    = " << tv(hasTexCoord(ifs)) << endl;
    cerr << "  }" << endl;
    cerr << endl;
  }

  if(success==false) return -1;

  //////////////////////////////////////////////////////////////////////
  // process

  if(D._removeNormal && hasNormal(ifs)) {
    if(D._debug) { cerr << "  removing normals" << endl; }
    ifs.getNormal().clear();
    ifs.getNormalIndex().clear();
  }
  if(D._removeColor && hasColor(ifs)) {
    if(D._debug) { cerr << "  removing colors" << endl; }
    ifs.getColor().clear();
    ifs.getColorIndex().clear();
  }
  if(D._removeTexCoord && hasTexCoord(ifs)) {
    if(D._debug) { cerr << "  removing texture coordinates" << endl; }
    ifs.getTexCoord().clear();
    ifs.getTexCoordIndex().clear();
  }

  if(D._removeNormal   ||
     D._removeColor    ||
     D._removeTexCoord ) {
    if(D._debug) cerr << "" << endl;
  }

  //////////////////////////////////////////////////////////////////////
  // write
  
  if(D._debug) {
    cerr << "  saving Ifs {" << endl;
    cerr << "    fileName       = \"" << D._outFile << "\"" << endl;
  }

  success = saverFactory.save(D._outFile.c_str(),ifs);
  int nCoordWritten = ifs.getNumberOfCoord();

  if(D._debug) {
    cerr << "    success        = " << tv(success)          << endl;
    cerr << "    nCoordWritten  = " << nCoordWritten        << endl;
    cerr << "    hasNormal      = " << tv(hasNormal(ifs))   << endl;
    cerr << "    hasColor       = " << tv(hasColor(ifs))    << endl;
    cerr << "    hasTexCoord    = " << tv(hasTexCoord(ifs)) << endl;
    cerr << "  }" << endl;
    cerr << endl;
  }

  //////////////////////////////////////////////////////////////////////

  if(D._debug) {
    cerr << "}" << endl;
    fflush(stderr);
  }

  return 0;
}
