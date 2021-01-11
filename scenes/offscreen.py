#!/usr/bin/python3
import os
import SofaRuntime
import Sofa

SofaRuntime.importPlugin("liboffscreen.so")

root = Sofa.Simulation.load(os.path.join(os.path.dirname(__file__), "offscreen.scn"))
Sofa.Simulation.init(root)
