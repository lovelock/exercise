#ifndef  CDisplayListH
#define  CDisplayListH

#include "system/CGlobals.h"

/* 用来提供一个控制OpenGl显式的创建的简单的结构 */
class cDisplayList
{
    public:
        cDisplayList();
        virtual ~cDisplayList();

    public:
        void invalidate();
        bool render(const bool a_useDisplayList = true);
        bool begin(const bool a_useDisplayList = true);
        void end(const bool a_executeDisplayList = true);
        unsigned int getDisplayListGL() { return (m_displayList); }

    private:
        unsigned int m_displayList;
        bool m_flagCreationDisplayList;
}

#endif
