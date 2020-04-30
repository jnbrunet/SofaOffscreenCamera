#include "OffscreenCamera.h"
#include "QtDrawToolGL.h"

#include <memory>

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/Node.h>
#include <sofa/simulation/Simulation.h>

OffscreenCamera::OffscreenCamera()
: p_application(nullptr)
, d_filepath(initData(&d_filepath, "filepath", "Path of the image file."))
{
    if (! QCoreApplication::instance()) {
        // In case we are not inside a Qt application (such as with SofaQt),
        // and a previous OffscreenCamera hasn't created it
        static int argc = 1;
        static char * arg0 = strdup("Offscreen");
        p_application = std::make_unique<QGuiApplication>(argc, &arg0);
        QCoreApplication::processEvents();
    }
}

void OffscreenCamera::init() {
    const auto & width = p_widthViewport.getValue();
    const auto & height = p_heightViewport.getValue();

    QSurfaceFormat format;
    format.setSamples(1);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setProfile(QSurfaceFormat::CoreProfile);

    p_surface = new QOffscreenSurface;
    p_surface->create();

    p_surface->setFormat(format);

    QOpenGLContext * context;
    if (QOpenGLContext::currentContext()) {
        context = QOpenGLContext::currentContext();
    } else {
        p_context = new QOpenGLContext(p_surface);
        context = p_context;
        context->setFormat(format);

        if (not context->create()) {
            msg_error() << "Failed to create the OpenGL context";
        }
    }

    QSurface * current_surface = context->surface();
    if (not context->makeCurrent(p_surface)) {
        msg_error() << "Failed to swap the surface of OpenGL context.";
    }

    Base::init();
    computeZ();

    initGL();

    p_framebuffer = std::make_unique<QOpenGLFramebufferObject>(width, height, GL_TEXTURE_2D);
    if (not p_framebuffer->bind()) {
        msg_error() << "Failed to bind the OpenGL framebuffer.";
    }

    context->makeCurrent(current_surface);
}

void OffscreenCamera::bwdInit() {
    QCoreApplication::processEvents();
    auto image = grab_frame();
    QCoreApplication::processEvents();
    const std::string &path = d_filepath.getValue();
    image.save(QString::fromStdString(path));
}

QImage OffscreenCamera::grab_frame() {
    if (! p_framebuffer) {
        throw std::runtime_error("Framebuffer hasn't been created. Have you run the "
                                 "init() method of the OffscreenCamera component?");
    }

    QOpenGLContext * context;
    if (QOpenGLContext::currentContext()) {
        context = QOpenGLContext::currentContext();
    } else {
        context = p_context;
    }

    QSurface * current_surface = context->surface();
    if (not context->makeCurrent(p_surface)) {
        msg_error() << "Failed to swap the surface of OpenGL context.";
    }

    const auto & width = p_framebuffer->width();
    const auto & height = p_framebuffer->height();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLdouble projectionMatrix[16];
    getOpenGLProjectionMatrix(projectionMatrix);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixd(projectionMatrix);

    GLdouble modelViewMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    getOpenGLModelViewMatrix(modelViewMatrix);
    glMultMatrixd(modelViewMatrix);

    sofa::core::visual::VisualParams visual_parameters;
    visual_parameters.zNear() = getZNear();
    visual_parameters.zFar() = getZFar();
    visual_parameters.viewport() = sofa::helper::fixed_array<int, 4> (0, 0, width, height);
    visual_parameters.setProjectionMatrix(projectionMatrix);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glColor4f(1, 1, 1, 1);
    glDisable(GL_COLOR_MATERIAL);

    auto * node = dynamic_cast<sofa::simulation::Node*>(getContext());

    sofa::core::visual::QtDrawToolGL draw_tool;
    visual_parameters.drawTool() = &draw_tool;
    visual_parameters.setSupported(sofa::core::visual::API_OpenGL);
    sofa::simulation::getSimulation()->draw(&visual_parameters, node);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QImage frame = p_framebuffer->toImage();
    context->swapBuffers(p_surface);
    context->makeCurrent(current_surface);
    return frame;
}

void OffscreenCamera::initGL() {
    static GLfloat light_position[] = {-0.7, 0.3, 0, 1};
    static GLfloat specref[]        = {1, 1, 1, 1};
    static GLfloat ambient_light[]  = {0.5, 0.5, 0.5, 1};
    static GLfloat diffuse_light[]  = {0.9, 0.9, 0.9, 1};
    static GLfloat specular[]       = {1, 1, 1, 1};
    static GLfloat lmodel_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static GLfloat lmodel_twoside[] = { GL_FALSE };
    static GLfloat lmodel_local[]   = { GL_FALSE };

    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);
    glEnable(GL_NORMALIZE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Set light model
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_local);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    // Setup 'light 0'
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Enable color tracking
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);

    // Turn on our light and enable color along with the light
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int OffscreenCameraClass = sofa::core::RegisterObject("Offscreen rendering camera.")
    .add< OffscreenCamera >()
;