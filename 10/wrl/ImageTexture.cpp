
#include <iostream>
#include "ImageTexture.hpp"
ImageTexture::ImageTexture()
{

  //  
  setRepeatS(true);
  setRepeatT(true);
}

ImageTexture::~ImageTexture()
{

  //  
  _url.clear();
}

vector<string> &ImageTexture::getUrl()
{

  //  
  return _url;
}

string ImageTexture::getUrl(int i)
{

  //  
  return _url.at(i);
}

void ImageTexture::adToUrl(const string &str)
{

  //  
  _url.push_back(str);
}