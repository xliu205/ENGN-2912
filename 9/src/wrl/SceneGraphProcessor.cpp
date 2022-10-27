

#include <unistd.h>
#include "SceneGraph.hpp"
#include "SceneGraphProcessor.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "Appearance.hpp"
#include "Material.hpp"


using namespace Eigen;

SceneGraphProcessor::SceneGraphProcessor(SceneGraph &wrl) : 
_wrl(wrl),
_nGrid(0),
_nPoints(0),
_next((int *)0),
_first((int *)0){
}

SceneGraphProcessor::~SceneGraphProcessor()
{
    _deletePartition();
}

int SceneGraphProcessor::_createPartition(Vec3f &min, Vec3f &max, int depth, vector<float> &coord)
{
    int nPointsInPartition = 0;
    _deletePartition();
    float dx = max.x - min.x;
    float dy = max.y - min.y;
    float dz = max.z - min.z;
    if (dx > 0.0f && dy > 0.0f && dz > 0.0f)
    {
        _nPoints = (int)(coord.size() / 3);
        if (depth < 0)
            depth = 0;
        _nGrid = 1 << depth;
        int nCells = _nCells();
        _next = new int[_nPoints];
        _first = new int[nCells];
        int iCell, iPoint, ix, iy, iz;
        float x, y, z, nG = (float)_nGrid;
        for (iCell = 0; iCell < nCells; iCell++)
            _first[iCell] = -1;
        for (iPoint = 0; iPoint < _nPoints; iPoint++)
        {
            if ((x = coord[3 * iPoint]) < min.x || x > max.x)
                continue;
            if ((y = coord[3 * iPoint + 1]) < min.y || y > max.y)
                continue;
            if ((z = coord[3 * iPoint + 2]) < min.z || z > max.z)
                continue;
            ix = (int)((nG * (x - min.x)) / dx);
            iy = (int)((nG * (y - min.y)) / dy);
            iz = (int)((nG * (z - min.z)) / dz);
            iCell = ix + _nGrid * (iy + _nGrid * iz);
            _next[iPoint] = _first[iCell];
            _first[iCell] = iPoint;
            nPointsInPartition++;
        }
    }
    return nPointsInPartition;
}

void SceneGraphProcessor::_deletePartition()
{
    if (_first != (int *)0)
        delete[] _first;
    if (_next != (int *)0)
        delete[] _next;
    _nGrid = 0;
    _nPoints = 0;
    _next = (int *)0;
    _first = (int *)0;
}

void SceneGraphProcessor::normalClear()
{
    IndexedFaceSet::Operator o  =_normalClear;
    applytoIFS(o);
}

void SceneGraphProcessor::_normalClear(IndexedFaceSet &ifs)
{
    vector<float> &normal = ifs.getNormal();
    vector<int> &normalIndex = ifs.getNormalIndex();
    ifs.setNormalPerVertex(true);
    normal.clear();
    normalIndex.clear();
}

void SceneGraphProcessor::normalInvert()
{
    IndexedFaceSet::Operator o = _normalInvert;
    applytoIFS(o);
}

void SceneGraphProcessor::_normalInvert(IndexedFaceSet &ifs)
{
    vector<float> &normal = ifs.getNormal();
    for (int i = 0; i < (int)normal.size(); i++){
        normal[i] = -normal[i];
    }
}

void SceneGraphProcessor::computeNormalPerFace()
{
    IndexedFaceSet::Operator o = _computeNormalPerFace;
    applytoIFS(o);
}

void SceneGraphProcessor::_computeNormalPerFace(IndexedFaceSet &ifs)
{
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_FACE)
        return;

    vector<float> &coord = ifs.getCoord();
    vector<int> &coordIndex = ifs.getCoordIndex();
    vector<float> &normal = ifs.getNormal();
    vector<int> &normalIndex = ifs.getNormalIndex();

    normal.clear();
    normalIndex.clear();
    ifs.setNormalPerVertex(false);
    int iF, iV, i, i0, i1;
    float n0, n1, n2, x0, x1, x2;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++)
    {
        Vector3d N(0, 0, 0);
        if (coordIndex[i1] < 0)
        {
            _computeFaceNormal(coord, coordIndex, i0, i1, N, true);
            normal.push_back(N.x());
            normal.push_back(N.y());
            normal.push_back(N.z());
            i0 = i1 + 1;
            iF++;
        }
    }
}

void SceneGraphProcessor::_computeFaceNormal(vector<float> &coord, vector<int> &coordIndex,
                                             int i0, int i1, Vector3d &n, bool normalize)
{
    if (coordIndex.size() == 0)
        return;

    int iV, i;
    float x0, x1, x2;

    if (coordIndex[i1] < 0)
    { 
        vector<Vector3d> list;
        for (i = i0; i < i1; i++)
        {
            iV = coordIndex[i];
            x0 = coord[3 * iV];
            x1 = coord[3 * iV + 1];
            x2 = coord[3 * iV + 2];

            list.push_back(Vector3d(x0, x1, x2));
        }
    
        if (list.size() == 3)
        {
            Vector3d v = list.at(0) - list.at(1);
            Vector3d w = list.at(0) - list.at(2);
            n = v.cross(w);
        }
        else
        {
            Vector3d p;
            int i = 0;
            while (i < list.size())
            {
                p.x() += list.at(i).x();
                p.y() += list.at(i).y();
                p.z() += list.at(i).z();
                i++;
            }
            p.x() /= list.size();
            p.y() /= list.size();
            p.z() /= list.size();
            for (int k = 0; k < list.size(); k++)
            {
                if (k == list.size() - 1)
                {
                    Vector3d v(list.at(k).x() - p.x(), list.at(k).y() - p.y(), list.at(k).z() - p.z());
                    Vector3d w(list.at(0).x() - p.x(), list.at(0).y() - p.y(), list.at(0).z() - p.z());
                    Vector3d temp = v.cross(w);
                    n.x() += temp.x();
                    n.y() += temp.y();
                    n.z() += temp.z();
                }
                else
                {
                    Vector3d v(list.at(k).x() - p.x(), list.at(k).y() - p.y(), list.at(k).z() - p.z());
                    Vector3d w(list.at(k + 1).x() - p.x(), list.at(k + 1).y() - p.y(), list.at(k + 1).z() - p.z());
                    Vector3d temp = v.cross(w);
                    n.x() += temp.x();
                    n.y() += temp.y();
                    n.z() += temp.z();
                }
            }
        }
        list.clear();
        if (normalize)
        {
            float normal = sqrt(n.x() * n.x() + n.y() * n.y() + n.z() * n.z());
            n /= normal;
        }
    }
}


void SceneGraphProcessor::computeNormalPerVertex()
{
    IndexedFaceSet::Operator o = _computeNormalPerVertex;
    applytoIFS(o);
}

void SceneGraphProcessor::_computeNormalPerVertex(IndexedFaceSet &ifs)
{
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_VERTEX)
        return;

    vector<float> &coord = ifs.getCoord();
    vector<int> &coordIndex = ifs.getCoordIndex();
    vector<float> &normal = ifs.getNormal();
    vector<int> &normalIndex = ifs.getNormalIndex();
    int nV = coord.size() / 3;
    ifs.getNormal().clear();
    normalIndex.clear();
    ifs.setNormalPerVertex(false);

    int iF, iV, i, i0, i1;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++)
    {
        Vector3d N(0, 0, 0);
        if (coordIndex[i1] < 0)
        {
            _computeFaceNormal(coord, coordIndex, i0, i1, N, false);
            normal.push_back(N.x());
            normal.push_back(N.y());
            normal.push_back(N.z());
            i0 = i1 + 1;
            iF++;
        }
    }
    
    vector<float> faceNormal;
    faceNormal.swap(normal);
    for (int i = 0; i < coord.size(); i++)
    {
        normal.push_back(0.0f);
    }

    ifs.setNormalPerVertex(true);
    int iV0, iV2;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++)
    {
        if (coordIndex[i1] < 0)
        { 
            for (i = i0; i < i1; i++)
            {
                iV = coordIndex[i];
              
                normal.at(iV * 3) += faceNormal.at(3 * iF);
                normal.at(iV * 3 + 1) += faceNormal.at(3 * iF + 1);
                normal.at(iV * 3 + 2) += faceNormal.at(3 * iF + 2);
                if (i == i0)
                    iV0 = coordIndex[i1 - 1];
                else
                    iV0 = coordIndex[i - 1]; // [i1-1] if i==0
                if (i == i1 - 1)
                    iV2 = coordIndex[i0];
                else
                    iV2 = coordIndex[i + 1]; // [i0] if i==i0-1
                Vector3d v(coord[iV * 3] - coord[iV2 * 3], coord[iV * 3 + 1] - coord[iV2 * 3 + 1], coord[iV * 3 + 2] - coord[iV2 * 3 + 2]);
                Vector3d w(coord[iV * 3] - coord[iV0 * 3], coord[iV * 3 + 1] - coord[iV0 * 3 + 1], coord[iV * 3 + 2] - coord[iV0 * 3 + 2]);
                Vector3d n = v.cross(w);
                normal.at(iV * 3) += n.x();
                normal.at(iV * 3 + 1) += n.y();
                normal.at(iV * 3 + 2) += n.z();
            }
            i0 = i1 + 1;
            iF++;
        }
    }
    for (iV = 0; iV < nV; iV++)
    {
     
        float normalize = sqrt(normal.at(iV * 3) * normal.at(iV * 3) + normal.at(iV * 3 + 1) * normal.at(iV * 3 + 1) + normal.at(iV * 3 + 2) * normal.at(iV * 3 + 2));
        normal.at(iV * 3) /= normalize;
        normal.at(iV * 3 + 1) /= normalize;
        normal.at(iV * 3 + 2) /= normalize;
    }
}

void SceneGraphProcessor::computeNormalPerCorner()
{
    IndexedFaceSet::Operator o = _computeNormalPerCorner;
    applytoIFS(o);
}


void SceneGraphProcessor::_computeNormalPerCorner(IndexedFaceSet &ifs)
{
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_CORNER)
        return;
    vector<float> coord = ifs.getCoord();
    vector<int> &coordIndex = ifs.getCoordIndex();
    vector<float> &normal = ifs.getNormal();
    vector<int> &normalIndex = ifs.getNormalIndex();

    normal.clear();
    normalIndex.clear();
    ifs.setNormalPerVertex(true);
    int iF, iV0, iV1, iV2, i, i0, i1, iN;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++)
    {
        if (coordIndex[i1] < 0)
        { 
            for (i = i0; i < i1; i++)
            {
                if (i == i0)
                    iV0 = coordIndex[i1 - 1];
                else
                    iV0 = coordIndex[i - 1]; // [i1-1] if i==0
                iV1 = coordIndex[i];
                if (i == i1 - 1)
                    iV2 = coordIndex[i0];
                else
                    iV2 = coordIndex[i + 1]; // [i0] if i==i0-1
             
                Vector3d v(coord[iV1 * 3] - coord[iV2 * 3], coord[iV1 * 3 + 1] - coord[iV2 * 3 + 1], coord[iV1 * 3 + 2] - coord[iV2 * 3 + 2]);
                Vector3d w(coord[iV1 * 3] - coord[iV0 * 3], coord[iV1 * 3 + 1] - coord[iV0 * 3 + 1], coord[iV1 * 3 + 2] - coord[iV0 * 3 + 2]);
                Vector3d n = w.cross(v);
                float normalize = sqrt(n.x() * n.x() + n.y() * n.y() + n.z() * n.z());
                iN = normal.size() / 3;
                normal.push_back(n.x() / normalize);
                normal.push_back(n.y() / normalize);
                normal.push_back(n.z() / normalize);
                
                normalIndex.push_back(iN);
            }
  
            normalIndex.push_back(-1);
            i0 = i1 + 1;
            iF++;
        }
    }
}
bool SceneGraphProcessor::_hasIndexedFaceSetProperty(IndexedFaceSet::Property p)
{
    bool value = false;

    
    Node *child;
    SceneGraphTraversal *t = new SceneGraphTraversal(_wrl);
    t->start();
    child = t->next();
    while (child != NULL)
    {
        if (child->isShape())
        {
            Shape *shape = (Shape *)child;
            if (shape->hasGeometryIndexedFaceSet())
                if (p(*(IndexedFaceSet *)(shape->getGeometry())))
                    value = true;
        }
        child = t->next();
    }
    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetFaces()
{
    
    return _hasIndexedFaceSetProperty(_hasFaces);
}

bool SceneGraphProcessor::_hasFaces(IndexedFaceSet &ifs)
{
    bool value = false;

    if (ifs.getNumberOfFaces() > 0)
        value = true;

    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalNone()
{
    return _hasIndexedFaceSetProperty(_hasNormalNone);
}

bool SceneGraphProcessor::_hasNormalNone(IndexedFaceSet &ifs)
{
    bool value = false;

    if (ifs.getNormalBinding() == IndexedFaceSet::PB_NONE)
        value = true;
    return value;
}

bool SceneGraphProcessor::_hasNormalPerFace(IndexedFaceSet &ifs)
{
    bool value = false;
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_FACE)
        value = true;
    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerFace()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerFace);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerVertex()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerVertex);
}


bool SceneGraphProcessor::_hasNormalPerVertex(IndexedFaceSet &ifs)
{
    bool value = false;

    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_VERTEX)
        value = true;
    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerCorner()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerCorner);
}

bool SceneGraphProcessor::_hasNormalPerCorner(IndexedFaceSet &ifs)
{
    bool value = false;
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_CORNER)
        value = true;
    return value;
}

bool SceneGraphProcessor::hasNormalNone(IndexedFaceSet &ifs)
{
    return (ifs.getNumberOfCoord() == 0 && ifs.getNumberOfFaces() == 0) ||
           (ifs.getNormalBinding() == IndexedFaceSet::PB_NONE);
}
bool SceneGraphProcessor::hasNormalPerFace(IndexedFaceSet &ifs)
{
    return (ifs.getNumberOfFaces() > 0) && 
           (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_FACE);
}
bool SceneGraphProcessor::hasNormalPerVertex(IndexedFaceSet &ifs)
{
    return (ifs.getNumberOfCoord() > 0) && 
           (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_VERTEX);
}
bool SceneGraphProcessor::hasNormalPerCorner(IndexedFaceSet &ifs)
{
    return (ifs.getNumberOfCoord() > 0) && 
           (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_CORNER);
}
void SceneGraphProcessor::applytoIFS(IndexedFaceSet::Operator o){
    SceneGraphTraversal trav( _wrl);
    trav.start();
    Node* node;
    while ((node = trav.next())!=(Node*)0){
        if(node->isShape()){
            Shape* shape = (Shape*)node;
            node = shape->getGeometry();
            if (node != (Node*)0 && node->isIndexedFaceSet()){
               IndexedFaceSet &ifs = *((IndexedFaceSet *)node);
               o(ifs);
            }
        }
        
    }
}
