#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chai3d.h"

#ifndef MACOSX
#include "GL/glut.h"
#else
#include "GLUT/glut.h"
#endif

using namespace std;

const int WINDOW_SIZE_W                 = 800;
const int WINDOW_SIZE_H                 = 800;

const int OPTION_FULLSCREEN             = 1;
const int OPTION_WINDOWDISPLAY          = 2;

const bool USE_STEREO_DISPLAY           = false;

int displayW = 0;
int displayH = 0;

cWorld* world;
cCamera* camera;

cSpotLight* light;
cMultiMesh* object;

cHapticDeviceHandler* handler;
cGenericHapticDevice* hapticDevice;
cToolCursor* tool;
cLabel* labelHapticRate;

bool simulationRunning = false;
bool simulationFinished = false;
cFrequencyCounter frequencyCounter;
string resourceRoot;

#define RESOURCE_PATH(p) (char*)((resourceRoot+string(p)).c_str())


// define functions
void resizeWindow(int w, int h);
void keySelect(unsigned char key, int x, int y);
void menuSelect(int value);
void close(void);
void graphicsTimer(int data);
void updateGraphics(void);
void updateHaptics(void);

int main(int argc, char* argv[])
{
    cout << endl;
    cout << "-------------------------------------------" << endl;
    cout << "CHAI3D" << endl;
    cout << "Thesis Demo: Heart haptic demo" << endl;
    //cout << "Copyright 2014" << endl;
    cout << "-------------------------------------------" << endl;
    cout << endl << endl;
    cout << "Keyboard Options: " << endl << endl;
    cout << "[1] - Texture ON " << endl;
    cout << "[2] - Texture OFF " << endl;
    cout << "[3] - Wireframe ON " << endl;
    cout << "[4] - Wireframe OFF" << endl;
    cout << "[5] - Normals ON " << endl;
    cout << "[6] - Normals OFF" << endl;
    cout << "[x] - Exit application" << endl;
    cout << endl << endl;

    // parse first arg to locate resources
    resourceRoot = string(argv[0]).substr(0, string(argv[0]).find_last_of("/\\")+1);

    world = new cWorld();
    world->m_backgroundColor.setBlack();
    camera = new cCamera(world);
    world->addChild(camera);

    // set the position of the camera.
    camera->set( cVector3d (3.0, 0.0, 0.6), // camera position (eye);
                 cVector3d (0.0, 0.0, 1.0), // lookat position (target); 
                 cVector3d (0.0, 0.0, 1.0)); // direction of the (up) vector;

    camera->setClippingPlanes(0.01, 10.0);
    camera->setUseShadowCasting(true);
    
    light = new cSpotLight(world);
    camera->addChild(light);

    light->setEnabled(true);
    light->setLocalPos(0.0, 0.5, 0.0);
    light->setDir(-3.0, -0.5, 0.0);
    light->setShadowMapEnabled(true);

    // haptic devices / tools
    // create a haptic device handler 
    handler = new cHapticDeviceHandler();

    // get the handler of the first available haptic device
    handler->getDevice(hapticDevice, 0);

    // retrieve information about the current haptic device
    cHapticDeviceInfo info = hapticDevice->getSpecifications();

    // create a 3d tool and add it to the world
    tool = new cToolCursor();
    world->addChild(tool);

    // set the haptic device to the tool
    tool->setHapticDevice(hapticDevice);

    tool->start();

    // map the physical workspace of the haptic device to a larger virtual workspace.
    tool->setWorkspaceRadius(1.0);

    // define the radius of the tool
    double toolRadius = 0.04;

    tool->setRadius(toolRadius);

    // hide the device shpere, only show proxy.
    tool->setShowContactPoints(true, false);
    // enable if objects in the sencen are going to rotate or translate or possibly
    // aginst the tool. If the environment is entirely static, this parameter would
    // be set to "false"
    tool->enableDynamicObjects(true);

    double workspaceScaleFactor = tool->getWorkspaceScaleFactor();
    double stiffnessMax = info.m_maxLinearStiffness / workspaceScaleFactor;

    // creating object
    object = new cMultiMesh();
    world->addChild(object);
    object->setLocalPos(0.0, 0.0, 0.0);

    // rotate the object 90 degrees
    object->rotateAboutGlobalAxisDeg(0, 0, 1), 90);

    bool fileload;
    // here in the original example is a .obj file
    fileload = object->loadFromFile(RESOURCE_PATH("resources/models/heart_muscle.3DS"));
    if (!fileload)
    {
        #if defined(_MSVC)
        fileload = object->loadFromFile("../../../bin/resources/models/heart_muscle.3DS");
        #endif
    }
    if (!fileload)
    {
        cout << "Error - 3D Model failed to load correctly." << endl;
        close();
        return (-1);
    }

    object->computeBoundaryBox(true);
    double size = cSub(object->getBoundaryMax(), object->getBoundaryMin()).length();

    // resize object to screen
    if (size > 0)
    {
        object->scale(2.0 * tool->getWorkspaceRadius() / size);
    }

    object->creatrAABBCollisionDetector(toolRadius);

    cMaterial mat;
    mat.setRenderTriangles(true, true);
    object->setMaterial(mat);

    cTexture2d* texture = new cTexture2d();
    
    fileload = texture->loadFromFile(RESOURCE_PATH("resources/images/chrome.bmp"));
    if (!fileload)
    {
        #if defined(_MSVC)
        fileload = texture->loadFromFile("../../../bin/resources/images/chrome.bmp");
        #endif
    }

    if (!fileload)
    {
        cout << "Error - Texture image failed to load correctly." << endl;
        close();
        return (-1);
    }


    texture->setSphericalMappingEnabled(true);
    object->setTexture(texture, true);
    object->setUseTexture(true, true);

    object->setUseCulling(false, true);
    object->setStiffness(stiffnessMax, true);
    object->setFriction(0.1, 0.2, true);

    cFont *font = NEW_CFONTCALIBRI20();
    labelHapticRate = new cLabel(font);
    labelHapticRate->m_fontColor.setWhite();
    camera->m_frontLayer->addChild(labelHapticRate);

    simulationRunning = true;
    glutInit(&argc, argv);
    int screenW = glutGet(GLUT_SCRREN_WIDTH);
    int screenH = glutGet(GLUT_SCREEN_HIGHT);
    int windowPosX = (screenW - WINDOW_SIZE_W) / 2;
    int windowPosY = (screenH - WINDOW_SIZE_H) / 2;

    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WINDOW_SIZE_W, WINDOW_SIZE_H);

    if (USE_STEREO_DISPLAY)
    {
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STEREO);
        camera->setUseStereo(true);
    }
    else
    {
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
        camera->setUseStereo(false);
    }

    glutCreateWindow(argv[0]);
    glutDisplayFunc(updateGraphics);
    glutKeyboardFunc(keySelect);
    glutReshapeFunc(resizeWindow);
    glutSetWindowTitle("CHAI3D");

    glutCreateMenu(menuSelect);
    glutAddMenuEntry("full screen", OPTION_FULLSCREEN);
    glutAddMenuEntry("window display", OPTION_WINDOWDISPLAY);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    cThread* hapticsThread = new cThread();
    hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

    glutTimerFunc(30, graphicsTimer, 0);
    glutMainLoop();
    close();
    return (0);
}

void resizeWindow(int w, int h)
{
    displayW = w;
    displayH = h;
    glViewPort(0, 0, displayW, displayH);
}
/* if way
void keySelect(unsigned char key, int x, int y)
{
    if ((key == 27) || (key == 'x'))
    {
        close();
        exit(0);
    }
    if (key == '1')
    {
        object->setUseTexture(true);
    }
    if (key == '2')
    {
        object->setUseTexture(false);
    }

    if (key == '3')
    {
        object->setUseWireMode(true);
    }

    if (key == '4')
    {
        object->setUseWireMode(false);
    }

    if (key == '5')
    {
        object->setShowNormals(true);
        object->setNormalsProperties(0.05, cColorf(1.0, 0.0, 0.0));
    }

    if (key == '6')
    {
        object->setShowNormals(false);
    }

}
*/

void keySelect(unsigned int key, int x, int y)
{
    switch(key)
    {
        case '27' || 'x':
            close();
            exit(0);
        case '1':
            object->setUseTexture(true);
            break;
        case '2':
            object->setUseTexture(false);
            break;
        case '3':
            object-setUseWireMode(true);
            break;
        case '4':
            object->setUseWireMode(false);
            break;
        case '5':
            object->setShowNormals(true);
            object->setNormalsProperties(0.05, cColorf(1.0, 0.0, 0.0));
            break;
        case '6':
            object->setShowNormals(false);
            break;
    }
}

void menuSelect(int value)
{
    static int _wx, _wy, _ww, _wh;
    switch(value)
    {
        case OPTION_FULLSCREEN:
            _wx = glutGet(GLUT_WINDOW_X);
            _wy = glutGet(GLUT_WINDOW_Y);
            _ww = glutGet(GLUT_WINDOW_WIDTH);
            _wh = glutGet(GLUT_WINDOW_HIGHT);
            glutFullScreen();
            break;
        case OPTION_WINDOWDISPLAY:
            glutPositionWindow(_wx, _wy);
            glutReshapeWindow(_ww, _wh);
            break;
    }
}

void close(void)
{
    simulationRunning = false;
    while (!simulationFinished)
    {
        cSleepMs(100);
    }
    tool->stop();
}


void graphicsTimer(int data)
{
    if (simulationRunning)
    {
        glutPostRedisplay();
    }
    glutTimerFunc(30, graphicsTimer, 0);
}

void updateGraphics(void)
{
    labelHapticRate->setString("hapic rate: "+cStr(frequencyCounter.getFrequency(), 0) + "[Hz]");
    int px = (int)(0.5 * (displayW - labelHapticRate->getWidth()));
    labelHapticRate->setLocalPos(px, 15);
    camera->renderView(displayW, displayH);
    glutSwapBuffers();

    GLenum err;
    err = glGetError();
    if (err ! = GL_NO_ERROR)
    {
        printf("Error: %s\n", gluErrorString(err));
    }
}

void updateHaptics(void)
{
    // angular velocit
    cVector3d angVel(0, 0, 0);

    // reset clock
    cPrecisionClock clock;
    clock.reset();

    while(simulationRunning)
    {
        // simulation time
        // stop the simulation clock
        clock.stop();
        // read the time increment in seconds
        double timeInterval = clock.getCurrentTimeSeconds();

        // restart the simulation clock
        clock.reset();
        clock.start();

        // update the frequency counter
        frequencyCounter.signal(1);

        // compute global reference frames for each object
        world->computeGlobalPositions(true);

        // update position and orientation of tool
        tool->updatePos();

        // compute interaction forces
        tool->computeInteractionForces();

        // send forces to device
        tool->applyForces();

        // get position of cursor in global coordinates
        cVector3d toolPos = tool->getDeviceGlobalPos();
        
        // get position of object in global coordinates
        cVector3d objectPos = object->getGlobalPos();

        // compute a vector from the center of mass of the object (point of rotation) to the tool
        cVector3d v = cSub(toolPos, objectPos);

        // compute angular acceleration based on the interations forces
        // between the tool and object
        cVector3d angAcc(0, 0, 0);
        if (v.length() > 0.0)
        {
            // get the last force applied to the cursor in global coordinates
            // we negate the result to obtain the opposite force that is applied
            // on the object
            cVector3d toolForce = cNegate(tool->m_lastComputedGlobalForce);
            // compute the effective force that contributes to rotating the object
            cVector3d force = toolForce - cPorject(toolForce, v);
            // compute the resulting torque
            cVector3d torque = cMul(v.length(), cCross(cNormalize(v), force));

            // compute a torque to restore the face to its original position  
            cVector3d dirFace = object->getLocalRot().getCol0();
            cVector3d dirTorque = cCross(dirFace, cVector3d(0, 1, 0));
            dirTorque.mul(3.0);
            torque.add(dirTorque);

            cVector3d upFace = object->getLocalRot().getCol2();
            cVector3d upTorque = cCross(upFace, cVector3d(0, 0, 1));
            dirTorque.mul(3.0);
            torque.add(upTorque);

            // update rotational acceleration
            const double INERTIA = 0.4;
            angAcc = (1.0 / INERTIA) * torque;
        }

        angVel.add(timerInterval * angAcc);
        const double MAX_ANG_VEL = 10.0;
        if (vel > MAX_ANG_VEL)
        {
            angVel.mul(MAX_ANG_VEL / vel);
        }

        const double DAMPING = 0.1;
        angVel.mul(1.0 - DAMPING * timeInterval);
        if (tool->getUserSwitch(0) == 1)
        {
        angVel.zero();
        }

        if (angVel.length() > C_SMALL)
        {
            object->rotateAboutGlobalAxisRad(cNormalize(angVel), timeInterval * angVel.length());
        }
    }
    simulationFinished = true;
}
