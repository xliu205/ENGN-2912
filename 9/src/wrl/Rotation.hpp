

#ifndef _ROTATION_HPP_
#define _ROTATION_HPP_

#include <vector>
#include "Node.hpp"

class Rotation {

private:

  Vec3f _axis;  // unit length vector
  float _angle; // angle in radians

  public:

  Rotation();
  Rotation(float x, float y, float z, float angle);
  Rotation(Vec3f& axis, float angle);

 
  void   set(float x, float y, float z, float angle);
  void   set(Vec4f& value);
  void   operator=(Vec4f& value);
  Vec3f& getAxis();
  float  getAngle();

};

#endif // _ROTATION_HPP_
