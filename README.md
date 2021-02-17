# SofaOffscreenCamera plugin

**Contact**: Jean-Nicolas Brunet (jnbrunet2000@gmail.com)

This SOFA plugin brings an offscreen camera that is able to take screenshot with or without GUI.
Multiple offscreen cameras can be added to a single simulation scene, and take screenshots from
different angles at the same time. 

Here is a quick example. 
```xml
<OffscreenCamera 
    name="beam_and_ball"  
    filepath="camera_%s_%i.png" 
    save_frame_before_first_step="true" 
    save_frame_after_each_n_steps="5"
    multisampling="4"
    widthViewport="800"
    heightViewport="600"
    position="-20 0 0" lookAt="0 0 0" zNear="0.01" zFar="100" projectionType="1"/>
```

The `multisampling` data argument is an anti-aliasing parameter that will set the number of 
sampling points per pixel. Power of two should be used.

The `filepath` data argument will replace `%s` and `%i` for the component
name and step number, respectively. In this example, a screenshot will be taken just before
the simulation starts, and then at the end of every 5 steps. Hence, for a simulation of 10 steps,
the following _800x600_ images will be created:
1. camera_beam_and_ball_0.png
2. camera_beam_and_ball_5.png
3. camera_beam_and_ball_10.png

Offscreen camera should only render the component within their context tree. Hence, in the
following example, the first camera will take a screenshot containing both the beam and the
ball, while the second camera will only see the ball. In this example, both camera capture 
the same point of view, but this is not restricted.

```xml
<Node name="beam">
    (...)
    <HexahedronFEMForceField (...) />
    <OffscreenCamera
        name="beam_and_ball"
        filepath="camera_%s_%i.png"
        save_frame_before_first_step="true"
        position="-20 0 0" lookAt="0 0 0" zNear="0.01" zFar="100" projectionType="1"/>
    <Node name="ball">
        <OglModel (...) />
        <OffscreenCamera
            name="only_and_ball"
            filepath="camera_%s_%i.png"
            save_frame_before_first_step="true"
            position="-20 0 0" lookAt="0 0 0" zNear="0.01" zFar="100" projectionType="1"/>
    </Node>
</Node>
```

<div align="center">
  <img alt="beam and ball" src="https://user-images.githubusercontent.com/6951981/108219783-55683f00-7136-11eb-8ab9-6d288b6ea45b.png" width="49%">
  <img alt="only ball" src="https://user-images.githubusercontent.com/6951981/108220006-919b9f80-7136-11eb-9b15-6e531f874472.png" width="49%">
</div>

### Scripting with python

Python bindings are also availabe. Hence, from a python script, you can manually call
`OffscreenCamera::save_frame(filepath)`. The `position` and `lookAt` data attributes
can be dynamically changed at any time from the script to move around the camera.
In the following example, a set of predefined positions is given to the camera at each
time step. A screenshot is also rendered every time.
```python
camera_positions = get_camera_positions()
camera = root.beam.camera
for i in range(n_frames):
        Sofa.Simulation.animate(root, 1)
        Sofa.Simulation.updateVisual(root)
        
        # Move the camera to the next position
        with camera.position.writeableArray() as wa:
            wa[0:3] = camera_positions[i+1]

        # Save screenshot at time i+1
        camera.save_frame(f'frame_{i+1}.jpg')
```
Note that this example could be rendered into an mp4 video using, for example, ffmpeg:
```shell
ffmpeg -framerate 60 -i 'frame_%d.jpg' -c:v libx264 -vf fps=30 -pix_fmt yuv420p simulation.mp4
```
Here, the input frame per second is set to 60, and the output to 30, which means that the video
will last 2 times the simulation time.

The file **examples/rotating_camera.py** contains
an example where two cameras are moved in an ellipse around the bending beam. The frames of
both cameras are manually render into the "only_ball" and "beam_and_ball" directories,
respectively. The python script also dynamically change the position of the camera after each
simulation steps. The following result was computed by running the **examples/rotating_camera.py**
file on an headless (no GUI available) server.


<div align="center">
  <img alt="beam and ball" src="https://user-images.githubusercontent.com/6951981/108218000-83e51a80-7134-11eb-9cea-a5fd6addcdf9.gif" width="49%">
  <img alt="only ball" src="https://user-images.githubusercontent.com/6951981/108218421-ec33fc00-7134-11eb-8f94-84fde6a1894e.gif" width="49%">
</div>
