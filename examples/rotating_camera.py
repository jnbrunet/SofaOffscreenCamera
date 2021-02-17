#!/usr/bin/python3

"""
This scene simulate a 20x4x4 rectangular beam fixed on one extremity, and having its other one pulled down,
hence creating bending deformations. A rigid ball is embedded inside the beam. Two cameras are added to the
scene. One will only record the ball, while the other one will record both the beam's FE mesh and the ball.
Both camera will move around the beam following an ellipse path.

The frames of both cameras will be output to:
    $PWD/beam_and_ball/frame_%i.jpg
    $PWD/only_ball/frame_%i.jpg
where '%i' is the frame number. To compute an mp4 video from these frame, ffmpeg can be used :
    ffmpeg -framerate 60 -i 'beam_and_ball/frame_%d.jpg' -c:v libx264 -vf fps=60 -pix_fmt yuv420p beam_and_ball.mp4
    ffmpeg -framerate 60 -i 'only_ball/frame_%d.jpg' -c:v libx264 -vf fps=60 -pix_fmt yuv420p only_ball.mp4

Note: the framerate (both from -framerate and -vf fps options) should reflect the simulation FPS (see the first
      variables of this script)
"""

import os
import pathlib
import numpy as np
from math import cos, sin, pi

import Sofa
from SofaTypes import Vec3d
from SofaOffscreenCamera import OffscreenCamera

##############
# Parameters #
##############
simulation_time = 2  # Number of simulation time (in seconds)
FPS = 60  # Number of frames per second of simulation
dt = 1/FPS  # Time step
resolution = (800, 600)  # Frame resolution
samples = 4  # Anti-aliasing multisampling
ellipse = (40, 40)  # Width and height of the ellipse that will be followed by the camera


# Generating the positions of both cameras at each frames (following an ellipse path).
# - The standard parametric equation of an ellipse is :
#      (x, y) = (a cost(t), b sin(t)) for 0 <= t <= 2pi
n_frames = FPS*simulation_time
a, b = ellipse
s = 2*pi/n_frames
# First half of the oval during the first half of the simulation
camera_positions =  [[b*sin((t*s)-(pi/2)), 0, a*cos((t*s)-(pi/2))] for t in range(0, n_frames//2)]
# Same half of the oval, but in reverse for the second half of the simulation
camera_positions += [[b*sin((t*s)-(pi/2)), 0, a*cos((t*s)-(pi/2))] for t in range(n_frames//2+1, 0, -1)]

if __name__ == "__main__":
    # Create output directories if they don't exist
    cwd = pathlib.Path().absolute()
    beam_and_ball_directory = os.path.join(cwd, "beam_and_ball")
    only_ball_directory = os.path.join(cwd, "only_ball")

    print(f"Screenshots will be saved in \n\t{beam_and_ball_directory}\n\t{only_ball_directory}")
    pathlib.Path(beam_and_ball_directory).mkdir(parents=True, exist_ok=True)
    pathlib.Path(only_ball_directory).mkdir(parents=True, exist_ok=True)

    # Load the scene from the "offscreen.scn" XML scene file
    root = Sofa.Simulation.load(os.path.join(pathlib.Path(__file__).parent.absolute(), "offscreen.scn"))
    root.dt = dt

    # Get our two camera
    camera_1 = root.beam.beam_and_ball
    camera_2 = root.beam.ball.only_ball

    # Use a perspective projection
    camera_1.projectionType = 0
    camera_2.projectionType = 0

    # Turn of automatic screenshot since we will do it manually here
    camera_1.save_frame_before_first_step = False
    camera_1.save_frame_after_each_n_steps = 0

    camera_2.save_frame_before_first_step = False
    camera_2.save_frame_after_each_n_steps = 0

    # Set the resolution
    camera_1.widthViewport, camera_1.heightViewport = resolution
    camera_2.widthViewport, camera_2.heightViewport = resolution

    # Make the traction scale to the number of frames needed, and make it so the traction returns to 0
    root.beam.traction.times = [0, n_frames/2, n_frames]
    with root.beam.traction.forces.writeableArray() as wa:
        wa[0] = wa[1]/n_frames

    # Initialize the scene and the visual
    Sofa.Simulation.init(root)
    Sofa.Simulation.initTextures(root)

    # Place cameras for the first step
    with camera_1.position.writeableArray() as wa:
        wa[0:3] = np.asarray(camera_positions[0])

    with camera_2.position.writeableArray() as wa:
        wa[0:3] = np.asarray(camera_positions[0])

    # Save screenshot at time 0
    camera_1.save_frame(os.path.join(beam_and_ball_directory, 'frame_0.jpg'))
    camera_2.save_frame(os.path.join(only_ball_directory, 'frame_0.jpg'))

    # Run the simulation and save a screenshot after each time step
    print(f'Simulating {n_frames} steps. This may take a while.')
    for i in range(n_frames):
        Sofa.Simulation.animate(root, 1)
        Sofa.Simulation.updateVisual(root)

        # Move the camera to the next position on the ellipse
        with camera_1.position.writeableArray() as wa:
            wa[0:3] = np.asarray(camera_positions[i+1])

        with camera_2.position.writeableArray() as wa:
            wa[0:3] = np.asarray(camera_positions[i+1])

        # Save screenshot at time i+1
        camera_1.save_frame(os.path.join(beam_and_ball_directory, f'frame_{i+1}.jpg'))
        camera_2.save_frame(os.path.join(only_ball_directory, f'frame_{i+1}.jpg'))
        if (i+1) % FPS == 0:
            print(f"{int((i+1)/FPS)} of {simulation_time} seconds simulated ({i+1} frames rendered)", flush=True)
