

#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <wrl/Node.hpp>
#include "StrException.hpp"

using namespace std;

class Tokenizer : public string
{

protected:
  FILE *_fp;
  bool _skip; // if(_skip) skip comments

public:
  Tokenizer( FILE *fp);

  void setSkipComments(const bool value);
  bool get();
  void get(const string &errMsg) /* throw(StrException*) */;
  bool getBool(bool &b);
  bool getFloat(float &f);
  bool getColor(Color &c);
  bool getVec3f(Vec3f &v);
  bool getVec4f(Vec4f &v);
  bool getVec2f(Vec2f &v);
  bool equals(const char *str);
  bool expecting(const string &str);
  bool expecting(const char *str);
};

#endif // _TOKENIZER_HPP_
