#!/usr/bin/python3

import sys, os
import re

import numpy as np
import matplotlib.pyplot as plt

tab = []

with open('xdebugc') as fin:
    for line in fin.readlines():
        k, v = line.strip().split(':')
        if k=='baselinesize':
            tab.append({k:[v], 'ddsinit':[], 'refsize':[], 'ddsinitCluster':[], 'incsize':[], 'ddsperinc':[], 'secsperdiv':[]})
        else:
            if k in tab[-1]:
                tab[-1][k].append(v)

# print(tab)

def strlist(lst):
    return '|'.join(lst)

def calcIncPerTime(dct):
    incsize = dct['incsize']
    inctime = dct['ddsperinc']
    if not len(incsize)==len(inctime):
        return 0
    s = 0
    for i in range(len(incsize)):
        s += float(inctime[i])/float(incsize[i])
    s = s/len(incsize)
    return s

tabHead = ['baselinesize', 'refsize', 'incsize', 'secsperdiv', 'ddsinit', 'ddsinitCluster', 'ddsperinc']

# save
print('\t'.join(['base_line_size', 'refsize', 'incsize', 'div/ms', 'init/ms', 'initCluster/ms', 'inc/ms', 'inc/incsize']))
for dct in tab:
    line = ''
    for k in tabHead:
        line += strlist(dct[k])+'\t'
    line += '%.6f\t' % (calcIncPerTime(dct))

    print(line)
