

#ifndef _SCENE_GRAPH_PROCESSOR_HPP_
#define _SCENE_GRAPH_PROCESSOR_HPP_

#include <iostream>
#include "SceneGraph.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include <Eigen/Dense>
#include <Eigen/Geometry>
using namespace Eigen;

class SceneGraphProcessor {

public:

  SceneGraphProcessor(SceneGraph& wrl);
  ~SceneGraphProcessor() ;

  void normalClear();
  void normalInvert();
  void computeNormalPerFace();
  void computeNormalPerVertex();
  void computeNormalPerCorner();

  bool hasIndexedFaceSetFaces();
  bool hasIndexedFaceSetNormalNone();
  bool hasIndexedFaceSetNormalPerFace();
  bool hasIndexedFaceSetNormalPerVertex();
  bool hasIndexedFaceSetNormalPerCorner();

  static bool hasNormalNone(IndexedFaceSet& ifs);
  static bool hasNormalPerFace(IndexedFaceSet& ifs);
  static bool hasNormalPerVertex(IndexedFaceSet& ifs);
  static bool hasNormalPerCorner(IndexedFaceSet& ifs);

private:

  SceneGraph& _wrl;
  int  _nGrid;
  int  _nPoints;
  int* _next;
  int* _first;
  int  _nCells() { return _nGrid*_nGrid*_nGrid; }
  int  _createPartition(Vec3f& min, Vec3f& max, int depth, vector<float>&coord);
  void _deletePartition();

  void applytoIFS(IndexedFaceSet::Operator p);

  static void _normalClear(IndexedFaceSet &ifs);
  static void _normalInvert(IndexedFaceSet& ifs);
  static void _computeNormalPerFace(IndexedFaceSet& ifs);
  static void _computeNormalPerVertex(IndexedFaceSet& ifs);
  static void _computeNormalPerCorner(IndexedFaceSet& ifs);

  static void _computeFaceNormal
        (vector<float>& coord, vector<int>&   coordIndex,
         int i0, int i1, Vector3d& n, bool normalize);

  bool        _hasIndexedFaceSetProperty(IndexedFaceSet::Property p);
  static bool _hasFaces(IndexedFaceSet& ifs);
  static bool _hasNormalNone(IndexedFaceSet& ifs);
  static bool _hasNormalPerFace(IndexedFaceSet& ifs);
  static bool _hasNormalPerVertex(IndexedFaceSet& ifs);
  static bool _hasNormalPerCorner(IndexedFaceSet& ifs);


};

#endif // _SCENE_GRAPH_PROCESSOR_HPP_