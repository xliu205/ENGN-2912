

#include "Rotation.hpp"

Rotation::Rotation():
  _axis(0.0f,0.0f,1.0f),
  _angle(0.0f) {
}
 
Rotation::Rotation(float x, float y, float z, float angle):
  _axis(x,y,z),
  _angle(angle) {
}

Rotation::Rotation(Vec3f& axis, float angle):
  _axis(axis),
  _angle(angle) {
}

void Rotation::set(float x, float y, float z, float angle) {
  _axis.x = x; _axis.y = y; _axis.z = z;
  _angle  = angle;
}
void Rotation::set(Vec4f& value) {
  _axis.x = value.x; _axis.y = value.y; _axis.z = value.z;
  _angle  = value.w;
}
void Rotation::operator=(Vec4f& value) {
  _axis.x = value.x; _axis.y = value.y; _axis.z = value.z;
  _angle  = value.w;
}

Vec3f& Rotation::getAxis() {
  return _axis;
}
float  Rotation::getAngle() {
  return _angle;
}
