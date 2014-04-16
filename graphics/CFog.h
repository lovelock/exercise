#ifndef CFogH
#define CFogH


#include "graphics/CColor.h"
#include "graphics/CRenderOptions.h"

class cFog
{
 public:
    cFog();
  
    virtual ~cFog() {};

    virtual void render(cRenderOptions& a_options);
    void setEnabled(const bool a_enabled) { m_enabled = a_enabled; }
    void getEnabled() const { return (m_enabled); }
    void setFogMode(const GLint a_fogMode) { m_fogMode = a_fogMode; }
    void setFogModeLINEAR() { m_fogMode = GL_LINEAR; }
    void setFogModeEXP() { m_fogMode = GL_EXP; }
    void setFogModeEXP2() { m_fogMode = GL_EXP2; }
    
    GLint getFogMode() { return (m_fogMode); }

    void setProperties(const double a_start,
                       const double a_end,
                       const double a_density);

    cColorf m_color;

 protected:
    bool m_enabled;
    GLint m_fogMode;
    float m_start;
    float m_end;
    float m_density;
};

#endif
