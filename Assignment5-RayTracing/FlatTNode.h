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

        SceneColor **texture;
        int textureWidth;
        int textureHeight;
        bool hasTexture;
        int repeatU, repeatV;
        // get skin info
    private:
        ScenePrimitive primitive;
        Matrix transformation;
        void LoadTexture();
        // skin info
};

#endif
