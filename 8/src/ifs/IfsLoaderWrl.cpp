//------------------------------------------------------------------------
//  Time-stamp: <2021-11-07 22:59:57 taubin>
//------------------------------------------------------------------------
//
// IfsLoaderWrl.cpp
//
// Software developed for the Fall 2021 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2015-2021, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "IfsLoaderWrl.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "../util/Tokenizer.hpp"


#define VRML_HEADER "#VRML V2.0 utf8"

const char* IfsLoaderWrl::_ext = "wrl";

bool my_stof(string str, float &num)
{
  double temp;
  string::size_type idx;

  try
  {
    temp = stod(str, &idx);
  }
  catch (const std::invalid_argument &ia)
  {
    std::cerr << "Invalid argument: " << ia.what() << '\n';
    return false;
  }

  string redsidual = str.substr(idx);

  if (redsidual.length() > 0)
  {
    return false;
  }
  else
  {
    num = float(temp);
    return true;
  }
}

bool my_stoi(string str, int &num)
{
  string::size_type idx;

  try
  {
    num = stoi(str, &idx);
  }
  catch (const std::invalid_argument &ia)
  {
    cerr << "Invalid argument: " << ia.what() << endl;
    return false;
  }
  string redsidual = str.substr(idx);

  if (redsidual.length() > 0)
    return false;
  else
    return true;
}

bool checki(int num, const char *str)
{
  string num_str = to_string(num);
  if (num_str != string(str))
  {
    cout << num << " " << str << endl;
    return false;
  }
  else
    return true;
}

int IfsLoaderWrl::_load_point(Tokenizer &tokenizer, vector<float> *point_vector)
{
  if (!tokenizer.expecting("["))
    return -1;

  while (tokenizer.get() && tokenizer != "]")
  {
    float num;

    if (!my_stof(tokenizer, num))
      return -2; //number error
    else
      point_vector->push_back(num);
  }

  if (tokenizer != "]")
    return -3; //missing ] or syntax error

  if (!tokenizer.expecting("}"))
    return -4; //missing }
  return 1;
}

int IfsLoaderWrl::_load_index(Tokenizer &tokenizer, vector<int> *vector)
{

  if (!tokenizer.expecting("["))
    return -1;

  while (tokenizer.get() && tokenizer != "]")
  {
    int num;
    if (!my_stoi(tokenizer, num))
      return -2;
    else
      vector->push_back(num);
  }

  if (tokenizer != "]")
    return -3;

  return 1;
}

int IfsLoaderWrl::_load_bool(Tokenizer &tokenizer, bool *boolVar)
{

  tokenizer.get();
  if (tokenizer == "FALSE")
  {
    *boolVar = false;
  }
  else if (tokenizer == "TRUE")
  {
    *boolVar = true;
  }
  else
  {
    return -5; //syntax error with bool variable;
  }

  return 1;
}

bool IfsLoaderWrl::load(const char* filename, Ifs& ifs) {
  bool success = false;
  //ifstream ifstr;

  if(filename!=(char*)0) {
    // TODO
    //ifstream ifstr(filename);
    //FILE* ifstr = fopen(filename, "r");
    ifs.clear();
    bool& _ccw = ifs.getCcw();
    bool& _convex = ifs.getConvex();
    float& _creaseAngle = ifs.getCreaseangle();
    bool& _solid = ifs.getSolid();
    bool& _normalPerVertex = ifs.getNormalPerVertex();
    bool& _colorPerVertex = ifs.getColorPerVertex();
    vector<float>& _coord = ifs.getCoord();
    vector<int>& _coordIndex = ifs.getCoordIndex();

    vector<float>& _normal = ifs.getNormal();
    vector<int>& _normalIndex = ifs.getNormalIndex();
    vector<float>& _color = ifs.getColor();
    vector<int>& _colorIndex = ifs.getColorIndex();
    vector<float>& _texCoord = ifs.getTexCoord();
    vector<int> &_texCoordIndex = ifs.getCoordIndex();

    ifstream ifstr;
    ifstr.open(filename,ifstream::in);
    if(!ifstr)
    {
       cerr<<"error: unable to open input file: .wrl"<<endl;
       return success;
   }

    char cstr[512];
    ifstr.getline(cstr, 512);
    
    string str = cstr;
       
    if(str!= VRML_HEADER)
    {
        cerr<<"error: the head of the .wrl file is wrong!"<<endl;
        ifstr.close();
        return success;
    }

    Tokenizer tokenizer(ifstr);

    while (tokenizer.get() && tokenizer != "IndexedFaceSet")
      ;

    if (tokenizer != "IndexedFaceSet")
    {
      cerr << "Error: No IndexedFaceSet is found here" << endl;
      ifstr.close();
      return success;
    }

    //After the IndexedFaceSet is found
    //We need to further tokenize the IndexedFaceSet are
    while (tokenizer.get())
    {

      try
      {
        if (tokenizer == "coord")
        {

          try
          {
            if (!tokenizer.expecting("Coordinate"))
              throw("Syntax Error: 'Coordinate'");
            if (!tokenizer.expecting("{"))
              throw("Syntax Error: Coordinate missing {");
            if (!tokenizer.expecting("point"))
              throw("Syntax Error: 'point'");
          }
          catch (const char *error)
          {
            cerr << string(error) << endl;
            return success;
          }

          //Point

          vector<float> *tempCoord = &(ifs.getCoord());
          int flag = _load_point(tokenizer, tempCoord);
          // cout<<flag<<" "<<tempCoord<<endl;
          if (flag < 0)
            throw(flag);

          //coord index
        }
        else if (tokenizer == "coordIndex")
        {

          vector<int> *tempCoordIndex = &(ifs.getCoordIndex());
          int flag = _load_index(tokenizer, tempCoordIndex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "ccw")
        {
          bool *tempCcw = &(ifs.getCcw());
          int flag = _load_bool(tokenizer, tempCcw);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "convex")
        {
          bool *tempConvex = &(ifs.getConvex());
          int flag = _load_bool(tokenizer, tempConvex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "creaseAngle")
        {
          float *tempCreaseAngle = &(ifs.getCreaseangle());
          float angle_num;

          tokenizer.get();
          if (!my_stof(tokenizer, angle_num))
            throw -2;
          else
            *tempCreaseAngle = angle_num;
        }
        else if (tokenizer == "solid")
        {
          bool *tempSolid = &(ifs.getSolid());
          int flag = _load_bool(tokenizer, tempSolid);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "colorPerVertex")
        {

          tokenizer.get();
          if (tokenizer == "FALSE")
          {
            ifs.setColorPerVertex(false);
          }
          else if (tokenizer == "TRUE")
          {
            ifs.setColorPerVertex(true);
          }
          else
          {
            cerr << "Syntax error: colorPerVertex." << endl;
            return success;
          }
        }
        else if (tokenizer == "color")
        {
          try
          {
            if (!tokenizer.expecting("Color"))
              throw("Syntax error: Color");
            if (!tokenizer.expecting("{"))
              throw("Syntax error: Color missing {");
            if (!tokenizer.expecting("color"))
              throw("Syntax error: color");
          }
          catch (const char *error)
          {
            cerr << string(error) << endl;
            return success;
          }
          vector<float> *tempColor = &(ifs.getColor());
          int flag = _load_point(tokenizer, tempColor);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "colorIndex")
        {

          vector<int> *tempColorIndex = &(ifs.getColorIndex());
          int flag = _load_index(tokenizer, tempColorIndex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "normalPerVertex")
        {
          tokenizer.get();
          if (tokenizer == "FALSE")
          {
            ifs.setNormalPerVertex(false);
          }
          else if (tokenizer == "TRUE")
          {
            ifs.setNormalPerVertex(true);
          }
          else
          {
            cerr << "Syntax error: normalPerVertex" << endl;
            return success;
          }
        }
        else if (tokenizer == "normal")
        {
          try
          {
            if (!tokenizer.expecting("Normal"))
              throw("Syntax error: Normal");
            if (!tokenizer.expecting("{"))
              throw("Syntax error: Color missing {");
            if (!tokenizer.expecting("vector"))
              throw("Syntax error: (in field normal) vector");
          }
          catch (const char *error)
          {
            cerr << string(error) << endl;
            return success;
          }

          vector<float> *tempNormal = &(ifs.getNormal());
          int flag = _load_point(tokenizer, tempNormal);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "normalIndex")
        {

          vector<int> *tempNormalIndex = &(ifs.getNormalIndex());
          int flag = _load_index(tokenizer, tempNormalIndex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "texCoord")
        {

          try
          {
            if (!tokenizer.expecting("TextureCoordinate"))
              throw("Syntax error: TextureCoordinate");
            if (!tokenizer.expecting("{"))
              throw("Syntax error: Color missing {");
            if (!tokenizer.expecting("point"))
              throw("Syntax error: texture point");
          }
          catch (const char *error)
          {
            cerr << string(error) << endl;
            return success;
          }

          vector<float> *tempTex = &(ifs.getTexCoord());
          int flag = _load_point(tokenizer, tempTex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "texCoordIndex")
        {

          vector<int> *tempTexIndex = &(ifs.getTexCoordIndex());
          int flag = _load_index(tokenizer, tempTexIndex);
          if (flag < 0)
            throw(flag);
        }
        else if (tokenizer == "}")
        {
          //have finished reading the IndexedFaceSet filed. Let us leave
          break;
        }
      }
      catch (int error)
      {
        switch (error)
        {
        case -1:
          cerr << "Syntax error: missing [" << endl;
          break;
        case -2:
          cerr << "Syntax error: wrong number" << endl;
          break;
        case -3:
          cerr << "Syntax error: missing ]" << endl;
          break;
        case -4:
          cerr << "Syntax error: missing }" << endl;
          break;
        default:
          break;
        }
        return success;
      } //try

    } //while(get())
    if (tokenizer != "}")
    {
      cerr << "Syntax Error: Not ending with '}'" << endl;
      ifstr.close();
      return success;
    }

    success = true;
    ifstr.close();
  }

  return success;
}
