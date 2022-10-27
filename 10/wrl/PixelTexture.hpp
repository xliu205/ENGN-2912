

#ifndef _PIXEL_TEXTURE_HPP_
#define _PIXEL_TEXTURE_HPP_


#include "Node.hpp"

class PixelTexture : public Node {

private:

  // we will not implement this field yet
  // Image _image;

  bool _repeatS;
  bool _repeatT;

public:
  
  PixelTexture();
  virtual ~PixelTexture();

  bool getRepeatS();
  bool getRepeatT();

  void setRepeatS(bool value);
  void setRepeatT(bool value);

  virtual bool isPixelTexture() const { return true; }
  virtual string getType() const { return "PixelTexture"; }

  typedef bool (*Property)(PixelTexture &pixelTexture);
  typedef void (*OPerator)(PixelTexture &pixelTexture);
};

#endif // _PIXEL_TEXTURE_HPP_
