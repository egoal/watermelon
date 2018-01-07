#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

u = np.linspace(-np.pi, np.pi, 400)
v = np.linspace(-np.pi/2, np.pi/2, 200)

uu, vv = np.meshgrid(u, v)

r, pt = 1, (2, 2, 2)
x, y, z = r*np.cos(vv)*np.cos(uu), r*np.cos(vv)*np.sin(uu), r*np.sin(vv)

d = (x-pt[0])**2+(y-pt[1])**2+(z-pt[2])**2
d = d**0.5

# draw
ax = Axes3D(plt.figure())
# ax.plot_surface(uu, vv, d, cmap='rainbow')
ax.plot_wireframe(uu, vv, d, rstride=10, cstride=10, cmap='rainbow')

if True:
    # add my points
    def addScatter(x, y, z, color='r'):
        def xyz2uv(x, y, z):
            return np.arctan2(y, x), np.arcsin(z)
        u, v = xyz2uv(x, y, z)
        d = ((x-2)**2+(y-2)**2+(z-2)**2)**0.5

        ax.scatter(u, v, d, c=color, s=100)

    addScatter(0, -1, 0) # 0
    addScatter(0.670936, -0.315736, 0.670936) # 1
    addScatter(0.545712, 0.635923, 0.545712, 'g') # 2



plt.show()
