#!/usr/bin/env python3
# Generate a series of meshes for different levels of detail
# of a cylinder 'Culture Orbital'
# Have multiple levels of detail with sections chopped out
# so they can mesh with the other levels.

import math
import numpy as np

# meters
width_m = 35e6
diameter_m = 4.5e9
edgewall_m = 2e6
scale_down = 1.0 / width_m

angle_span = 10.0
angle_steps = 5.0

with open('cyl_section.obj', 'w') as obj:
    obj.write('o orbital_section\n')
    angle_inc = angle_span / angle_steps
    for count, angle_deg in enumerate(np.arange(-angle_span * 0.5,
                                                angle_span * 0.5,
                                                angle_inc)):
        # print('{} {}'.format(count, angle_deg))
        angle = math.radians(angle_deg + angle_inc * 0.5)

        radius = diameter_m * scale_down * 0.5
        half_width = width_m * scale_down * 0.5

        # offset so numbers are close to zero
        x = radius * math.cos(angle) - radius
        y = radius * math.sin(angle)

        z = -half_width
        obj.write('v {} {} {}\n'.format(x, y, z))

        z = half_width
        obj.write('v {} {} {}\n'.format(x, y, z))

    for i in range(count):
        v0 = i * 2 + 1
        v1 = v0 + 1
        v2 = (i + 1) * 2 + 2
        v3 = v2 - 1
        obj.write('f {}//{} {}//{} {}//{} {}//{}\n'.format(v0, v0, v1, v1, v2, v2, v3, v3))
