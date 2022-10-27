

#ifndef _LOADER_HPP_
#define _LOADER_HPP_

#include <wrl/SceneGraph.hpp>

class Loader {

public:

  virtual bool  load(const char* filename, SceneGraph& wrl) = 0;
  virtual const char* ext() const = 0;

};

#endif // _LOADER_HPP_
