/*
 * =====================================================================================
 *
 *       Filename:  CCollisionAABBTree.cpp
 *
 *    Description:  CCollisionAABBTree.cpp
 *
 *        Version:  1.0
 *        Created:  04/07/2014 06:18:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Frost Wong (Qingchun Wang), frostwong@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "collisions/CCollisionAABBTree.h"

/* pointer for creating new AABB tree nodes, declared in CCollisionAABB.cpp */
extern CCollisionAABBInternal* g_nextFreeNode;

inline bool intersect(const cCollisionAABBBox& a_0, const cCollisionAABBBox& a_1)
{
    /* Check for overlap along each axis */
    if (a_0.getLowerX() > a_1.getUpperX()) return false;
    if (a_0.getLowerY() > a_1.getUpperY()) return false;
    if (a_0.getLowerZ() > a_1.getUpperZ()) return false;
    if (a_1.getLowerX() > a_0.getUpperX()) return false;
    if (a_1.getLowerY() > a_0.getUpperY()) return false;
    if (a_1.getLowerZ() > a_0.getUpperZ()) return false;

    /* If the boxes are not separated along any axis, a collision has occured */
    return true;
}


void cCollisionAABBLeaf::render(int a_depth)
{
    if (( a_depth < 0) && (abs(a_depth) >= m_depth)) || a_depth == m_depth)
    {
        if (a_depth < 0)
        {
            cColorf c(1.0, 0.0, 0.0, 1.0);
            glColor4fv(c.pColor());
        }
        m_bbox.render();
    }
}

void cCollisionAABBLeaf::fitBBox(double a_radius)
{
    /* Empty Box */
    m_bbox.setEmpty();


    /* Enclose all vertices of triangle */
    if(m_triangle != NULL)
    {
        a_radius = 2*a_radius;
        m_bbox.enclose(m_triangle->getVertex0()->getPos())
        m_bbox.enclose(m_triangle->getVertex1()->getPos())
        m_bbox.enclose(m_triangle->getVertex2()->getPos())
        cVector3d min = m_bbox.m_min;
        cVector3d max = m_bbox.m_max;
        min.sub(a_radius, a_radius, a_radius);
        max.add(a_radius, a_radius, a_radius);
        m_bbox.setValue(min, max);
    }
}

bool cCollisionAABBLeaf::computeCollision(cVector3d& a_segmentPoiontA,
                                          cVector3d& a_segmentPoiontB,
                                          cCollisionAABBBox& a_lineBox,
                                          cCollisionRecorder& a_recorder,
                                          cCollisionSetting& a_settings)
{
    bool result = m_triangle->computeCollsion(a_segmentPoiontA,
                                              a_segmentPoiontB,
                                              a_recorder,
                                              a_settings);
    return (result);
}




