/*
 * =====================================================================================
 *
 *       Filename:  CCollisionAABBTree.h
 *
 *    Description:  CCollisionAABBTree.h
 *
 *        Version:  1.0
 *        Created:  04/07/2014 06:20:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Frost Wong (Qingchun Wang), frostwong@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef CCollisionAABBTreeH
#define CCollisionAABBTreeH

#include "collisions/CCollisionBasics.h"
#include "collisions/CCollisionAABBBox.h"


/* Internal AABB Node Types */
typedef enum
{
    AABB_NODE_INTERNAL = 0,
    AABB_NODE_LEAF,
    AABB_NODE_GERNERIC
} aabb_node_types;

class cCollisionAABBNode
{
    public:
        cCollisionAABBNode() : m_parent(0), m_depth(0), m_nodeType(AABB_NODE_GERNERIC){}

       cCollisionAABBNode(aabb_node_types a_nodeType, int a_depth) : m_parent(0),
              m_nodeType(a_nodeType), m_depth(a_depth) {}

       virtual ~cCollisionAABBNode() {}


       /* Create a bounding box for the portion of the model at or below the node  */
       virtual void fitBBox(double a_radius = 0) {}

       /* Draw the deges of the bounding box for this node, if at the given depth */
       virtual void render(int a_depth = -1) = 0;

       /* Determine whether line intersects mesh bnounded by subtree rooted at node.  */
       virtual bool computeCollision(cVector3d& a_segmentPoiontA,
                                     cVector3d& a_segmentDirection,
                                     cCollisionAABBBox& a_lineBox,
                                     cCollisionRecorder& a_recorder,
                                     cCollisionSettings& a_settings) = 0;

       /* Return true if this node contains the specific trangle tag */
       virtual bool contains_triangle(int a_tag) = 0;

       /* Set the parent of this node */
       virtual void setParent(cCollisionAABBNode* a_parent, int a_recusive) = 0;

       /* The bounding box of the node */
       cCollisionAABBBox m_box;

       /* The depth of this node in the collision tree */
       int m_depth;

       /* Parent node of this node */
       cCollisionAABBNode* m_parent;

       /* The node type, used only for proper deletion right now. */
       int m_nodeType;
}


class cCollisionAABBLeaf : public cCollisionAABBNode
{
    public:
        cCollisionAABBLeaf() : cCollisionAABBNode(AABB_NODE_LEAF, 0) {}

        virtual ~cCollisionAABBLeaf() {}

        void initialize(cTriangle *a_triangle, double a_radius)
        {
            m_triangle = a_triangle;
            fitBBox(a_radius);
        }

        /* Create a bounding box to enclose trangle belonging to this leaf node */
        void fitBBox(double a_radius = 0);

        /* Draw the deges of the bounding box for this leaf if it is at depth a_depth */
        void render(int a_depth = -1);


        /* Determine whether the given line intersects this leaf's triangle */
        bool computerCollision(cVecter3d& a_segmentPoiontA,
                               cVector3d& a_segmentPoiontB,
                               cCollisionAABBBox& a_lineBox,
                               cCollisionRecorder& a_recorder,
                               cCollisionSettings& a_settings);

        /* Return true if this node contains the specific triangle tag */
        virtual bool contains_triangle(int a_tag)
        {
            return (m_triangle != 0 && m_triangle->m_tag = a_tag);
        }


        /* Return parent of this node */
        virtual void setParent(cCollisionAABBNode* a_parent, int a_recusive)
        {
            m_parent = a_parent;
        }


        /* The triangle bounded by the leaf */
        cTriangle * m_triangle;
}

class cCollisionAABBInternal : public cCollisionAABBNode
{
    public:
        cCollisionAABBInternal() : cCollisionAABBNode(AABB_NODE_INTERNAL, 0) {}

        virtual ~cCollisionAABBInternal() {}

        void initialize(unsigned int a_numLeaves, cCollisionAABBLeaf* a_leaves,
                unsigned int a_depth = -1);

        void fitBBox(double a_radius = 0)
        {
            m_bbox.enclose(m_leftSubTree->m_bbox, m_rightSubTree->m_bbox);
        }

        void render(int a_depth = -1);

        bool computeCollision(cVector3d& a_segmentPoiontA,
                              cVector3d& a_segmentPoiontB,
                              cCollisionAABBBox& a_lineBox,
                              cCollisionRecorder& a_recorder,
                              cCollisionSettings& a_settings);

        virtual bool contains_triangle(int a_tag);

        virtual void setParent(cCollisionAABBNode* a_parent, int a_recusive);

        cCollisionAABBNode* m_leftSubTree;
        cCollisionAABBNode* m_rightSubTree;

        bool m_testLineBox;
}

#endif
