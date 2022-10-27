

#ifndef _SAVER_FACTORY_HPP_
#define _SAVER_FACTORY_HPP_

#include <map>
#include <string>
#include "Saver.hpp"

using namespace std;

class SaverFactory {

public:

  SaverFactory() {}
  ~SaverFactory() {}

  bool save(const char* filename, SceneGraph& wrl);
  void add(Saver* saver);

private:

  map<string, Saver*> _registry;

};

#endif // _SAVER_FACTORY_HPP_
