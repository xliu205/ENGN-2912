

#include "LoaderFactory.hpp"

bool LoaderFactory::load(const char* filename, SceneGraph& wrl) {
  bool success = false;
  if(filename) {
    // int n = (int)strlen(filename);
    string f(filename);
    int n = static_cast<int>(f.size());
    int i;
    for(i=n-1;i>=0;i--)
      if(filename[i]=='.')
        break;
    if(i>=0) {
      string ext(filename+i+1);
      Loader* loader = _registry[ext];
      if(loader)
        success = loader->load(filename,wrl);
    }
  }
  return success;
}

void LoaderFactory::add(Loader* loader) {
  if(loader) {
    string ext(loader->ext()); // constructed from const char*
    pair<string,Loader*> ext_loader(ext,loader);
    _registry.insert(ext_loader);
  }
}
