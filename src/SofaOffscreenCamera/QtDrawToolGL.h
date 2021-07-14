#pragma once

#include <sofa/core/config.h>
#include <sofa/helper/visual/DrawTool.h>
#include <sofa/defaulttype/Quat.h>
#include <sofa/helper/types/RGBAColor.h>

#include <QOpenGLFunctions>
#include <sofa/helper/vector.h>

namespace sofa::helper::visual {
class SOFA_CORE_API QtDrawToolGL : public DrawTool {
public:
    using Base = DrawTool;
    using RGBAColor = Base::RGBAColor;
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
    void drawPoint(const Vector3 &p, const RGBAColor &c) override;
    void drawPoint(const Vector3 &p, const Vector3 &n, const RGBAColor &c) override;
    void drawPoints(const std::vector<Vector3> &points, float size,  const RGBAColor& color) override;
    void drawPoints(const std::vector<Vector3> &points, float size, const std::vector<RGBAColor>& color) override;

    //======
    // LINES
    //======
    void drawLine(const Vector3 &p1, const Vector3 &p2, const RGBAColor& color) override;
    void drawInfiniteLine(const Vector3 &point, const Vector3 &direction, const RGBAColor& color) override;
    void drawLines(const std::vector<Vector3> &points, float size, const RGBAColor& color) override;
    void drawLines(const std::vector<Vector3> &points, float size, const std::vector<RGBAColor>& colors) override;
    void drawLines(const std::vector<Vector3> &points, const std::vector< Vec2i > &index, float size, const RGBAColor& color) override;

    void drawLineStrip(const std::vector<Vector3> &points, float size, const RGBAColor& color) override;
    void drawLineLoop(const std::vector<Vector3> &points, float size, const RGBAColor& color) override;

    //==========
    // TRIANGLES
    //==========
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal) override;
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal, const RGBAColor &c) override;
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal, const RGBAColor &c1, const RGBAColor &c2, const RGBAColor &c3) override;
    void drawTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3, const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const RGBAColor &c1, const RGBAColor &c2, const RGBAColor &c3) override;

    void drawTriangles(const std::vector<Vector3> &points, const RGBAColor& color) override;
    void drawTriangles(const std::vector<Vector3> &points, const std::vector< RGBAColor > &color) override;
    void drawTriangles(const std::vector<Vector3> &points, const Vector3& normal, const RGBAColor& color) override;
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector< Vec3i > &index,
                               const std::vector<Vector3>  &normal,
                               const RGBAColor& color) override;
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector< Vec3i > &index,
                               const std::vector<Vector3>  &normal,
                               const std::vector<RGBAColor>& color) override;
    void drawTriangles(const std::vector<Vector3> &points,
                               const std::vector<Vector3>  &normal,
                               const std::vector< RGBAColor > &color) override;

    void drawTriangleStrip(const std::vector<Vector3> &points,
                                   const std::vector<Vector3>  &normal,
                                   const RGBAColor& color) override;

    void drawTriangleFan(const std::vector<Vector3> &points,
                                 const std::vector<Vector3>  &normal,
                                 const RGBAColor& color) override;

    //======
    // QUADS
    //======
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal) override;
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal, const RGBAColor &c) override;
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal,
                  const RGBAColor &c1, const RGBAColor &c2, const RGBAColor &c3, const RGBAColor &c4) override;
    void drawQuad(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,
                  const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const Vector3 &normal4,
                  const RGBAColor &c1, const RGBAColor &c2, const RGBAColor &c3, const RGBAColor &c4) override;
    void drawQuads(const std::vector<Vector3> &points, const RGBAColor& color) override;
    void drawQuads(const std::vector<Vector3> &points, const std::vector<RGBAColor>& colors) override;

    //=============
    // TETRAHEDRONS
    //=============
    void drawTetrahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const RGBAColor &color) override;
    void drawTetrahedra(const std::vector<Vector3> &points, const RGBAColor& color) override;
    void drawScaledTetrahedra(const std::vector<Vector3> &points, const RGBAColor& color, const float scale) override;

    //============
    // HEXAHEDRONS
    //============
    void drawHexahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
                        const Vector3 &p4, const Vector3 &p5, const Vector3 &p6, const Vector3 &p7, const RGBAColor &color) override;
    void drawHexahedra(const std::vector<Vector3> &points, const RGBAColor& color) override;
    void drawScaledHexahedra(const std::vector<Vector3> &points, const RGBAColor& color, const float scale) override;

    //========
    // SPHERES
    //========
    void drawSphere( const Vector3 & /*p*/, float /*radius*/) override {}
    void drawSphere(const Vector3 & /*p*/, float /*radius*/, const RGBAColor & /*color*/) override {}
    void drawSpheres (const std::vector<Vector3> & /*points*/, const std::vector<float>& /*radius*/, const RGBAColor& /*color*/) override {}
    void drawSpheres (const std::vector<Vector3> & /*points*/, float /*radius*/, const RGBAColor& /*color*/) override {}
    void drawFakeSpheres(const std::vector<Vector3> &/*points*/, const std::vector<float>& /*radius*/, const RGBAColor& /*color*/) override {}
    void drawFakeSpheres(const std::vector<Vector3> &/*points*/, float /*radius*/, const RGBAColor& /*color*/) override {}

    //=======
    // ARROWS
    //=======
    void drawArrow   (const Vector3& /*p1*/, const Vector3 &/*p2*/, float /*radius*/, const RGBAColor& /*color*/,  int /*subd=16*/) override {}
    void drawArrow   (const Vector3& /*p1*/, const Vector3 &/*p2*/, float /*radius*/, float /*coneLength*/, const RGBAColor& /*color*/,  int/* subd=16*/) override {}
    void drawArrow   (const Vector3& /*p1*/, const Vector3 &/*p2*/, float /*radius*/, float /*coneLength*/, float /*coneRadius*/, const RGBAColor& /*color*/,  int /*subd=16*/) override {}


    //==============
    // MISCELLANEOUS
    //==============
    void drawDisk(float radius, double from, double to, int resolution, const RGBAColor& color) override;
    void drawCircle(float radius, float lineThickness, int resolution, const RGBAColor& color) override;
    void drawFrame(const Vector3& /*position*/, const Quaternion &/*orientation*/, const Vec3f &/*size*/) override {}
    void drawFrame(const Vector3& /*position*/, const Quaternion &/*orientation*/, const Vec3f &/*size*/, const RGBAColor &/*color*/) override {}
    void drawCone    (const Vector3& p1, const Vector3 &p2, float radius1, float radius2, const RGBAColor& color, int subd) override;
    void drawCube    (const float& radius, const RGBAColor& color, const int& subd) override;
    void drawCylinder(const Vector3& p1, const Vector3 &p2, float radius, const RGBAColor& color,  int subd) override;
    void drawCapsule(const Vector3& p1, const Vector3 &p2, float radius, const RGBAColor& color,  int subd) override;
    void drawCross(const Vector3&p, float length, const RGBAColor& color) override;
    void drawPlus    (const float& radius, const RGBAColor& color, const int& subd) override;
    void drawEllipsoid(const Vector3 &/*p*/, const Vector3 &/*radii*/) override {}
    void drawBoundingBox( const Vector3 &min, const Vector3 &max, float size) override;
    void draw3DText(const Vector3 &/*p*/, float /*scale*/, const RGBAColor &/*color*/, const char* /*text*/) override {}
    void draw3DText_Indices(const std::vector<Vector3> &/*positions*/, float /*scale*/, const RGBAColor &/*color*/) override {}
    void writeOverlayText( int /*x*/, int /*y*/, unsigned /*fontSize*/, const RGBAColor &/*color*/, const char* /*text*/ ) override {}


    void clear() override {}
    void setMaterial(const RGBAColor &color) override;
    void resetMaterial(const RGBAColor &color) override;
    void resetMaterial() override;

    void pushMatrix() override;
    void popMatrix() override;
    void multMatrix(float* glTransform ) override;
    void scale( float s ) override;
    void translate(float x, float y, float z) override;


    void enablePolygonOffset(float factor, float units) override;
    void disablePolygonOffset() override;

    void enableBlending() override;
    void disableBlending() override;

    void enableLighting() override;
    void disableLighting() override;

    void enableDepthTest() override;
    void disableDepthTest() override;

    void saveLastState() override;
    void restoreLastState() override;

    void readPixels(int x, int y, int w, int h, float* rgb, float* z) override;

    void setLightingEnabled(bool enabled) override;

    bool getLightEnabled();

    void setPolygonMode(int _mode, bool _wireframe) override;

    int getPolygonMode() {return p_polygon_mode;}
    bool getWireFrameEnabled() {return p_wireframe_enabled;}

private:
    QOpenGLFunctions * p_opengl_functions;
    bool p_light_enabled;
    int  p_polygon_mode;      //0: no cull, 1 front (CULL_CLOCKWISE), 2 back (CULL_ANTICLOCKWISE)
    bool p_wireframe_enabled;

};
}