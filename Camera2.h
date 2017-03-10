

#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
    public:
        Camera();
        ~Camera();
        void Orient(Point& eye, Point& focus, Vector& up);
        void Orient(Point& eye, Vector& look, Vector& up);
        void Reset();
        void SetViewAngle (double viewAngle);
        void SetNearPlane (double nearPlane);
        void SetFarPlane (double farPlane);
        void SetScreenSize (int screenWidth, int screenHeight);

        Matrix GetProjectionMatrix();
        Matrix GetModelViewMatrix();

        void RotateV(double angle);
        void RotateU(double angle);
        void RotateW(double angle);
        void Rotate(Point p, Vector axis, double degree);
        void Translate(const Vector &v);

        Point GetEyePoint();
        Vector GetLookVector();
        Vector GetUpVector();
        double GetViewAngle();
        double GetNearPlane();
        double GetFarPlane();
        int GetScreenWidth();
        int GetScreenHeight();

        double GetFilmPlanDepth();
        double GetScreenWidthRatio();
        Matrix GetRxyz2uvw();
        Matrix GetInvScaleMatrix();
    private:
        void setUVW();
        void RotateAroundAxis(Vector a, double degrees);

        Point eye;
        Point prev_eye;
        Point focus;
        Vector look;
        Vector up;
        int screenWidth;
        int prev_screenWidth;
        int screenHeight;
        int prev_screenHeight;

        double nearPlane;
        double prev_nearPlane;
        double farPlane;
        double prev_farPlane;

        double viewAngle;
        double prev_viewAngle;

        Vector u;
        Vector v;
        Vector w;
        Vector prev_u;
        Vector prev_v;
        Vector prev_w;
        Matrix modelViewMatrix;
        Matrix projectionMatrix;
};
#endif

