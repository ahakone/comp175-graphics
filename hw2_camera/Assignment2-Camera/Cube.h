#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
    // points = new vertex[vertexCount];
    Point** points;
    // normals
    Vector** normals;
    // triangles = nx3 array of pointers to Points
    Face** triangles;

	Cube() {
    };
	~Cube() {
    };


	void draw() {
        int i, j;
        // num triangles on a side
        int numFaces = m_segmentsX * m_segmentsY * 2;
        // int numFaces = m_segmentsX * 2;
        calculatePoints();
        for (j = 0; j < 6; j++) {
            for (i = 0; i < numFaces; i++) {
                glBegin(GL_TRIANGLES);
                normalizeNormal(normals[j][triangles[j][i].p[0]]);
                    glVertex3f(points[j][triangles[j][i].p[0]][0], points[j][triangles[j][i].p[0]][1], points[j][triangles[j][i].p[0]][2]);
                normalizeNormal(normals[j][triangles[j][i].p[1]]);
                    glVertex3f(points[j][triangles[j][i].p[1]][0], points[j][triangles[j][i].p[1]][1], points[j][triangles[j][i].p[1]][2]);
                normalizeNormal(normals[j][triangles[j][i].p[2]]);
                    glVertex3f(points[j][triangles[j][i].p[2]][0], points[j][triangles[j][i].p[2]][1], points[j][triangles[j][i].p[2]][2]);
                glEnd();
            }
        }
	};


	void drawNormal() {
        int i, j;
        int pointCount = (m_segmentsX + 1) * (m_segmentsY + 1);
        calculatePoints();
        for (j = 0; j < 6; j++) {
            for (i = 0; i < pointCount; i++) {
                glBegin(GL_LINES);
                    glVertex3f(points[j][i][0], points[j][i][1], points[j][i][2]);
                    Point endpoint = (normals[j][i] * .1) + points[j][i];
                    glVertex3f(endpoint[0], endpoint[1], endpoint[2]);
                glEnd();
            }
        }
	};


private:
    void calculatePoints () {
        if(m_redraw)
        {
            // destroy old arrays
            
            int i, j, k;
            float lenx, leny;
            float initX = -.5, initY = .5;

            // create new arrays
            int pointCount = (m_segmentsX + 1) * (m_segmentsY + 1);

            // change 6 to msegmentsX or Y depending on shape
            points = new Point*[6];
            triangles = new Face*[6];
            normals = new Vector*[6];

            for (i = 0; i < 6; i++) {
                points[i] = new Point[pointCount];
                triangles[i] = new Face[m_segmentsX * m_segmentsY * 2];
                normals[i] = new Vector[pointCount];
            }

            lenx = 1. / m_segmentsX;
            leny = 1. / m_segmentsY;
            // initializing points
            // need to do top and bottom faces
            Vector* transVector;
            for (k = 0; k < 6; k++) {
                for (j =  0; j <= m_segmentsY; j++) {
                    for (i = 0; i <= m_segmentsX; i++) {
                        points[k][(m_segmentsX + 1) * j + i][0] = i * lenx + initX;
                        points[k][(m_segmentsX + 1) * j + i][1] = initY - (j * leny);
                        points[k][(m_segmentsX + 1) * j + i][2] = 0;

                        normals[k][(m_segmentsX + 1) * j + i][0] = 0;
                        normals[k][(m_segmentsX + 1) * j + i][1] = 0;
                        normals[k][(m_segmentsX + 1) * j + i][2] = 1;

                        if (k == 4) {
                            transVector = new Vector(0, .5, 0);
                            points[k][(m_segmentsX + 1) * j + i] = rotX_mat(3 * PI / 2) * points[k][(m_segmentsX + 1) * j + i];
                            points[k][(m_segmentsX + 1) * j + i] = trans_mat(*transVector) * points[k][(m_segmentsX + 1) * j + i];

                            normals[k][(m_segmentsX + 1) * j + i] = rotX_mat(3 * PI / 2) * normals[k][(m_segmentsX + 1) * j + i];
                        }
                        else if (k == 5) {
                            transVector = new Vector(0, -.5, 0);
                            points[k][(m_segmentsX + 1) * j + i] = rotX_mat(PI / 2) * points[k][(m_segmentsX + 1) * j + i];
                            points[k][(m_segmentsX + 1) * j + i] = trans_mat(*transVector) * points[k][(m_segmentsX + 1) * j + i];

                            normals[k][(m_segmentsX + 1) * j + i] = rotX_mat(PI / 2) * normals[k][(m_segmentsX + 1) * j + i];
                        }
                        else {
                            transVector = new Vector(.5 * cos((PI * k / 2) - (PI / 2)), 0, .5 * sin((PI * k / 2) + (PI / 2)));
                            points[k][(m_segmentsX + 1) * j + i] = rotY_mat(PI * k /2) * points[k][(m_segmentsX + 1) * j + i];
                            points[k][(m_segmentsX + 1) * j + i] = trans_mat(*transVector) * points[k][(m_segmentsX + 1) * j + i];

                            normals[k][(m_segmentsX + 1) * j + i] = rotY_mat(PI * k /2) * normals[k][(m_segmentsX + 1) * j + i];
                        }
                    }
                }
            }

            // initializing faces
            for (k = 0; k < 6; k++) {
                for (j =  0; j < m_segmentsY; j++) {
                    for (i = 0; i <= m_segmentsX; i++) {
                        // only face to right of point
                        if (i == 0) {
                            triangles[k][m_segmentsX * j * 2 + i * 2].p[0] = (m_segmentsX + 1) * j + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2].p[1] = (m_segmentsX + 1) * (j + 1) + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2].p[2] = (m_segmentsX + 1) * j + (i + 1);
                        }
                        // only face to left of point
                        else if (i == m_segmentsX) {
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[0] = (m_segmentsX + 1) * j + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[1] = (m_segmentsX + 1) * (j + 1) + (i - 1);
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[2] = (m_segmentsX + 1) * (j + 1) + i;   
                        }
                        // face to left then face to right
                        else {
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[0] = (m_segmentsX + 1) * j + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[1] = (m_segmentsX + 1) * (j + 1) + (i - 1);
                            triangles[k][m_segmentsX * j * 2 + i * 2 - 1].p[2] = (m_segmentsX + 1) * (j + 1) + i;

                            triangles[k][m_segmentsX * j * 2 + i * 2].p[0] = (m_segmentsX + 1) * j + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2].p[1] = (m_segmentsX + 1) * (j + 1) + i;
                            triangles[k][m_segmentsX * j * 2 + i * 2].p[2] = (m_segmentsX + 1) * j + (i + 1);   
                        }                    
                    }
                }           
            }
            m_redraw = false;
        }
    };
};

#endif
