#ifndef CGenericForceAlgorithmH
#define CGenericForceAlgorithmH

#include "math/CVector3d.h"
#include "world/CGenericObject.h"
#include <vector>

class cWorld;

class CGenericForceAlgorithm
{
    public:
        cGenericForceAlgorithm();
        virtual ~cGenericForceAlgorithm() {};
        
        virtual void render(cRenderOptions& a_options) {}
        cWorld* getWorld() { return (m_world); }
        virtual void initialize(cWorld* a_world, const cVector3d& a_initialPos) {};
        virtual cVector3d computeForces(const cVector3d& a_toolPos, const cVector3d& a_toolVel) { return (cVector3d(0.0, 0.0, 0.0)); }

        void setShowEnable(bool a_showEnabled) { m_showEnabled = a_showEnabled; }
        bool getShowEnabled() { return (m_showEnabled); }

    protected:
        cWorld* m_world;
        bool m_showEnabled;

};

#endif
