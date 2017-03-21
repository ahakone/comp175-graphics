#include <GL/glui.h>
#include "solarSystem.h"

#define NUM_PLANETS 4
#define RAND_MAX 100

solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){
	
	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitX = 0.15;
	static float orbitSpeed = 0.2;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;

	
	float pRadius[NUM_PLANETS] = {.5f, 1.5f, 2.5f, 3.5f };
	float pSize[NUM_PLANETS] = {.5, .1, .2, .4};

	// The Sun
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glColor3f(0.96f,0.85f,0.26f);
			glutSolidSphere(1.2,10,10);
			glPushMatrix();
				glTranslatef(2.0f, 0.0f, 0.0f);
				glRotatef(rotation,0,1,0);
				glColor3f(0.00f,0.85f,0.26f);
				glutSolidSphere(pSize[0],10,10);
				glPushMatrix();
					glTranslatef(.6f, 0.0f, 0.0f);
					glRotatef(rotation,0,1,0);
					glColor3f(rand()*.01,0.85f,0.26f);
					glutSolidSphere(pSize[1],10,10);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(3.0f, 0.0f, 1.0f);
				glRotatef(rotation,0,1,0);
				glColor3f(0.00f,rand()*.01,rand()*.01);
				glutSolidSphere(pSize[2],10,10);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(4.0f, 0.0f, 1.5f);
				glRotatef(rotation,0,1,0);
				glColor3f(rand() * 0.01,0.85f,0.26f);
				glutSolidSphere(pSize[3],10,10);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(5.0f, 0.0f, 2.5f);
				glRotatef(rotation,0,1,0);
				glColor3f(0.00f,rand()*.01,rand()*.01);
				glutSolidSphere(pSize[1],10,10);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(7.0f, 0.0f, 0.0f);
				glRotatef(rotation,0,1,0);
				glColor3f(0.00f,0.85f,0.26f);
				glutSolidSphere(pSize[1],10,10);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(9.0f, 0.0f, 2.0f);
				glRotatef(rotation,0,1,0);
				glColor3f(0.00f,0.85f,0.26f);
				glutSolidSphere(pSize[1],10,10);
			glPopMatrix();
		glPopMatrix();
	// Add more planets, moons, and rings here!	


	rotation+= 0.05;
	orbitX+=orbitSpeed;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}
