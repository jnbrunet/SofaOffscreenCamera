#include <memory>

#include <QGuiApplication>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QOffscreenSurface>
#include <QOpenGLContext>

#include <sofa/component/visual/BaseCamera.h>

class OffscreenCamera : public sofa::component::visual::BaseCamera {
    using Base = sofa::component::visual::BaseCamera;
    template <typename T> using Data = sofa::core::objectmodel::Data<T>;

public:
    SOFA_CLASS(OffscreenCamera, sofa::component::visual::BaseCamera);

    /**
     * Main constructor of the camera
     */
    OffscreenCamera();

    /**
     * Render the current frame from the point of view of the camera into an image and return it.
     */
    QImage grab_frame();

    /**
     * Render the current frame and save it into a file. Note that if the filepath contains '%s' and '%i', they will
     * be replaced by the component's name and the current simulation step number, respectively.
     *
     * @param filepath Path to the file where the frame will be saved.
     */
    void save_frame(const std::string & filepath);

private:
    void init() final;
    void reset() final { p_step_number = 0; }
    void handleEvent(sofa::core::objectmodel::Event*) final;
    void manageEvent(sofa::core::objectmodel::Event*) final {}
    void initGL();
    std::string parse_file_path() const;

    // Data members
    Data<std::string> d_filepath;
    Data<bool> d_save_frame_before_first_step;
    Data<unsigned int> d_save_frame_after_each_n_steps;
    Data<unsigned int> d_multisampling;

    // Private members
    bool p_textures_have_been_initialized = false;
    unsigned int p_step_number = 0;
    std::unique_ptr<QGuiApplication> p_application;
    QOffscreenSurface * p_surface{};
    QOpenGLFramebufferObject * p_framebuffer;
    QOpenGLContext * p_context{};
};
