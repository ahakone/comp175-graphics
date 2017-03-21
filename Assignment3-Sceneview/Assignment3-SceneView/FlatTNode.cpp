#include "FlatTNode.h"

FlatTNode::FlatTNode() {
}

FlatTNode::FlatTNode(ScenePrimitive prim, Matrix trans) {
  primitive = prim;
  transformation = trans;
}

FlatTNode::~FlatTNode() {
}

ScenePrimitive FlatTNode::GetPrimitive() {
  return primitive;
}

Matrix FlatTNode::GetTMatrix() {
  return transformation;
}
