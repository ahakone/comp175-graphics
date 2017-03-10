#ifndef FLATTNODE_H
#define FLATTNODE_H

#include "Algebra.h"
#include "SceneData.h"

using namespace std;

class FlatTNode
{
 public:
  FlatTNode();
  FlatTNode(ScenePrimitive primitive, Matrix transformation);
  ~FlatTNode();
  ScenePrimitive GetPrimitive();
  Matrix GetTMatrix();
  // get skin info
 private:
  ScenePrimitive primitive;
  Matrix transformation;
  // skin info
};

#endif
