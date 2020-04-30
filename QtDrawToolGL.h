#pragma once

#include <sofa/core/core.h>
#include <sofa/core/visual/DrawTool.h>
#include <sofa/defaulttype/Quat.h>
#include <sofa/helper/types/RGBAColor.h>

#include <QOpenGLFunctions>

namespace sofa::core::visual {
class SOFA_CORE_API QtDrawToolGL : public DrawTool {
public:
    using Base = DrawTool;
    using Vec4f = Base::Vec4f;
    using Vec3f = Base::Vec3f;
    using Vector3 = Base::Vector3;
    using Vec3i = Base::Vec3i;
    using Vec2i = Base::Vec2i;
    using Quaternion = Base::Quaternion;

    QtDrawToolGL() : p_opengl_functions(nullptr) {}

    void init() override {}

    //=======
    // POINTS
    //=======
    void drawPoint(const Vector3 &p, const Vec4f &c) override {}
    void drawPoint(const Vector3 &p, const Vector3 &n, const Vec4f &c) override {}
    void drawPoints(const std::vector<Vector3> &points, float size,  const Vec4f& color) override {}
    void drawPoints(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& color) override {}

    //======
    // LINES
    //======
    void drawLine(const Vector3 &p1, const Vector3 &p2, const Vec4f& color) override {}
    void drawInfiniteLine(const Vector3 &point, const Vector3 &direction, const Vec4f& color) override {}
    void drawLines(const std::vector<Vector3> &points, float size, const Vec4f& color) override {}
    void drawLines(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& colors) override {}
    void drawLines(const std::vector<Vector3> &points, const std::vector< Vec2i > &index, float size, const Vec4f& color) override {}

    void drawLineStrip(const std::vector<Vector3> &points, float size, const Vec4f& color) override {}
    void drawLineLoop(const std::vector<Vector3> &points, float size, const Vec4f& color) override {}

    //==========
    // TRIANGLES
    //==========
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal) override {}
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal, const Vec4f &c) override {}
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal, const Vec4f &c1, const Vec4f &c2, const Vec4f &c3) override {}
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const Vec4f &c1, const Vec4f &c2, const Vec4f &c3) override {}

    void drawTriangles(const std::vector<Vector3> &points, const Vec4f& color) override;
    void drawTriangles(const std::vector<Vector3> &points, const std::vector< Vec4f > &color) override {}
    void drawTriangles(const std::vector<Vector3> &points, const Vector3& normal, const Vec4f& color) override {}
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector< Vec3i > &index,
                               const std::vector<Vector3>  &normal,
                               const Vec4f& color) override {}
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector< Vec3i > &index,
                               const std::vector<Vector3>  &normal,
                               const std::vector<Vec4f>& color) override {}
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector<Vector3>  &normal,
                               const std::vector< Vec4f > &color) override {}

    void drawTriangleStrip(const std::vector<Vector3> &points,
                                   const std::vector<Vector3>  &normal,
                                   const Vec4f& color) override {}

    void drawTriangleFan(const std::vector<Vector3> &points,
                                 const std::vector<Vector3>  &normal,
                                 const Vec4f& color) override {}

    //======
    // QUADS
    //======
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal) override {}
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal, const Vec4f &c) override {}
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal,
                  const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4) override {}
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const Vector3 &normal4,
                  const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4) override {}
    void drawQuads(const std::vector<Vector3> &points, const Vec4f& color) override {}
    void drawQuads(const std::vector<Vector3> &points, const std::vector<Vec4f>& colors) override {}

    //=============
    // TETRAHEDRONS
    //=============
    void drawTetrahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vec4f &color) override {}
    void drawTetrahedra(const std::vector<Vector3> &points, const Vec4f& color) override {}
    void drawScaledTetrahedra(const std::vector<Vector3> &points, const Vec4f& color, const float scale) override {}

    //============
    // HEXAHEDRONS
    //============
    void drawHexahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
                        const Vector3 &p4, const Vector3 &p5, const Vector3 &p6, const Vector3 &p7, const Vec4f &color) override {}
    void drawHexahedra(const std::vector<Vector3> &points, const Vec4f& color) override {}
    void drawScaledHexahedra(const std::vector<Vector3> &points, const Vec4f& color, const float scale) override {}

    //========
    // SPHERES
    //========
    void drawSphere( const Vector3 &p, float radius) override {}
    void drawSphere(const Vector3 &p, float radius, const Vec4f &color) override {}
    void drawSpheres (const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& color) override {}
    void drawSpheres (const std::vector<Vector3> &points, float radius, const Vec4f& color) override {}
    void drawFakeSpheres(const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& color) override {}
    void drawFakeSpheres(const std::vector<Vector3> &points, float radius, const Vec4f& color) override {}

    //=======
    // ARROWS
    //=======
    void drawArrow   (const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& color,  int subd=16) override {}
    void drawArrow   (const Vector3& p1, const Vector3 &p2, float radius, float coneLength, const Vec4f& color,  int subd=16) override {}
    void drawArrow   (const Vector3& p1, const Vector3 &p2, float radius, float coneLength, float coneRadius, const Vec4f& color,  int subd=16) override {}


    //==============
    // MISCELLANEOUS
    //==============
    void drawDisk(float radius, double from, double to, int resolution, const Vec4f& color) override {}
    void drawCircle(float radius, float lineThickness, int resolution, const Vec4f& color) override {}
    void drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size) override {}
    void drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size, const Vec4f &color) override {}
    void drawCone    (const Vector3& p1, const Vector3 &p2, float radius1, float radius2, const Vec4f& color, int subd=16) override {}
    void drawCube    (const float& radius, const Vec4f& color, const int& subd=16) override {}
    void drawCylinder(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& color,  int subd=16) override {}
    void drawCapsule(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& color,  int subd=16) override {}
    void drawCross(const Vector3&p, float length, const Vec4f& color) override {}
    void drawPlus    (const float& radius, const Vec4f& color, const int& subd=16) override {}
    void drawEllipsoid(const Vector3 &p, const Vector3 &radii) override {}
    void drawBoundingBox( const Vector3 &min, const Vector3 &max, float size = 1.0) override {}
    void draw3DText(const Vector3 &p, float scale, const Vec4f &color, const char* text) override {}
    void draw3DText_Indices(const std::vector<Vector3> &positions, float scale, const Vec4f &color) override {}
    void writeOverlayText( int x, int y, unsigned fontSize, const Vec4f &color, const char* text ) override {}


    void clear() override {}
    void setMaterial(const Vec4f &color) override;
    void resetMaterial(const Vec4f &color) override;
    void resetMaterial() override;

    void pushMatrix() override {}
    void popMatrix() override {}
    void multMatrix(float* glTransform ) override {}
    void scale( float s ) override {}
    void translate(float x, float y, float z) override {}


    void enablePolygonOffset(float factor, float units) override {}
    void disablePolygonOffset() override {}

    void enableBlending() override {}
    void disableBlending() override {}

    void enableLighting() override {glEnable(GL_LIGHTING);}
    void disableLighting() override {glDisable(GL_LIGHTING);}

    void enableDepthTest() override {}
    void disableDepthTest() override {}

    void saveLastState() override {}
    void restoreLastState() override {}

    void readPixels(int x, int y, int w, int h, float* rgb, float* z = nullptr) override {}

    void internalDrawSpheres(const helper::vector<Vector3>& centers, const float& radius, const unsigned int rings, const unsigned int sectors) {}
    void internalDrawSphere(const Vector3& center, const float& radius, const unsigned int rings, const unsigned int sectors) {}

    void setLightingEnabled(bool enabled) override {
        if (enabled) {
            enableLighting();
        } else {
            disableLighting();
        }
        p_light_enabled = enabled;
    }

    bool getLightEnabled() {return p_light_enabled;}

    void setPolygonMode(int _mode, bool _wireframe) override {}

    int getPolygonMode() {return 0;}
    bool getWireFrameEnabled() {return false;}

private:
    QOpenGLFunctions * p_opengl_functions;
    bool p_light_enabled;

};
}