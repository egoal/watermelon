#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

def f(x, A):
    return np.abs(x-A).sum(axis=0)

def g(x, A, w):
    return (np.sign(x-A)*w).sum(axis=0)

DIM = 3

A = np.vstack((np.random.random(size=(20, DIM))*.5+.25, np.random.random(size=(10, DIM)), 
    np.random.random(size=(5, DIM))+1., ))
w = np.ones((A.shape[0], 1))

x = np.zeros(DIM) # A.mean(axis=0)

lstX = [x, ]
for i in range(10):
    s = -g(x, A, w)*0.002
    print(x, s, f(x, A))
    x += s
    lstX.append(x.copy(), )

# show
x = np.array(lstX)
plt.subplot(211)
plt.plot(A[:, 0], A[:, 1], '.', x[:, 0], x[:, 1], '-o')
plt.subplot(212)
ovals = [f(xx, A) for xx in x]
plt.plot(ovals)
plt.show()
