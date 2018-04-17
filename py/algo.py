#!/usr/bin/python3

import sys, os
import argparse
import json

import numpy as np
import matplotlib.pyplot as plt

# 4.1
def ex_4_1():
    def m(dx):
        fx = 11.
        gx = np.r_[2, -20]
        H = np.c_[[38, 0], [0, 20]]
        return fx+gx.T.dot(dx)+.5*dx.T.dot(H).dot(dx)

    # plot 
    # contours
    x, y = np.linspace(-1, 1), np.linspace(-1, 1)
    xx, yy = np.meshgrid(x, y)
    zz = np.array([m(np.r_[a, b]) for a in x for b in y]).reshape(50, 50)
    cnts = plt.contour(xx, yy, zz, 20)
    # plt.clabel(cnts, color='black', inline=True)

    # trust region circle
    plt.plot(0., 0., 'bo')
    for r in np.linspace(0, 2, 5):
        a = np.linspace(0, 2*np.pi)
        plt.plot(np.cos(a)*r, np.sin(a)*r, 'b--')
    plt.axis('equal')
    plt.axis([-2, 2, -2, 2])
    plt.show()

# 4.2
class EX_4_2(object):
    def __init__(self, x0, f, g, H):
        self.x0, self.f, self.g, self.H = x0, f, g, H

        self.k = 0
        self.MAX_ITERATIONS = 20
        self.MIN_RADIUS = 1e-3
        self.MIN_STEP = 1e-3
        self.dx = np.r_[self.MIN_STEP*2] # pass terminal check
        self.infos = {} # statistic
        pass

    def set_options(**args):
        pass

    def run(self):
        self.init_region = 1
        self.max_region = 10
        self.init_eta = .1 # minimal decrease to update

        x, r, eta = self.x0, self.init_region, self.init_eta
        self.infos = { 'r': [r], 'x': [x], 'e': [self.f(x)]}
        while self.not_terminal():
            dx, des = self.compute_step(x, r)
            
            # estimate& update region
            rho = (self.f(x)-self.f(x+dx))/des
            if rho<.25:
                r *= .25
            elif rho>.75 and np.abs(dx.dot(dx)-r**2)<1e-3:
                r = np.min(np.r_[2.*r, self.max_region])
            # else, keep radius

            # update
            if rho>self.init_eta:
                x = x+dx
                self.dx = dx # record for terminal check
            else:
                # print('decline update: {}'.format(rho))
                pass

            self.infos['r'].append(r)
            self.infos['e'].append(self.f(x))
            self.infos['x'].append(x)

        print('done: {} {}'.format(len(self.infos['x']), self.infos['x'][-1]))

    def not_terminal(self):
        self.k += 1

        return self.k<self.MAX_ITERATIONS and self.infos['r'][-1]>self.MIN_RADIUS \
            and self.dx.dot(self.dx)>self.MIN_STEP**2

    def compute_step(self, x, r):
        fk, gk, Hk = self.f(x), self.g(x), self.H(x)
        def calc_decrease(p):
            return -(gk.dot(p)+.5*p.dot(Hk).dot(p))

        # unconstrained p^B
        pB = -np.linalg.inv(Hk).dot(gk)
        if pB.dot(pB)<=r**2:
            # in the region, accept
            return pB, calc_decrease(pB)

        # minimizer along the steepest descent direction
        pU = -gk.T.dot(gk)/(gk.T.dot(Hk).dot(gk))*gk
        if pU.dot(pU)>=r**2:
            # out of region, truncate
            p = r/np.sqrt(pU.dot(pU))*pU
            return p, calc_decrease(p)
        else:
            # second line truncate
            p = self.second_truncate(pU, pB, r)
            return p, calc_decrease(p)

    def second_truncate(self, s, e, l):
        # find c in s->e that |c|=l
        s2, e2, se = s.dot(s), e.dot(e), s.dot(e)
        a, b, c = s2-se+e2, 2*(se-s2), s2-l**2
        # note that \lambda>0
        r = (-b+np.sqrt(b**2-4*a*c))/(2.*a)
        assert(r>0. and r<1.)

        return (1-r)*s+r*e


def ex_4_2():
    # f = 100(x2-x1^2)^2+(1-x1)^2
    def f(x):
        return 100.*(x[1]-x[0]**2)**2+(1-x[0])**2
    def g(x):
        return np.r_[-400.*(x[1]-x[0]**2)*x[0]-2*(1.-x[0]), 
            200.*(x[1]-x[0]**2)]
    def H(x):
        return np.c_[[1200.*x[0]**2-400.*x[1]+2, -400.*x[0]], 
            [-400.*x[0], 200.]]

    o = EX_4_2(np.r_[0, 0], f, g, H)
    o.run()

    # plot
    plt.subplot(211)
    plt.plot(o.infos['e'])
    plt.subplot(212)
    plt.plot(o.infos['r'])

    x, y = np.linspace(-.5, 1.5), np.linspace(-.5, 1.5)
    xx, yy = np.meshgrid(x, y)
    zz = np.array([f(np.r_[a, b]) for a in x for b in y]).reshape(50, 50)
    plt.figure()
    plt.contourf(xx, yy, zz, 40, alpha=.7)
    cnts = plt.contour(xx, yy, zz, 40)
    # lt.clabel(cnts, inline=True)
    x = np.array([a[0] for a in o.infos['x']])
    y = np.array([a[1] for a in o.infos['x']])
    plt.plot(x, y, 'ro-')

    plt.show()

# 5.1
def ex_5_1():
    def construct_hilbert(order):
        h = np.zeros((order, order))
        for i in range(order):
            for j in range(order):
                h[i, j] = 1./(i+j+1)

        return h

    def cg_solve(A, b, x0):
        x, r, k = x0, A.dot(x0)-b, 0
        p = -r
        while np.linalg.norm(H.dot(x)-b)>1e-6:
            # line search update x
            a = r.dot(r)/(p.T.dot(A).dot(p))
            x = x+a*p

            # give new direction
            nr = r+a*A.dot(p)
            beta = nr.dot(nr)/(r.dot(r))
            p = -nr+beta*p
            r = nr

            k += 1
            # print(np.linalg.norm(H.dot(x)-b))

        return x, k

    order = 80
    H = construct_hilbert(order)
    b, x0 = np.ones(order), np.zeros(order)

    x, k = cg_solve(H, b, x0)

    print('{}: {}, with {}'.format(x, k, np.linalg.norm(H.dot(x)-b)))

'''
    f = x^2-1, x0 = 1/2
    a = 1/2
    H = l*(x^2-1)+(1-l)*(x-a)
'''
def homotopy_demo():
    X0 = .5
    STEP_LENGTH = .1
    MAX_ITERATIONS = 20
    
    a = X0
    lbd, x = 0, a
    lstXL = [[lbd, x]]
    ldx, ldlbd = 0., 0. # last update

    for k in range(MAX_ITERATIONS):
        # caculate direction
        dx, dlbd = x**2-x+a-1, -(lbd*2.*x+(1.-lbd))
        ds = (dx**2+dlbd**2)**.5
        dx, dlbd = dx/ds*STEP_LENGTH, dlbd/ds*STEP_LENGTH
        # check sign
        if (k==0 and dlbd<0) or (k>0 and (ldx*dx+ldlbd*dlbd)<0):
            dx, dlbd = -dx, -dlbd

        # project to H=0
        if lbd+dlbd>1.:
            dx *= (1-lbd)/dlbd
            dlbd = 1-lbd

        # fix dx
        x += dx
        lbd = (a-x)/(x**2-x+a-1)

        # update
        ldx, ldlbd = x-lstXL[-1][1], lbd-lstXL[-1][0]

        lstXL.append([lbd, x])

        if 1.-lbd<.001:
            break

    print('%d: %.3f, %.3f' % (len(lstXL), lstXL[-1][0], lstXL[-1][1]))
    # plot
    lstXL = np.array(lstXL)
    plt.plot(lstXL[:, 0], lstXL[:, 1], '-o')
    plt.xlim([0, 1])
    plt.grid('on')
    plt.show()

if __name__=='__main__':
    homotopy_demo()
