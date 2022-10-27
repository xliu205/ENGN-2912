

#ifndef _SAVER_HPP_
#define _SAVER_HPP_

#include <wrl/SceneGraph.hpp>

class Saver {

public:

  virtual bool  save(const char* filename, SceneGraph& wrl) const = 0;
  virtual const char* ext() const = 0;

};

#endif // _SAVER_HPP_
