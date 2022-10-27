

#ifndef _LOADER_FACTORY_HPP_
#define _LOADER_FACTORY_HPP_

#include <map>
#include <string>
#include "Loader.hpp"

using namespace std;

class LoaderFactory {

public:

  LoaderFactory() {}
  ~LoaderFactory() {}

  bool load(const char* filename, SceneGraph& wrl);
  void add(Loader* loader);

private:

  map<string, Loader*> _registry;

};

#endif // _LOADER_FACTORY_HPP_
