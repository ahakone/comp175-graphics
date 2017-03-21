#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Point** points;
    // normals
    Vector** normals;
    // triangles = nx3 array of pointers to Points
    Face** triangles;

	Sphere() {};
	~Sphere() {};

	void draw() {
		int i, j;
        // num triangles on a side
        int numFaces = 2 * m_segmentsY - 2;
		calculatePoints();
        // for (j = 0; j < 1; j++) {
        	for (j = 0; j < m_segmentsX; j++) {
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
	    }
	};

	void drawNormal() {
        int i, j;
        int pointCount = 2 * m_segmentsY;
        calculatePoints();

        for (j = 0; j < m_segmentsX; j++) {
            for (i = 0; i < pointCount; i++) {
                glBegin(GL_LINES);
                    glVertex3f(
                    	points[j][i][0], 
                    	points[j][i][1], 
                    	points[j][i][2]);
                    Point endpoint = (normals[j][i] * .1) + points[j][i];
                    glVertex3f(
                    	endpoint[0], 
                    	endpoint[1], 
                    	endpoint[2]);
                glEnd();
            }
        }
	};

private:
	void calculatePoints () {
        if(m_redraw)
        {
            int i, j;
            float theta = 2 * PI / m_segmentsX; 
            float phi = PI / m_segmentsY;
            printf("phi: %f theta: %f\n", phi, theta);
            // float initX = -.5, initY = .5;

            int pointCount = 2 * m_segmentsY;
            int faceCount = 2 * m_segmentsY - 2;

            points = new Point*[m_segmentsX];
            triangles = new Face*[m_segmentsX];
            normals = new Vector*[m_segmentsX];

            for (i = 0; i < m_segmentsX; i++) {
                points[i] = new Point[pointCount];
                triangles[i] = new Face[faceCount];
                normals[i] = new Vector[pointCount];
            }
            // printf("pointcount: %d faceCount: %d\n", pointCount, faceCount);
            
            for (i = 0; i < m_segmentsX; i++) {
            	// top point
            	points[i][0][0] = 0;
            	points[i][0][1] = 0.5;
            	points[i][0][2] = 0;
            	
            	normals[i][0][0] = 0;
            	normals[i][0][1] = 1;
            	normals[i][0][2] = 0;

            	// bottom point
            	points[i][pointCount - 1][0] = 0;
            	points[i][pointCount - 1][1] = -0.5;
            	points[i][pointCount - 1][2] = 0;

            	normals[i][pointCount - 1][0] = 0;
            	normals[i][pointCount - 1][1] = -1;
            	normals[i][pointCount - 1][2] = 0;

            	for (j = 1; j < m_segmentsY; j++) {
            		// printf("i: %d j: %d phi*i: %f\n", i, j, phi * i);
            		// left point
            		points[i][2 * j - 1][0] = .5 * sin(phi * j) * cos(-theta * i);
            		points[i][2 * j - 1][1] = .5 * cos(phi * j);
            		points[i][2 * j - 1][2] = .5 * sin(phi * j) * sin(-theta * i);

            		normals[i][2 * j - 1][0] = points[i][2 * j - 1][0] / .5;
					normals[i][2 * j - 1][1] = points[i][2 * j - 1][1] / .5;
					normals[i][2 * j - 1][2] = points[i][2 * j - 1][2] / .5;

            		// printf("leftx: %f rightx: %f\n", .5 * sin(phi * (j - 1)) * cos(theta * (i - 1)), .5 * sin(phi * (j - 1)) * cos(theta * i));
            		// right point
            		points[i][2 * j][0] = .5 * sin(phi * j) * cos(-theta * (i + 1));
            		points[i][2 * j][1] = .5 * cos(phi * j);
            		points[i][2 * j][2] = .5 * sin(phi * j) * sin(-theta * (i + 1));

            		normals[i][2 * j][0] = points[i][2 * j][0] / .5;
					normals[i][2 * j][1] = points[i][2 * j][1] / .5;
					normals[i][2 * j][2] = points[i][2 * j][2] / .5;
            	}
            }

            for (j = 0; j < m_segmentsX; j++) {
	            for (i = 0; i < pointCount; i++) {
	            	printf("side %d point %d x: %f y: %f z: %f\n", j, i, points[j][i][0], points[j][i][1], points[j][i][2]);
	            }
			}

            for (i = 0; i < m_segmentsX; i++) {
            	// top triangle
            	triangles[i][0].p[0] = 0;
            	triangles[i][0].p[1] = 1;
            	triangles[i][0].p[2] = 2;

            	// bottom triangle
            	triangles[i][faceCount - 1].p[0] = pointCount - 3;
            	triangles[i][faceCount - 1].p[1] = pointCount - 1;
            	triangles[i][faceCount - 1].p[2] = pointCount - 2;

            	for (j = 1; j < m_segmentsY - 1; j++) {
            		// left triangle
            		triangles[i][2 * j - 1].p[0] = 2 * j - 1;
					triangles[i][2 * j - 1].p[1] = 2 * j + 1;
					triangles[i][2 * j - 1].p[2] = 2 * (j + 1);

					// // right triangle
            		triangles[i][2 * j].p[0] = 2 * j - 1;
					triangles[i][2 * j].p[1] = 2 * (j + 1);
					triangles[i][2 * j].p[2] = 2 * j;
            	}
            }
        	m_redraw = false;
        }
    }
};

#endif
