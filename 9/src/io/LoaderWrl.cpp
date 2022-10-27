

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "LoaderWrl.hpp"
#include "StrException.hpp"
#include "Tokenizer.hpp"

#define VRML_HEADER "#VRML V2.0 utf8"

const char *LoaderWrl::_ext = "wrl";

bool LoaderWrl::load(const char *filename, SceneGraph &wrl)
{

  bool success = false;
  FILE *fp = nullptr;
  

  try
  {

    // open the file
    ifstream ifs(filename);
  
    if (filename == (char *)0)throw "filename is empty";
    fp = fopen(filename, "r");
    if (fp == (FILE *)0)throw "file path can't be created";
    
    //ifstream ifs;
    //ifs.open(filename);

    /*if (filename != (char *)0){
      ifstream ifstr;
      ifstr.open(filename, ifstream::in);
    }
    
    if (filename != (char *)0){
      ifs.open(filename, ifstream::in);
    }
    */
      

    // clear the container
    wrl.clear();
    // save the filename
    wrl.setUrl(filename);

    // read and check header line
    char header[32] = "";
    // first line of file must be equal to VRML_HEADER
    ifs.getline(header, 32);
    string str = header;
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    if (string(str) != VRML_HEADER)
      throw new StrException("header!=VRM_HEADER");

    Tokenizer tkn(fp);
    bool res = loadSceneGraph(tkn, wrl);
    wrl.updateBBox();
    ifs.close();

    if (res && !tkn.get())
      success = true;
    }
  catch (StrException *e)
  {
    delete e;
    wrl.clear();
    wrl.setUrl("");
    success = false;
  }

  return success;
}

bool LoaderWrl::loadSceneGraph(Tokenizer &tkn, SceneGraph &wrl)
{

  bool success = false;

  string name = "";
  while (tkn.get())
  {
    if (tkn == "DEF")
    {
      tkn.get();
      name = tkn;
    }
    else if (tkn == "Group")
    {
      Group *g = new Group();
      g->setName(name);
      wrl.addChild(g);
      g->setParent(&wrl);
      loadGroup(tkn, *g);
    }
    else if (tkn == "Transform")
    {
      Transform *t = new Transform();
      t->setName(name);
      wrl.addChild(t);
      t->setParent(&wrl);
      loadTransform(tkn, *t);
    }
    else if (tkn == "Shape")
    {
      Shape *s = new Shape();
      s->setName(name);
      wrl.addChild(s);
      s->setParent(&wrl);
      loadShape(tkn, *s);
    }
    else if (tkn == "}")
    {
      break;
    }
    else
    {
      // fprintf(stderr,"tkn=\"%s\"\n",tkn.c_str());
      throw new StrException("unexpected token while parsing Group");
    }
  }
  success = true;
  return success;
}



bool LoaderWrl::loadGroup(Tokenizer &tkn, Group &group)
{

  bool success = false;
  if (!tkn.expecting("{"))
    throw new StrException("expecting \"{\"");
  while (success == false && tkn.get())
  {
    if (tkn == "children")
    {
      //   ...
      if (!tkn.expecting("["))
      {
        throw new StrException("expectiong \"[\"");
      }
      loadChildren(tkn, group);
    }
    else if (tkn == "bboxCenter")
    {
      tkn.get();
      double x = atof(tkn.c_str());
      tkn.get();
      double y = atof(tkn.c_str());
      tkn.get();
      double z = atof(tkn.c_str());
      Vec3f vec3(x, y, z);
      group.getBBoxCenter() = vec3;
    }
    else if (tkn == "bboxSize")
    {
      tkn.get();
      double x = atof(tkn.c_str());
      tkn.get();
      double y = atof(tkn.c_str());
      tkn.get();
      double z = atof(tkn.c_str());
      Vec3f vec3(x, y, z);
      group.getBBoxSize() = vec3;
    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing Group");
    }
  }

  return success;
}

bool LoaderWrl::loadTransform(Tokenizer &tkn, Transform &transform)
{


  bool success = false;


  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  while (success == false && tkn.get())
  {
    if (tkn == "children")
    {

      //   ...
      if (!tkn.expecting("["))
      {
        throw new StrException("expectiong \"[\"");
      }
      loadChildren(tkn, transform);
    }
    else if (tkn == "center")
    {

      //   ...
      tkn.get();
      float x = atof(tkn.c_str());
      tkn.get();
      float y = atof(tkn.c_str());
      tkn.get();
      float z = atof(tkn.c_str());
      Vec3f vec3(x, y, z);
      transform.setCenter(vec3);
    }
    else if (tkn == "rotation")
    {

      //   ...
      tkn.get();
      float x = atof(tkn.c_str());
      tkn.get();
      float y = atof(tkn.c_str());
      tkn.get();
      float z = atof(tkn.c_str());
      tkn.get();
      float k = atof(tkn.c_str());
      Rotation rotation(x, y, z, k);
      transform.setRotation(rotation);
    }
    else if (tkn == "scale")
    {

      //   ...
      tkn.get();
      float x = atof(tkn.c_str());
      tkn.get();
      float y = atof(tkn.c_str());
      tkn.get();
      float z = atof(tkn.c_str());
      Vec3f vec3(x, y, z);
      transform.setScale(vec3);
    }
    else if (tkn == "scaleOrientation")
    {

      //   ...
      tkn.get();
      float x = atof(tkn.c_str());
      tkn.get();
      float y = atof(tkn.c_str());
      tkn.get();
      float z = atof(tkn.c_str());
      tkn.get();
      float k = atof(tkn.c_str());
      Rotation scaleOrientation(x, y, z, k);
      transform.setScaleOrientation(scaleOrientation);
    }
    else if (tkn == "translation")
    {

      //   ...
      tkn.get();
      float x = atof(tkn.c_str());
      tkn.get();
      float y = atof(tkn.c_str());
      tkn.get();
      float z = atof(tkn.c_str());
      Vec3f translation(x, y, z);
      transform.setTranslation(translation);
    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing Group");
    }
  }
  return success;
}

bool LoaderWrl::loadShape(Tokenizer &tkn, Shape &shape)
{

  bool success = false;

  string name = "";
  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  //   ...
  while (success == false && tkn.get())
  {
    if (tkn == "appearance")
    {
      Appearance *appearance = new Appearance();
      shape.setAppearance(appearance);
      appearance->setParent(&shape);
      loadAppearance(tkn, *appearance);
    }
    else if (tkn == "geometry")
    {
      tkn.get();
      if (tkn == "DEF")
      {
        name = tkn;
        tkn.get();
      }
      if (tkn == "IndexedFaceSet")
      {
        IndexedFaceSet *indexedFaceSet = new IndexedFaceSet();
        indexedFaceSet->setName(name);
        shape.setGeometry(indexedFaceSet);
        indexedFaceSet->setParent(&shape);
        loadIndexedFaceSet(tkn, *indexedFaceSet);
      }

    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing Transform");
    }
  }

  return success;
}


bool LoaderWrl::loadAppearance(Tokenizer &tkn, Appearance &appearance)
{

  bool success = false;

  //  
  tkn.get();
  appearance.setName(tkn);
  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  //   ...
  while (success == false && tkn.get())
  {
    if (tkn == "material")
    {
      Material *material = new Material();
      appearance.setMaterial(material);
      loadMaterial(tkn, *material);
    }
    else if (tkn == "texture")
    {
      ImageTexture *imageTexture = new ImageTexture();
      appearance.setTexture(imageTexture);
      loadImageTexture(tkn, *imageTexture);
    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing Appearance");
    }
  }
  return success;
  return success;
}


bool LoaderWrl::loadMaterial(Tokenizer &tkn, Material &material)
{
  bool success = false;

  //  
  tkn.get();
  material.setName(tkn);
  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  //   ...
  while (success == false && tkn.get())
  {
    if (tkn == "ambientIntensity")
    {
      tkn.get();
      float ambientIntensity = atof(tkn.c_str());
      material.setAmbientIntensity(ambientIntensity);
    }
    else if (tkn == "diffuseColor")
    {
      tkn.get();
      double x = atof(tkn.c_str());
      tkn.get();
      double y = atof(tkn.c_str());
      tkn.get();
      double z = atof(tkn.c_str());
      Color color(x, y, z);
      material.setDiffuseColor(color);
    }
    else if (tkn == "emissiveColor")
    {
      tkn.get();
      double x = atof(tkn.c_str());
      tkn.get();
      double y = atof(tkn.c_str());
      tkn.get();
      double z = atof(tkn.c_str());
      Color color(x, y, z);
      material.setEmissiveColor(color);
    }
    else if (tkn == "shininess")
    {
      tkn.get();
      float shininess = atof(tkn.c_str());
      material.setShininess(shininess);
    }
    else if (tkn == "specularColor")
    {
      tkn.get();
      double x = atof(tkn.c_str());
      tkn.get();
      double y = atof(tkn.c_str());
      tkn.get();
      double z = atof(tkn.c_str());
      Color color(x, y, z);
      material.setSpecularColor(color);
    }
    else if (tkn == "transparency")
    {
      tkn.get();
      float transparency = atof(tkn.c_str());
      material.setTransparency(transparency);
    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing Material");
    }
  }
  return success;
}

bool LoaderWrl::loadImageTexture(Tokenizer &tkn, ImageTexture &imageTexture)
{

  bool success = false;

  //  
  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  //   ...
  while (success == false && tkn.get())
  {
    if (tkn == "url")
    {
      if (tkn.expecting("["))
        throw new StrException("expecting \"[\"");
      while (tkn.get() && tkn != "]")
      {
        if (tkn.substr(tkn.length() - 3) == "JPG" || tkn.substr(tkn.length() - 4) == "JPEG" || tkn.substr(tkn.length() - 3) == "PNG" || tkn.substr(tkn.length() - 3) == "GIF")
          imageTexture.adToUrl(tkn);
        else
          throw new StrException("invalid imageTexture");
      }
    }
    else if (tkn == "repeatS")
    {
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      imageTexture.setRepeatS(b);
    }
    else if (tkn == "repearT")
    {
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      imageTexture.setRepeatT(b);
    }
    else if (tkn == "}")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing ImageTexture");
    }
  }
  return success;
}

bool LoaderWrl::loadIndexedFaceSet(Tokenizer &tkn, IndexedFaceSet &ifs)
{

  bool success = false;

  //  
  if (tkn.expecting("{") == false)
    throw new StrException("expecting \"{\"");
  while (success == false && tkn.get())
  {
    if (tkn == "solid" || tkn == "solid\r" || tkn == "solid\n" || tkn == "solid\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      if (tkn.compare(tkn.size() - string(1, '\n').size(), string(1, '\n').size(), string(1, '\n')) == 0)
      {
        tkn.erase(std::remove(tkn.begin(), tkn.end(), '\r'), tkn.end());
        tkn.erase(std::remove(tkn.begin(), tkn.end(), '\n'), tkn.end());
      }
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for solid");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.getSolid() = b;
    }
    else if (tkn == "coord" || tkn == "coord\r" || tkn == "coord\n" || tkn == "coord\r\n")
    {
      if (!tkn.expecting("Coordinate"))
        throw new StrException("Error loading file: missing coord name");
      if (!tkn.expecting("{"))
        throw new StrException("Error loading file: missing '{' in the file");
      if (!tkn.expecting("point"))
        throw new StrException("Error loading file: missing 'point'");
      if (!tkn.expecting("["))
        throw new StrException("Error loading file: missing '[' in the file");
      int count = 0;
      while (tkn.get() && tkn != "]")
      {
        if (tkn.compare(tkn.size() - string(1, '\n').size(), string(1, '\n').size(), string(1, '\n')) == 0 || count % 3 == 2)
        {
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\r'), tkn.end());
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\n'), tkn.end());
          count++;
        }
        else
        {
          count++;
        }
        float coord = atof(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length())
        {
          throw 4;
          return success;
        }
        else
        {
          ifs.getCoord().push_back(coord);
        }
      }
      if (tkn != "]")
        throw new StrException("Missing ']' in the file");
      if (!tkn.expecting("}"))
      {
        throw new StrException("Error loading file: missing '}' in the file");
      }
    }
    else if (tkn == "coordIndex" || tkn == "coordIndex\r" || tkn == "coordIndex\r\n" || tkn == "coordIndex\n")
    {
      if (!tkn.expecting("["))
      {
        throw new StrException("Error loading file: missing '[' in the file");
      }
      while (tkn.get() && tkn != "]")
      {
        int index = atoi(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length() || (index < -1))
        {
          throw new StrException("Error loading file: Invalid coordIndex point");
          //                          return success;
        }
        else
        {
          ifs.getCoordIndex().push_back(index);
        }
      }
      if (tkn != "]")
        throw new StrException("Missing ']' in the file");
    }
    else if (tkn == "color" || tkn == "color\r" || tkn == "color\n" || tkn == "color\r\n")
    {
      if (!tkn.expecting("Color"))
        throw new StrException("Error loading file: missing coord name");
      if (!tkn.expecting("{"))
        throw new StrException("Error loading file: missing '{' in the file");
      if (!tkn.expecting("color"))
        throw new StrException("Error loading file: missing 'point'");
      if (!tkn.expecting("["))
        throw new StrException("Error loading file: missing '[' in the file");
      unsigned count = 0;
      while (tkn.get() && tkn != "]")
      {
        if (tkn.compare(tkn.size() - string(1, '\n').size(), string(1, '\n').size(), string(1, '\n')) == 0 || count % 3 == 2)
        {
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\r'), tkn.end());
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\n'), tkn.end());
          count++;
        }
        else
        {
          count++;
        }
        float color = atof(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length())
        {
          throw new StrException("Error loading file: error color value");
          return success;
        }
        else
        {
          ifs.getColor().push_back(color);
        }
      }
      if (tkn != "]")
        throw new StrException("Missing ']' in the file");
      if (!tkn.expecting("}"))
      {
        throw new StrException("Error loading file: missing '}' in the file");
      }
    }
    else if (tkn == "colorIndex" || tkn == "colorIndex\r" || tkn == "colorIndex\r\n" || tkn == "colorIndex\n")
    {
      if (!tkn.expecting("["))
      {
        throw new StrException("Error loading file: missing '[' in the file");
      }
      while (tkn.get() && tkn != "]")
      {
        int index = atoi(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length() || index < -1)
        {
          throw new StrException("Error loading file: Invalid colorIndex point");
          return success;
        }
        else
        {
          ifs.getColorIndex().push_back(index);
        }
      }
    }
    else if (tkn == "colorPerVertex" || tkn == "colorPerVertex\r" || tkn == "colorPerVertex\n" || tkn == "colorPerVertex\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for colorPerVertex");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.setColorPerVertex(b);
    }
    else if (tkn == "texCoord" || tkn == "texCoord\r" || tkn == "texCoord\n" || tkn == "texCoord\r\n")
    {
      if (!tkn.expecting("TextureCoordinate"))
        throw new StrException("Error loading file: missing coord name");
      if (!tkn.expecting("{"))
        throw new StrException("Error loading file: missing '{' in the file");
      if (!tkn.expecting("point"))
        throw new StrException("Error loading file: missing 'point'");
      if (!tkn.expecting("["))
        throw new StrException("Error loading file: missing '[' in the file");
      while (tkn.get() && tkn != "]")
      {
        if (tkn.compare(tkn.size() - string(1, '\n').size(), string(1, '\n').size(), string(1, '\n')) == 0)
        {
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\r'), tkn.end());
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\n'), tkn.end());
        }
        float texcoord = atof(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length())
        {
          throw new StrException("Error loading file: error textureCoordinate value");
        }
        else
        {
          ifs.getTexCoord().push_back(texcoord);
        }
      }
      if (!tkn.expecting("}"))
      {
        throw new StrException("Error loading file: missing '}' in the file");
      }
    }
    else if (tkn == "texCoordIndex" || tkn == "texCoordIndex\r" || tkn == "texCoordIndex\n" || tkn == "texCoordIndex\r\n")
    {
      if (!tkn.expecting("["))
      {
        throw new StrException("Error loading file: missing '[' in the file");
      }
      while (tkn.get() && tkn != "]")
      {
        int index = atoi(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length() || index < -1)
        {
          throw new StrException("Error loading file: Invalid texCoordIndex point");
        }
        else
        {
          ifs.getTexCoordIndex().push_back(index);
        }
      }
    }
    else if (tkn == "normalPerVertex" || tkn == "normalPerVertex\r" || tkn == "normalPerVertex\n" || tkn == "normalPerVertex\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for normalPerVertex");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.setNormalPerVertex(b);
    }
    else if (tkn == "normal" || tkn == "normal\r" || tkn == "normal\n" || tkn == "normal\r\n")
    {
      if (!tkn.expecting("Normal"))
        throw new StrException("Error loading file: missing coord name");
      if (!tkn.expecting("{"))
        throw new StrException("Error loading file: missing '{' in the file");
      if (!tkn.expecting("vector"))
        throw new StrException("Error loading file: missing 'point'");
      if (!tkn.expecting("["))
        throw new StrException("Error loading file: missing '[' in the file");
      while (tkn.get() && tkn != "]")
      {
        if (tkn.compare(tkn.size() - string(1, '\n').size(), string(1, '\n').size(), string(1, '\n')) == 0)
        {
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\r'), tkn.end());
          tkn.erase(std::remove(tkn.begin(), tkn.end(), '\n'), tkn.end());
        }
        float normal = atof(tkn.c_str());
        char *endPtr;
        strtod(tkn.c_str(), &endPtr);
        int length = endPtr - tkn.c_str();
        if (length != tkn.length())
        {
          throw new StrException("Error loading file: error normal value");
          return success;
        }
        else
        {
          ifs.getNormal().push_back(normal);
        }
      }
      if (!tkn.expecting("}"))
      {
        throw new StrException("Error loading file: missing '}' in the file");
      }
    }
    else if (tkn == "ccw" || tkn == "ccw\r" || tkn == "ccw\n" || tkn == "ccw\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for ccw");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.getCcw() = b;
    }
    else if (tkn == "convex" || tkn == "convex\r" || tkn == "convex\n" || tkn == "convex\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for convex");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.getConvex() = b;
    }
    else if (tkn == "creaseAngle" || tkn == "creaseAngle\r" || tkn == "creaseAngle\n" || tkn == "creaseAngle\r\n")
    {
      if (!tkn.get())
        throw new StrException("Error loading file: hit the end of the file");
      string value = tkn;
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (!value.compare("true") && !value.compare("false"))
      {
        throw new StrException("Error loading file: no set value for creaseAngle");
      }
      std::istringstream is(value);
      bool b;
      is >> std::boolalpha >> b;
      ifs.getCreaseangle() = b;
    }
    else if (tkn == "}" || tkn == "}\r" || tkn == "}\n" || tkn == "}\r\n")
    {
      success = true;
    }
    else
    {
      throw new StrException("unexpected token while parsing IndexedFaceSet");
    }
  }

  return success;
}

bool LoaderWrl::loadChildren(Tokenizer &tkn, Group &group)
{
  bool success = false;
  return success;
}
