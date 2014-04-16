/*
 * =====================================================================================
 *
 *       Filename:  CCollisionAABBBox.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/07/2014 01:36:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Frost Wong (Qingchun Wang), frostwong@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef CCollisionAABBBoxH
#define CCollisionAABBBoxH

#include "math/CMaths.h"
#include "graphics/CTriangle.h"
#include "graphics/CVertex.h"
#include <vectro>

class cCollisionAABBBox
{
    public:
        cCollisionAABBBox() {};

        cCollisionAABBBox(const cVector3d& a_min, const cVector3d& a_max)
        {
            setValue(a_min, a_max);
        }

        virtual ~cCollisionAABBBox() {};


        /*  Return the center of the bouding box */
        inline cVector3d getCenter() const { return (m_center); }

        /*  Return the extent of the bouding box */
        inline cVector3d getExtent() const { return (m_extent); }

        /*  Set the center of the bouding box */
        inline void setCenter(const cVector3d& a_center) { m_center = a_center; }

        /*  Set the extent of the bouding box */
        inline void setExtent(const cVector3d& a_extent) { m_extent = a_extent; }

        /*  Set the center and extent of the box based on two points */
        inline void setValue(const cVector3d& a_min, const cVector3d& a_max)
        {
            m_extent = cMul(0.5, cSub(a_max, a_min));
            m_center = cAdd(a_min, a_extent);
            m_min = a_min;
            m_max = a_max;
        }
        /* Test whether this box contains the given point */
        inline bool contains(const cVector3d& a_p) const
        {
            if (a_p.x > m_min.x && a_p.y > m_min.y && a_p.z > m_min.z &&
                a_p.x < a_max.x && a_p.y < m_max.y && a_p.z < m_max.z)
                return (true);
            else
                return (false);
        }

        /*  Set the bounding box to bnoud the two given bounding box */
        inline void  enclose(const cCollisionAABBBox& a_boxA, const cCollisionAABBBox& a_boxB)
        {
            /* Find the minimum coordinate along each axis */
            cVector3d lower(cMin(a_boxA.getLowerX(), a_boxB.getLowerX()),
                            cMin(a_boxA.getLowerY(), a_boxB.getLowerY()),
                            cMin(a_boxA.getLowerZ(), a_boxB.getLowerZ()));

            /* Find the maximum coordinate along each axis */
            cVector3d upper(cMin(a_boxA.getUpperX(), a_boxB.getUpperX()),
                            cMin(a_boxA.getUpperY(), a_boxB.getUpperY()),
                            cMin(a_boxA.getUpperZ(), a_boxB.getUpperZ()));
            /* set the center and extent of the box to enclose the two extrem poionts */
            setValue(lower, upper);
        }

        /* Modify the bounding box as needed to bound the given point */

        inline void enclose (const cVector3d& a_point)
        {
            /* Decrease coordinate as needed to include given points */
            cVector3d lower(cMin(getLowerX(), a_point.x),
                            cMin(getLowerY(), a_point.y),
                            cMin(getLowerZ(), a_point.z));

            /* Iccrease coordinate as needed to include given points */
            cVector3d upper(cMin(getUpperX(), a_point.x),
                            cMin(getUpperY(), a_point.y),
                            cMin(getUpperZ(), a_point.z));

            /* Set the center and extent of the box to enclose the given point */
            setValue(lower, upper);
        }


        /* Modify the bounding box to bound another box */
        inline void enclose(const cCollisionAABBBox& a_box)
        {
            enclose(*this, a_box);
        }

        /* Initialize a bounding box to center at origin and ifinite extent */
        inline void setEmpty()
        {
            const double CHAI_INFINITY = 1.0e50;
            m_center.zero();
            m_extent = cVector3d(-CHAI_INFINITY, -CHAI_INFINITY, -CHAI_INFINITY);
            m_min_set(CHAI_INFINITY, CHAI_INFINITY, CHAI_INFINITY);
            m_max_set(-CHAI_INFINITY, -CHAI_INFINITY, -CHAI_INFINITY);
        }

        /* Return the smallest coordinate along X axis */
        inline double getLowerX() { return (m_min.x); }

        /* Return the smallest coordinate along Y axis */
        inline double getLowerY() { return (m_min.y); }

        /* Return the smallest coordinate along Z axis */
        inline double getLowerZ() { return (m_min.z); }

        /* Return the smallest coordinate along X axis */
        inline double getLowerX() { return (m_max.x); }

        /* Return the smallest coordinate along Y axis */
        inline double getLowerY() { return (m_max.y); }

        /* Return the smallest coordinate along Z axis */
        inline double getLowerZ() { return (m_max.z); }

        /* Return the length of the longest axis of the bounding box */
        double size() const;
        
        inline int longestAxis() const
        {
            if ((m_extent.x >= m_extent.y) && (m_extent.x >= m_extent.z)) 
                return 0;
            else if ((m_extent.y >= m_extent.x) && (m_extent.y >= m_extent.z))
                return 1;
            else
                return 2;

        }

        inline void render()
        {
            cDrawWireBox(m_min.x, m_max.x, m_min.y, m_max.y, m_min.z, m_max.z);
        }

        /* The center of the bounding box */
        cVector3d m_center;

        /* The extent of the bounding box */
        cVector3d m_extent;

        /* The minimum poiont of the bounding box */
        cVector3d m_min;

        /* The maximum poiont of the bounding box */
        cVector3d m_max;

}

#endif
