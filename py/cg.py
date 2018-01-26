#!/usr/bin/python3

'''
    a simple cookguide
'''

import sys, os
import argparse

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy import optimize, special

_cases = '''index: 
use_command_line_parser
plot_3d
    point cloud
    surface
    mesh
    contour line map
curve_fit
find_minimal
'''

def use_command_line_parser():
    parser = argparse.ArgumentParser(usage=__doc__)
    parser.add_argument('--order', type=int, default=3, help='order of Bessel function')
    parser.add_argument('--name', type=str, default='unamed', help='save name')

    args = parser.parse_args()
    print(args.order, args.name)

def plot_3d():
    def f(x, y):
        return x**2-2*x*y+3*y**2
    x, y = np.linspace(-1, 1), np.linspace(-1, 1)
    xx, yy = np.meshgrid(x, y)
    zz = f(xx, yy)

    # point, 
    ax = Axes3D(plt.figure())
    ax.scatter(xx, yy, zz, )

    # surface, mesh
    ax = Axes3D(plt.figure())
    ax.plot_surface(xx, yy, zz, cmap='rainbow', )
    ax.plot_wireframe(xx, yy, zz, rstride=10, cstride=10, cmap='rainbow')

    # contour
    plt.figure()
    plt.contourf(xx, yy, zz, 20, alpha=.75, )
    cts = plt.contour(xx, yy, zz, 20, colors='black')
    plt.clabel(cts, inline=True, fontsize=10, )

    plt.show()

def curve_fit():
    def f(x, a, b, c):
        return a*x**b+c
    a, b, c = 1, 2, 3
    ox = np.random.rand(20)*5
    oy = a*ox**b+c+(np.random.rand(20)-0.5)*2

    prm, cov = optimize.curve_fit(f, ox, oy)
    print(prm, '\n', cov)

def find_minimal():
    f = lambda x: -special.jv(1, x)
    
    # search local minimal, start from x=1.
    sol = optimize.minimize(f, 1.)

    print(sol.x, sol.fun)

if __name__=='__main__':
    print(_cases)

