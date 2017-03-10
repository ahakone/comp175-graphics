#ifndef CONE_H
#define CONE_H

#include "Shape.h"
#include <typeinfo>

class Cone : public Shape {
public:
    // points as coordinates
    Point** points;
    // normals
    Vector** normals;
    // triangles array of pointers to Points
    Face** triangles;

	Cone() {};
	~Cone() {};

	void draw() {
        calculatePoints();

        int i, j;
        int sideCount = m_segmentsX;
        int numFaces = 2 * m_segmentsY - 1;
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
            // draw bottom triangles
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
        }
	};

	void drawNormal() {
        calculatePoints();

        int i, j;
        int sideCount = m_segmentsX;
        int sidePointCount = ( 2 * m_segmentsY ) + 1;
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
        for ( j = 0; j <= sideCount + 1; j++ ){
            // draw bottom normal
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
        }
	};

    void calculatePoints() {
        if(m_redraw)
        {
            int i, j, k;
            float initY, initR;
            float segAngle, segHeight, segRadius;

            int sideCount = m_segmentsX;
            int sidePointCount = ( 2 * m_segmentsY ) + 1;
            int sideFacesCount = ( 2 * m_segmentsY ) - 1;

            segAngle = 2 * PI / sideCount;
            segHeight = 1. / m_segmentsY;
            segRadius = 0.5 / m_segmentsY;

            initY = -0.5;
            initR = 0.5;

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
            // additional +1 face for the top and bottom of the cylinder
            points = new Point*[sideCount + 1];
            normals = new Vector*[sideCount + 1];
            triangles = new Face*[sideCount + 1];

            // init arrays for side points 
            for (i = 0; i < sideCount; i++) {
                points[i] = new Point[sidePointCount];
                normals[i] = new Vector[sidePointCount];
                triangles[i] = new Face[sideFacesCount];
            }
            // init array for bottom points
            // additional 2 points for: 
            // + 1 for ending point that is the same as the starting point
            // + 1 for origin point in center of top and bottom 
            points[sideCount] = new Point[m_segmentsX + 2];
            normals[sideCount] = new Vector[m_segmentsX + 2];
            triangles[sideCount] = new Face[m_segmentsX];

            // initialize points for each side
            Vector* transVector;
            for ( k = 0; k < sideCount; k++ ){
                for ( j = 0; j <= m_segmentsY; j++ ){
                    // side points for current height
                    points[k][( 2 * j )][0] = 
                        (initR - (j * segRadius)) * cos(k * segAngle); 
                    points[k][( 2 * j )][1] = 
                        initY + ( segHeight * j ); 
                    points[k][( 2 * j )][2] = 
                        (initR - (j * segRadius)) * sin(k * segAngle);  

                    // not top of cone normal
                    normals[k][ 2 * j ][0] =  2. / sqrt(5.);
                    // printf("testing sqrt %f\n", 2 / sqrt(5));
                    normals[k][ 2 * j ][1] = 1. / sqrt(5.);
                    normals[k][ 2 * j ][2] = 0;

                    normals[k][ 2 * j] = 
                        rotY_mat(k * -segAngle) * normals[k][ 2 * j ];

                    if ( j < m_segmentsY ) {
                        // top of cone has 1 vertex, do not attempt to draw a second
                        points[k][( 2 * j ) + 1][0] = 
                            (initR - (j * segRadius)) * 
                            cos(segAngle + (k * segAngle)); 
                        points[k][( 2 * j ) + 1][1] =
                            initY + ( segHeight * j ); 
                        points[k][( 2 * j ) + 1][2] = 
                            (initR - (j * segRadius)) * 
                            sin(segAngle + (k * segAngle));  

                        normals[k][( 2 * j ) + 1][0] =  2 / sqrt(5);
                        normals[k][( 2 * j ) + 1][1] = 1 / sqrt(5);
                        normals[k][( 2 * j ) + 1][2] = 0;

                        normals[k][( 2 * j ) + 1] =  rotY_mat(-segAngle + (k * -segAngle)) * normals[k][( 2 * j ) + 1];
                    }
                }
            }

            Vector* topNormal = new Vector(normals[0][sidePointCount - 1][0], 
                                            normals[0][sidePointCount - 1][1], 
                                            normals[0][sidePointCount - 1][2]);
            for (i = 1; i < sideCount; i++) {
                topNormal = new Vector(*topNormal + normals[i][sidePointCount - 1]);
            }
            topNormal->normalize();
            for (i = 0; i < sideCount; i++) {
                normals[i][sidePointCount - 1] = *topNormal;   
            }

            for ( k = 0; k <= sideCount; k++ ){
                // bottom points
                points[sideCount][k][0] = 
                    0.5 * cos(k * segAngle); 
                points[sideCount][k][1] = 0;
                points[sideCount][k][2] = 
                    0.5 * sin(k * segAngle);  
                transVector = new Vector(0, -0.5, 0);
                points[sideCount][k] = 
                    trans_mat(*transVector) * points[sideCount][k]; 

                normals[sideCount][k][0] = 0;
                normals[sideCount][k][1] = -1;
                normals[sideCount][k][2] = 0;
            }
            // add origin points for bottom
            points[sideCount][m_segmentsX + 1][0] = 0; 
            points[sideCount][m_segmentsX + 1][1] = 0;  
            points[sideCount][m_segmentsX + 1][2] = 0; 
            transVector = new Vector(0, -0.5, 0);
            points[sideCount][m_segmentsX + 1] = 
                trans_mat(*transVector) * 
                points[sideCount][m_segmentsX + 1];
            normals[sideCount][m_segmentsX + 1][0] = 0;
            normals[sideCount][m_segmentsX + 1][1] = -1;
            normals[sideCount][m_segmentsX + 1][2] = 0;

            // group triangles into side faces
            for ( k = 0; k < sideCount; k++ ){
                for ( j = 0; j < m_segmentsY; j++ ){
                    // left triangle
                    triangles[k][( 2 * j )].p[0] = 
                        ( 2 * j );
                    triangles[k][( 2 * j )].p[1] = 
                        ( 2 * j ) + 1;
                    triangles[k][( 2 * j )].p[2] = 
                        ( 2 * ( j + 1));

                    if ( j < m_segmentsY - 1 ) {
                        // right triangle
                        triangles[k][( 2 * j ) + 1].p[0] = 
                            ( 2 * j ) + 1;
                        triangles[k][( 2 * j ) + 1].p[1] = 
                            ( 2 * (j + 1) ) + 1;
                        triangles[k][( 2 * j ) + 1].p[2] = 
                            ( 2 * (j + 1) );
                    }
                }
            }
            // group triangles into bottom faces
            for ( i = 0; i < m_segmentsX; i++ ){
                // origin point
                triangles[sideCount][i].p[0] = m_segmentsX + 1;
                triangles[sideCount][i].p[1] = i;
                triangles[sideCount][i].p[2] = i + 1;
            }

            m_redraw = false;
        }
    };
};

#endif
