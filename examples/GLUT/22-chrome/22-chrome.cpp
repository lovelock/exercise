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

