#ifndef TORUS_H
#define TORUS_H

#include "Shape.h"

class Torus : public Shape {
public:
    // points as coordinates
    Point** points;
    // normals
    Vector** normals;
    // triangles array of pointers to Points
    Face** triangles;

    // default variables
    static const float unitLen = 1.0;
    static const float radius = 0.5;

    // variables updated in calculatePoints
    int sideCount;      // m_segmentsX;
    int sidePointCount; // 2 * (m_segmentsY + 1);
    int sideTriCount;   // sidePointCount - 2;

	Torus() {};
	~Torus() {};

	void draw() {
        calculatePoints();

        int i, j;
        for (j = 0; j < sideCount; j++) {
            for (i = 0; i < sideTriCount; i++) {
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
        }
	};

	void drawNormal() {
        calculatePoints();

        int i, j;
        Point endpoint;
        for (j = 0; j < sideCount; j++) {
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
	};

    void calculatePoints() {
        if(m_redraw)
        {
            int i, j, k;

            sideCount = m_segmentsY;
            sidePointCount = 2 * (m_segmentsX + 1);
            sideTriCount = sidePointCount - 2;

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
            points = new Point*[sideCount];
            normals = new Vector*[sideCount];
            triangles = new Face*[sideCount];
            for (i = 0; i < sideCount; i++) {
                points[i] = new Point[sidePointCount];
                normals[i] = new Vector[sidePointCount];
                triangles[i] = new Face[sideTriCount];
            }

            float r = 0.15;
            float R = 0.5 - r ;

            // theta == latitude
            // phi == longitude
            float theta = 2 * PI / sideCount;
            // - 1 to account for repeat points at beginning/end
            float phi = 2 * PI / ((sidePointCount / 2) - 1);

            // initialize points for each side
            Vector* transVector;
            for ( k = 0; k < sideCount; k++ ){
                for ( j = 0; j < sidePointCount / 2; j++ ){
                    // left point
                    points[k][2 * j][0] = 
                        (R + r*cos(k * theta))*cos(j * phi);
                    points[k][2 * j][1] =
                        (R + r*cos(k * theta))*sin(j * phi);
                    points[k][2 * j][2] = 
                        r*sin(k * theta);

                    normals[k][2 * j][0] = 1;
                    normals[k][2 * j][1] = 0;
                    normals[k][2 * j][2] = 0;
                    normals[k][2 * j] = 
                        rotY_mat(-1*k * theta) *
                        normals[k][2 * j];
                    normals[k][2 * j] = 
                        rotZ_mat(j * phi) *
                        normals[k][2 * j];

                    // right point
                    points[k][( 2 * j ) + 1][0] = 
                        (R + r*cos(theta + (k * theta)))*cos(j * phi);
                    points[k][( 2 * j ) + 1][1] =
                        (R + r*cos(theta + (k * theta)))*sin(j * phi);
                    points[k][( 2 * j ) + 1][2] = 
                        r*sin(theta + (k * theta));

                    normals[k][( 2 * j ) + 1][0] = 1;
                    normals[k][( 2 * j ) + 1][1] = 0;
                    normals[k][( 2 * j ) + 1][2] = 0;
                    normals[k][( 2 * j ) + 1] = 
                        rotY_mat(-1*(k + 1) * theta) * 
                        normals[k][( 2 * j ) + 1];
                    normals[k][( 2 * j ) + 1] = 
                        rotZ_mat(j * phi) *
                        normals[k][( 2 * j ) + 1];
                }
            }

            // group triangles into side faces
            for ( k = 0; k < sideCount; k++ ){
                for ( j = 0; j < sideTriCount / 2; j++ ){
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

            m_redraw = false;
        }
    };

};
#endif
