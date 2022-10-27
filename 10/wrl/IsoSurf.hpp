

#ifndef _ISO_SURF_HPP_
#define _ISO_SURF_HPP_

#include<vector>

using namespace std;

class IsoSurf {

public:

  static int makeCellFaces
    (bool b[/*8*/], int iE[/*12*/], vector<int>& coordIndex);

  static const int (*getEdgeTable())[2];

private:

  static const int _faceTable[12][12];
  static const int _edgeTable[12][2];

};

#endif // _ISO_SURF_HPP_
