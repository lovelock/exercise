#include "forces/CAlgorithmFingerProxy.h"
#include "world/CWorld.h"

cAlgorithmFingerProxy :: cAlgorithmFingerProxy()
{
    // initialize world pointer
    m_world = NULL;

    m_numCollisionEvents = 0;
    setEpsilonBaseValue(0.00001);
    m_epsilonBaseValue = 0.00001;
    m_epsilonMinimalValue = 0.01 * m_epsilonBaseValue;
    m_epsilon = m_epsilonBaseValue;
    m_epsilonCollisionDetection = 1.0 * m_epsilon;

    m_frictionDynHysteresisMultiplier = 0.6;
    m_forceShadingAngleThreshold = 0.75;

    m_deviceGlobalPos.zero();
    m_proxyGlobalPos.zero();
    m_lastGlobalForce.zero();

    m_radius = 0.01f;
    m_useDynamicProxy = false;

    // initialize dynamic proxy members
    m_collisionRecorderConstraint0.m_nearestCollision.clear();
    m_collisionRecorderConstraint1.m_nearestCollision.clear();
    m_collisionRecorderConstraint2.m_nearestCollision.clear();

    m_slipping = true;

    m_collisionSettings.m_checkForNearestCollisionOnly = true;
    m_collisionSettings.m_returnMinimalCollisionData = false;
    m_collisionSettings.m_checkVisibleObjects = false;
    m_collisionSettings.m_checkHapticObjects = true;
    m_collisionSettings.m_adjustObjectMotion = m_useDynamicProxy;

    m_collisionEvents[0] = &(m_collisionRecorderConstraint0.m_nearestCollision);
    m_collisionEvents[1] = &(m_collisionRecorderConstraint1.m_nearestCollision);
    m_collisionEvents[2] = &(m_collisionRecorderConstraint2.m_nearestCollision);

    m_algoCounter = 0;

    // render settings for debug purpose
    m_showEnabled = true;
}

void cAlgorithmFingerProxy::initialize(cWorld* a_world, const cVector3d& a_initialGlobalPosition)
{
    m_lastGlobalForce.zero();
    m_numCollisionEvents = 0;
    m_slipping = true;
    m_deviceGlobalPos = a_initialGlobalPosition;
    m_proxyGlobalPos = a_initialGlobalPosition;

    m_world = a_world;
}

void cAlgorithmFingerProxy::reset()
{
    m_lastGlobalForce.zero();
    m_proxyGlobalPos = m_deviceGlobalPos;
    m_numCollisionEvents = 0;
}

void cAlgorithmFingerProxy::setEpsilonBaseValue(double a_value)
{
    m_epsilonBaseValue = a_value;
    m_epsilonMinimalValue = 0.01 * m_epsilonBaseValue;
    m_epsilon = m_epsilonBaseValue;
    m_epsilonCollisionDetection = 1.0 * m_epsilon;
}

cVector3d cAlgorithmFingerProxy::computeForces(const cVector3d& a_toolPos)
{
    m_deviceGlobalPos = a_toolPos;

    if (m_world != NULL)
    {
        computeNextBaseProxyPosition(m_deviceGlobalPos);
        m_proxyGlobalPos = m_nextBestProxyGlobalPos;
        updateForce();
        return (m_lastGlobalForce);
    }
    else
    {
        return (cVector3d(0.0, 0.0, 0.0));
    }
}

void cAlgorithmFingerProxy::computeNextBestProxyPosition(const cVector3d& a_goal)
{
    if (m_useDynamicProxy)
    {
        bool hit0, hit1, hit2;
        hit0 = computeNextProxyPositionWithConstraints0(a_goal);
        m_proxyGlobalPos = m_nextBestProxyGlobalPos;
        if (!hit0) { return; }

        hit1 = computeNextProxyPositionWithConstraints1(a_goal);
        m_proxyGlobalPos = m_nextBestProxyGlobalPos;
        if (!hit1) { return; }

        hit2 = computeNextProxyPositionWithConstraints2(a_goal);
        m_proxyGlobalPos = m_nextBestProxyGlobalPos;
    }
    else
    {
        switch(m_algoCounter)
        {
            case 0:
                computeNextProxyPositionWithConstraints0(a_goal);
                break;
            case 1:
                computeNextProxyPositionWithConstraints1(a_goal);
                break;
            case 2:
                computeNextProxyPositionWithConstraints2(a_goal);
                break;
        }
    }
}

bool cAlgorithmFingerProxy::computeNextProxyPositionWithConstraints0(const cVector3d& a_goalGlobalPos)
{
    cVector3d goalGlobalPos = a_goalGlobalPos;
    m_epsilonInitialValue = cAbs(0.0001 * m_radius);
    if (m_epsilonInitialValue < m_epsilonBaseValue)
    {
        m_epsilonInitialValue = m_epsilonBaseValue;
    }

    if (m_numCollisionEvents == 0)
    {
        m_epsilon = m_epsilonInitialValue;
        m_slipping = true;
    }

    if (!m_useDynamicProxy)
    {
        if (goalAchieved(m_proxyGlobalPos, goalGlobalPos))
        {
            m_nextBestProxyGlobalPos = m_proxyGlobalPos;
            m_algoCounter = 0;
            return (false);
        }
    }

    double distanceProxyGoal = cDistance(m_proxyGlobalPos, goalGlobalPos);

    cVector3d vProxyToGoal;
    cVector3d vProxyToGoalNormalized;
    bool proxyAndDeviceEqual;
    
    if (distanceProxyGoal > m_epsilon)
    {
        goalGlobalPos.subr(m_proxyGlobalPos, vProxyToGoal);
        vProxyToGoal.normalizer(vProxyToGoalNormalized);
        proxyAndDeviceEqual = false;
    }
    else
    {
        vProxyToGoal.zero();
        vProxyToGoalNormalized.zero();
        proxyAndDeviceEqual = true;
    }

    cVector3d targetPos;
    if (m_useDynamicProxy)
    {
        targetPos = goalGlobalPos;
    }
    else
    {
        targetPos = goalGlobalPos +
                    cMul(m_epsilonCollisionDetection, vProxyToGoalNormalized);
    }
    m_collisionSettings.m_collisionRadius = m_radius;

    m_collisionSettings.m_adjustObjectMotion = m_useDynamicProxy;
    m_collisionRecorderConstraint0.clear();
    bool hit = m_world->computeCollisionDetection(m_proxyGlobalPos,
                                                  targetPos,
                                                  m_collisionRecorderConstraint0,
                                                  m_collisionSettings);
    double collisionDistance;

    if (hit)
    {
        collisionDistance = sqrt(m_collisionRecorderConstraint0.m_nearestCollision.m_squareDistance);
        if(m_useDynamicProxy)
        {
            cVector3d posLocal = m_collisionRecorderConstraint0.m_nearestCollision.m_adjustSegmentAPoint;
            cGenericObject* obj = m_collisionRecorderConstraint0.m_nearestCollision.m_object;
            cVector3d posGlobal = cAdd(obj->getGlobalPos(), cMul(obj->getGlobalRot(), posLocal));
            m_proxyGlobalPos = posGlobal;
            distanceProxyGoal = cDistance(m_proxyGlobalPos, goalGlobalPos);
            goalGlobalPos.subr(m_proxyGlobalPos, vProxyToGoal);
            vProxyToGoal.normalizer(vProxyToGoalNormalized);
        }

        if (collisionDistance > (distanceProxyGoal + C_SMALL))
        {
            hit = false;
        }

        if (hit)
        {
            if (collisionDistance < m_epsilon)
            {
                m_epsilon = collisionDistance;
                if (m_epsilon < m_epsilonMinimalValue)
                {
                    m_epsilon = m_epsilonMinimalValue);
                }
            }
        }
    }

    if (!hit)
    {
        m_numCollisionEvents = 0;
        m_algoCounter = 0;
        m_slipping = true;
        m_nextBestProxyGlobalPos = goalGlobalPos;
        return (false);
    }

    // a first collision has occurred
    m_algoCounter = 1;

    double cosAngle = vProxyToGoalNormalized.dot(m_collisionRecorderConstraint0.m_nearestCollision.m_globalNormal);
    double distanceTriangleProxy = m_epsilon / cAbs(cosAngle);
    if (distanceTriangleProxy > collisionDistance)
    {
        distanceTriangleProxy = cMax(collisionDistance, m_epsilon);
    }

    cVector3d vCollisionToProxy;
    m_proxyGlobalPos.subr(m_collisionEvents[0]->m_globalPos, vCollisionToProxy);

    cVector3d vVolNextGoal;
    vProxyToGoalNormalized.mulr(-distanceTriangleProxy, vColNextGoal);
    cVector3d nextProxyPos;
    m_collisionEvents[0]->m_globalPos.addr(vColNextGoal, nextProxyPos);

    m_nextBestProxyGlobalPos = nextProxyPos;

    if (goalAchieved(goalGlobalPos, nextProxyPos))
    {
        m_numCollisionEvents = 1;
        m_algoCounter = 0;
        return (true);
    }
    return (true);
}


bool cAlgorithmFingerProxy::computeNextProxyPositionWithConstraints1(const cVector3d& a_goalGlobalPos)
{
    cVector3d goalGlobalPos;
    if (m_collisionRecorderConstraint0.m_nearestCollision.m_object->m_material->getUseHapticShading())
    {
        cVector3d normal = computeShadedSurfaceNormal(&m_collisionRecorderConstraint0.m_nearestCollision);

    }
}
