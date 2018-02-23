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
plot_text_and_annotation
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

def plot_text_and_annotation():
    # make demo for cg tutorial
    import matplotlib.patches as mp

    isLineSearch = True

    plt.figure(figsize=(4, 4), dpi=150)
    # plt.ylim([.0, .6])
    plt.axis('off')

    # trace
    # arrows
    lst = [.2, .2, .5, .15, .6, .5, .8, .5]
    for i in range(len(lst)//2-1):
        plt.annotate('', xytext=(lst[2*i], lst[2*i+1]), xy=(lst[2*i+2], lst[2*i+3]), arrowprops={'arrowstyle': '->'})
    plt.arrow(.1, 0., .1, .2,  color='r', head_width=.02, length_includes_head=True)

    # strings
    lstt = [.2, .25, '$x_k$', 
            .65, .35, '$x_{k+1}$', 
            .8, .4, '$p_k$', 
            .5, .1, '$z_1$', 
            .35, .1, '$d_0$', 
        ]
    for i in range(0, len(lstt), 3):
        plt.text(lstt[i], lstt[i+1], lstt[i+2], )

    if isLineSearch:
        # search direction
        plt.annotate('', xytext=(.2, .2), xy=(.8, .5), arrowprops={'arrowstyle': '->', 'color': 'g', })
        # step
        plt.annotate('', xytext=(.2, .2), xy=(.7, .45), arrowprops={'arrowstyle': 'simple', 'color': 'r', 'alpha': .4})
    else:
        # trust region
        ax = plt.gca()
        ax.add_patch(mp.Circle(xy=(.2, .2), radius=.55, alpha=.4, color='g', fill=False))
        plt.annotate('', xytext=(.2, .2), xy=(.67, .5), arrowprops={'arrowstyle': 'simple', 'color': 'b', 'alpha': .4})

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

