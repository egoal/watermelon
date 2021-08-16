#!/usr/bin/python

'''
rotations convertor
aa  AngleAxis
q   Quaternion  w, x, y, z, 
rm  RotationMatrix  3x3, row major
ea  EulerAngle radians

usage: rotation.cvtRotation(data, 'aa2q')
return: np.array
'''

import sys, os

import numpy as np


def aa2q(aa):
    _aa = np.r_[aa]
    theta2 = _aa.dot(_aa.T)
    
    if theta2>0:
        theta = theta2**0.5
        return np.r_[np.cos(theta/2), _aa*np.sin(theta/2)/theta]
    else:
        return np.r_[1, _aa*0.5]

def q2aa(q):
    _q = np.r_[q]
    sin_theta2 = _q[1:].dot(_q[1:].T)
    
    if sin_theta2>0:
        sin_theta = sin_theta2**0.5
        cos_theta = _q[0]

        theta = np.arctan2(-sin_theta, -cos_theta) if cos_theta<0.0 else \
            np.arctan2(sin_theta, cos_theta)
        return _q[1:]*(2*theta/sin_theta)

    else:
        return _q[:]*2

def rm2q(rm):
    # refer to ceres-solver: rotation.h
    _rm = np.r_[rm].reshape((3, 3))
    tr = _rm.trace()
    if tr>=0:
        t = (tr+1)**0.5

        return np.r_[t/2, (_rm[1, 2]-_rm[2, 1])/(2*t), \
            (_rm[2, 0]-_rm[0, 2])/(2*t), (_rm[0, 1]-_rm[1, 0])/(2*t)]    
    else:
        i = 0 # max index
        i = 1 if _rm[1, 1]>_rm[0, 0] else i
        i = 2 if _rm[2, 2]>_rm[i, i] else i

        j = (i+1)%3
        k = (j+1)%3

        t = (_rm[i, i]-_rm[j, j]-_rm[k, k]+1)**0.5
        q = np.zeros(4)
        q[i+1], q[0] = t/2, (_rm[j, k]-_rm[k, j])/(2*t)
        q[j+1] = (_rm[i, j]+_rm[j, i])/(2*t)
        q[k+1] = (_rm[i, k]+_rm[k, i])/(2*t)
        
        return q


def rm2aa(rm):
    return q2aa(rm2q(rm))

def aa2rm(aa):
    _aa = np.r_[aa]
    theta2 = _aa.dot(_aa.T)
    if theta2>0:
        theta = theta2**0.5
        w = _aa/theta
        ct, st = np.cos(theta), np.sin(theta)

        return np.r_[ct+w[0]**2*(1-ct), w[2]*st+w[0]*w[1]*(1-ct), -w[1]*st+w[0]*w[2]*(1-ct), \
            w[0]*w[1]*(1-ct)-w[2]*st, ct+w[1]*w[1]*(1-ct), w[0]*st+w[1]*w[2]*(1-ct), \
            w[1]*st+w[0]*w[2]*(1-ct), -w[0]*st+w[1]*w[2]*(1-ct), ct+w[2]*w[2]*(1-ct)].reshape((3, 3)).T

    else:
        return np.r_[1, aa[2], -aa[1], \
            -aa[2], 1, aa[0], \
            aa[1], -aa[0], 1].reshape((3, 3)).T

def ea2rm(ea):
    _ea = np.r_[ea]
    c, s = np.cos(_ea), np.sin(_ea)

    return np.r_[c[0]*c[1], -s[0]*c[2]+c[0]*s[1]*s[2], s[0]*s[2]+c[0]*s[1]*c[2], \
        s[0]*c[1], c[0]*c[2]+s[0]*s[1]*s[2], -c[0]*s[2]+s[0]*s[1]*s[2], \
        -s[1], c[1]*s[2], c[1]*c[2]].reshape((3, 3)).T

def q2rm(q):
    return aa2rm(q2aa(q))

def cvtRotation(data, flag=''):
    cvts = { 
        'aa2q': aa2q, 'q2aa': q2aa, 'rm2q': rm2q, 'rm2aa': rm2aa, 
        'aa2rm': aa2rm, 'ea2rm': ea2rm, 'q2rm': q2rm, 
    }

    if flag not in cvts:
        strkey = ''
        for k in cvts.keys():
            strkey += k+', '

        print('bad flag\nsupported flags: '+strkey)
    else:
        return cvts[flag](data)

if __name__=='__main__':
    print('tests: ')
    aa = np.r_[np.pi/2, 0, 0]
    q = cvtRotation(aa, 'aa2q')
    rm = cvtRotation(aa, 'aa2rm')

    np.set_printoptions(precision=3, suppress=True)
    print(aa)
    print(q)
    print(rm)
    print('----------')
    print(cvtRotation(q, 'q2aa'))
    print(cvtRotation(rm, 'rm2q'))
    print(cvtRotation(rm, 'rm2aa'))
    print(cvtRotation(np.eye(3), 'rm2q'))


