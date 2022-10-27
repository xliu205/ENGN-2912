
#include <iostream>
#include "Appearance.hpp"


Appearance::Appearance() : 
_material((Node *)0),
_texture((Node *)0) {
}

Appearance::~Appearance()
{
}

Node *Appearance::getMaterial()
{


  return _material;
}

Node *Appearance::getTexture()
{

  

  return _texture;
}



void Appearance::setMaterial(Node *material)
{

  //  
  _material = material;
}

void Appearance::setTexture(Node *texture)
{
  texture->setParent(this);
  _texture = texture;
}
