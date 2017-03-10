#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
    // points as coordinates
    Point** points;
    // normals
    Vector** normals;
    // triangles array of pointers to Points
    Face** triangles;

	Cylinder() {};
	~Cylinder() {};

	void draw() {
        calculatePoints();

        int i, j;
        int sideCount = m_segmentsX;
        int numFaces = 2 * m_segmentsY;
        for (j = 0; j < sideCount; j++) {
            // draw side triangles
            for (i = 0; i < numFaces; i++) {
                glBegin(GL_TRIANGLES);
                    normalizeNormal(normals[j][triangles[j][i].p[0]]);
                    glVertex3f(
                            points[j][triangles[j][i].p[0]][0], 
                            points[j][triangles[j][i].p[0]][1], 
                            points[j][triangles[j][i].p[0]][2]);
                    normalizeNormal(normals[j][triangles[j][i].p[1]]);
                    glVertex3f(
                            points[j][triangles[j][i].p[1]][0], 
                            points[j][triangles[j][i].p[1]][1], 
                            points[j][triangles[j][i].p[1]][2]);
                    normalizeNormal(normals[j][triangles[j][i].p[2]]);
                    glVertex3f(
                            points[j][triangles[j][i].p[2]][0], 
                            points[j][triangles[j][i].p[2]][1], 
                            points[j][triangles[j][i].p[2]][2]);
                glEnd();
            }
            // draw top triangles
            glBegin(GL_TRIANGLES);
                normalizeNormal(
                        normals[sideCount][triangles[sideCount][j].p[0]]);
                glVertex3f(
                        points[sideCount][triangles[sideCount][j].p[0]][0],
                        points[sideCount][triangles[sideCount][j].p[0]][1],
                        points[sideCount][triangles[sideCount][j].p[0]][2]);
                normalizeNormal(
                        normals[sideCount][triangles[sideCount][j].p[1]]);
                glVertex3f(
                        points[sideCount][triangles[sideCount][j].p[1]][0],
                        points[sideCount][triangles[sideCount][j].p[1]][1],
                        points[sideCount][triangles[sideCount][j].p[1]][2]);
                normalizeNormal(
                        normals[sideCount][triangles[sideCount][j].p[2]]);
                glVertex3f(
                        points[sideCount][triangles[sideCount][j].p[2]][0], 
                        points[sideCount][triangles[sideCount][j].p[2]][1], 
                        points[sideCount][triangles[sideCount][j].p[2]][2]);
            glEnd();
            // draw bottom triangles
            glBegin(GL_TRIANGLES);
                normalizeNormal(
                        normals[sideCount + 1][triangles[sideCount][j].p[0]]);
                glVertex3f(
                        points[sideCount + 1][triangles[sideCount][j].p[0]][0],
                        points[sideCount + 1][triangles[sideCount][j].p[0]][1],
                        points[sideCount + 1][triangles[sideCount][j].p[0]][2]);
                normalizeNormal(
                        normals[sideCount + 1][triangles[sideCount][j].p[1]]);
                glVertex3f(
                        points[sideCount + 1][triangles[sideCount][j].p[1]][0],
                        points[sideCount + 1][triangles[sideCount][j].p[1]][1],
                        points[sideCount + 1][triangles[sideCount][j].p[1]][2]);
                normalizeNormal(
                        normals[sideCount + 1][triangles[sideCount][j].p[2]]);
                glVertex3f(
                        points[sideCount + 1][triangles[sideCount][j].p[2]][0], 
                        points[sideCount + 1][triangles[sideCount][j].p[2]][1], 
                        points[sideCount + 1][triangles[sideCount][j].p[2]][2]);
            glEnd();
        }
	};

	void drawNormal() {
        calculatePoints();

        int i, j;
        int sideCount = m_segmentsX;
        int sidePointCount = 2 * (m_segmentsY + 1);
        Point endpoint;
        for (j = 0; j < sideCount; j++) {
            // draw side normals
            for (i = 0; i < sidePointCount; i++) {
                glBegin(GL_LINES);
                    glVertex3f(
                            points[j][i][0], 
                            points[j][i][1], 
                            points[j][i][2]);
                    endpoint = (normals[j][i] * .1) + points[j][i];
                    glVertex3f(
                            endpoint[0], 
                            endpoint[1], 
                            endpoint[2]);
                glEnd();
            }
        }
        for (j = 0; j <= sideCount + 1; j++) {
            // draw top normals
            glBegin(GL_LINES);
                glVertex3f(
                        points[sideCount][j][0], 
                        points[sideCount][j][1], 
                        points[sideCount][j][2]);
                endpoint = 
                    (normals[sideCount][j] * .1) + 
                    points[sideCount][j];
                glVertex3f(
                        endpoint[0], 
                        endpoint[1], 
                        endpoint[2]);
            glEnd();
            // draw bottom normals
            glBegin(GL_LINES);
                glVertex3f(
                        points[sideCount + 1][j][0], 
                        points[sideCount + 1][j][1], 
                        points[sideCount + 1][j][2]);
                endpoint = 
                    (normals[sideCount + 1][j] * .1) + 
                    points[sideCount + 1][j];
                glVertex3f(
                        endpoint[0], 
                        endpoint[1], 
                        endpoint[2]);
            glEnd();
        }
	};

    void calculatePoints() {
        if(m_redraw)
        {
            int i, j, k;
            float initY;
            float lenY;
            float segAngle;

            int sideCount = m_segmentsX;
            int sidePointCount = 2 * (m_segmentsY + 1);

            segAngle = 2 * PI / sideCount;

            // length of triangle segements in a single face
            initY = 0.5;
            lenY = 1. / m_segmentsY;

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
            
            // create new arrays
            // additional 2 faces for the top and bottom of the cylinder
            points = new Point*[sideCount + 2];
            normals = new Vector*[sideCount + 2];
            triangles = new Face*[sideCount + 2];

            // init arrays for side points 
            for (i = 0; i < sideCount; i++) {
                points[i] = new Point[sidePointCount];
                normals[i] = new Vector[sidePointCount];
                triangles[i] = new Face[2 * m_segmentsY];
            }
            // init arrays for top/bottom points
            for (i = 0; i < 2; i++) {
                // additional 2 points for: 
                // + 1 for ending point that is the same as the start point
                // + 1 for origin point in center of top and bottom 
                points[sideCount + i] = new Point[m_segmentsX + 2];
                normals[sideCount + i] = new Vector[m_segmentsX + 2];
                triangles[sideCount + i] = new Face[m_segmentsX];
            }

            // initialize points for each side
            Vector* transVector;
            for ( k = 0; k < sideCount; k++ ){
                for ( i = 0; i < 2; i++ ){
                    for ( j = 0; j <= m_segmentsY; j++ ){
                        // side points
                        points[k][( 2 * j ) + i][0] = 
                            0.5 * cos((segAngle * i) + (k * segAngle)); 
                        points[k][( 2 * j ) + i][1] =
                            initY - ( lenY * j ); 
                        points[k][( 2 * j ) + i][2] = 
                            0.5 * sin(segAngle * i + (k * segAngle));  

                        normals[k][( 2 * j ) + i][0] = 
                            0.5 * cos((segAngle * i) + (k * segAngle)); 
                        normals[k][( 2 * j ) + i][1] = 0;
                        normals[k][( 2 * j ) + i][2] =
                            0.5 * sin(segAngle * i + (k * segAngle));  
                    }
                } 
            }
            for ( k = 0; k <= sideCount; k++ ){
                // top points
                points[sideCount][k][0] = 
                    0.5 * cos(k * segAngle); 
                points[sideCount][k][1] = 0;
                points[sideCount][k][2] = 
                    0.5 * sin(k * segAngle);  
                transVector = new Vector(0, 0.5, 0);
                points[sideCount][k] = 
                    trans_mat(*transVector) * 
                    points[sideCount][k]; 
                normals[sideCount][k][0] = 0;
                normals[sideCount][k][1] = 0.5;
                normals[sideCount][k][2] = 0;
                // bottom points
                points[sideCount + 1][k][0] = 
                    0.5 * cos(k * segAngle); 
                points[sideCount + 1][k][1] = 0;
                points[sideCount + 1][k][2] = 
                    0.5 * sin(k * segAngle);  
                transVector = new Vector(0, -0.5, 0);
                points[sideCount + 1][k] = 
                    trans_mat(*transVector) * 
                    points[sideCount + 1][k]; 
                normals[sideCount + 1][k][0] = 0;
                normals[sideCount + 1][k][1] = -0.5;
                normals[sideCount + 1][k][2] = 0;
            }
            // add origin points for top/bottom
            points[sideCount][m_segmentsX + 1][0] = 0; 
            points[sideCount][m_segmentsX + 1][1] = 0;  
            points[sideCount][m_segmentsX + 1][2] = 0; 
            transVector = new Vector(0, 0.5, 0);
            points[sideCount][m_segmentsX + 1] = 
                trans_mat(*transVector) * 
                points[sideCount][m_segmentsX + 1];
            normals[sideCount][m_segmentsX + 1][0] = 0;
            normals[sideCount][m_segmentsX + 1][1] = 0.5;
            normals[sideCount][m_segmentsX + 1][2] = 0;

            points[sideCount + 1][m_segmentsX + 1][0] = 0; 
            points[sideCount + 1][m_segmentsX + 1][1] = 0;  
            points[sideCount + 1][m_segmentsX + 1][2] = 0; 
            transVector = new Vector(0, -0.5, 0);
            points[sideCount + 1][m_segmentsX + 1] = 
                trans_mat(*transVector) * 
                points[sideCount + 1][m_segmentsX + 1];
            normals[sideCount + 1][m_segmentsX + 1][0] = 0;
            normals[sideCount + 1][m_segmentsX + 1][1] = -0.5;
            normals[sideCount + 1][m_segmentsX + 1][2] = 0;

            // group triangles into side faces
            for ( k = 0; k < sideCount; k++ ){
                for ( j = 0; j < m_segmentsY; j++ ){
                    // left triangle
                    triangles[k][( 2 * j )].p[0] = 
                        ( 2 * j );
                    triangles[k][( 2 * j )].p[1] = 
                        ( 2 * (j + 1));
                    triangles[k][( 2 * j )].p[2] = 
                        ( 2 * (j + 1)) + 1;
                    // right triangle
                    triangles[k][( 2 * j ) + 1].p[0] = 
                        ( 2 * j ) + 1;
                    triangles[k][( 2 * j ) + 1].p[1] = 
                        ( 2 * j );
                    triangles[k][( 2 * j ) + 1].p[2] = 
                        ( 2 * (j + 1)) + 1;
                }
            }
            // group triangles into top/bottom faces
            for ( k = 0; k < 2; k++ ){
                for ( i = 0; i < m_segmentsX; i++ ){
                    // origin point
                    triangles[sideCount + k][i].p[0] = m_segmentsX + 1;
                    triangles[sideCount + k][i].p[1] = i;
                    triangles[sideCount + k][i].p[2] = i + 1;
                }
            }

            m_redraw = false;
        }
    };

};
#endif
