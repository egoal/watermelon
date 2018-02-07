#!/usr/bin/python3

'''
    a simple cookguide
'''

import sys, os
import argparse
import json

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
plot_interactively          交互式绘图
plot_in_tk
curve_fit
find_minimal
use_json    -> xlsx2json
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

def plot_interactively():
    x, y = np.linspace(0, 10, ), np.linspace(0, 10)

    plt.ion()
    for i in range(50):
        plt.plot(x, y, 'ro')
        plt.pause(.01)  # 动画

    plt.ioff()  # 回归阻塞
    plt.show()  # 确保绘制完不会自动关闭

def plot_in_tk():
    import matplotlib
    matplotlib.use('TkAgg')
    from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg

    import tkinter as tk

    # create image
    fig = matplotlib.figure.Figure(figsize=(5, 4), dpi=100)
    a = fig.add_subplot(111)

    t = np.arange(0., 3., .01)
    s = np.sin(2*np.pi*t)

    a.plot(t, s)

    # create tk window
    root = tk.Tk()
    root.wm_title('Embedding in TK')
    # create& add canvas
    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.show()
    canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    # add ui
    # default toolbar
    # toolbar = NavigationToolbar2TkAgg(canvas, root)
    # toolbar.update()
    # canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    # buttons
    def on_btnQuit_clicked():
        root.quit()
        root.destroy()
    btnQuit = tk.Button(master=root, text='Quit', command=on_btnQuit_clicked)
    btnQuit.pack(side=tk.BOTTOM)

    def on_btnClear_clicked():
        a.cla()
        a.plot(np.linspace(0, 10), np.linspace(0, 10), 'r')
        canvas.show()

    btnClear = tk.Button(master=root, text='Clear', command=on_btnClear_clicked)
    btnClear.pack(side=tk.BOTTOM)

    # tk loop
    tk.mainloop()

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

