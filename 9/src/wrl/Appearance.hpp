

#ifndef _APPEARANCE_HPP_
#define _APPEARANCE_HPP_
#include "Node.hpp"

class Appearance : public Node {

private:

  Node* _material;
  Node* _texture;



public:

  Appearance();
  virtual ~Appearance();

  Node* getMaterial();
  Node* getTexture();

  // we will not implement this field
  //
  // Node* getTextureTransform();

  void setMaterial(Node* material);
  void setTexture(Node* texture);

  // void setTextureTransform(Node* textureTransform);

  virtual bool isAppearance() const { return true; }
  virtual string getType() const { return "Appearance"; };

  typedef bool (*Property)(Appearance &appearance);
  typedef void (*Operator)(Appearance &appearance);
};

#endif // _APPEARANCE_HPP_
