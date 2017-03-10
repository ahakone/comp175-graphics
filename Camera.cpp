#include "Camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::~Camera() {
}

#define GET_X unpack()[0]
#define GET_Y unpack()[1]
#define GET_Z unpack()[2]

double vectorSize(Vector &v) {
    double *p = v.unpack();
    double size = sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2]));
    return size;
}

void Camera::setUVW() {
    double look_len = vectorSize(this->look);
    this->w = (-1*this->look) / look_len;
    Vector UPxW = cross(this->up, this->w);
    double UPxWsize = vectorSize(UPxW);
    this->u = UPxW / UPxWsize;
    this->v = cross(this->w, this->u);
}

/* "Finally, while not all the functions are called explicitly
   in the program, you are still required to fill in all
   all the empty functions."
   */
void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    this->eye = eye;
    this->focus = focus;
    this->look = focus - eye;
    this->up = up;

    this->look.normalize();
    this->up.normalize();

    setUVW();
}


// eye = position in space where the camera is (worldview coords)
// look = direction the camera is facing
// up vector = which direction is up
void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    this->eye = eye;
    this->look = look;
    this->focus = Point(look.GET_X, look.GET_Y, look.GET_Z);
    this->up = up;

    this->look.normalize();
    this->up.normalize();

    setUVW();
}

// TODO: reset the rest of the values
void Camera::Reset() {
    this->nearPlane = .001;
    this->farPlane = 30;
}


Matrix Camera::GetProjectionMatrix() {
    if (GetNearPlane() != prev_nearPlane || GetFarPlane() != prev_farPlane ||
            GetViewAngle() != prev_viewAngle || screenWidth != prev_screenWidth 
            || screenHeight != prev_screenHeight ) {
        std::cout << "projection matrix changed" << std::endl;
        double near = GetNearPlane();
        double far = GetFarPlane();

        double theta_h = GetViewAngle();

        double c = -near / far;
        Matrix Suvw = Matrix(
                (1.0 / (tan(theta_h / 2.0) * far)) / GetScreenWidthRatio(), 0, 0, 0,
                0, 1.0 / (tan(theta_h / 2.0) * far), 0, 0,
                0, 0, (1.0 / far), 0,
                0, 0, 0, 1
                );
        Matrix Mpp = Matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, -1.0 / (c+1), c / (c+1),
                0, 0, -1, 0
                );
        prev_nearPlane = near;
        prev_farPlane = far;
        prev_viewAngle = theta_h;
        prev_screenWidth = screenWidth;
        prev_screenHeight = screenHeight;
        projectionMatrix = Mpp * Suvw;
    }
    return projectionMatrix;
}


void Camera::SetViewAngle (double viewAngle) {
    viewAngle *= PI / 180.0;
    this->viewAngle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    this->nearPlane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    this->farPlane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

Matrix Camera::GetRxyz2uvw() {
    double *u = (this->u).unpack();
    double *v = (this->v).unpack();
    double *w = (this->w).unpack();
    Matrix Rxyz2uvw = Matrix(
            u[0], u[1], u[2], 0,
            v[0], v[1], v[2], 0,
            w[0], w[1], w[2], 0,
            0,     0,    0, 1
            );
    return Rxyz2uvw;
}

Matrix Camera::GetModelViewMatrix() {
    if (prev_u != u || prev_v != v || prev_w != w
            || prev_eye != eye) {
        double *u = (this->u).unpack();
        double *v = (this->v).unpack();
        double *w = (this->w).unpack();

        prev_u = this->u;
        prev_v = this->v;
        prev_w = this->w;

        Matrix Rxyz2uvw = GetRxyz2uvw();
        Point e = GetEyePoint();
        prev_eye = e;
        Matrix Txyz = Matrix(
                1, 0, 0, -e.GET_X,
                0, 1, 0, -e.GET_Y,
                0, 0, 1, -e.GET_Z,
                0, 0, 0, 1
                );
        modelViewMatrix = Rxyz2uvw * Txyz;
    }
    return modelViewMatrix;
}

// a = the axis about which we are rotating
void Camera::RotateAroundAxis(Vector a, double degrees) {
    double angle = degrees * PI / 180;

    Matrix full_rot = rot_mat(a, angle);
    this->look = full_rot * this->look;
    this->up = full_rot * this->up;

    Orient(this->eye, this->look, this->up);
}

// Pitch (Up/down)
void Camera::RotateU(double angle) {
    RotateAroundAxis(this->u, angle);
}

// Yaw (Left/right)
void Camera::RotateV(double angle) {
    RotateAroundAxis(this->v, angle);
}

// Roll (CW or CCW)
void Camera::RotateW(double angle) {
    RotateAroundAxis(this->w, -angle);
}

void Camera::Translate(const Vector &v) {
    Matrix trans = trans_mat(v);
    this->eye = trans * this->eye;
}

// TODO: Should this change p (aka by "Point &p")? Should it actually return it?
void Camera::Rotate(Point p, Vector axis, double degrees) {
    double angle = degrees * PI / 180;
    Vector a = axis;
    Matrix M_1 = rotY_mat(-atan(a.GET_Z / a.GET_X));
    Matrix M_2 = rotZ_mat(-atan(a.GET_Y /
                sqrt(
                    a.GET_X * a.GET_X + 
                    a.GET_Z * a.GET_Z
                    )));
    Matrix M_3 = rotX_mat(angle);
    Matrix M_1_inv = inv_rotY_mat(-atan(a.GET_Z / a.GET_X));
    Matrix M_2_inv = inv_rotZ_mat(-atan(a.GET_Y /
                sqrt(
                    a.GET_X * a.GET_X + 
                    a.GET_Z * a.GET_Z
                    )));
    Matrix full_rot = M_1_inv * M_2_inv * M_3 * M_2 * M_1;
    p = full_rot * p;
}


Point Camera::GetEyePoint() {
    return this->eye;
}

Vector Camera::GetLookVector() {
    return this->look;
}

Vector Camera::GetUpVector() {
    return this->up;
}

// theta_h
double Camera::GetViewAngle() {
    return this->viewAngle;
}

double Camera::GetNearPlane() {
    return this->nearPlane;
}

double Camera::GetFarPlane() {
    return this->farPlane;
}

int Camera::GetScreenWidth() {
    return this->screenWidth;
}

int Camera::GetScreenHeight() {
    return this->screenHeight;
}

double Camera::GetFilmPlanDepth() {
    return GetNearPlane();
}

double Camera::GetScreenWidthRatio() {
    return ((float)this->screenWidth / (float)this->screenHeight);
}
