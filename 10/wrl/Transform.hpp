

#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include "Group.hpp"
#include "Rotation.hpp"

using namespace std;

class Transform : public Group
{

private:
  Vec3f _center;              // 0 0 0
  Rotation _rotation;         // 0 0 1 0
  Vec3f _scale;               // 1 1 1
  Rotation _scaleOrientation; // 0 0 1 0
  Vec3f _translation;         // 0 0 0


public:
  Transform();
  virtual ~Transform();

  Vec3f &getCenter();
  Rotation &getRotation();
  Vec3f &getScale();
  Rotation &getScaleOrientation();
  Vec3f &getTranslation();

  void setCenter(Vec3f &value);
  void setRotation(Rotation &value);
  void setRotation(Vec4f &value);
  void setScale(Vec3f &value);
  void setScaleOrientation(Rotation &value);
  void setScaleOrientation(Vec4f &value);
  void setTranslation(Vec3f &value);

  void getMatrix(Eigen::Matrix4f T /*[16]*/);

  virtual bool isTransform() const { return true; }
  virtual string getType() const { return "Transform"; }

  typedef bool (*Property)(Transform &transform);
  typedef void (*Operator)(Transform &transform);

private:
  static void _makeRotation(Rotation &r, float *R /*[9]*/);
};

#endif // _TRANSFORM_HPP_
