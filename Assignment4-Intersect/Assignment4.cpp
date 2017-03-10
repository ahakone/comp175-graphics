#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"
#include "FlatTNode.h"
#include <deque>

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;


int camRotU = 0;
int camRotV = 0;
int camRotW = 0;
int viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data\/general\/test.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();
std::vector<FlatTNode*> flatTree;
FlatTNode **flatTreeArr;
int flatTreeSize;
void setupCamera();
void updateCamera();
Matrix combineTransformations(
        std::vector<SceneTransformation*> transformations, Matrix m);
void flattenTree(SceneNode* node, std::deque<Matrix> transDeque);

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
    buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
    buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
    buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

void callback_start(int id) {
    cout << "start button clicked!" << endl;

    if (parser == NULL) {
        cout << "no scene loaded yet" << endl;
        return;
    }

    pixelWidth = screenWidth;
    pixelHeight = screenHeight;

    updateCamera();

    if (pixels != NULL) {
        delete pixels;
    }
    pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
    memset(pixels, 0, pixelWidth  * pixelHeight * 3);

    cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;


    Point eye_world = camera->GetEyePoint();
    Vector eye_pointV(eye_world[0], eye_world[1], eye_world[2]);
    eye_pointV.normalize();
    Matrix film2world = inv_trans_mat(-eye_pointV) * transpose(camera->GetRxyz2uvw()) * camera->GetInvScaleMatrix();

    for (int i = 0; i < pixelWidth; i++) {
      for (int j = 0; j < pixelHeight; j++) {
        Point p_film(((2.0*i) /(double)(pixelWidth - 1)) - 1.0, 
                     ((2.0*j) /(double)(pixelHeight - 1)) - 1.0,
                         -1.0);
            Point p_world = film2world * p_film;
            Vector d(p_world - eye_world);
            d.normalize(); 

            double t;
            double min_t;
            FlatTNode* currMinNode;
            for (int k = 0; k < flatTreeSize; k++) {
                switch (flatTreeArr[k]->GetPrimitive().type) {
                    case SHAPE_CUBE:
                        t = cube->Intersect(eye_world, d, flatTreeArr[k]->GetTMatrix());
                        break;
                    case SHAPE_SPHERE:
                        t = sphere->Intersect(eye_world, d, flatTreeArr[k]->GetTMatrix());
                        break;
                    case SHAPE_CYLINDER:
                        t = cylinder->Intersect(eye_world, d, flatTreeArr[k]->GetTMatrix());
                        break;
                    case SHAPE_CONE:
                        t = cone->Intersect(eye_world, d, flatTreeArr[k]->GetTMatrix());
                        break;
                    default:
                        break;
                }


                if (IN_RANGE(k, 0)) {
                    min_t = t;
                    currMinNode = flatTreeArr[k];                    
                } else if (IN_RANGE(min_t, -1) || (!IN_RANGE(t, -1) && t < min_t)) {
                    min_t = t;
                    currMinNode = flatTreeArr[k];                    
                }
            }

            if (min_t > 0) {// || IN_RANGE(min_t, 0)) {
                if (isectOnly) {
                    setPixel(pixels, i, j, 255, 255, 255);
                }
                else {
                    // convert ray and eye point to object space
                    Matrix inv_mat = invert(currMinNode->GetTMatrix());
                    Point eye_obj(inv_mat * eye_world);
                    Vector d_obj(inv_mat * d);

                    Vector norm;

                    // get the normal in object space
                    switch (currMinNode->GetPrimitive().type) {
                        case SHAPE_CUBE: 
                            norm = cube->findIsectNormal(eye_obj, d_obj, min_t);
                            break;
                       case SHAPE_SPHERE:
                           norm = sphere->findIsectNormal(eye_obj, d_obj, min_t);
                           break;
                       case SHAPE_CYLINDER:
                           norm = cylinder->findIsectNormal(eye_obj, d_obj, min_t);
                           break;
                       case SHAPE_CONE: 
                           norm = cone->findIsectNormal(eye_obj, d_obj, min_t);
                           break;
                        default:
                          break;
                    }

                    // convert normal from object space to world space
                    norm = transpose(inv_mat) * norm;
                    norm.normalize();

                    // get data for illumination calculation
                    SceneGlobalData k;
                    parser->getGlobalData(k);
                    double k_a = k.ka;
                    double k_d = k.kd;

                    double oal_r = currMinNode->GetPrimitive().material.cAmbient.r;
                    double oal_g = currMinNode->GetPrimitive().material.cAmbient.g;
                    double oal_b = currMinNode->GetPrimitive().material.cAmbient.b;

                    double odl_r = currMinNode->GetPrimitive().material.cDiffuse.r;
                    double odl_g = currMinNode->GetPrimitive().material.cDiffuse.g;
                    double odl_b = currMinNode->GetPrimitive().material.cDiffuse.b;

                    double intensityR, intensityG, intensityB;

                    double sumLightsR = 0;
                    double sumLightsG = 0;
                    double sumLightsB = 0;

                    // get intersection point in world space
                    Point intersectionP = eye_world + min_t * d;
                    SceneLightData lightData;
                    for (int l = 0; l < parser->getNumLights(); l++) {
                        parser->getLightData(l, lightData);
                        //Vector lightV(intersectionP - lightData.pos);
                        Vector lightV(lightData.pos - intersectionP);
                        lightV.normalize();

                        double dot_prod = dot(norm, lightV);
                        if (dot_prod < 0) {
                            dot_prod = 0;
                        }

                        sumLightsR += k_d * odl_r * lightData.color.r * dot_prod;
                        sumLightsG += k_d * odl_g * lightData.color.g * dot_prod;
                        sumLightsB += k_d * odl_b * lightData.color.b * dot_prod;
                    }

                    intensityR = (k_a * oal_r + sumLightsR);
                    intensityR = intensityR > 1 ? 1 : intensityR;
                    intensityG = (k_a * oal_g + sumLightsG);
                    intensityG = intensityG > 1 ? 1 : intensityG;
                    intensityB = (k_a * oal_b + sumLightsB);
                    intensityB = intensityB > 1 ? 1 : intensityB;

                    setPixel(pixels, i, j, 
                            intensityR * 255., 
                            intensityG * 255.,
                            intensityB * 255.);

                }
            }
        }
    }
    std::cout << "Finished!\n";
    glutPostRedisplay();
}

Matrix combineTransformations(
        std::vector<SceneTransformation*> transformations, Matrix m) {
    int i;
    for (i = 0; i < transformations.size(); i++) {
        if (transformations[i]->type == TRANSFORMATION_TRANSLATE) {
            m = m * trans_mat(transformations[i]->translate);
        }
        else if (transformations[i]->type == TRANSFORMATION_SCALE) {
            m = m * scale_mat(transformations[i]->scale);
        }
        else if (transformations[i]->type == TRANSFORMATION_ROTATE) {
            m = m * rot_mat(transformations[i]->rotate, transformations[i]->angle);
        }
        else {
            m = m * transformations[i]->matrix;
        }
    }
    return m;
}

void flattenTree(SceneNode* node, Matrix m) {
    int i;
    int j;

    if(!(node->transformations.empty())) {
        m = combineTransformations(node->transformations, m);
    }

    if (!(node->primitives.empty())) {
        for (i = 0; i < node->primitives.size(); i++) {
            FlatTNode* flatNode = new FlatTNode(*node->primitives[i], m);
            flatTree.push_back(flatNode);
        }
    }

    if (!(node->children.empty())) {
        for (i = 0; i < node->children.size(); i++) {
            flattenTree(node->children[i], m);
        }
    }

    return;
}

void callback_load(int id) {
    char curDirName [2048];
    if (filenameTextField == NULL) {
        return;
    }
    printf ("%s\n", filenameTextField->get_text());

    if (parser != NULL) {
        delete parser;
    }
    parser = new SceneParser (filenamePath);
    bool success = parser->parse();
    cout << "success? " << success << endl;
    if (success == false) {
        delete parser;
        vector<FlatTNode*>().swap(flatTree);
        parser = NULL;
    }
    else {
        setupCamera();
        SceneNode* node = parser->getRootNode();
        flatTree.clear();
        Matrix m = Matrix();
        flattenTree(node, m);
        flatTreeArr = &flatTree[0]; 
        flatTreeSize = flatTree.size();
    }
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
    /* According to the GLUT specification, the current window is
       undefined during an idle callback.  So we need to explicitly change
       it if necessary */
    if (glutGetWindow() != main_window)
        glutSetWindow(main_window);

    glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
    float xy_aspect;

    xy_aspect = (float)x / (float)y;
    glViewport(0, 0, x, y);
    camera->SetScreenSize(x, y);

    screenWidth = x;
    screenHeight = y;

    glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
    SceneCameraData cameraData;
    parser->getCameraData(cameraData);

    camera->Reset();
    camera->SetViewAngle(cameraData.heightAngle);
    if (cameraData.isDir == true) {
        camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
    }
    else {
        camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
    }

    viewAngle = camera->GetViewAngle();
    Point eyeP = camera->GetEyePoint();
    Vector lookV = camera->GetLookVector();
    eyeX = eyeP[0];
    eyeY = eyeP[1];
    eyeZ = eyeP[2];
    lookX = lookV[0];
    lookY = lookV[1];
    lookZ = lookV[2];
    camRotU = 0;
    camRotV = 0;
    camRotW = 0;
    GLUI_Master.sync_live_all();
}

void updateCamera()
{
    camera->Reset();

    Point guiEye (eyeX, eyeY, eyeZ);
    Point guiLook(lookX, lookY, lookZ);
    camera->SetViewAngle(viewAngle);
    Vector up(camera->GetUpVector());
    camera->Orient(guiEye, guiLook, up);
    camera->RotateU(camRotU);
    camera->RotateV(camRotV);
    camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (parser == NULL) {
        return;
    }

    if (pixels == NULL) {
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
}

void onExit()
{
    delete cube;
    delete cylinder;
    delete cone;
    delete sphere;
    delete camera;
    if (parser != NULL) {
        delete parser;
    }
    if (pixels != NULL) {
        delete pixels;
    }
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    atexit(onExit);

    /****************************************/
    /*   Initialize GLUT and create window  */
    /****************************************/

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);

    main_window = glutCreateWindow("COMP 175 Assignment 4");
    glutDisplayFunc(myGlutDisplay);
    glutReshapeFunc(myGlutReshape);

    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/

    GLUI* glui = GLUI_Master.create_glui("GLUI");

    filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
    filenameTextField->set_w(300);
    glui->add_button("Load", 0, callback_load);
    glui->add_button("Start!", 0, callback_start);
    glui->add_checkbox("Isect Only", &isectOnly);

    GLUI_Panel *camera_panel = glui->add_panel("Camera");
    (new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
        ->set_int_limits(-179, 179);
    (new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
        ->set_int_limits(-179, 179);
    (new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
        ->set_int_limits(-179, 179);
    (new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
        ->set_int_limits(1, 179);

    glui->add_column_to_panel(camera_panel, true);

    GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
    eyex_widget->set_float_limits(-10, 10);
    GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
    eyey_widget->set_float_limits(-10, 10);
    GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
    eyez_widget->set_float_limits(-10, 10);

    GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
    lookx_widget->set_float_limits(-10, 10);
    GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
    looky_widget->set_float_limits(-10, 10);
    GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
    lookz_widget->set_float_limits(-10, 10);

    glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

    glui->set_main_gfx_window(main_window);

    /* We register the idle callback with GLUI, *not* with GLUT */
    GLUI_Master.set_glutIdleFunc(myGlutIdle);

    glutMainLoop();

    return EXIT_SUCCESS;
}



