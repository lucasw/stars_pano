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
# edgewall_m = 2e6
edgewall_m = 2e6
scale_down = 1.0 / width_m

angle_span = 10.0
angle_steps = 32.0

with open('cyl_section.obj', 'w') as obj:
    obj.write('o orbital_section\n')
    half_width = width_m * scale_down * 0.5
    radius = diameter_m * scale_down * 0.5
    wall_radius = (diameter_m * 0.5 - edgewall_m) * scale_down
    print('{} {}'.format(radius, wall_radius))
    angle_inc = angle_span / angle_steps
    for count, angle_deg in enumerate(np.arange(-angle_span * 0.5,
                                                angle_span * 0.5,
                                                angle_inc)):
        # print('{} {}'.format(count, angle_deg))
        angle = math.radians(angle_deg + angle_inc * 0.5)

        # offset x so numbers are close to zero
        wall_x = wall_radius * math.cos(angle) - radius
        wall_y = wall_radius * math.sin(angle)

        x = radius * math.cos(angle) - radius
        y = radius * math.sin(angle)

        z = -half_width
        z_e = 0.01
        obj.write('v {} {} {}\n'.format(z - z_e, -x, y))
        obj.write('v {} {} {}\n'.format(z - z_e, -wall_x, wall_y))
        obj.write('v {} {} {}\n'.format(z + z_e, -wall_x, wall_y))
        obj.write('v {} {} {}\n'.format(z + z_e, -x, y))

        # obj.write('v {} {} {}\n'.format(z, -x, y))
        z = half_width
        # obj.write('v {} {} {}\n'.format(z, -x, y))

        obj.write('v {} {} {}\n'.format(z - z_e, -x, y))
        obj.write('v {} {} {}\n'.format(z - z_e, -wall_x, wall_y))
        obj.write('v {} {} {}\n'.format(z + z_e, -wall_x, wall_y))
        obj.write('v {} {} {}\n'.format(z + z_e, -x, y))

    pts_per_segment = 8
    for i in range(count):
        for j in range(pts_per_segment - 1):
            v0 = i * pts_per_segment + j + 1
            v1 = v0 + 1
            v2 = (i + 1) * pts_per_segment + j + 2
            v3 = v2 - 1
            obj.write('f {}//{} {}//{} {}//{} {}//{}\n'.format(v0, v0, v1, v1, v2, v2, v3, v3))
