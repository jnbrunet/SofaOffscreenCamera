# SofaOffscreenCamera plugin

This SOFA plugin brings an offscreen camera that is able to take screenshot with or without GUI.
Multiple offscreen cameras can be added to a single simulation scene, and take screenshots from
different angles. 

Here is a quick example. 
```xml
<OffscreenCamera 
    name="beam_and_ball"  
    filepath="camera_%s_%i.png" 
    save_frame_before_first_step="true" 
    save_frame_after_each_n_steps="5" 
    position="-20 0 0" lookAt="0 0 0" zNear="0.01" zFar="100" projectionType="1"/>
```

The `filepath` data argument will replace `%s` and `%i` for the component
name and step number, respectively. In this example, a screenshot will be taken just before
the simulation starts, and then at the end of every 5 steps. Hence, for a simulation of 10 steps,
the following files will be created:
1. camera_beam_and_ball_0.png
2. camera_beam_and_ball_5.png
2. camera_beam_and_ball_10.png

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
