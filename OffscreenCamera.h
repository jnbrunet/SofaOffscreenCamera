#include <memory>

#include <QGuiApplication>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QOffscreenSurface>
#include <QOpenGLContext>

#include <SofaBaseVisual/BaseCamera.h>

class OffscreenCamera : public sofa::component::visualmodel::BaseCamera {
public:
    SOFA_CLASS(OffscreenCamera, sofa::component::visualmodel::BaseCamera);

    using Base = sofa::component::visualmodel::BaseCamera;

    template <typename T>
    using Data = sofa::core::objectmodel::Data<T>;

    OffscreenCamera();

    QImage grab_frame();

    void init() final;
    void bwdInit() final;

    void manageEvent(sofa::core::objectmodel::Event*) override {}
private:
    void initGL();

    // Data members
    Data<std::string> d_filepath;

    std::unique_ptr<QGuiApplication> p_application;
    QOffscreenSurface * p_surface;
    std::unique_ptr<QOpenGLFramebufferObject> p_framebuffer;
    QOpenGLContext * p_context;
};
