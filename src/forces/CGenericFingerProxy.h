#ifndef CGenericFingerProxyH
#define CGenericFingerProxyH

#include "math/CVector3d.h"
#include "math/CMatrix3d.h"
#include "collisions/CGenericCollision.h"
#include "forces/CGenericForceAlgorithm.h"
#include <map>

class cWorld;

class cAlgorithmFingerProxy : public cGenericForceAlgorithm
{
    public:
        cAlgorithmFingerProxy();
        virtual ~cAlgorithmFingerProxy() {}

        // Render force algorithms graphicaly in OpenGL for debug purpose.
        virtual void render(cRenderOptions& a_options);

        // Initialize the algorithm
        void Initialize(cWorld* a_world, const cVector3d& a_initialGlobalPosition);

        // Reset the algorithm, set proxy position to device position.
        void reset();

        virtual cVector3d computeForces(const cVector3d& a_toolPos, const cVector3d& a_toolVel);

        void setProxyRadius(const double& a_radius)
        {
            m_radius = a_radius;
            m_collisionSettings.m_collisionRadius = m_radius; 
        }

        inline double getProxyRadius() const { return (m_radius);}

        inline cVector3d getProxyGlobalPosition() const 
        {
            return (m_proxyGlobalPos);
        }

        inline void setProxyGlobalPosition(const cVector3d& a_position)
        {
            m_proxyGlobalPos = a_position;
        }

        inline cVector3d getDeviceGlobalPosition() const
        {
            return (m_deviceGlobalPos);
        }

        inline cVector3d getForce() { return (m_lastGlobalForce); }

        inline cVector3d getNormalForce() { return (m_normalForce); }

        inline cVector3d getTangentialForce() { return (m_tangentialForce); }

        int getNumCollisionEvents() { return (m_numCollisionEvents); }

        cCollisionEvent* m_collisionEvents[3];

        // Methods force models
        bool m_useDynamicProxy;

        double m_frictionDynHysteresisMultiplier;

        double m_forceShadingAngleThreshold;

        cCollisionSettings m_collisionSettings;

        // Methods - resolution and errors
        void setEpsilonBaseValue(double a_value);
        
        double getEpsilonBaseValue() { return (m_epsilonBaseValue); }

    protected:
        // test whether the proxy has reached the goal point
        virtual bool goalAchieved(const cVector3d& a_proxy, const cVector3d& a_goal) const;

        virtual void computeNextBestProxyPosition(const cVector3d& a_goal);

        virtual void testFrictionAndMoveProxy(const cVector3d& a_goal, const cVector3d& a_proxy, cVector3d& a_normal, cGenericObject* a_parent);

        // compute force to apply to device
        virtual void updateForce();

        cVector3d m_proxyGlobalPos;
        cVector3d m_deviceGlobalPos;
        cVector3d m_lastGlobalForce;
        cVector3d m_nextBestProxyGlobalPos;

        // are we currently in a 'slip friction' mode?
        bool m_slipping;

        cVector3d m_normalForce;
        cVector3d m_tangentialForce;
        unsigned int m_numCollisionEvents;
        double m_radius;


        cCollisionRecorder m_collisionRecorderConstraint0;
        cCollisionRecorder m_collisionRecorderConstraint1;
        cCollisionRecorder m_collisionRecorderConstraint2;

        double m_epsilonInitValue;
        double m_epsilon;
        double m_epsilonCollisionDetection;
        double m_epsilonBaseValue;
        double m_epsilonMinimalValue;
        unsigned int m_algoCounter;

        bool computeNextProxyPositionWithConstraint0(const cVector3d& a_goalGlobalPos);
        bool computeNextProxyPositionWithConstraint1(const cVector3d& a_goalGlobalPos);
        bool computeNextProxyPositionWithConstraint2(const cVector3d& a_goalGlobalPos);

        cVector3d computeShadedSurfaceNormal(cCollisionEvent* a_contactPoint);

        cVector3d surfaceNormal;
};

#endif



