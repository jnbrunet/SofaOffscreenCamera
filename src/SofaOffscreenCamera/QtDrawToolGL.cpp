#include <sofa/helper/logging/Messaging.h>
#include "QtDrawToolGL.h"

namespace sofa::helper::visual {

//=========
// MATERIAL
//=========

void QtDrawToolGL::setMaterial(const Base::RGBAColor &color) {
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

void QtDrawToolGL::resetMaterial(const Base::RGBAColor &color) {
    if (color[3] < 1) {
        resetMaterial();
    }
}

void QtDrawToolGL::resetMaterial() {
    glDisable(GL_BLEND);
    glDepthMask(1);
}

//=======
// POINTS
//=======

void internalDrawPoint(const QtDrawToolGL::Vector3 &p, const QtDrawToolGL::RGBAColor &c)
{
    glColor4f(c[0],c[1],c[2],c[3]);
    glVertex3d(p[0],p[1],p[2]);
}

void internalDrawPoint(const QtDrawToolGL::Vector3 &p, const QtDrawToolGL::Vector3 &n, const QtDrawToolGL::RGBAColor &c)
{
    glColor4f(c[0],c[1],c[2],c[3]);
    glNormal3d(n[0],n[1],n[2]);
    glVertex3d(p[0],p[1],p[2]);
}

void QtDrawToolGL::drawPoint(const QtDrawToolGL::Vector3 &p, const QtDrawToolGL::RGBAColor &c) {
    glBegin(GL_POINTS);
    internalDrawPoint(p,c);
    glEnd();
}

void QtDrawToolGL::drawPoint(const QtDrawToolGL::Vector3 &p, const QtDrawToolGL::Vector3 &n,
                             const QtDrawToolGL::RGBAColor &c) {
    glBegin(GL_POINTS);
    internalDrawPoint(p, n, c);
    glEnd();
}

void QtDrawToolGL::drawPoints(const std::vector<Vector3> &points, float size, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glPointSize(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_POINTS);
    {
        for (const auto & point : points)
        {
            internalDrawPoint(point, color);
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    resetMaterial(color);
    glPointSize(1);
}

void QtDrawToolGL::drawPoints(const std::vector<Vector3> &points, float size, const std::vector<RGBAColor> &color) {
    glPointSize(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_POINTS);
    {
        for (std::size_t i=0; i<points.size(); ++i)
        {
            setMaterial(color[i]);
            internalDrawPoint(points[i], color[i]);
            resetMaterial(color[i]);
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    glPointSize(1);
}

//======
// Lines
//======

void internalDrawLine(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2, const QtDrawToolGL::RGBAColor& color)
{
    internalDrawPoint(p1, color );
    internalDrawPoint(p2, color );
}

void QtDrawToolGL::drawLine(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                            const QtDrawToolGL::RGBAColor &color) {
    glBegin(GL_LINES);
    internalDrawLine(p1,p2,color);
    glEnd();
}

void QtDrawToolGL::drawInfiniteLine(const QtDrawToolGL::Vector3 &point, const QtDrawToolGL::Vector3 &direction,
                                    const QtDrawToolGL::RGBAColor &color) {
    glBegin(GL_LINES);
    glColor4f(color[0],color[1],color[2],color[3]);
    glVertex4d(point[0], point[1], point[2], 1.0);
    glVertex4d(direction[0], direction[1], direction[2], 0.0);
    glEnd();
}

void QtDrawToolGL::drawLines(const std::vector<Vector3> &points, float size, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glLineWidth(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_LINES);
    {
        const std::size_t nb_lines = points.size()/2;
        for (std::size_t i=0; i<nb_lines; ++i) {
            internalDrawLine(points[2*i],points[2*i+1]  , color );
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    glLineWidth(1);
    resetMaterial(color);
}

void QtDrawToolGL::drawLines(const std::vector<Vector3> &points, float size, const std::vector<RGBAColor> &colors) {
    if (points.size() != colors.size()*2) {
        msg_warning("DrawToolGL") << "Sizes mismatch in drawLines method, points.size(): " << points.size() << " should be equal to colors.size()*2: " << colors.size()*2;
        return drawLines(points, size, RGBAColor::red());
    }

    glLineWidth(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_LINES);
    {
        const std::size_t nb_lines = points.size()/2;
        for (std::size_t i=0; i<nb_lines; ++i){
            setMaterial(colors[i]);
            internalDrawLine(points[2*i],points[2*i+1], colors[i] );
            resetMaterial(colors[i]);
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    glLineWidth(1);
}

void QtDrawToolGL::drawLines(const std::vector<Vector3> &points, const std::vector<Vec2i> &index, float size,
                             const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glLineWidth(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_LINES);
    {
        for (auto i : index) {
            internalDrawLine(points[ i[0] ],points[ i[1] ], color );
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    resetMaterial(color);
    glLineWidth(1);
}

void QtDrawToolGL::drawLineStrip(const std::vector<Vector3> &points, float size, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glLineWidth(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_LINE_STRIP);
    {
        for (const auto & point : points) {
            internalDrawPoint(point  , color );
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    resetMaterial(color);
    glLineWidth(1);
}

void QtDrawToolGL::drawLineLoop(const std::vector<Vector3> &points, float size, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glLineWidth(size);
    if (getLightEnabled())
        disableLighting();
    glBegin(GL_LINE_LOOP);
    {
        for (const auto & point : points) {
            internalDrawPoint(point  , color );
        }
    }
    glEnd();
    if (getLightEnabled())
        enableLighting();
    resetMaterial(color);
    glLineWidth(1);
}

//==========
// TRIANGLES
//==========

void internalDrawTriangle(const QtDrawToolGL::Vector3 &p1,const QtDrawToolGL::Vector3 &p2,const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &n)
{
    glNormal3d(n[0],n[1],n[2]);
    glVertex3d(p1[0],p1[1],p1[2]);
    glVertex3d(p2[0],p2[1],p2[2]);
    glVertex3d(p3[0],p3[1],p3[2]);
}

void internalDrawTriangle( const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2, const QtDrawToolGL::Vector3 &p3,
                           const QtDrawToolGL::Vector3 &n, const  QtDrawToolGL::RGBAColor &c)
{
    glNormal3d(n[0],n[1],n[2]);
    glColor4fv(c.array());
    glVertex3d(p1[0],p1[1],p1[2]);
    glVertex3d(p2[0],p2[1],p2[2]);
    glVertex3d(p3[0],p3[1],p3[2]);
}

void internalDrawTriangle(const QtDrawToolGL::Vector3 &p1,const QtDrawToolGL::Vector3 &p2,const QtDrawToolGL::Vector3 &p3,
                          const QtDrawToolGL::Vector3 &n,
                          const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2, const QtDrawToolGL::RGBAColor &c3)
{
    glNormal3d(n[0],n[1],n[2]);
    glColor4fv(c1.array());
    glVertex3d(p1[0],p1[1],p1[2]);
    glColor4fv(c2.array());
    glVertex3d(p2[0],p2[1],p2[2]);
    glColor4fv(c3.array());
    glVertex3d(p3[0],p3[1],p3[2]);
}

void internalDrawTriangle(const QtDrawToolGL::Vector3 &p1,   const QtDrawToolGL::Vector3 &p2,   const QtDrawToolGL::Vector3 &p3,
                          const QtDrawToolGL::Vector3 &n1,   const QtDrawToolGL::Vector3 &n2,   const QtDrawToolGL::Vector3 &n3,
                          const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2, const QtDrawToolGL::RGBAColor &c3)
{
    glNormal3d(n1[0],n1[1],n1[2]);
    glColor4fv(c1.array());
    glVertex3d(p1[0],p1[1],p1[2]);

    glNormal3d(n2[0],n2[1],n2[2]);
    glColor4fv(c2.array());
    glVertex3d(p2[0],p2[1],p2[2]);

    glNormal3d(n3[0],n3[1],n3[2]);
    glColor4fv(c3.array());
    glVertex3d(p3[0],p3[1],p3[2]);
}


void QtDrawToolGL::drawTriangle(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                                const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &normal) {
    glBegin(GL_TRIANGLES);
    internalDrawTriangle(p1, p2, p3, normal);
    glEnd();
}

void QtDrawToolGL::drawTriangle(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                                const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &normal,
                                const QtDrawToolGL::RGBAColor &c) {
    glBegin(GL_TRIANGLES);
    internalDrawTriangle(p1, p2, p3, normal, c);
    glEnd();
}

void QtDrawToolGL::drawTriangle(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                                const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &normal,
                                const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2,
                                const QtDrawToolGL::RGBAColor &c3) {
    glBegin(GL_TRIANGLES);
    internalDrawTriangle(p1, p2, p3, normal, c1, c2, c3);
    glEnd();
}

void QtDrawToolGL::drawTriangle(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                                const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &normal1,
                                const QtDrawToolGL::Vector3 &normal2, const QtDrawToolGL::Vector3 &normal3,
                                const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2,
                                const QtDrawToolGL::RGBAColor &c3) {
    glBegin(GL_TRIANGLES);
    internalDrawTriangle(p1, p2, p3, normal1, normal2, normal3, c1, c2, c3);
    glEnd();
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    {
        const std::size_t nb_triangles = points.size()/3;
        for (std::size_t i=0; i<nb_triangles; ++i)
        {
            const Vector3& a = points[ 3*i+0 ];
            const Vector3& b = points[ 3*i+1 ];
            const Vector3& c = points[ 3*i+2 ];
            Vector3 n = cross((b-a),(c-a));
            n.normalize();
            internalDrawTriangle(a,b,c,n,color);
        }
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const std::vector<RGBAColor> &color) {
    std::vector<Vector3> normal;
    normal.clear();
    this->drawTriangles(points,normal,color);
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const QtDrawToolGL::Vector3 &normal,
                                 const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    {
        const std::size_t nb_triangles = points.size()/3;
        for (std::size_t i=0; i<nb_triangles; ++i)
            internalDrawTriangle(points[ 3*i+0 ],points[ 3*i+1 ],points[ 3*i+2 ], normal, color);
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const std::vector<Vec3i> &index,
                                 const std::vector<Vector3> &normal, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    {
        const std::size_t nb_triangles = index.size();
        for (std::size_t i=0; i<nb_triangles; ++i) {
            internalDrawTriangle(points[ index[i][0] ],points[ index[i][1] ],points[ index[i][2] ],normal[i],color);
        }
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const std::vector<Vec3i> &index,
                                 const std::vector<Vector3> &normal, const std::vector<RGBAColor> &color) {
    //todo !
    SOFA_UNUSED(points);
    SOFA_UNUSED(index);
    SOFA_UNUSED(normal);
    SOFA_UNUSED(color);
}

void QtDrawToolGL::drawTriangles(const std::vector<Vector3> &points, const std::vector<Vector3> &normal,
                                 const std::vector<RGBAColor> &color) {
    const std::size_t nbTriangles=points.size()/3;
    bool computeNormals= (normal.size() != nbTriangles);
    if (nbTriangles == 0) return;
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    setMaterial(color[0]);
    glBegin(GL_TRIANGLES);
    {
        for (std::size_t i=0; i<nbTriangles; ++i)
        {
            if (!computeNormals)
            {
                internalDrawTriangle(points[3*i+0],points[3*i+1],points[3*i+2],normal[i],
                                     color[3*i+0],color[3*i+1],color[3*i+2]);
            }
            else
            {
                const Vector3& a = points[ 3*i+0 ];
                const Vector3& b = points[ 3*i+1 ];
                const Vector3& c = points[ 3*i+2 ];
                Vector3 n = cross((b-a),(c-a));
                n.normalize();

                internalDrawPoint(a,n,color[3*i+0]);
                internalDrawPoint(b,n,color[3*i+1]);
                internalDrawPoint(c,n,color[3*i+2]);

            }
        }
    } glEnd();
    glDisable(GL_COLOR_MATERIAL);
    resetMaterial(color[0]);
}

void QtDrawToolGL::drawTriangleStrip(const std::vector<Vector3> &points, const std::vector<Vector3> &normal,
                                     const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLE_STRIP);
    {
        const std::size_t nb_triangles = normal.size();
        for (std::size_t i=0; i<nb_triangles; ++i)
        {
            const auto n = normal[i];
            const auto p1 = points[2*i];
            const auto p2 = points[2*i+1];
            glNormal3d(n[0],n[1],n[2]);
            glVertex3d(p1[0],p1[1],p1[2]);
            glVertex3d(p2[0],p2[1],p2[2]);
        }
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawTriangleFan(const std::vector<Vector3> &points, const std::vector<Vector3> &n,
                                   const QtDrawToolGL::RGBAColor &color) {
    if (points.size() < 3) return;
    setMaterial(color);
    glBegin(GL_TRIANGLE_FAN);

    glNormal3d(n[0][0],n[0][1],n[0][2]);
    glVertex3d(points[0][0],points[0][1],points[0][2]);
    glVertex3d(points[1][0],points[1][1],points[1][2]);
    glVertex3d(points[2][0],points[2][1],points[2][2]);

    const std::size_t nb_of_points = points.size();
    for (std::size_t i=3; i<nb_of_points; ++i)
    {
        glNormal3d(n[i][0],n[i][1],n[i][2]);
        glVertex3d(points[i][0],points[i][1],points[i][2]);
    }

    glEnd();
    resetMaterial(color);
}

//======
// QUADS
//======

void internalDrawQuad(const QtDrawToolGL::Vector3 &p1,const QtDrawToolGL::Vector3 &p2,const QtDrawToolGL::Vector3 &p3,const QtDrawToolGL::Vector3 &p4,
                      const QtDrawToolGL::Vector3 &n)
{
    glNormal3d(n[0],n[1],n[2]);
    glVertex3d(p1[0],p1[1],p1[2]);
    glVertex3d(p2[0],p2[1],p2[2]);
    glVertex3d(p3[0],p3[1],p3[2]);
    glVertex3d(p4[0],p4[1],p4[2]);
}

void internalDrawQuad(const QtDrawToolGL::Vector3 &p1,const QtDrawToolGL::Vector3 &p2,const QtDrawToolGL::Vector3 &p3,const QtDrawToolGL::Vector3 &p4,
                      const QtDrawToolGL::Vector3 &n, const QtDrawToolGL::RGBAColor &c)
{
    glColor4fv(c.array());
    internalDrawQuad(p1, p2, p3, p4, n);
}

void internalDrawQuad(const QtDrawToolGL::Vector3 &p1,const QtDrawToolGL::Vector3 &p2,const QtDrawToolGL::Vector3 &p3,const QtDrawToolGL::Vector3 &p4,
                      const QtDrawToolGL::Vector3 &n,
                      const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2, const QtDrawToolGL::RGBAColor &c3, const QtDrawToolGL::RGBAColor &c4)
{
    glNormal3d(n[0],n[1],n[2]);
    glColor4fv(c1.array());
    glVertex3d(p1[0],p1[1],p1[2]);
    glColor4fv(c2.array());
    glVertex3d(p2[0],p2[1],p2[2]);
    glColor4fv(c3.array());
    glVertex3d(p3[0],p3[1],p3[2]);
    glColor4fv(c4.array());
    glVertex3d(p4[0],p4[1],p4[2]);
}

void internalDrawQuad(const QtDrawToolGL::Vector3 &p1,   const QtDrawToolGL::Vector3 &p2,   const QtDrawToolGL::Vector3 &p3,   const QtDrawToolGL::Vector3 &p4,
                      const QtDrawToolGL::Vector3 &n1,   const QtDrawToolGL::Vector3 &n2,   const QtDrawToolGL::Vector3 &n3,   const QtDrawToolGL::Vector3 &n4,
                      const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2, const QtDrawToolGL::RGBAColor &c3, const QtDrawToolGL::RGBAColor &c4)
{
    internalDrawPoint(p1, n1, c1);
    internalDrawPoint(p2, n2, c2);
    internalDrawPoint(p3, n3, c3);
    internalDrawPoint(p4, n4, c4);
}

void QtDrawToolGL::drawQuad(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                            const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &p4,
                            const QtDrawToolGL::Vector3 &normal) {
    glBegin(GL_QUADS);
    internalDrawQuad(p1, p2, p3, p4, normal);
    glEnd();
}

void QtDrawToolGL::drawQuad(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                            const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &p4,
                            const QtDrawToolGL::Vector3 &normal, const QtDrawToolGL::RGBAColor &c) {
    glBegin(GL_QUADS);
    internalDrawQuad(p1, p2, p3, p4, normal, c);
    glEnd();
}

void QtDrawToolGL::drawQuad(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                            const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &p4,
                            const QtDrawToolGL::Vector3 &normal, const QtDrawToolGL::RGBAColor &c1,
                            const QtDrawToolGL::RGBAColor &c2, const QtDrawToolGL::RGBAColor &c3,
                            const QtDrawToolGL::RGBAColor &c4) {
    glBegin(GL_QUADS);
    internalDrawQuad(p1, p2, p3, p4, normal, c1, c2, c3, c4);
    glEnd();
}

void QtDrawToolGL::drawQuad(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2,
                            const QtDrawToolGL::Vector3 &p3, const QtDrawToolGL::Vector3 &p4,
                            const QtDrawToolGL::Vector3 &normal1, const QtDrawToolGL::Vector3 &normal2,
                            const QtDrawToolGL::Vector3 &normal3, const QtDrawToolGL::Vector3 &normal4,
                            const QtDrawToolGL::RGBAColor &c1, const QtDrawToolGL::RGBAColor &c2,
                            const QtDrawToolGL::RGBAColor &c3, const QtDrawToolGL::RGBAColor &c4) {
    glBegin(GL_QUADS);
    internalDrawQuad(p1, p2, p3, p4, normal1, normal2, normal3, normal4, c1, c2, c3, c4);
    glEnd();
}

void QtDrawToolGL::drawQuads(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_QUADS);
    {
        const std::size_t nb_quads = points.size()/4;
        for (std::size_t i=0; i<nb_quads; ++i) {
            const Vector3& a = points[ 4*i+0 ];
            const Vector3& b = points[ 4*i+1 ];
            const Vector3& c = points[ 4*i+2 ];
            const Vector3& d = points[ 4*i+3 ];
            Vector3 n = cross((b-a),(c-a));
            n.normalize();
            internalDrawQuad(a,b,c,d,n,color);
        }
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawQuads(const std::vector<Vector3> &points, const std::vector<RGBAColor> &colors) {
    glBegin(GL_QUADS);
    {
        const std::size_t nb_quads = points.size()/4;
        for (std::size_t i=0; i<nb_quads; ++i) {
            const Vector3& a = points[ 4*i+0 ];
            const Vector3& b = points[ 4*i+1 ];
            const Vector3& c = points[ 4*i+2 ];
            const Vector3& d = points[ 4*i+3 ];

            const RGBAColor& col_a = colors[ 4*i+0 ];
            const RGBAColor& col_b = colors[ 4*i+1 ];
            const RGBAColor& col_c = colors[ 4*i+2 ];
            const RGBAColor& col_d = colors[ 4*i+3 ];

            RGBAColor average_color;
            for(int j=0; j < 4; j++) {
                average_color[j] = (col_a[j] + col_b[j] + col_c[j] + col_d[j]) * 0.25f;
            }

            Vector3 n = cross((b-a),(c-a));
            n.normalize();
            internalDrawQuad(a,b,c,d,n,average_color);
        }
    } glEnd();
}

//=============
// TETRAHEDRONS
//=============

void QtDrawToolGL::drawTetrahedron(const QtDrawToolGL::Vector3 &p0, const QtDrawToolGL::Vector3 &p1,
                                   const QtDrawToolGL::Vector3 &p2, const QtDrawToolGL::Vector3 &p3,
                                   const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    {
        internalDrawTriangle(p0,p1,p2, cross((p1-p0),(p2-p0)), color);
        internalDrawTriangle(p0,p1,p3, cross((p1-p0),(p3-p0)), color);
        internalDrawTriangle(p0,p2,p3, cross((p2-p0),(p3-p0)), color);
        internalDrawTriangle(p1,p2,p3, cross((p2-p1),(p3-p1)), color);
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawTetrahedra(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    for (auto it = points.begin(), end = points.end(); it != end;) {
        const Vector3& p0 = *(it++);
        const Vector3& p1 = *(it++);
        const Vector3& p2 = *(it++);
        const Vector3& p3 = *(it++);

        internalDrawTriangle(p0, p1, p2, cross((p1 - p0), (p2 - p0)), color);
        internalDrawTriangle(p0, p1, p3, cross((p1 - p0), (p3 - p0)), color);
        internalDrawTriangle(p0, p2, p3, cross((p2 - p0), (p3 - p0)), color);
        internalDrawTriangle(p1, p2, p3, cross((p2 - p1), (p3 - p1)), color);
    }
    glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawScaledTetrahedra(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color,
                                        const float scale) {
    setMaterial(color);
    glBegin(GL_TRIANGLES);
    for (auto it = points.begin(), end = points.end(); it != end;)
    {
        const Vector3& p0 = *(it++);
        const Vector3& p1 = *(it++);
        const Vector3& p2 = *(it++);
        const Vector3& p3 = *(it++);

        Vector3 center = (p0 + p1 + p2 + p3) / 4.0;

        Vector3 np0 = ((p0 - center)*scale) + center;
        Vector3 np1 = ((p1 - center)*scale) + center;
        Vector3 np2 = ((p2 - center)*scale) + center;
        Vector3 np3 = ((p3 - center)*scale) + center;

        internalDrawTriangle(np0, np1, np2, cross((p1 - p0), (p2 - p0)), color);
        internalDrawTriangle(np0, np1, np3, cross((p1 - p0), (p3 - p0)), color);
        internalDrawTriangle(np0, np2, np3, cross((p2 - p0), (p3 - p0)), color);
        internalDrawTriangle(np1, np2, np3, cross((p2 - p1), (p3 - p1)), color);
    }
    glEnd();
    resetMaterial(color);
}

//============
// HEXAHEDRONS
//============

void QtDrawToolGL::drawHexahedron(const QtDrawToolGL::Vector3 &p0, const QtDrawToolGL::Vector3 &p1,
                                  const QtDrawToolGL::Vector3 &p2, const QtDrawToolGL::Vector3 &p3,
                                  const QtDrawToolGL::Vector3 &p4, const QtDrawToolGL::Vector3 &p5,
                                  const QtDrawToolGL::Vector3 &p6, const QtDrawToolGL::Vector3 &p7,
                                  const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);
    glBegin(GL_QUADS);
    {
        internalDrawQuad(p0, p1, p2, p3, cross((p1 - p0), (p2 - p0)), color);
        internalDrawQuad(p4, p7, p6, p5, cross((p7 - p5), (p6 - p5)), color);
        internalDrawQuad(p1, p0, p4, p5, cross((p0 - p1), (p4 - p1)), color);
        internalDrawQuad(p1, p5, p6, p2, cross((p5 - p1), (p6 - p1)), color);
        internalDrawQuad(p2, p6, p7, p3, cross((p6 - p2), (p7 - p2)), color);
        internalDrawQuad(p0, p3, p7, p4, cross((p3 - p0), (p7 - p0)), color);
    } glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawHexahedra(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color) {
    setMaterial(color);

    glBegin(GL_QUADS);
    for (auto it = points.begin(), end = points.end(); it != end;)
    {
        const Vector3& p0 = *(it++);
        const Vector3& p1 = *(it++);
        const Vector3& p2 = *(it++);
        const Vector3& p3 = *(it++);
        const Vector3& p4 = *(it++);
        const Vector3& p5 = *(it++);
        const Vector3& p6 = *(it++);
        const Vector3& p7 = *(it++);

        internalDrawQuad(p0, p1, p2, p3, cross((p1 - p0), (p2 - p0)), color);
        internalDrawQuad(p4, p7, p6, p5, cross((p7 - p5), (p6 - p5)), color);
        internalDrawQuad(p1, p0, p4, p5, cross((p0 - p1), (p4 - p1)), color);
        internalDrawQuad(p1, p5, p6, p2, cross((p5 - p1), (p6 - p1)), color);
        internalDrawQuad(p2, p6, p7, p3, cross((p6 - p2), (p7 - p2)), color);
        internalDrawQuad(p0, p3, p7, p4, cross((p3 - p0), (p7 - p0)), color);
    }
    glEnd();
    resetMaterial(color);
}

void QtDrawToolGL::drawScaledHexahedra(const std::vector<Vector3> &points, const QtDrawToolGL::RGBAColor &color,
                                       const float scale) {
    setMaterial(color);

    glBegin(GL_QUADS);
    for (auto it = points.begin(), end = points.end(); it != end;)
    {
        const Vector3& p0 = *(it++);
        const Vector3& p1 = *(it++);
        const Vector3& p2 = *(it++);
        const Vector3& p3 = *(it++);
        const Vector3& p4 = *(it++);
        const Vector3& p5 = *(it++);
        const Vector3& p6 = *(it++);
        const Vector3& p7 = *(it++);

        //barycenter
        Vector3 center = (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7)/8.0;

        Vector3 np0 = ((p0 - center)*scale) + center;
        Vector3 np1 = ((p1 - center)*scale) + center;
        Vector3 np2 = ((p2 - center)*scale) + center;
        Vector3 np3 = ((p3 - center)*scale) + center;
        Vector3 np4 = ((p4 - center)*scale) + center;
        Vector3 np5 = ((p5 - center)*scale) + center;
        Vector3 np6 = ((p6 - center)*scale) + center;
        Vector3 np7 = ((p7 - center)*scale) + center;

        internalDrawQuad(np0, np1, np2, np3, cross((p1 - p0), (p2 - p0)), color);
        internalDrawQuad(np4, np7, np6, np5, cross((p7 - p5), (p6 - p5)), color);
        internalDrawQuad(np1, np0, np4, np5, cross((p0 - p1), (p4 - p1)), color);
        internalDrawQuad(np1, np5, np6, np2, cross((p5 - p1), (p6 - p1)), color);
        internalDrawQuad(np2, np6, np7, np3, cross((p6 - p2), (p7 - p2)), color);
        internalDrawQuad(np0, np3, np7, np4, cross((p3 - p0), (p7 - p0)), color);
    }
    glEnd();
    resetMaterial(color);
}

//==============
// MISCELLANEOUS
//==============

void
QtDrawToolGL::drawCone(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2, float radius1, float radius2,
                       const QtDrawToolGL::RGBAColor &color, int subd) {
    Vector3 tmp = p2-p1;
    setMaterial(color);
    /* create Vectors p and q, co-planar with the cylinder's cross-sectional disk */
    Vector3 p=tmp;
    if (fabs(p[0]) + fabs(p[1]) < 0.00001*tmp.norm())
        p[0] += 1.0;
    else
        p[2] += 1.0;
    Vector3 q;
    q = p.cross(tmp);
    p = tmp.cross(q);
    /* do the normalization outside the segment loop */
    p.normalize();
    q.normalize();

    int i2;

    /* build the cylinder from rectangular subd */
    std::vector<Vector3> points;
    std::vector<Vector3> normals;

    std::vector<Vector3> pointsCloseCylinder1;
    std::vector<Vector3> normalsCloseCylinder1;
    std::vector<Vector3> pointsCloseCylinder2;
    std::vector<Vector3> normalsCloseCylinder2;

    Vector3 dir=p1-p2; dir.normalize();
    pointsCloseCylinder1.push_back(p1);
    normalsCloseCylinder1.push_back(dir);
    pointsCloseCylinder2.push_back(p2);
    normalsCloseCylinder2.push_back(-dir);


    for (i2=0 ; i2<=subd ; i2++)
    {
        /* sweep out a circle */
        float theta =  (float)( i2 * 2.0f * M_PI / subd );
        float st = sin(theta);
        float ct = cos(theta);
        /* construct normal */
        tmp = p*ct+q*st;
        /* set the normal for the two subseqent points */
        normals.push_back(tmp);

        /* point on disk 1 */
        Vector3 w(p1);
        w += tmp*fabs(radius1);
        points.push_back(w);
        pointsCloseCylinder1.push_back(w);
        normalsCloseCylinder1.push_back(dir);

        /* point on disk 2 */
        w=p2;
        w += tmp*fabs(radius2);
        points.push_back(w);
        pointsCloseCylinder2.push_back(w);
        normalsCloseCylinder2.push_back(-dir);
    }
    pointsCloseCylinder1.push_back(pointsCloseCylinder1[1]);
    normalsCloseCylinder1.push_back(normalsCloseCylinder1[1]);
    pointsCloseCylinder2.push_back(pointsCloseCylinder2[1]);
    normalsCloseCylinder2.push_back(normalsCloseCylinder2[1]);


    drawTriangleStrip(points, normals,color);
    if (radius1 > 0) drawTriangleFan(pointsCloseCylinder1, normalsCloseCylinder1,color);
    if (radius2 > 0) drawTriangleFan(pointsCloseCylinder2, normalsCloseCylinder2,color);

    resetMaterial(color);
}

void
QtDrawToolGL::drawDisk(float radius, double from, double to, int resolution, const QtDrawToolGL::RGBAColor &color) {
    if (from > to)
        to += 2.0 * M_PI;
    glBegin(GL_TRIANGLES);
    {
        glColor4f(color.r(), color.g(), color.b(), color.a());
        bool first = true;
        float prev_alpha = 0;
        float prev_beta = 0;
        bool stop = false;
        for (int i  = 0 ; i <= resolution ; ++i)
        {
            double angle = (double(i) / double(resolution) * 2.0 * M_PI) + from;
            if(angle >= to)
            {
                angle = to;
                stop = true;
            }
            float alpha = float(std::sin(angle));
            float beta = float(std::cos(angle));

            if (first)
            {
                first = false;
                prev_alpha = alpha;
                prev_beta = beta;
            }
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(radius * prev_alpha, radius * prev_beta, 0.0);
            glVertex3f(radius * alpha, radius * beta, 0.0);
            if (stop)
                break;
            prev_alpha = alpha;
            prev_beta = beta;
        }
    }
    glEnd();
}

void QtDrawToolGL::drawCircle(float radius, float lineThickness, int resolution, const QtDrawToolGL::RGBAColor &color) {
    glLineWidth(lineThickness);
    glEnable(GL_LINE_SMOOTH);

    glBegin(GL_LINE_STRIP);
    {
        glColor4f(color.r(), color.g(), color.b(), color.a());
        for (int i  = 0 ; i <= resolution ; ++i)
        {
            float angle = float(double(i) / double(resolution) * 2.0 * M_PI);
            float alpha = std::sin(angle);
            float beta = std::cos(angle);

            glVertex3f(radius * alpha, radius * beta, 0.0);
        }
    }
    glEnd();

    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
}

void QtDrawToolGL::drawCube(const float &radius, const QtDrawToolGL::RGBAColor &color, const int &subd) {
    // X Axis
    drawCylinder( Vector3(-1.0, -1.0, -1.0), Vector3(1.0, -1.0, -1.0), radius, color, subd);
    drawCylinder( Vector3(-1.0,  1.0, -1.0), Vector3(1.0,  1.0, -1.0), radius, color, subd);
    drawCylinder( Vector3(-1.0, -1.0,  1.0), Vector3(1.0, -1.0,  1.0), radius, color, subd);
    drawCylinder( Vector3(-1.0,  1.0,  1.0), Vector3(1.0,  1.0,  1.0), radius, color, subd);
    // Y Axis
    drawCylinder( Vector3(-1.0, -1.0, -1.0), Vector3(-1.0, 1.0, -1.0), radius, color, subd);
    drawCylinder( Vector3(-1.0, -1.0,  1.0), Vector3(-1.0, 1.0,  1.0), radius, color, subd);
    drawCylinder( Vector3( 1.0, -1.0, -1.0), Vector3( 1.0, 1.0, -1.0), radius, color, subd);
    drawCylinder( Vector3( 1.0, -1.0,  1.0), Vector3( 1.0, 1.0,  1.0), radius, color, subd);
    // Z Axis
    drawCylinder( Vector3(-1.0, -1.0, -1.0), Vector3(-1.0, -1.0, 1.0), radius, color, subd);
    drawCylinder( Vector3(-1.0,  1.0, -1.0), Vector3(-1.0,  1.0, 1.0), radius, color, subd);
    drawCylinder( Vector3( 1.0, -1.0, -1.0), Vector3( 1.0, -1.0, 1.0), radius, color, subd);
    drawCylinder( Vector3( 1.0,  1.0, -1.0), Vector3( 1.0,  1.0, 1.0), radius, color, subd);
}

void QtDrawToolGL::drawCylinder(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2, float radius,
                                const QtDrawToolGL::RGBAColor &color, int subd) {
    drawCone( p1,p2,radius,radius,color,subd);
}

void QtDrawToolGL::drawCapsule(const QtDrawToolGL::Vector3 &p1, const QtDrawToolGL::Vector3 &p2, float radius,
                               const QtDrawToolGL::RGBAColor &color, int subd) {
    Vector3 tmp = p2-p1;
    setMaterial(color);
    /* create Vectors p and q, co-planar with the capsules's cross-sectional disk */
    Vector3 p=tmp;
    if (fabs(p[0]) + fabs(p[1]) < 0.00001*tmp.norm())
        p[0] += 1.0;
    else
        p[2] += 1.0;
    Vector3 q;
    q = p.cross(tmp);
    p = tmp.cross(q);
    /* do the normalization outside the segment loop */
    p.normalize();
    q.normalize();

    int i2;
    /* build the cylinder part of the capsule from rectangular subd */
    std::vector<Vector3> points;
    std::vector<Vector3> normals;

    for (i2=0 ; i2<=subd ; i2++)
    {
        /* sweep out a circle */
        float theta =  (float)( i2 * 2.0f * M_PI / subd );
        float st = sin(theta);
        float ct = cos(theta);
        /* construct normal */
        tmp = p*ct+q*st;
        /* set the normal for the two subseqent points */
        normals.push_back(tmp);

        Vector3 w(p1);
        w += tmp*fabs(radius);
        points.push_back(w);

        w=p2;
        w += tmp*fabs(radius);
        points.push_back(w);
    }

    //we draw here the cylinder part
    drawTriangleStrip(points, normals,color);

    //now we must draw the two hemispheres
    //but it's easier to draw spheres...
    drawSphere(p1,radius);
    drawSphere(p2,radius);

    resetMaterial(color);
}

void QtDrawToolGL::drawCross(const QtDrawToolGL::Vector3 &p, float length, const QtDrawToolGL::RGBAColor &color) {
    std::vector<sofa::type::Vector3> bounds;

    for ( unsigned int i=0 ; i<3 ; i++ )
    {
        sofa::type::Vector3 p0 = p;
        sofa::type::Vector3 p1 = p;

        p0[i] -= length;
        p1[i] += length;

        bounds.push_back(p0);
        bounds.push_back(p1);
    }
    drawLines(bounds, 1, color);
}

void QtDrawToolGL::drawPlus(const float &radius, const QtDrawToolGL::RGBAColor &color, const int &subd) {
    drawCylinder( Vector3(-1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), radius, color, subd);
    drawCylinder( Vector3(0.0, -1.0, 0.0), Vector3(0.0, 1.0, 0.0), radius, color, subd);
    drawCylinder( Vector3(0.0, 0.0, -1.0), Vector3(0.0, 0.0, 1.0), radius, color, subd);
}

void QtDrawToolGL::drawBoundingBox(const QtDrawToolGL::Vector3 &min, const QtDrawToolGL::Vector3 &max, float size) {
    glLineWidth(size);
    glBegin( GL_LINES );

    // 0-1
    glVertex3f( (float)min[0], (float)min[1], (float)min[2] );
    glVertex3f( (float)max[0], (float)min[1], (float)min[2] );
    // 2-3
    glVertex3f( (float)max[0], (float)max[1], (float)min[2] );
    glVertex3f( (float)min[0], (float)max[1], (float)min[2] );
    // 4-5
    glVertex3f( (float)min[0], (float)min[1], (float)max[2] );
    glVertex3f( (float)max[0], (float)min[1], (float)max[2] );
    // 6-7
    glVertex3f( (float)max[0], (float)max[1], (float)max[2] );
    glVertex3f( (float)min[0], (float)max[1], (float)max[2] );
    // 0-3
    glVertex3f( (float)min[0], (float)min[1], (float)min[2] );
    glVertex3f( (float)min[0], (float)max[1], (float)min[2] );
    // 1-2
    glVertex3f( (float)max[0], (float)min[1], (float)min[2] );
    glVertex3f( (float)max[0], (float)max[1], (float)min[2] );
    // 4-7
    glVertex3f( (float)min[0], (float)min[1], (float)max[2] );
    glVertex3f( (float)min[0], (float)max[1], (float)max[2] );
    // 5-6
    glVertex3f( (float)max[0], (float)min[1], (float)max[2] );
    glVertex3f( (float)max[0], (float)max[1], (float)max[2] );
    // 0-4
    glVertex3f( (float)min[0], (float)min[1], (float)min[2] );
    glVertex3f( (float)min[0], (float)min[1], (float)max[2] );
    // 1-5
    glVertex3f( (float)max[0], (float)min[1], (float)min[2] );
    glVertex3f( (float)max[0], (float)min[1], (float)max[2] );
    // 2-6
    glVertex3f( (float)max[0], (float)max[1], (float)min[2] );
    glVertex3f( (float)max[0], (float)max[1], (float)max[2] );
    // 3-7
    glVertex3f( (float)min[0], (float)max[1], (float)min[2] );
    glVertex3f( (float)min[0], (float)max[1], (float)max[2] );

    glEnd();
    glLineWidth(1.0);
}

//===============
// TRANSFORMATION
//===============

void QtDrawToolGL::pushMatrix() {
    glPushMatrix();
}

void QtDrawToolGL::popMatrix() {
    glPopMatrix();
}

void QtDrawToolGL::multMatrix(float *glTransform) {
    glMultMatrixf(glTransform);
}

void QtDrawToolGL::scale(float s) {
    glScalef(s, s, s);
}

void QtDrawToolGL::translate(float x, float y, float z) {
    glTranslatef(x, y, z);
}

//=================
// GL MISCELLANEOUS
//=================

void QtDrawToolGL::enablePolygonOffset(float factor, float units) {
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(factor, units);
}

void QtDrawToolGL::disablePolygonOffset() {
    glDisable(GL_POLYGON_OFFSET_LINE);
}

void QtDrawToolGL::enableBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void QtDrawToolGL::disableBlending() {
    glDisable(GL_BLEND);
}

void QtDrawToolGL::enableLighting() {
    glEnable(GL_LIGHTING);
}

void QtDrawToolGL::disableLighting() {
    glDisable(GL_LIGHTING);
}

void QtDrawToolGL::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

void QtDrawToolGL::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void QtDrawToolGL::saveLastState() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void QtDrawToolGL::restoreLastState() {
    glPopAttrib();
}

void QtDrawToolGL::readPixels(int x, int y, int w, int h, float *rgb, float *z) {
    if(rgb != nullptr && sizeof(*rgb) == 3 * sizeof(float) * w * h)
        glReadPixels(x, y, w, h, GL_RGB, GL_FLOAT, rgb);

    if(z != nullptr && sizeof(*z) == sizeof(float) * w * h)
        glReadPixels(x, y, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, z);
}

void QtDrawToolGL::setLightingEnabled(bool enabled) {
    if (enabled) {
        enableLighting();
    } else {
        disableLighting();
    }
    p_light_enabled = enabled;
}

bool QtDrawToolGL::getLightEnabled() {
    return p_light_enabled;
}

void QtDrawToolGL::setPolygonMode(int _mode, bool _wireframe) {
    p_polygon_mode=_mode;
    p_wireframe_enabled=_wireframe;
    if (!p_polygon_mode)
    {
        if (p_wireframe_enabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (p_polygon_mode == 1)
    {
        if (p_wireframe_enabled) glPolygonMode(GL_FRONT, GL_LINE);
        else                     glPolygonMode(GL_FRONT, GL_FILL);
    }
    else if (p_polygon_mode == 2)
    {
        if (p_wireframe_enabled) glPolygonMode(GL_BACK, GL_LINE);
        else                     glPolygonMode(GL_BACK, GL_FILL);
    }
}

} // namespace sofa::core::visual
