#ifndef SPECIAL1_H
#define SPECIAL1_H

#include "Shape.h"
// r = .5sin(5theta)
// dr/dtheta = 5cos(5theta)/2
// dy/dx = (dr/dtheta)sin(theta) + rcos(theta) 
//         -----------------------------------
//         (dr/dtheta)cos(theta) - rsin(theta) 

class Special1 : public Shape {
public:
    // points as coordinates
    Point*** points;
    // normals
    Vector*** normals;
    // triangles array of pointers to Points
    Face*** triangles;

    // default variables
    static const float unitLen = 1.0;
    static const float radius = 0.5;

    // variables updated in calculatePoints
    int num_leaves;
    int pointCount;
    int faceCount;
	Special1() {};
	~Special1() {};

    // m_segmentsX is z division
    // m_segmentsY is division of total theta
	void draw() {
        calculatePoints();

        int i, j, k;
        for (k = 0; k < num_leaves; k++) {
            for (j = 0; j < m_segmentsX; j++) {
                for (i = 0; i < faceCount; i++) {
                    glPointSize(5); 
                    glBegin(GL_TRIANGLES);
                        normalizeNormal(normals[k][j][triangles[k][j][i].p[0]]);
                        glVertex3f(points[k][j][triangles[k][j][i].p[0]][0],
                                    points[k][j][triangles[k][j][i].p[0]][1],
                                    points[k][j][triangles[k][j][i].p[0]][2]);

                        normalizeNormal(normals[k][j][triangles[k][j][i].p[1]]);
                        glVertex3f(points[k][j][triangles[k][j][i].p[1]][0],
                                    points[k][j][triangles[k][j][i].p[1]][1],
                                    points[k][j][triangles[k][j][i].p[1]][2]);
                        
                        normalizeNormal(normals[k][j][triangles[k][j][i].p[2]]);
                        glVertex3f(points[k][j][triangles[k][j][i].p[2]][0],
                                    points[k][j][triangles[k][j][i].p[2]][1],
                                    points[k][j][triangles[k][j][i].p[2]][2]);
                    glEnd();
                }
            }
        }
	};

	void drawNormal() {
        calculatePoints();

        int i, j, k;
        for (k = 0; k < num_leaves; k++) {
            for (j = 0; j < m_segmentsX; j++) {
                for (i = 0; i < pointCount; i++) {
                    glBegin(GL_LINES);
                        glVertex3f(points[k][j][i][0],
                                    points[k][j][i][1],
                                    points[k][j][i][2]);
                        Point endpoint = (normals[k][j][i] * .05) + points[k][j][i];
                        glVertex3f(endpoint[0], endpoint[1], endpoint[2]);
                    glEnd();
                }
            }
        }
	};

    void calculatePoints() {
        if(m_redraw)
        {
            int i, j, k;
            num_leaves = 5;
            pointCount = 2 * m_segmentsY;

            float start_theta = 2 * PI / 5.;
            float total_theta = PI / (num_leaves * 2.);
            float rotY = 2 * PI / float(m_segmentsX);
            float rotLeaf = 2 * PI / float(num_leaves);
            // degree division
            float theta = total_theta / float(m_segmentsY);
            faceCount = 2 * m_segmentsY - 2;
            float zMax = .2;

            // destroy old arrays
            /*
            if( points != NULL || triangles != NULL || normals != NULL) {
                for (i = 0; i < sideCount; i++) {
                    delete points[i];
                    delete triangles[i];
                    delete normals[i];
                }
                delete points;
                delete triangles;
                delete normals;
            }
            */
            
            // create new arrays and initialize
            points = new Point**[num_leaves];
            normals = new Vector**[num_leaves];
            triangles = new Face**[num_leaves];
            for (i = 0; i < num_leaves; i++) {
                points[i] = new Point*[m_segmentsX];
                normals[i] = new Vector*[m_segmentsX];
                triangles[i] = new Face*[m_segmentsX];

                for (j = 0; j < m_segmentsX; j++) {
                    points[i][j] = new Point[pointCount];
                    normals[i][j] = new Vector[pointCount];
                    triangles[i][j] = new Face[faceCount];
                }
            }

            Vector* transVector;
            for (i = 0; i < num_leaves; i++) {
                for (j = 0; j < m_segmentsX; j++) {
                    // bottom point
                    points[i][j][0][0] = 0;
                    points[i][j][0][1] = 0;
                    points[i][j][0] = rotZ_mat(i * rotLeaf) * points[i][j][0];
                    points[i][j][0][2] = 0;
                    normals[i][j][0][0] = 0;
                    normals[i][j][0][1] = 0;
                    normals[i][j][0][2] = 0;

                    for (k = 1; k < m_segmentsY; k++) {
                        // right point
                        points[i][j][2 * k - 1][0] = .5 * sin(num_leaves * (k * theta + start_theta)) * cos(k * theta + start_theta);
                        points[i][j][2 * k - 1][1] = .5 * sin(num_leaves * (k * theta + start_theta)) * sin(k * theta + start_theta);
                        points[i][j][2 * k - 1][2] = 0;

                        normals[i][j][2 * k - 1][0] = (.5 * 5 * cos(5 * (k * theta + start_theta))) * 
                                                        sin(k * theta + start_theta) + 
                                                        .5 * sin(num_leaves * (k * theta + start_theta)) *
                                                        cos(k * theta + start_theta);
                        normals[i][j][2 * k - 1][1] = -((.5 * 5 * cos(5 * (k * theta + start_theta))) * 
                                                        cos(k * theta + start_theta) -
                                                        .5 * sin(num_leaves * (k * theta + start_theta)) *
                                                        sin(k * theta + start_theta));
                        normals[i][j][2 * k - 1][2] = 0;
                        normals[i][j][2 * k - 1].normalize();

                        points[i][j][2 * k - 1] = rotY_mat(j * -rotY) * points[i][j][2 * k - 1];  
                        normals[i][j][2 * k - 1] = rotY_mat(j * -rotY) * normals[i][j][2 * k - 1];  

                        // left point
                        points[i][j][2 * k] = rotY_mat(-rotY) * points[i][j][2 * k - 1];
                        normals[i][j][2 * k] = rotY_mat(-rotY) * normals[i][j][2 * k - 1];

                        // rotating by leaves
                        points[i][j][2 * k - 1] = rotZ_mat(i * rotLeaf) * points[i][j][2 * k - 1];
                        points[i][j][2 * k] = rotZ_mat(i * rotLeaf) * points[i][j][2 * k];
                        normals[i][j][2 * k - 1] = rotZ_mat(i * rotLeaf) * normals[i][j][2 * k - 1];
                        normals[i][j][2 * k] = rotZ_mat(i * rotLeaf) * normals[i][j][2 * k];
                    }

                    // top point
                    points[i][j][pointCount - 1][0] = 0;
                    points[i][j][pointCount - 1][1] = .5;
                    points[i][j][pointCount - 1] = rotZ_mat(i * rotLeaf) * points[i][j][pointCount - 1];
                    points[i][j][pointCount - 1][2] = 0;
                    normals[i][j][pointCount - 1][0] = 0;
                    normals[i][j][pointCount - 1][1] = 1;
                    normals[i][j][pointCount - 1] = rotZ_mat(i * rotLeaf) * normals[i][j][pointCount - 1];
                    normals[i][j][pointCount - 1][2] = 0;
                }
            }

            for (i = 0; i < num_leaves; i++) {
                for (j = 0; j < m_segmentsX; j++) {
                    // bottom triangle
                    triangles[i][j][0].p[0] = 0;
                    triangles[i][j][0].p[1] = 1;
                    triangles[i][j][0].p[2] = 2;

                    for (k = 1; k < m_segmentsY - 1; k++) {
                        // right triangle
                        triangles[i][j][2 * k - 1].p[0] = 2 * k - 1;
                        triangles[i][j][2 * k - 1].p[1] = 2 * k + 1;
                        triangles[i][j][2 * k - 1].p[2] = 2 * k;

                        // left triangle
                        triangles[i][j][2 * k].p[0] = 2 * k;
                        triangles[i][j][2 * k].p[1] = 2 * k + 1;
                        triangles[i][j][2 * k].p[2] = 2 * (k + 1);
                    }
                    // top triangle
                    triangles[i][j][faceCount - 1].p[0] = pointCount - 1;
                    triangles[i][j][faceCount - 1].p[1] = pointCount - 3;
                    triangles[i][j][faceCount - 1].p[2] = pointCount - 2;
                }
            }
            m_redraw = false;
        }
    };

};
#endif
