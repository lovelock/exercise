#ifndef CAlgorithmPotentialFieldH
#define CAlgorithmPotentialFieldH

#include "forces/CGenericForceAlgorithm.h"
#include "forces/CInteractionBasics.h"

class cAlgorithmPotentialField : public cGenericForceAlgorithm
{
    public:
            cAlgorithmPotentialField();
        
            virtual ~cAlgorithmPotentialField() {};
        
            virtual void render(cRenderOptions& a_options) {};
        
            void initialize(cWorld* a_world, const cVector3d& a_initialPos)  { m_world = a_world; }
        
            virtual cVector3d computeForces(const cVector3d& a_toolPos, const cVector3d& a_toolVel);
            cInteractionRecorder m_interactionRecorder;
    private:
            unsigned int m_IDN;
            static unsigned int m_IDNcounter;
};

#endif
