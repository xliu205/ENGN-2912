

#include <math.h>
#include <iostream>
#include "Transform.hpp"
  
Transform::Transform():
  _center(0.0f,0.0f,0.0f),
  _rotation(0.0f,0.0f,1.0f,0.0f),
  _scale(1.0f,1.0f,1.0f),
  _scaleOrientation(0.0f,0.0f,1.0f,0.0f),
  _translation(0.0f,0.0f,0.0f) {
}

Transform::~Transform() {

}

Vec3f&    Transform::getCenter()                     {  return           _center; }
Rotation& Transform::getRotation()                   {  return         _rotation; }
Vec3f&    Transform::getScale()                      {  return            _scale; }
Rotation& Transform::getScaleOrientation()           {  return _scaleOrientation; }
Vec3f&    Transform::getTranslation()                {  return      _translation; }

void Transform::setCenter(Vec3f& value)              {           _center = value; }
void Transform::setRotation(Rotation& value)         {         _rotation = value; }
void Transform::setScale(Vec3f& value)               {            _scale = value; }
void Transform::setScaleOrientation(Rotation& value) { _scaleOrientation = value; }
void Transform::setTranslation(Vec3f& value)         {      _translation = value; }

void Transform::setRotation(Vec4f& value) {
  _rotation = value;
}

void Transform::setScaleOrientation(Vec4f& value) {
  _scaleOrientation = value;
}

void Transform::getMatrix(float* M /*[16]*/) {
  M[ 0] = 1.0f; M[ 1] = 0.0f; M[ 2] = 0.0f; M[ 3] = 0.0f;
  M[ 4] = 0.0f; M[ 5] = 1.0f; M[ 6] = 0.0f; M[ 7] = 0.0f;
  M[ 8] = 0.0f; M[ 9] = 0.0f; M[10] = 1.0f; M[11] = 0.0f;
  M[12] = 0.0f; M[13] = 0.0f; M[14] = 0.0f; M[15] = 1.0f;

  float O[9];
  _rodrigues(_scaleOrientation,O);
  float s[3] = { _scale.x, _scale.y, _scale.z };
  float OSOt[9];
  int i,ii,j,jj,k,kk;
  for(ii=i=0;i<3;ii+=3,i++)
    for(jj=j=0;j<3;jj+=3,j++)
      for(OSOt[ii+j]=0.0f,k=0;k<3;k++)
        OSOt[ii+j] += O[ii+k]*s[k]*O[jj+k];
  // A = R*O*S*O^t
  float R[9];
  _rodrigues(_rotation,R);
  float A[9];
  for(ii=i=0;i<3;ii+=3,i++)
    for(jj=j=0;j<3;jj+=3,j++)
      for(A[ii+j]=0.0f,kk=k=0;k<3;kk+=3,k++)
        A[ii+j] += R[ii+k]*OSOt[kk+j];
  // B = (I-A)*C+T
  float c[3] = { _center.x, _center.y, _center.z};
  float t[3] = { _translation.x, _translation.y, _translation.z };
  float B[3];
  for(ii=i=0;i<3;ii+=3,i++)
    for(B[i]=c[i]+t[i],j=0;j<3;j++)
      B[i] -= A[ii+j]*c[j];
  //     | A B |
  // M = |     |
  //     | 0 1 |
  M[ 0] = A[0]; M[ 1] = A[1]; M[ 2] = A[2]; M[ 3] = B[0]; 
  M[ 4] = A[3]; M[ 5] = A[4]; M[ 6] = A[5]; M[ 7] = B[1]; 
  M[ 8] = A[6]; M[ 9] = A[7]; M[10] = A[8]; M[11] = B[2]; 
  M[12] = 0.0f; M[13] = 0.0f; M[14] = 0.0f; M[15] = 1.0f; 
}

void Transform::_rodrigues(Rotation& r, float* R /*[9]*/) {



  Vec3f& axis  = r.getAxis();
  float  angle = r.getAngle();
  if(angle==0.0f) {
    R[0] = 1.0f; R[1] = 0.0f; R[2] = 0.0f;
    R[3] = 0.0f; R[4] = 1.0f; R[5] = 0.0f;
    R[6] = 0.0f; R[7] = 0.0f; R[8] = 1.0f; 
  } else {
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    float t = 1.0f-c;
    R[0] =    c+t*x*x; R[1] = -s*z+t*x*y; R[2] =  s*y+t*x*z;
    R[3] =  s*z+t*x*y; R[4] =    c+t*y*y; R[5] = -s*x+t*y*z;
    R[6] = -s*y+t*x*z; R[7] =  s*x+t*y*z; R[8] =    c+t*z*z; 
  }
}

