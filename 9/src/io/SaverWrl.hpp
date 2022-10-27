
#ifndef _WRL_SAVER_HPP_
#define _WRL_SAVER_HPP_

#include "Saver.hpp"
#include <wrl/Shape.hpp>
#include <wrl/Appearance.hpp>
#include <wrl/Material.hpp>
#include <wrl/IndexedFaceSet.hpp>
#include <wrl/ImageTexture.hpp>
#include <wrl/Transform.hpp>
#include <wrl/SceneGraphTraversal.hpp>

class SaverWrl : public Saver {

private:

const static char* _ext;

public:

  SaverWrl()  {};
  ~SaverWrl() {};

  virtual bool  save(const char* filename, SceneGraph& wrl) const;
  virtual const char* ext() const { return _ext; }
  
private:
  
  void saveAppearance
  (FILE* fp, string indent, Appearance* appearance) const;
  void saveGroup
  (FILE* fp, string indent, Group* group) const;
  void saveImageTexture
  (FILE* fp, string indent, ImageTexture* imageTexture) const;
  void saveIndexedFaceSet
  (FILE* fp, string indent, IndexedFaceSet* indexedFaceSet) const;
  void saveMaterial
  (FILE* fp, string indent, Material* material) const;
  void saveShape
  (FILE* fp, string indent, Shape* shape) const;
  void saveTransform
  (FILE* fp, string indent, Transform* transform) const;
  
};

#endif // _WRL_SAVER_HPP_
