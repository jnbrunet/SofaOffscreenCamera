#include "OffscreenCamera.h"
#include "QtDrawToolGL.h"

#include <memory>

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/Node.h>
#include <sofa/simulation/Simulation.h>
#include <sofa/simulation/VisualVisitor.h>

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
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    format.setVersion(3, 2);

    p_surface = new QOffscreenSurface;
    p_surface->create();

    p_surface->setFormat(format);

    p_context = new QOpenGLContext(p_surface);
    p_context->setFormat(format);

    if (not p_context->create()) {
        msg_error() << "Failed to create the OpenGL context";
        return;
    }

    Base::init();
    computeZ();

    QOpenGLContext * current_context = QOpenGLContext::currentContext();
    QSurface * current_surface = (current_context ? current_context->surface() : nullptr);
    if (not p_context->makeCurrent(p_surface)) {
        msg_error() << "Failed to swap the surface of OpenGL context.";
        return;
    }

    initGL();

    p_framebuffer = std::make_unique<QOpenGLFramebufferObject>(width, height, GL_TEXTURE_2D);
    if (not p_framebuffer->bind()) {
        msg_error() << "Failed to bind the OpenGL framebuffer.";
    }

    if (current_context) {
        current_context->makeCurrent(current_surface);
    }
}

void OffscreenCamera::bwdInit() {
    auto image = grab_frame();
    const std::string &path = d_filepath.getValue();
    image.save(QString::fromStdString(path));
}

QImage OffscreenCamera::grab_frame() {
    if (! p_framebuffer) {
        throw std::runtime_error("Framebuffer hasn't been created. Have you run the "
                                 "init() method of the OffscreenCamera component?");
    }

    QOpenGLContext * current_context = QOpenGLContext::currentContext();
    QSurface * current_surface = (current_context ? current_context->surface() : nullptr);
    if (not p_context->makeCurrent(p_surface)) {
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
    auto * root = dynamic_cast<sofa::simulation::Node*>(node->getRoot());
    auto * vloop = node->getVisualLoop();

    if (!p_textures_initialized) {
        sofa::simulation::getSimulation()->initTextures(node);
        p_textures_initialized = true;
    }

    sofa::core::visual::QtDrawToolGL draw_tool;
    visual_parameters.drawTool() = &draw_tool;
    visual_parameters.setSupported(sofa::core::visual::API_OpenGL);
    visual_parameters.update();

    auto root_visual_managers = root->visualManager;
    for (auto * visual_manager : root_visual_managers) {
        visual_manager->preDrawScene(&visual_parameters);
    }
    bool rendered = false; // true if a manager did the rendering
    for (auto * visual_manager : root_visual_managers) {
        rendered = visual_manager->drawScene(&visual_parameters);
        if (rendered)
            break;
    }

    if (!rendered) {
        visual_parameters.pass() = sofa::core::visual::VisualParams::Std;
        sofa::simulation::VisualDrawVisitor act ( &visual_parameters );
        act.setTags(this->getTags());
        node->execute ( &act );

        visual_parameters.pass() = sofa::core::visual::VisualParams::Transparent;
        sofa::simulation::VisualDrawVisitor act2 ( &visual_parameters );
        act2.setTags(this->getTags());
        node->execute ( &act2 );
    }

    for (auto visual_manager = root_visual_managers.rbegin(); visual_manager != root_visual_managers.rend(); ++visual_manager) {
        (*visual_manager)->postDrawScene(&visual_parameters);
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QImage frame = p_framebuffer->toImage();
    p_context->swapBuffers(p_surface);
    if (current_context) {
        current_context->makeCurrent(current_surface);
    }
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