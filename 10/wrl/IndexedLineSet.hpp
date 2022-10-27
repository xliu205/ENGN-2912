
#ifndef _INDEXED_LINE_SET_HPP_
#define _INDEXED_LINE_SET_HPP_

#include "Node.hpp"
#include <vector>

using namespace std;

class IndexedLineSet : public Node {

private:

  vector<float> _coord;
  vector<int>   _coordIndex;
  vector<float> _color;
  vector<int>   _colorIndex;
  bool          _colorPerVertex;

public:
  
  IndexedLineSet();

  void           clear();
  bool&          getColorPerVertex();
  vector<float>& getCoord();
  vector<int>&   getCoordIndex();
  vector<float>& getColor();
  vector<int>&   getColorIndex();

  int            getNumberOfPolylines();

  int            getNumberOfCoord();
  int            getNumberOfColor();

  void           setColorPerVertex(bool value);

  virtual bool    isIndexedLineSet() const { return             true; }
  virtual string  getType()          const { return "IndexedLineSet"; }

  typedef bool    (*Property)(IndexedLineSet& ifs);
  typedef void    (*Operator)(IndexedLineSet& ifs);

};

#endif // _INDEXED_LINE_SET_HPP_
