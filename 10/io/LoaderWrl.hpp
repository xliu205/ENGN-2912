

#ifndef _LOADER_WRL_HPP_
#define _LOADER_WRL_HPP_

#include "Loader.hpp"
#include "Tokenizer.hpp"
#include <wrl/Transform.hpp>
#include <wrl/Shape.hpp>
#include <wrl/Appearance.hpp>
#include <wrl/Material.hpp>
#include <wrl/ImageTexture.hpp>
#include <wrl/IndexedFaceSet.hpp>
#include <sstream>

class LoaderWrl : public Loader
{

private:
  const static char *_ext;

public:
  LoaderWrl(){};
  ~LoaderWrl(){};

  bool load(const char *filename, SceneGraph &wrl);
  const char *ext() const { return _ext; }

private:


  bool loadSceneGraph(Tokenizer &tkn, SceneGraph &wrl);
  bool loadGroup(Tokenizer &tkn, Group &group);
  bool loadTransform(Tokenizer &tkn, Transform &transform);
  bool loadChildren(Tokenizer &tkn, Group &group);
  bool loadShape(Tokenizer &tkn, Shape &transform);
  bool loadAppearance(Tokenizer &tkn, Appearance &appearance);
  bool loadMaterial(Tokenizer &tkn, Material &material);
  bool loadImageTexture(Tokenizer &tkn, ImageTexture &imageTexture);
  bool loadIndexedFaceSet(Tokenizer &tkn, IndexedFaceSet &ifs);

};

#endif // _LOADER_WRL_HPP_