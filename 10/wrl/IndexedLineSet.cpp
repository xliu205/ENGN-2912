

#include <iostream>
#include "IndexedLineSet.hpp"



IndexedLineSet::IndexedLineSet():
  _colorPerVertex(true)
{}

void IndexedLineSet::clear() {

 
  if (_coord.size() != 0)
    _coord.clear();
  if (_color.size() != 0)
    _color.clear();
  if (_coordIndex.size() != 0)
    _coordIndex.clear();
  if (_colorIndex.size() != 0)
    _colorIndex.clear();
}

bool& IndexedLineSet::getColorPerVertex()
{ return _colorPerVertex; }
vector<float>& IndexedLineSet::getCoord()
{ return _coord; }
vector<int>& IndexedLineSet::getCoordIndex()
{ return _coordIndex; }
vector<float>& IndexedLineSet::getColor()
{ return _color; }
vector<int>& IndexedLineSet::getColorIndex()
{ return _colorIndex; }
int IndexedLineSet::getNumberOfCoord()
{ return (int)(_coord.size()/3); }
int IndexedLineSet::getNumberOfColor()
{ return (int)(_color.size()/3); }

int IndexedLineSet::getNumberOfPolylines()   {
  int nPolylines = 0;

  // count number of -1's in coordIndex
  nPolylines = count(_coordIndex.begin(), _coordIndex.end(), -1);

  return nPolylines;
}

void IndexedLineSet::setColorPerVertex(bool value) {
  _colorPerVertex = value;
}

