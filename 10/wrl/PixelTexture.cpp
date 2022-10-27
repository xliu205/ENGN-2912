
#include <iostream>
#include "PixelTexture.hpp"


  
PixelTexture::PixelTexture() :

  //   
  _repeatS(true),
  _repeatT(true)
  {
  }

PixelTexture::~PixelTexture() {

}

bool PixelTexture::getRepeatS()
{
  //  
  return _repeatS;
}
bool PixelTexture::getRepeatT()
{
  return _repeatT;
}

void PixelTexture::setRepeatS(bool value)
{
  _repeatS = value;
}

void PixelTexture::setRepeatT(bool value)
{
  _repeatT = value;
}
