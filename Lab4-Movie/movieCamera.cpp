/*  =================== File Information =================
    File Name: movieCamera.cpp
    Description:
    Author: 

    Purpose:
    Examples:


    Relevant keywords and OpenGL calls to (understand/complete/create cool effects) for this lab:
    -glFrustrum
    -glDepthRange
    -glMultMatrixf
    -glTranslate/glRotate/glScale
    -glOrtho
    -OpenGL field of view


    ===================================================== */
#include <math.h>
#include "movieCamera.h"

// Static initializers
float movieCamera::rotate_around = 0;


/*  ===============================================
    Desc: Default Constructor
    Precondition: Setup your matrix to the identity
    Postcondition:
    =============================================== */ 
movieCamera::movieCamera(){
  myCamera = new Camera();
}

/*  ===============================================
    Desc: Default Destructor
    Precondition:
    Postcondition:
    =============================================== */ 
movieCamera::~movieCamera(){

}

/*
  Implement the clipping plane
*/
/*  ===============================================
    Desc: Zoom in on an object.  Optionally clip out objects that are 'far' away from this object
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::closeUp(float your_x, float your_y, float your_z, float near, float far){
  glRotatef(10, 0, 1, 0); // rotate right
  glTranslatef(-your_x, -your_y - .5, -your_z - 3);


  // Point* currentEye = new Point(myCamera->GetEyePoint());
  // Vector* zoomVector = new Vector(your_x - currentEye->at(0), your_y - currentEye->at(1), your_z - currentEye->at(2));
  // myCamera->SetNearPlane(near);
  // myCamera->SetFarPlane(far);
  // myCamera->Translate(*zoomVector);

  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // Matrix projection = myCamera->GetProjectionMatrix();
  // glMultMatrixd(projection.unpack());

  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // Matrix modelView = myCamera->GetModelViewMatrix();
  // glMultMatrixd(modelView.unpack());


}

/*
  Change the field of view
*/
/*  ===============================================
    Desc: Change the viewing angle of the camera lense
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::wideAngle(float _fov){

}

/*
  Set the camera to an orthogonal view
*/
/*  ===============================================
    Desc: Remove the depth attribute from the camera.
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::orthogonal(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal){
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  glOrtho(left, right, bottom, top, nearVal, farVal);
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
   glRotatef(-90, 1, 0, 0); // rotate down
  //glRotatef(90, 0, 1, 0); // rotate right
  //  glRotatef(90, 0, 0, 1);
  glTranslatef(3, -6, 6);  
  
  
  // Point* eye = new Point(0, 2, 10);
  // Vector* look = new Vector(0, 0, -1);
  // Vector* up = new Vector(0, 1, 0);
  // myCamera->Orient(*eye, *look, *up);
  // myCamera->SetScreenSize((int)(right - left), (int)(top - bottom)); 
  // myCamera->SetNearPlane(0);
  // myCamera->SetFarPlane(20); 

  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // Matrix projection = myCamera->GetProjectionMatrixOrthographic();
  // glMultMatrixd(projection.unpack());

  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // Matrix modelView = myCamera->GetModelViewMatrix();
  // glMultMatrixd(modelView.unpack());

}

/*  ===============================================
    Desc: Render the scene in a normal perspective field of view.
    The parameters for this function match that of the OpenGL Utility library.
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){

}
/*  ===============================================
    Desc: Set the camera to a 'look at' a target from a specified position and follow it.
    your_x would be your position, and the target could be a moving or non-moving target
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::follow(float your_x, float your_y, float your_z,
                         float target_x, float target_y, float target_z,
                         float up_x, float up_y, float up_z){
  // Point eye(your_x, your_y, your_z);
  // //  Vector look(your_x - target_x, your_y - target_y, your_z - target_z);
  // Vector look(target_x - your_x, target_y - your_y, target_z - your_z);
  // Vector up(up_x, up_y, up_z);
  // myCamera->Orient(eye, look, up);
  
  
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // glMultMatrixd(myCamera->GetModelViewMatrix().unpack());

  //rotate y
  //rotate y and z
    glRotatef(90, 0, 1, 0);
  //glRotatef(10, 0, 1, 1);
   glTranslatef(-your_x - 2.5, -your_y - 3, -your_z);


}
/*  ===============================================
    Desc: Spin around a point in space at a distance(i.e. "radius")
    Precondition:
    Postcondition:
    =============================================== */ 
void movieCamera::spinAroundPoint(float your_x, float your_y, float your_z, float distance){
  rotate_around += 1;
  float theta = rotate_around / 360.;
  // Point eye(distance * cos(theta) + your_x, your_y, distance * sin(theta) + your_z);

  // Vector look(your_x - (distance * cos(theta) + your_x), 
  //             your_y - 1,
  //             your_z - (distance * sin(theta) + your_z));
  // Vector up(0, 1, 0);
  // myCamera->Orient(eye, look, up);
  
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // glMultMatrixd(myCamera->GetModelViewMatrix().unpack());
  glTranslatef(your_x, your_y, your_z);
  glRotatef(rotate_around, 0, 1, 0);
   glTranslatef(distance * cos(theta), 0, distance * sin(theta));

  if(rotate_around>360){ rotate_around = 0; }
}
