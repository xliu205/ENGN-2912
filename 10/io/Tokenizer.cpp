

#include "Tokenizer.hpp"

Tokenizer::Tokenizer(FILE* fp):_fp(fp),_skip(true) {
}

void Tokenizer::setSkipComments(const bool value) {
  _skip = value;
}

bool Tokenizer::get() {
  char c='\0';
  do {
    clear();
    // skip blank space
    while((c=static_cast<char>(0xff&getc(_fp)))!=EOF)
      if(!(c==' ' || c=='\t' || c=='\n' || c==',' || c=='\015'))
        { push_back(c); break; }
    // collect token characters
    while(((c=static_cast<char>(0xff&getc(_fp)))!=EOF) &&
          !(c==' ' || c=='\t' || c=='\n' || c==',' || c=='\015')) // c=="^M"
      push_back(c);
    // if comment, get the rest of the line, including blank spaces
    if(size()>0 && (*this)[0]=='#') {
      // last blank space character read is not in tkn yet
      if(c!='\n') {
        push_back(c);
        while((c=static_cast<char>(0xff&getc(_fp)))!=EOF && c!='\n')
          push_back(c);
      }
    }
    // terminate the string
    // push_back('\0');
  } while(_skip && length()>0 && *(begin())=='#');
  
  return (length()>0)?true:false;
}

void Tokenizer::get(const string& errMsg) /*throw(StrException *)*/ {
  if(get()==false) throw new StrException(errMsg);
}

bool Tokenizer::getBool(bool& b) {
  bool success = false;
  if(get()) {
    if(this->equals("t") || this->equals("true") ||
       this->equals("T") || this->equals("TRUE")) {
      b = true;
      success = true;
       } else if(this->equals("f") || this->equals("false") ||
                 this->equals("F") || this->equals("FALSE")) {
      b = false;
      success = true;
    }
  }
  return success;
}

bool Tokenizer::getFloat(float& f) {
  bool success =
    (get() && (sscanf(&((*this)[0]),"%f",&f)==1));
  return success;
}

bool Tokenizer::getColor(Color& c) {
  bool success =
    (get() && (sscanf(&((*this)[0]),"%f",&(c.r))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(c.g))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(c.b))==1));
  return success;
}

bool Tokenizer::getVec4f(Vec4f& v) {
  bool success =
    (get() && (sscanf(&((*this)[0]),"%f",&(v.x))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.y))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.z))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.w))==1));
  return success;
}

bool Tokenizer::getVec3f(Vec3f& v) {
  bool success =
    (get() && (sscanf(&((*this)[0]),"%f",&(v.x))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.y))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.z))==1));
  return success;
}

bool Tokenizer::getVec2f(Vec2f& v) {
  bool success =
    (get() && (sscanf(&((*this)[0]),"%f",&(v.x))==1)) &&
    (get() && (sscanf(&((*this)[0]),"%f",&(v.y))==1));
  return success;
}

bool Tokenizer::equals(const char* str) {
  // return (strcmp(this->c_str(),str)==0);
    return ((*this)==str);
}

bool Tokenizer::expecting(const string& str) {
  return get() && this->equals(str.c_str());
}

bool Tokenizer::expecting(const char* str) {
  return get() && this->equals(str);
}
