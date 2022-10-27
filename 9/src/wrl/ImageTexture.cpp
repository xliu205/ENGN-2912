

#include "ImageTexture.hpp"
  
ImageTexture::ImageTexture() {

  //   
  _url = vector<string>();
}

ImageTexture::~ImageTexture() {

  //   
  _url.clear();
}

vector<string>& ImageTexture::getUrl() {
  return _url;
}

string ImageTexture::getUrl(int i) {

  //   

  return _url[i];
}

void ImageTexture::adToUrl(const string& str) {

  //   
  _url.push_back(str);
}

