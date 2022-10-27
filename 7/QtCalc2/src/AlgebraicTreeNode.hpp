#ifndef _AlgebraicTreeNode_hpp_
#define _AlgebraicTreeNode_hpp_
#include <string.h>
using namespace std;

enum AlgebraicTreeOperationType {
    ZERO,ADD,SUBTRACT,MULTIPLY,DIVIDE
};

class AlgebraicTreeNode {

public:

    
  virtual ~AlgebraicTreeNode();

  AlgebraicTreeNode();

  const bool isRoot() const;
  const bool isInvalid() const;

  virtual const bool isNumber() const;
  virtual const bool isOperation() const;
    virtual bool operator == (AlgebraicTreeNode* src) =0;

  virtual double   evaluate() const =0;
  virtual unsigned toStringLength() const =0;
  virtual unsigned toString(char& str) const =0;

  void setInvalid(bool value);
  void setParent(AlgebraicTreeNode* parent);

protected:

   bool               _invalid;
   AlgebraicTreeNode* _parent;

};

#endif // _AlgebraicTreeNode_hpp_
