

#ifndef _GROUP_HPP_
#define _GROUP_HPP_



#include <vector>
#include "Node.hpp"
#include <stack>
#include "eigen/Eigen/Dense"

using namespace std;

class Group : public Node
{

protected:
  vector<pNode> _children;
  Vec3f _bboxCenter;
  Vec3f _bboxSize;

public:
  Group();
  virtual ~Group();

  vector<pNode> &getChildren();
  Node *getChild(const string &name) const;
  int getNumberOfChildren() const;
  pNode operator[](const int i);
  void addChild(pNode child);
  void removeChild(pNode child);

  Vec3f &getBBoxCenter();
  Vec3f &getBBoxSize();
  float getBBoxDiameter();
  void setBBoxCenter(Vec3f &value);
  void setBBoxSize(Vec3f &value);
  void clearBBox();
  bool hasEmptyBBox() const;
  void appendBBoxCoord(vector<float> &coord);
  void updateBBox(vector<float> &coord);
  virtual void updateBBox();
  void dfs(stack<Eigen::Matrix4f> stack, Node *node, Eigen::Matrix4f T);

  virtual bool isGroup() const { return true; };
  virtual string getType() const { return "Group"; };

  typedef bool (*Property)(Group &group);
  typedef void (*Operator)(Group &group);

  
};

#endif // _GROUP_HPP_