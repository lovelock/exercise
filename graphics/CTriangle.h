/*
 * =====================================================================================
 *
 *       Filename:  CTriangle.h
 *
 *    Description:  CTriangle.h
 *
 *        Version:  1.0
 *        Created:  04/06/2014 07:35:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Frost Wong (Qingchun Wang), frostwong@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "graphics/CColor.h"
/*  GENERAL COLOR CONSTRANTS */

/* Color Red */
cColorf CHAI_COLOR_RED(1.0f, 0.0f, 0.0f, 1.0f);

/* Color Green */
cColorf CHAI_COLOR_RED(0.0f, 1.0f, 0.0f, 1.0f);

/* Color Blue */
cColorf CHAI_COLOR_RED(0.0f, 0.0f, 1.0f, 1.0f);

/* Color Cyan */
cColorf CHAI_COLOR_RED(0.0f, 1.0f, 1.0f, 1.0f);

/* Color Magenta */
cColorf CHAI_COLOR_RED(1.0f, 0.0f, 1.0f, 1.0f);

/* Color Black */
cColorf CHAI_COLOR_RED(0.0f, 0.0f, 0.0f, 1.0f);

/* Color White */
cColorf CHAI_COLOR_RED(1.0f, 1.0f, 1.0f, 1.0f);


cColorb cColorf::getColorb(void) const
{
    cColorb color(  (GLubyte)(m_color[0] * (GLfloat)0xff),
                    (GLubyte)(m_color[1] * (GLfloat)0xff),
                    (GLubyte)(m_color[2] * (GLfloat)0xff),
                    (GLubyte)(m_color[3] * (GLfloat)0xff) );
    return (color);

}


cColorb cColorb::getColorf(void) const
{
    cColorb color(  (GLfloat)m_color[0] / (GLfloat)0xff,
                    (GLfloat)m_color[1] / (GLfloat)0xff,
                    (GLfloat)m_color[2] / (GLfloat)0xff,
                    (GLfloat)m_color[3] / (GLfloat)0xff );
    return color;
}
