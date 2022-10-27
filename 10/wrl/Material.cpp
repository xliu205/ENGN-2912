
#include "Material.hpp"

Material::Material():
//   
  _ambientIntensity(0.2f),
  _diffuseColor(0.8f, 0.8f, 0.8f),
  _emissiveColor(0.0f, 0.0f, 0.0f),
  _shininess(0.2f),
  _specularColor(0.0f, 0.0f, 0.0f),
  _transparency(0.0f){
  }

Material::~Material() {
  //   
}

float Material::getAmbientIntensity()
{
  return _ambientIntensity;
}
Color &Material::getDiffuseColor()
{
  return _diffuseColor;
}
Color &Material::getEmissiveColor()
{
  return _emissiveColor;
}
float Material::getShininess()
{
  return _shininess;
}
Color &Material::getSpecularColor()
{
  return _specularColor;
}
float Material::getTransparency()
{
  return _transparency;
}

void Material::setAmbientIntensity(float value)
{
  _ambientIntensity = value;
}
void Material::setDiffuseColor(Color &value)
{
  _diffuseColor = value;
}
void Material::setEmissiveColor(Color &value)
{
  _emissiveColor = value;
}
void Material::setShininess(float value)
{
  _shininess = value;
}
void Material::setSpecularColor(Color &value)
{
  _specularColor = value;
}
void Material::setTransparency(float value)
{
  _transparency = value;
}