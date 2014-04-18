/*
 * =====================================================================================
 *
 *       Filename:  CColor.h
 *
 *    Description:  CColor.h
 *
 *        Version:  1.0
 *        Created:  04/06/2014 08:28:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Frost Wong (Qingchun Wang), frostwong@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef CColorH
#define CColorH

#include "extras/CGlobals.h"
#include "math/CMaths.h"

struct cColorb;
struct cColorf;

struct cColorf
{
    public:
        cColorf()
        {
            m_color[0] = 1.0;
            m_color[1] = 1.0;
            m_color[2] = 1.0;
            m_color[3] = 1.0;
        }

        cColorf(const GLfloat a_red, const GLfloat a_green, const GLfloat a_blue,
                const GLfloat a_alpha = 1.0)
        {
            m_color[0] = cClamp( a_red,     0.0f, 1.0f);
            m_color[1] = cClamp( a_green,   0.0f, 1.0f);
            m_color[2] = cClamp( a_blue,    0.0f, 1.0f);
            m_color[3] = cClamp( a_alpha,   0.0f, 1.0f);
        };

        ~cColorf() {};


        inline void set(const GLfloat a_red, const GLfloat a_green, const GLfloat a_blue,
                        const GLfloat a_alpha = 1.0)
        {
            m_color[0] = cClamp( a_red,     0.0f, 1.0f);
            m_color[1] = cClamp( a_green,   0.0f, 1.0f);
            m_color[2] = cClamp( a_blue,    0.0f, 1.0f);
            m_color[3] = cClamp( a_alpha,   0.0f, 1.0f);
        };

}
