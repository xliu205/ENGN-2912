

#include <unistd.h>
#include <iostream>
#include "SceneGraphProcessor.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "Appearance.hpp"
#include "Material.hpp"
#include "IndexedLineSet.hpp"
#include "IsoSurf.hpp"

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

void SceneGraphProcessor::normalInvert()
{
    IndexedFaceSet::Operator o = _normalInvert;
    applytoIFS(o);
}

void SceneGraphProcessor::computeNormalPerFace()
{
    IndexedFaceSet::Operator o = _computeNormalPerFace;
    applytoIFS(o);
}

void SceneGraphProcessor::computeNormalPerVertex()
{
    IndexedFaceSet::Operator o = _computeNormalPerVertex;
    applytoIFS(o);
}

void SceneGraphProcessor::computeNormalPerCorner()
{
    IndexedFaceSet::Operator o = _computeNormalPerCorner;
    applytoIFS(o);
}

void SceneGraphProcessor::applytoIFS(IndexedFaceSet::Operator o)
{
    SceneGraphTraversal trav(_wrl);
    trav.start();
    Node *node;
    while ((node = trav.next()) != (Node *)0)
    {
        if (node->isShape())
        {
            Shape *shape = (Shape *)node;
            node = shape->getGeometry();
            if (node != (Node *)0 && node->isIndexedFaceSet())
            {
                IndexedFaceSet &ifs = *((IndexedFaceSet *)node);
                o(ifs);
            }
        }
    }
}

void SceneGraphProcessor::_normalClear(IndexedFaceSet &ifs)
{
    vector<float> &normal = ifs.getNormal();
    vector<int> &normalIndex = ifs.getNormalIndex();
    ifs.setNormalPerVertex(true);
    normal.clear();
    normalIndex.clear();
}

void SceneGraphProcessor::_normalInvert(IndexedFaceSet &ifs)
{
    vector<float> &normal = ifs.getNormal();
    for (int i = 0; i < (int)normal.size(); i++)
    {
        normal[i] = -normal[i];
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


void SceneGraphProcessor::bboxAdd(int depth, float scale, bool cube)
{

    Shape *node = (Shape *)_wrl.find("BOUNDING-BOX");
    if (node == (Node *)0)
    {
        Shape *shape = new Shape();
        shape->setName("BOUNDING-BOX");
        Appearance *appearance = new Appearance();
        Material *material = new Material();
        Color color(1.0, 0.0, 0.0);
        material->setDiffuseColor(color);
        appearance->setMaterial(material);
        shape->setAppearance(appearance);
        
        shape->setGeometry(new IndexedLineSet());
        _wrl.addChild(shape);
        node = shape;
    }
    else if (node->hasGeometryIndexedFaceSet())
    {
        IndexedFaceSet *faceSet = (IndexedFaceSet *)node->getGeometry();
        delete faceSet;
        node->setGeometry(new IndexedLineSet());
    }
    else if (node->getGeometry() == (Node *)0)
    {
        node->setGeometry(new IndexedLineSet());
    }
    else if (node->hasGeometryIndexedLineSet())
    {
        IndexedLineSet *lineSet = (IndexedLineSet *)node->getGeometry();
        lineSet->clear();
    }
    _wrl.updateBBox();
    vector<float> coord;
    _wrl.appendBBoxCoord(coord);
    Vec3f center = _wrl.getBBoxCenter();
    Vec3f size = _wrl.getBBoxSize();
    if (cube)
    {
        float temp = 0;
        if (size.x > temp)
        {
            temp = size.x;
        }
        if (size.y > temp)
            temp = size.y;
        if (size.z > temp)
            temp = size.z;
        size.x = temp;
        size.y = temp;
        size.z = temp;
        _wrl.setBBoxSize(size);
    }
    Vec3f x0(center.x - 0.5 * scale * size.x, center.y - 0.5 * scale * size.y, center.z - 0.5 * scale * size.z);
    Vec3f x1(center.x + 0.5 * scale * size.x, center.y + 0.5 * scale * size.y, center.z + 0.5 * scale * size.z);
    Vec3f newSize(size.x * scale, size.y * scale, size.z * scale);
    _wrl.setBBoxSize(newSize);

    IndexedLineSet* indexedLineSet = (IndexedLineSet*)node->getGeometry();
    int temp;
    int i, j, k, iV, N = (int)pow(2, depth);
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            int count = 0;
            for (k = 0; k <= N; k++)
            {
                count++;
                iV = k + (N + 1) * (j + (N + 1) * i);
                int x = iV - (N + 1) * (N + 1);
                int y = iV - (N + 1);
                indexedLineSet->getCoord().push_back(((N - i) * x0.x + i * x1.x) / N);
                indexedLineSet->getCoord().push_back(((N - j) * x0.y + j * x1.y) / N);
                indexedLineSet->getCoord().push_back(((N - k) * x0.z + k * x1.z) / N);
                if (count == 2)
                {
                    indexedLineSet->getCoordIndex().push_back(temp);
                    indexedLineSet->getCoordIndex().push_back(iV);
                    indexedLineSet->getCoordIndex().push_back(-1);
                    temp = iV;
                    count = 1;
                }
                else
                {
                    temp = iV;
                }
                if (y >= 0 && j > 0)
                {
                    indexedLineSet->getCoordIndex().push_back(y);
                    indexedLineSet->getCoordIndex().push_back(iV);
                    indexedLineSet->getCoordIndex().push_back(-1);
                }
                if (x >= 0 && i > 0)
                {
                    indexedLineSet->getCoordIndex().push_back(x);
                    indexedLineSet->getCoordIndex().push_back(iV);
                    indexedLineSet->getCoordIndex().push_back(-1);
                }
            }
        }
    }
}

void SceneGraphProcessor::bboxRemove()
{
    int n = _wrl.getNumberOfChildren();
    for (int i = 0; i < n; i++)
    {
        if (_wrl[i]->getName() == "BOUNDING-BOX")
            _wrl.getChildren().erase(_wrl.getChildren().begin() + i);
    }
}

void SceneGraphProcessor::edgesAdd()
{

    Node *child;
    SceneGraphTraversal *t = new SceneGraphTraversal(_wrl);
    t->start();
    child = t->next();
    Shape *shape_1 = (Shape *)0;
    Shape *shape_2 = (Shape *)0;
    while (child != NULL)
    {
        if (child->isShape() && shape_1 == (Shape *)0)
        {
            shape_1 = (Shape *)child;
            if (!shape_1->hasGeometryIndexedFaceSet())
                shape_1 = (Shape *)0;
            else
            {
                IndexedFaceSet *faceSet = (IndexedFaceSet *)shape_1->getGeometry();
                Group *parent = (Group *)const_cast<Node *>(shape_1->getParent());
                if (parent == (Group *)0)
                    break;
                for (int i = 0; i < parent->getChildren().size(); i++)
                {
                    if (parent->getChildren().at(i)->isShape() && shape_1 != parent->getChildren().at(i))
                    {
                        if (child->nameEquals("EDGES"))
                        {
                            shape_2 = (Shape *)child;
                            IndexedLineSet *lineSet = new IndexedLineSet();
                            if (shape_2->hasGeometryNull())
                            {
                                shape_1->setGeometry(lineSet);
                            }
                            else if (!shape_2->hasGeometryIndexedLineSet())
                            {
                                delete shape_2->getGeometry();
                                shape_2->setGeometry((Node *)0);
                            }
                            else
                            {
                                lineSet = (IndexedLineSet *)shape_2->getGeometry();
                            }
                            lineSet->clear();
                        }
                    
                    }
                }
                if (shape_1 != (Shape *)0 && shape_2 == (Shape *)0)
                {
                    shape_2 = new Shape();
                    shape_2->setName("EDGES");
                    Appearance *appearance = new Appearance();
                    Material *material = new Material();
                    Color color(1.0, 0.0, 0.0);
                    material->setDiffuseColor(color);
                    appearance->setMaterial(material);
                    shape_2->setAppearance(appearance);
                    shape_2->setGeometry(new IndexedLineSet());
                    parent->addChild(shape_2);
                    shape_2->setParent(parent);
                }
                IndexedLineSet *lineSet = (IndexedLineSet *)shape_2->getGeometry();
                lineSet->clear();
                int i = 0;
                vector<int> temp;
                for (int i = 0; i < faceSet->getCoord().size(); i++)
                {
                    lineSet->getCoord().push_back(faceSet->getCoord().at(i));
                }
                while (i < faceSet->getCoordIndex().size())
                {
                    if (faceSet->getCoordIndex().at(i) != -1)
                    {
                        temp.push_back(faceSet->getCoordIndex().at(i));
                        i++;
                        continue;
                    }
                    else
                    {
                        for (int j = 0; j < temp.size(); j++)
                        {
                            if (j == temp.size() - 1)
                            {
                                lineSet->getCoordIndex().push_back(temp[j]);
                                lineSet->getCoordIndex().push_back(temp[0]);
                                lineSet->getCoordIndex().push_back(-1);
                            }
                            else
                            {
                                lineSet->getCoordIndex().push_back(temp.at(j));
                                lineSet->getCoordIndex().push_back(temp[j + 1]);
                                lineSet->getCoordIndex().push_back(-1);
                            }
                        }
                        i++;
                        temp.clear();
                    }
                }
            }
        }
        child = t->next();
        shape_1 = (Shape *)0;
        shape_2 = (Shape *)0;
    }
}

void SceneGraphProcessor::edgesRemove()
{

    Node *child;
    SceneGraphTraversal *t = new SceneGraphTraversal(_wrl);
    t->start();
    child = t->next();
    while (child != NULL)
    {
        //        if(child->isGroup()){
        //            Group* group = (Group*)child;
        Shape *edges = (Shape *)_wrl.find("EDGES");
        if (edges != (Shape *)0)
        {
            Group *parent = (Group *)edges->getParent();
            parent->removeChild(edges);
            delete edges;
            t->start();
        }
        child = t->next();
    }
}



void SceneGraphProcessor::shapeIndexedFaceSetShow()
{
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
            {
                IndexedFaceSet *faceSet = (IndexedFaceSet *)shape->getGeometry();
                faceSet->setShow(true);
                shape->setShow(true);
            }
        }
        child = t->next();
    }
}

void SceneGraphProcessor::shapeIndexedFaceSetHide()
{

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
            {
                IndexedFaceSet *faceSet = (IndexedFaceSet *)shape->getGeometry();
                faceSet->setShow(false);
                shape->setShow(false);
            }
        }
        child = t->next();
    }
}

void SceneGraphProcessor::shapeIndexedLineSetShow()
{

    Node *child;
    SceneGraphTraversal *t = new SceneGraphTraversal(_wrl);
    t->start();
    child = t->next();
    while (child != NULL)
    {
        if (child->isShape())
        {
            Shape *shape = (Shape *)child;
            if (shape->hasGeometryIndexedLineSet())
            {
                IndexedLineSet *lineSet = (IndexedLineSet *)shape->getGeometry();
                lineSet->setShow(true);
                shape->setShow(true);
            }
        }
        child = t->next();
    }
}

void SceneGraphProcessor::shapeIndexedLineSetHide()
{
    Node *child;
    SceneGraphTraversal *t = new SceneGraphTraversal(_wrl);
    t->start();
    child = t->next();
    while (child != NULL)
    {
        if (child->isShape())
        {
            Shape *shape = (Shape *)child;
            if (shape->hasGeometryIndexedLineSet())
            {
                IndexedLineSet *lineSet = (IndexedLineSet *)shape->getGeometry();
                lineSet->setShow(false);
                shape->setShow(false);
            }
        }
        child = t->next();
    }
}

bool SceneGraphProcessor::hasBBox()
{
    return _wrl.getChild("BOUNDING-BOX") != (Node *)0;
}

bool SceneGraphProcessor::_hasShapeProperty(Shape::Property p)
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
            if (p(*(Shape *)child))
            {
                value = true;
            }
        }
        child = t->next();
    }
    return value;
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

bool SceneGraphProcessor::_hasIndexedLineSetProperty(IndexedLineSet::Property p)
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
            if (shape->hasGeometryIndexedLineSet())
            {
                if (p(*(IndexedLineSet *)(shape->getGeometry())))
                    value = true;
            }
        }
        child = t->next();
    }
    return value;
}

bool SceneGraphProcessor::_hasFaces(IndexedFaceSet &ifs)
{
    bool value = false;

    if (ifs.getNumberOfFaces() > 0)
        value = true;

    return value;
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

bool SceneGraphProcessor::_hasNormalPerVertex(IndexedFaceSet &ifs)
{
    bool value = false;

    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_VERTEX)
        value = true;
    return value;
}

bool SceneGraphProcessor::_hasNormalPerCorner(IndexedFaceSet &ifs)
{
    bool value = false;
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_CORNER)
        value = true;
    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetFaces()
{

    return _hasIndexedFaceSetProperty(_hasFaces);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalNone()
{
    return _hasIndexedFaceSetProperty(_hasNormalNone);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerFace()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerFace);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerVertex()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerVertex);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerCorner()
{
    return _hasIndexedFaceSetProperty(_hasNormalPerCorner);
}

bool SceneGraphProcessor::_hasColorNone(IndexedLineSet &ils)
{
    bool value = false;

    //  
    if (ils.getColor().empty())
        value = true;
    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerVertex(IndexedLineSet &ils)
{
    bool value = false;

    //  
    if (ils.getColorPerVertex())
        value = true;
    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerPolyline(IndexedLineSet &ils)
{
    bool value = false;

    //  
    if (ils.getNumberOfColor() > 0 && ils.getNumberOfPolylines() > 0)
        value = true;

    return value;
}

bool SceneGraphProcessor::hasIndexedLineSetColorNone()
{
    return _hasIndexedLineSetProperty(_hasColorNone);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerVertex()
{
    return _hasIndexedLineSetProperty(_hasColorPerVertex);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerPolyline()
{
    return _hasIndexedLineSetProperty(_hasColorPerPolyline);
}

// this is a Property
bool SceneGraphProcessor::_hasEdges(Shape &shape)
{
    bool value = false;

    Group *group = (Group *)shape.getParent();
    if (group == (Node *)0)
        return value;
    for (int i = 0; i < group->getChildren().size(); i++)
    {
        if (group->getChildren()[i]->nameEquals("EDGES"))
            value = true;
    }

    return value;
}

bool SceneGraphProcessor::_hasIndexedFaceSetShown(Shape &shape)
{
    bool value = false;
 
    if (shape.hasGeometryIndexedFaceSet())
    {
        IndexedFaceSet *faceSet = (IndexedFaceSet *)shape.getGeometry();
        if (faceSet->getShow())
            value = true;
    }
    return value;
}

bool SceneGraphProcessor::_hasIndexedFaceSetHidden(Shape &shape)
{
    bool value = false;

    
    if (shape.hasGeometryIndexedFaceSet())
    {
        IndexedFaceSet *faceSet = (IndexedFaceSet *)shape.getGeometry();
        if (!faceSet->getShow())
            value = true;
    }
    return value;
}


bool SceneGraphProcessor::_hasIndexedLineSetShown(Shape &shape)
{
    bool value = false;


    if (shape.hasGeometryIndexedLineSet())
    {
        IndexedLineSet *lineSet = (IndexedLineSet *)shape.getGeometry();
        if (lineSet->getShow())
            value = true;
    }
    return value;
}


bool SceneGraphProcessor::_hasIndexedLineSetHidden(Shape &shape)
{
    bool value = false;

    if (shape.hasGeometryIndexedLineSet())
    {
        IndexedLineSet *lineSet = (IndexedLineSet *)shape.getGeometry();
        if (!lineSet->getShow())
            value = true;
    }
    return value;
}

bool SceneGraphProcessor::hasEdges()
{
    return _hasShapeProperty(_hasEdges);
}

bool SceneGraphProcessor::hasIndexedFaceSetShown()
{
    return _hasShapeProperty(_hasIndexedFaceSetShown);
}

bool SceneGraphProcessor::hasIndexedFaceSetHidden()
{
    return _hasShapeProperty(_hasIndexedFaceSetHidden);
}

bool SceneGraphProcessor::hasIndexedLineSetShown()
{
    return _hasShapeProperty(_hasIndexedLineSetShown);
}

bool SceneGraphProcessor::hasIndexedLineSetHidden()
{
    return _hasShapeProperty(_hasIndexedLineSetHidden);
}

void SceneGraphProcessor::removeSceneGraphChild(const string &name)
{

    Node *child = _wrl.find(name);
    if (child != NULL)
    {
        Group *parent = (Group *)child->getParent();
        parent->removeChild(child);
    }
}

void SceneGraphProcessor::pointsRemove()
{
    removeSceneGraphChild("POINTS");
}

void SceneGraphProcessor::surfaceRemove()
{
    removeSceneGraphChild("SURFACE");
}


IndexedFaceSet *SceneGraphProcessor::_getNamedShapeIFS(const string &name, bool create)
{

    IndexedFaceSet *ifs = (IndexedFaceSet *)0;
    Node *node = _wrl.find(name);
    if (node != (Node *)0 && node->isShape())
    {
        Shape *shape = (Shape *)node;
        if (shape->hasGeometryIndexedFaceSet())
        {
            Appearance *appear = (Appearance *)shape->getAppearance();
            if (appear->getMaterial() != (Node *)0)
                ifs = (IndexedFaceSet *)shape->getGeometry();
        }
    }
    else if (create == true)
    {
        Shape *shape = new Shape();
        Appearance *appear = new Appearance();
        appear->setMaterial(new Material());
        shape->setAppearance(appear);
        shape->setGeometry(new IndexedFaceSet());
        _wrl.addChild(shape);
        ifs = (IndexedFaceSet *)shape->getGeometry();
    }
    return ifs;
}



void eigenFit(const vector<float> &coordPoints,
              const Vec3f &min, const Vec3f &max, Vec4f &f)
{

    char str[256];
    cerr << "eigenFit() {" << endl;

    int nPoints = (int)((coordPoints.size()) / 3);
    float x0 = min.x, x1 = max.x, dx = x1 - x0;
    float y0 = min.y, y1 = max.y, dy = y1 - y0;
    float z0 = min.z, z1 = max.z, dz = z1 - z0;
    float dMax = dx;
    if (dy > dMax)
        dMax = dy;
    if (dz > dMax)
        dMax = dz;



    double x, y, z;
    double xMean = 0.0f;
    double yMean = 0.0f;
    double zMean = 0.0f;
    int nMean = 0;
    for (int iP = 0; iP < nPoints; iP++)
    {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1)
        {
            xMean += x;
            yMean += y;
            zMean += z;
            nMean++;
        }
    }

    cerr << "  nMean = " << nMean << endl;
    if (nMean == 0)
    {
       
        return;
    }

    xMean /= ((double)nMean);
    yMean /= ((double)nMean);
    zMean /= ((double)nMean);

    cerr << "  pMean = [" << endl;
    sprintf(str, "    %12.6f", xMean);
    cerr << str << endl;
    sprintf(str, "    %12.6f", yMean);
    cerr << str << endl;
    sprintf(str, "    %12.6f", zMean);
    cerr << str << endl;
    cerr << "  ]" << endl;


    double dxp, dyp, dzp;
    double M00 = 0.0, M01 = 0.0, M02 = 0.0, M11 = 0.0, M12 = 0.0, M22 = 0.0;
    for (int iP = 0; iP < nPoints; iP++)
    {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1)
        {
            dxp = (x - xMean) / dMax;
            dyp = (y - yMean) / dMax;
            dzp = (z - zMean) / dMax;
            M00 += dxp * dxp;
            M01 += dxp * dyp;
            M02 += dxp * dzp;
            M11 += dyp * dyp;
            M12 += dyp * dzp;
            M22 += dzp * dzp;
        }
    }

    double dMean = (double)nMean;
    M00 /= dMean;
    M01 /= dMean;
    M02 /= dMean;
    M11 /= dMean;
    M12 /= dMean;
    M22 /= dMean;
    Matrix3d M;
    M << M00, M01, M02, M01, M11, M12, M02, M12, M22;

    // double dMean = (double)nMean;
    // M(0,0) /= dMean; M(0,1) /= dMean; M(0,2) /= dMean;
    // M(1,0) /= dMean; M(1,1) /= dMean; M(1,2) /= dMean;
    // M(2,0) /= dMean; M(2,1) /= dMean; M(2,2) /= dMean;

    cerr << "  M = [" << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(0, 0), M(0, 1), M(0, 2));
    cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(1, 0), M(1, 1), M(1, 2));
    cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(2, 0), M(2, 1), M(2, 2));
    cerr << str << endl;
    cerr << "  ]" << endl;

    SelfAdjointEigenSolver<Matrix3d> eigensolver(M);

    if (eigensolver.info() != Success)
    {
    
        return;
    }

    Vector3d L(eigensolver.eigenvalues());
    Matrix3d E(eigensolver.eigenvectors());

    cerr << "  eigenvalues(M)" << endl;
    cerr << "  L = [" << endl;
    sprintf(str, "    %12.6f", L(0));
    cerr << str << endl;
    sprintf(str, "    %12.6f", L(1));
    cerr << str << endl;
    sprintf(str, "    %12.6f", L(2));
    cerr << str << endl;
    cerr << "  ]" << endl;

    cerr << "  eigenvectors(M)" << endl;
    cerr << "  E = [" << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(0, 0), E(0, 1), E(0, 2));
    cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(1, 0), E(1, 1), E(1, 2));
    cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(2, 0), E(2, 1), E(2, 2));
    cerr << str << endl;
    cerr << "  ]" << endl;

    // L(0)                   minimum eigenvalue
    // (E(0,0),E(1,0),E(2,0)) minimum eigenvector

    f.x = (float)(E(0, 0));
    f.y = (float)(E(1, 0));
    f.z = (float)(E(2, 0));
    f.w = -(float)(E(0, 0) * xMean + E(1, 0) * yMean + E(2, 0) * zMean);

    cerr << "  f = [" << endl;
    sprintf(str, "    %12.6f", f.x);
    cerr << str << endl;
    sprintf(str, "    %12.6f", f.y);
    cerr << str << endl;
    sprintf(str, "    %12.6f", f.z);
    cerr << str << endl;
    sprintf(str, "    %12.6f", f.w);
    cerr << str << endl;
    cerr << "  ]" << endl;

    cerr << "}" << endl;
}


void meanFit(const vector<float> &coordPoints,
             const vector<float> &normalPoints,
             const Vec3f &min, const Vec3f &max, Vec4f &f)
{

    int nPoints = (int)((coordPoints.size()) / 3);
    float x0 = min.x, x1 = max.x, dx = x1 - x0;
    float y0 = min.y, y1 = max.y, dy = y1 - y0;
    float z0 = min.z, z1 = max.z, dz = z1 - z0;
    float dMax = dx;
    if (dy > dMax)
        dMax = dy;
    if (dz > dMax)
        dMax = dz;


    double x, y, z, nn;
    double xMean = 0.0f;
    double yMean = 0.0f;
    double zMean = 0.0f;
    double nxMean = 0.0f;
    double nyMean = 0.0f;
    double nzMean = 0.0f;
    int nMean = 0;
    for (int iP = 0; iP < nPoints; iP++)
    {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1)
        {
            xMean += x;
            yMean += y;
            zMean += z;
            x = (double)(normalPoints[3 * iP]);
            y = (double)(normalPoints[3 * iP + 1]);
            z = (double)(normalPoints[3 * iP + 2]);
            nxMean += x;
            nyMean += y;
            nzMean += z;
            nMean++;
        }
    }

    if (nMean == 0)
    {
      
        return;
    }


    xMean /= ((double)nMean);
    yMean /= ((double)nMean);
    zMean /= ((double)nMean);
  
  
    nn = nxMean * nxMean + nyMean * nyMean + nzMean * nzMean;
    if (nn > 0.0)
    {
        nn = sqrt(nn);
        nxMean /= nn;
        nyMean /= nn;
        nzMean /= nn;
    }
   
    f.x = (float)(nxMean);
    f.y = (float)(nyMean);
    f.z = (float)(nzMean);
    f.w = -(float)(nxMean * xMean + nyMean * yMean + nzMean * zMean);
}

void SceneGraphProcessor::fitSinglePlane(const Vec3f &center, const Vec3f &size,
                                         const float scale, const bool cube, Vec4f &f)
{

    cerr << "SceneGraphProcessor::fitSinglePlane() {" << endl;


    if (_wrl.hasEmptyBBox())
    {
        throw new StrException("BBOX is EMPTY!");
        // return;
    }


    int n = _wrl.getNumberOfChildren();
    bool find = false;
    Shape *points = new Shape();
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "POINTS")
        {
            points = (Shape *)_wrl.getChildren()[i];
            find = true;
            if ((!points->hasGeometryIndexedFaceSet()) ||
                ((IndexedFaceSet *)points->getGeometry())->getNumberOfNormal() == 0)
            {
                throw new StrException("POINTS has NO coordinates or normal");
            }
        }
    }
    if (find == false)
    {
        points->setName("POINTS");
        _wrl.addChild(points);
        IndexedFaceSet *ifs = new IndexedFaceSet();
        points->setGeometry(ifs);
    }


    vector<float> &coordPoints = ((IndexedFaceSet *)points->getGeometry())->getCoord();
    vector<float> &normalPoints = ((IndexedFaceSet *)points->getGeometry())->getNormal();


    float x0, x1, y0, y1, z0, z1;
    if (cube)
    {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    }
    else
    {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);
    // compute the eight bbox corner coordinates
    Vec3f v[8];
    v[0].x = x0;
    v[1].x = x0;
    v[2].x = x0;
    v[3].x = x0;
    v[4].x = x1;
    v[5].x = x1;
    v[6].x = x1;
    v[7].x = x1;
    v[0].y = y0;
    v[1].y = y0;
    v[2].y = y1;
    v[3].y = y1;
    v[4].y = y0;
    v[5].y = y0;
    v[6].y = y1;
    v[7].y = y1;
    v[0].z = z0;
    v[1].z = z1;
    v[2].z = z0;
    v[3].z = z1;
    v[4].z = z0;
    v[5].z = z1;
    v[6].z = z0;
    v[7].z = z1;


    eigenFit(coordPoints, min, max, f);
    meanFit(coordPoints, normalPoints, min, max, f);

    n = _wrl.getNumberOfChildren();
    Shape *surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "SURFACE")
        {
            surface = (Shape *)_wrl.getChildren()[i];
            find = true;
            break;
        }
    }
    if (find == false)
    {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }

    IndexedFaceSet *plane = new IndexedFaceSet();
    surface->setGeometry(plane);
    plane->clear();

    // get from the SURFACE IndexedFaceSet
    vector<float> &coordIfs = plane->getCoord();
    vector<int> &coordIndexIfs = plane->getCoordIndex();

    float F[8]; // function values at bbox corners
    bool b[8];  // function is positive or negative 
    for (int i = 0; i < 8; i++)
    {
        // set (x,y,z) the the i=th corner coordinates computed in step 3) above
        // b[i] = (F[i] = x*f.x+y*f.y+z*f.z+f.w)<0.0f;
        float x = v[i].x;
        float y = v[i].y;
        float z = v[i].z;
        F[i] = x * f.x + y * f.y + z * f.z + f.w;
        b[i] = F[i] < 0.0f;
    }

    cerr << "//                6 ----- 7 = (x1,y1,z1)" << endl;
    cerr << "//               /|      /|             " << endl;
    cerr << "//              4 ----- 5 |             " << endl;
    cerr << "//              | |     | |             " << endl;
    cerr << "//              | 2 ----| 3             " << endl;
    cerr << "//              |/      |/              " << endl;
    cerr << "// (x0,y0,z0) = 0 ----- 1               " << endl;

    cerr << "  b = [" << endl;
    cerr << "    " << b[0] << endl;
    cerr << "    " << b[1] << endl;
    cerr << "    " << b[2] << endl;
    cerr << "    " << b[3] << endl;
    cerr << "    " << b[4] << endl;
    cerr << "    " << b[5] << endl;
    cerr << "    " << b[6] << endl;
    cerr << "    " << b[7] << endl;
    cerr << "  ]" << endl;

    //////////////////////////////////////////////////////////////////////
    //
    //    vertices      //    edges                 //    faces
    //      6-----7     //        [6]---11---[7]    //        1
    //     /|    /|     //        /|         /|     //        | 3
    //    4-----5 |     //       6 2        7 3     //        |/
    //    | 2---|-3     //      /  |       /  |     //    4---+---5
    //    |/    |/      //    [4]---10---[5]  |     //       /|
    //    0-----1       //     |   |      |   |     //      2 |
    //                  //     |  [2]--9--|--[3]    //        0
    //                  //     0  /       1  /      //
    //                  //     | 4        | 5       //
    //                  //     |/         |/        //
    //                  //    [0]---8----[1]        //
    //

    const int(*edge)[2] = IsoSurf::getEdgeTable();



    float tj, tk;
    int iE[12], iV, i, j, k;
    for (i = 0; i < 12; i++)
    {
        // set the isovertex index to 'no vertex' (-1)
        iV = -1;
        j = edge[i][0];
        k = edge[i][1];
        if (b[j] != b[k])
        {
            // isvertex index
            iV = (int)((coordIfs.size() / 3));
            // isovertex coordinates
            tk = F[j] / (F[j] - F[k]);
            tj = F[k] / (F[k] - F[j]);
            float x = tj * v[j].x + tk * v[k].x;
            float y = tj * v[j].y + tk * v[k].y;
            float z = tj * v[j].z + tk * v[k].z;
            coordIfs.push_back(x);
            coordIfs.push_back(y);
            coordIfs.push_back(z);
        }
        iE[i] = iV;
    }
    cerr << "  edge to isovertex table" << endl;
    cerr << "  iE = [" << endl;

    cerr << "  ]" << endl;

    int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
    cerr << "  nFaces = " << nFaces << endl;

    plane->setNormalPerVertex(false);
    vector<float> &normal = plane->getNormal();
    // normal.size()==0 here
    normal.push_back(f.x);
    normal.push_back(f.y);
    normal.push_back(f.z);

    //#endif // IMPLEMENTED

    cerr << "}" << endl;
}


void SceneGraphProcessor::fitMultiplePlanes(const Vec3f &center, const Vec3f &size,
                                            const int depth, const float scale, const bool cube,
                                            vector<float> &f)
{

    int n = _wrl.getNumberOfChildren();
    Shape *points = new Shape();
    bool find = false;
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "POINTS")
        {
            points = (Shape *)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false)
    {
        points->setName("POINTS");
        _wrl.addChild(points);
    }
    vector<float> &coordPoints = ((IndexedFaceSet *)points->getGeometry())->getCoord();
    vector<float> &normalPoints = ((IndexedFaceSet *)points->getGeometry())->getNormal();



    Shape *surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "SURFACE")
        {
            surface = (Shape *)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false)
    {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }
    IndexedFaceSet *ifs = new IndexedFaceSet();
    surface->setGeometry(ifs);
    ifs->clear();

  
    f.clear();



    if (size.x <= 0 || size.y <= 0 ||
        size.z <= 0 || scale <= 0)
        throw new StrException("BOUNDING-BOX is EMPTY!");

    float x0, x1, y0, y1, z0, z1;
    if (cube)
    {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    }
    else
    {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);



    _createPartition(min, max, depth, coordPoints);

    Appearance *appearance = new Appearance();
    Material *material = new Material();
    material->setAmbientIntensity(0.0f);
    Color color(.2, .5, .2);
    material->setDiffuseColor(color);
    appearance->setMaterial(material);
    surface->setAppearance(appearance);
    vector<float> &coordIfs = ifs->getCoord();
    vector<int> &coordIndexIfs = ifs->getCoordIndex();
    vector<float> coordTemp, normalTemp;

    for (int i = 0; i < _nGrid; i++)
    {
        for (int j = 0; j < _nGrid; j++)
        {
            for (int k = 0; k < _nGrid; k++)
            {
                int iCell = k + _nGrid * (j + _nGrid * i);
                cerr << "iCell = " << iCell << endl;
                coordTemp.clear();
                normalTemp.clear();
                if (_first[iCell] < 0)
                    continue;

                Vec3f minCell, maxCell;
                minCell.x = (((float)_nGrid - k) * min.x + (k)*max.x) / (_nGrid);
                maxCell.x = (((float)_nGrid - k - 1) * min.x + (k + 1) * max.x) / (_nGrid);
                minCell.y = (((float)_nGrid - j) * min.y + (j)*max.y) / (_nGrid);
                maxCell.y = (((float)_nGrid - j - 1) * min.y + (j + 1) * max.y) / (_nGrid);
                minCell.z = (((float)_nGrid - i) * min.z + (i)*max.z) / (_nGrid);
                maxCell.z = (((float)_nGrid - i - 1) * min.z + (i + 1) * max.z) / (_nGrid);

                Vec3f v[8];
                v[0].x = minCell.x;
                v[1].x = minCell.x;
                v[2].x = minCell.x;
                v[3].x = minCell.x;
                v[4].x = maxCell.x;
                v[5].x = maxCell.x;
                v[6].x = maxCell.x;
                v[7].x = maxCell.x;
                v[0].y = minCell.y;
                v[1].y = minCell.y;
                v[2].y = maxCell.y;
                v[3].y = maxCell.y;
                v[4].y = minCell.y;
                v[5].y = minCell.y;
                v[6].y = maxCell.y;
                v[7].y = maxCell.y;
                v[0].z = minCell.z;
                v[1].z = maxCell.z;
                v[2].z = minCell.z;
                v[3].z = maxCell.z;
                v[4].z = minCell.z;
                v[5].z = maxCell.z;
                v[6].z = minCell.z;
                v[7].z = maxCell.z;

                // traverse of the cell
                for (int iP = _first[iCell]; iP >= 0; iP = _next[iP])
                {
                    coordTemp.push_back(coordPoints[3 * iP]);
                    coordTemp.push_back(coordPoints[3 * iP + 1]);
                    coordTemp.push_back(coordPoints[3 * iP + 2]);
                    normalTemp.push_back(normalPoints[3 * iP]);
                    normalTemp.push_back(normalPoints[3 * iP + 1]);
                    normalTemp.push_back(normalPoints[3 * iP + 2]);
                }

                Vec4f fTemp;
                eigenFit(coordPoints, minCell, maxCell, fTemp);
                meanFit(coordPoints, normalPoints, minCell, maxCell, fTemp);
    
                f.push_back(fTemp.x);
                f.push_back(fTemp.y);
                f.push_back(fTemp.z);
                f.push_back(fTemp.w);

                float F[8]; // function values at bbox corners
                bool b[8];  // function is positive or negative 
                for (int c = 0; c < 8; c++)
                {
                    // set (x,y,z) the the i=th corner coordinates computed in step 3) above
                    // b[i] = (F[i] = x*f.x+y*f.y+z*f.z+f.w)<0.0f;
                    float x = v[c].x;
                    float y = v[c].y;
                    float z = v[c].z;
                    F[c] = x * fTemp.x + y * fTemp.y + z * fTemp.z + fTemp.w;
                    b[c] = F[c] < 0.0f;
                }

                const int(*edge)[2] = IsoSurf::getEdgeTable();
                float tj, tk;
                int iE[12], iV, q, w, e;
                for (q = 0; q < 12; q++)
                {
                    // set the isovertex index to 'no vertex' (-1)
                    iV = -1;
                    w = edge[q][0];
                    e = edge[q][1];
                    if (b[w] != b[e])
                    {
                        // isvertex index
                        iV = (int)((coordIfs.size() / 3));
                        // isovertex coordinates
                        tk = F[w] / (F[w] - F[e]);
                        tj = F[e] / (F[e] - F[w]);
                        float x = tj * v[w].x + tk * v[e].x;
                        float y = tj * v[w].y + tk * v[e].y;
                        float z = tj * v[w].z + tk * v[e].z;
                        coordIfs.push_back(x);
                        coordIfs.push_back(y);
                        coordIfs.push_back(z);
                    }
                    iE[q] = iV;
                }
                int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
                ifs->setNormalPerVertex(false);
                vector<float> &normal = ifs->getNormal();
                normal.push_back(fTemp.x);
                normal.push_back(fTemp.y);
                normal.push_back(fTemp.z);
            }
        }
    }

   
    ifs->setNormalPerVertex(false);
    _deletePartition();
}

void SceneGraphProcessor::fitContinuous(const Vec3f &center, const Vec3f &size,
                                        const int depth, const float scale, const bool cube)
{

    int n = _wrl.getNumberOfChildren();
    Shape *points = new Shape();
    bool find = false;
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "POINTS")
        {
            points = (Shape *)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false)
    {
        points->setName("POINTS");
        _wrl.addChild(points);
    }
    vector<float> &coordPoints = ((IndexedFaceSet *)points->getGeometry())->getCoord();
    vector<float> &normalPoints = ((IndexedFaceSet *)points->getGeometry())->getNormal();

    Shape *surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++)
    {
        if (_wrl.getChildren()[i]->getName() == "SURFACE")
        {
            surface = (Shape *)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false)
    {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }
    IndexedFaceSet *ifs = new IndexedFaceSet();
    surface->setGeometry(ifs);
    ifs->clear();

    if (size.x <= 0 || size.y <= 0 ||
        size.z <= 0 || scale <= 0)
        throw new StrException("BOUNDING-BOX is EMPTY!");


    float x0, x1, y0, y1, z0, z1;
    if (cube)
    {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    }
    else
    {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);

    _createPartition(min, max, depth, coordPoints);

    Appearance *appearance = new Appearance();
    Material *material = new Material();
    material->setAmbientIntensity(0.0f);
    Color color(.2, .5, .2);
    material->setDiffuseColor(color);
    appearance->setMaterial(material);
    surface->setAppearance(appearance);
    vector<float> &coordIfs = ifs->getCoord();
    vector<int> &coordIndexIfs = ifs->getCoordIndex();
    vector<float> &normal = ifs->getNormal();

    _nGrid = 1 << depth;
    // int nGridVertices = (_nGrid + 1) * (_nGrid + 1) * (_nGrid + 1);
    float ***fGridVertex;
    fGridVertex = new float **[_nGrid + 1];
    for (int x = 0; x < _nGrid + 1; ++x)
    {
        fGridVertex[x] = new float *[_nGrid + 1];
        for (int y = 0; y < _nGrid + 1; ++y)
        {
            fGridVertex[x][y] = new float[_nGrid + 1];
            for (int z = 0; z < _nGrid + 1; ++z)
            {
                fGridVertex[x][y][z] = 0.0f;
            }
        }
    }
    float ***wGridVertex;
    wGridVertex = new float **[_nGrid + 1];
    for (int x = 0; x < _nGrid + 1; ++x)
    {
        wGridVertex[x] = new float *[_nGrid + 1];
        for (int y = 0; y < _nGrid + 1; ++y)
        {
            wGridVertex[x][y] = new float[_nGrid + 1];
            for (int z = 0; z < _nGrid + 1; ++z)
            {
                wGridVertex[x][y][z] = 0.0f;
            }
        }
    }
    vector<float> coordTemp, normalTemp;

    for (int i = 0; i < _nGrid; i++)
    {
        for (int j = 0; j < _nGrid; j++)
        {
            for (int k = 0; k < _nGrid; k++)
            {
                int iCell = k + _nGrid * (j + _nGrid * i);
                // cerr << "iCell = " << iCell << endl;
                coordTemp.clear();
                normalTemp.clear();
                if (_first[iCell] < 0)
                    continue;

                Vec4f f;
                Vec3f minCell, maxCell;
                minCell.x = ((_nGrid - k) * min.x + (k)*max.x) / (_nGrid);
                maxCell.x = ((_nGrid - k - 1) * min.x + (k + 1) * max.x) / (_nGrid);
                minCell.y = ((_nGrid - j) * min.y + (j)*max.y) / (_nGrid);
                maxCell.y = ((_nGrid - j - 1) * min.y + (j + 1) * max.y) / (_nGrid);
                minCell.z = ((_nGrid - i) * min.z + (i)*max.z) / (_nGrid);
                maxCell.z = ((_nGrid - i - 1) * min.z + (i + 1) * max.z) / (_nGrid);
                Vec3f v[8];
                v[0].x = minCell.x;
                v[1].x = minCell.x;
                v[2].x = minCell.x;
                v[3].x = minCell.x;
                v[4].x = maxCell.x;
                v[5].x = maxCell.x;
                v[6].x = maxCell.x;
                v[7].x = maxCell.x;
                v[0].y = minCell.y;
                v[1].y = minCell.y;
                v[2].y = maxCell.y;
                v[3].y = maxCell.y;
                v[4].y = minCell.y;
                v[5].y = minCell.y;
                v[6].y = maxCell.y;
                v[7].y = maxCell.y;
                v[0].z = minCell.z;
                v[1].z = maxCell.z;
                v[2].z = minCell.z;
                v[3].z = maxCell.z;
                v[4].z = minCell.z;
                v[5].z = maxCell.z;
                v[6].z = minCell.z;
                v[7].z = maxCell.z;

                // traverse of the cell
                for (int iP = _first[iCell]; iP >= 0; iP = _next[iP])
                {
                    coordTemp.push_back(coordPoints[3 * iP]);
                    coordTemp.push_back(coordPoints[3 * iP + 1]);
                    coordTemp.push_back(coordPoints[3 * iP + 2]);
                    normalTemp.push_back(normalPoints[3 * iP]);
                    normalTemp.push_back(normalPoints[3 * iP + 1]);
                    normalTemp.push_back(normalPoints[3 * iP + 2]);
                }

                eigenFit(coordTemp, minCell, maxCell, f);
                meanFit(coordTemp, normalTemp, minCell, maxCell, f);
                normal.push_back(f.x);
                normal.push_back(f.y);
                normal.push_back(f.z);

                // evaluate the linear function at bounding box corners
                float F[8]; // function values at bbox corners
                // int map[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                for (int c = 0; c < 8; c++)
                {
                    float x = v[c].x;
                    float y = v[c].y;
                    float z = v[c].z;
                    F[c] = x * f.x + y * f.y + z * f.z + f.w;
                    
                }
                fGridVertex[i][j][k] += F[0];
                fGridVertex[i][j][k + 1] += F[4];
                fGridVertex[i][j + 1][k] += F[2];
                fGridVertex[i][j + 1][k + 1] += F[6];
                fGridVertex[i + 1][j][k] += F[1];
                fGridVertex[i + 1][j][k + 1] += F[5];
                fGridVertex[i + 1][j + 1][k] += F[3];
                fGridVertex[i + 1][j + 1][k + 1] += F[7];

                wGridVertex[i][j][k]++;
                wGridVertex[i][j][k + 1]++;
                wGridVertex[i][j + 1][k]++;
                wGridVertex[i][j + 1][k + 1]++;
                wGridVertex[i + 1][j][k]++;
                wGridVertex[i + 1][j][k + 1]++;
                wGridVertex[i + 1][j + 1][k]++;
                wGridVertex[i + 1][j + 1][k + 1]++;
            }
        }
    }

 
    for (int i = 0; i <= _nGrid; i++)
    {
        for (int j = 0; j <= _nGrid; j++)
        {
            for (int k = 0; k <= _nGrid; k++)
            {
                if (wGridVertex[i][j][k] > 0.0f)
                {
                    fGridVertex[i][j][k] /= wGridVertex[i][j][k];
                }
            }
        }
    }


    for (int i = 0; i < _nGrid; i++)
    {
        for (int j = 0; j < _nGrid; j++)
        {
            for (int k = 0; k < _nGrid; k++)
            {
                int iCell = k + _nGrid * (j + _nGrid * i);
                // cerr << "iCell = " << iCell << endl;
                if (_first[iCell] < 0)
                    continue;

                Vec3f minCell, maxCell;
                minCell.x = (((float)_nGrid - k) * min.x + (k)*max.x) / (float)(_nGrid);
                maxCell.x = (((float)_nGrid - k - 1) * min.x + (k + 1) * max.x) / (float)(_nGrid);
                minCell.y = (((float)_nGrid - j) * min.y + (j)*max.y) / (float)(_nGrid);
                maxCell.y = (((float)_nGrid - j - 1) * min.y + (j + 1) * max.y) / (float)(_nGrid);
                minCell.z = (((float)_nGrid - i) * min.z + (i)*max.z) / (float)(_nGrid);
                maxCell.z = (((float)_nGrid - i - 1) * min.z + (i + 1) * max.z) / (float)(_nGrid);

                Vec3f v[8];
                v[0].x = minCell.x;
                v[1].x = minCell.x;
                v[2].x = minCell.x;
                v[3].x = minCell.x;
                v[4].x = maxCell.x;
                v[5].x = maxCell.x;
                v[6].x = maxCell.x;
                v[7].x = maxCell.x;
                v[0].y = minCell.y;
                v[1].y = minCell.y;
                v[2].y = maxCell.y;
                v[3].y = maxCell.y;
                v[4].y = minCell.y;
                v[5].y = minCell.y;
                v[6].y = maxCell.y;
                v[7].y = maxCell.y;
                v[0].z = minCell.z;
                v[1].z = maxCell.z;
                v[2].z = minCell.z;
                v[3].z = maxCell.z;
                v[4].z = minCell.z;
                v[5].z = maxCell.z;
                v[6].z = minCell.z;
                v[7].z = maxCell.z;

                float F[8]; // function values at bbox corners
                bool b[8];  // function is positive or negative ?
                // int map[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                F[0] = fGridVertex[i][j][k]; // retrieve F from fGridVertex
                F[4] = fGridVertex[i][j][k + 1];
                F[2] = fGridVertex[i][j + 1][k];
                F[6] = fGridVertex[i][j + 1][k + 1];
                F[1] = fGridVertex[i + 1][j][k];
                F[5] = fGridVertex[i + 1][j][k + 1];
                F[3] = fGridVertex[i + 1][j + 1][k];
                F[7] = fGridVertex[i + 1][j + 1][k + 1];
                for (int c = 0; c < 8; c++)
                {
                    // int iV = iCell + map[c];
                    // F[c] = fGridVertex[iV];
                    b[c] = F[c] < 0.0f;
                }

                const int(*edge)[2] = IsoSurf::getEdgeTable();
                float tj, tk;
                int iE[12], iV, q, w, e;
                for (q = 0; q < 12; q++)
                {
                    // set the isovertex index to 'no vertex' (-1)
                    iV = -1;
                    w = edge[q][0];
                    e = edge[q][1];
                    if (b[w] != b[e])
                    {
                        // isvertex index
                        iV = (int)((coordIfs.size() / 3));
                        // isovertex coordinates
                        tk = F[w] / (F[w] - F[e]);
                        tj = F[e] / (F[e] - F[w]);
                        float x = tj * v[w].x + tk * v[e].x;
                        float y = tj * v[w].y + tk * v[e].y;
                        float z = tj * v[w].z + tk * v[e].z;
                        coordIfs.push_back(x);
                        coordIfs.push_back(y);
                        coordIfs.push_back(z);
                    }
                    iE[q] = iV;
                }
                int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
                
            }
        }
    }
    ifs->setNormalPerVertex(false);
    _deletePartition();
    delete[] fGridVertex;
    delete[] wGridVertex;
}
