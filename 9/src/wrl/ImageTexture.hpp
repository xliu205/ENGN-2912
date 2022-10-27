
#ifndef _IMAGE_TEXTURE_HPP_
#define _IMAGE_TEXTURE_HPP_


#include <vector>
#include "PixelTexture.hpp"

class ImageTexture : public PixelTexture {

private:

  vector<string> _url;

public:
  
  ImageTexture();
  virtual ~ImageTexture();

  vector<string>& getUrl();
  string getUrl(int i);
  void adToUrl(const string& str);

  virtual bool isImageTexture() const { return true; }
  virtual string getType() const { return "ImageTexture"; }

  typedef bool (*Property)(ImageTexture &imageTexture);
  typedef void (*Operator)(ImageTexture &imageTexture);
};

#endif // _IMAGE_TEXTURE_HPP_
