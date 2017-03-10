#include "FlatTNode.h"
#include <iostream>
#include <fstream>
#include <string>

FlatTNode::FlatTNode() {
}

FlatTNode::FlatTNode(ScenePrimitive prim, Matrix trans) {
  primitive = prim;
  transformation = trans;
  hasTexture = false;
  textureWidth = 0;
  textureHeight = 0;
  texture = NULL;
  LoadTexture();
}

FlatTNode::~FlatTNode() {
    for (int i = 0; i < textureHeight; i++) {
        delete [] texture[i];
    }

    delete [] texture;
}

ScenePrimitive FlatTNode::GetPrimitive() {
  return primitive;
}

Matrix FlatTNode::GetTMatrix() {
  return transformation;
}

void FlatTNode::LoadTexture()
{
    SceneFileMap *tMap = primitive.material.textureMap;
    if (tMap->isUsed) {
        ifstream tFile;
        tFile.open(tMap->filename);
        if (!tFile.is_open()) {
            std::cerr << "unable to open file" << std::endl;
            return;
        }
        repeatU = tMap->repeatU;
        repeatV = tMap->repeatV;

        string line;
        tFile >> line;

        std::getline(tFile, line); // get and ignore the file identifier
        std::getline(tFile, line); // get and ignore the comment

        // get width and height
        tFile >> textureWidth;
        tFile >> textureHeight;
        
        int maxColorVal;
        tFile >> maxColorVal;

        int newHeight = textureHeight;
        int newWidth = textureWidth;
        texture = new SceneColor*[newHeight];
        for (int i = 0; i < newHeight; i++) {
            texture[i] = new SceneColor[newWidth];
        }
        
        for (int i = 0; i < textureHeight; i++) {
            for (int j = 0; j < textureWidth; j++) {
                int unscaledR;
                tFile >> unscaledR;
                int unscaledG;
                tFile >> unscaledG;
                int unscaledB;
                tFile >> unscaledB;

                texture[i][j].r = (float) unscaledR / (float) maxColorVal;
                texture[i][j].g = (float) unscaledG / (float) maxColorVal;
                texture[i][j].b = (float) unscaledB / (float) maxColorVal;
            }
        }


        
        hasTexture = true;

        tFile.close();
        
    }

}


