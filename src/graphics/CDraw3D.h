#ifndef CDraw3DH
#define CDraw3DH

#include "math/CMaths.h"

void cLookAt(const cVector3d& a_eye,
             const cVector3d& a_at,
             const cVector3d& a_up);

void cDrawFrame(const double& a_scale = 1.0,
                const bool a_modifyMaterialState = true);

void cDrawFrame(const double& a_axisLengthScale,
                const double& a_axisThicknessScale,
                const bool a_modifyMaterialState);

void cDrawWireBox(const double& a_xMin, const double& a_xMax,
                  const double& a_yMin, const double& a_yMax,
                  const double& a_zMin, const double& a_zMax);

void cDraeSphere(const double& a_radius,
                 const unsigned int a_numSlices = 10,
                 const unsigned int a_numStacks = 10);

void cDrawArrow(const cVector3d& a_arrowStart,
                const cVector3d& a_arrowTip,
                const double a_width = 0.05);

void cDrawSolidTorus(const double& a_innerRadius,
                     const double& a_outerRadius,
                     const int a_sides,
                     const int a_rings);

#endif
