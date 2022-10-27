

#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_


#include "Node.hpp"

class Material : public Node {

private:

  float _ambientIntensity;
  Color _diffuseColor;
  Color _emissiveColor;
  float _shininess;
  Color _specularColor;
  float _transparency;

public:

  Material();
  virtual ~Material();

  float  getAmbientIntensity();
  Color& getDiffuseColor();
  Color& getEmissiveColor();
  float  getShininess();
  Color& getSpecularColor();
  float  getTransparency();

  void   setAmbientIntensity(float value);
  void   setDiffuseColor(Color& value);
  void   setEmissiveColor(Color& value);
  void   setShininess(float value);
  void   setSpecularColor(Color& value);
  void   setTransparency(float value);

  virtual bool isMaterial() const { return true; }
  virtual string getType() const { return "Material"; }

  typedef bool (*Property)(Material &material);
  typedef void (*Operator)(Material &material);
};

#endif // _MATERIAL_HPP_
