#include "QtDrawToolGL.h"

namespace sofa::core::visual {

//==========
// TRIANGLES
//==========

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const Vec4f &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    {
        for (std::size_t i=0; i<points.size()/3; ++i)
        {
            const Vector3& a = points[ 3*i+0 ];
            const Vector3& b = points[ 3*i+1 ];
            const Vector3& c = points[ 3*i+2 ];
            Vector3 n = cross((b-a),(c-a));
            n.normalize();
            glNormal3d(n[0], n[1], n[2]);
            glColor4fv(color.ptr());
            glVertex3d(a[0], a[1], a[2]);
            glVertex3d(b[0], b[1], b[2]);
            glVertex3d(c[0], c[1], c[2]);
        }
    } glEnd();
    resetMaterial(color);
}


void QtDrawToolGL::setMaterial(const Base::Vec4f &color) {
    glColor4f(color[0],color[1],color[2],color[3]);
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &color[0]);
    static const float emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f};
    static const float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, emissive);
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf  (GL_FRONT_AND_BACK, GL_SHININESS, 20);
    if (color[3] < 1)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(0);
    }
    else
    {
        glDisable(GL_BLEND);
        glDepthMask(1);
    }
}

void QtDrawToolGL::resetMaterial(const Base::Vec4f &color) {
    if (color[3] < 1) {
        resetMaterial();
    }
}

void QtDrawToolGL::resetMaterial() {
    glDisable(GL_BLEND);
    glDepthMask(1);
}

} // namespace sofa::core::visual
