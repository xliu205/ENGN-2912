//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2021-11-14 21:38:13 taubin>
//------------------------------------------------------------------------

#include <string>
#include <iostream>

using namespace std;

#include "wrl/SceneGraph.hpp"
#include "io/LoaderWrl.hpp"
#include "io/SaverWrl.hpp"
#include "io/LoaderFactory.hpp"
#include "io/SaverFactory.hpp"

class Data {
public:
  bool   _debug;
  string _inFile;
  string _outFile;
public:
  Data():
    _debug(false),
    _inFile(""),
    _outFile("")
  { }
};

const char* tv(bool value)        { return (value)?"true":"false";                 }

void options(Data& D) {
  cerr << "   -d|-debug               [" << tv(D._debug)          << "]" << endl;
}

void usage(Data& D) {
  cerr << "USAGE: wrltest [options] inFile outFile" << endl;
  cerr << "   -h|-help" << endl;
  options(D);
  cerr << endl;
  exit(0);
}

void error(const char *msg) {
  cerr << "ERROR: wrltest | " << ((msg)?msg:"") << endl;
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
    cerr << "wrltest {" << endl;
    cerr << endl;
    options(D);
    cerr << endl;
    fflush(stderr);
  }

  bool success;

  //////////////////////////////////////////////////////////////////////
  // create loader and saver and register IfsLoaders and IfsSavers
  LoaderFactory loaderFactory;
  LoaderWrl* wrlLoader = new LoaderWrl();
  loaderFactory.add(wrlLoader);

  SaverFactory saverFactory;
  SaverWrl* wrlSaver = new SaverWrl();
  saverFactory.add(wrlSaver);

  //////////////////////////////////////////////////////////////////////
  // read VRML file

  SceneGraph wrl;

  if(D._debug) {
    cerr << "  loading Wrl {" << endl;
    cerr << "    fileName       = \"" << D._inFile << "\"" << endl;
  }

  success = loaderFactory.load(D._inFile.c_str(),wrl);

  if(D._debug) {
    cerr << "    success        = " << tv(success)          << endl;
    cerr << "  }" << endl;
    cerr << endl;
  }

  if(success==false) return -1;

  //////////////////////////////////////////////////////////////////////
  // process

  // ...

  //////////////////////////////////////////////////////////////////////
  // write VRML file
  
  if(D._debug) {
    cerr << "  saving Wrl {" << endl;
    cerr << "    fileName       = \"" << D._outFile << "\"" << endl;
  }

  success = saverFactory.save(D._outFile.c_str(),wrl);

  if(D._debug) {
    cerr << "    success        = " << tv(success)          << endl;
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
