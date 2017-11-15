#!/usr/bin/python3
import os, sys
import json

jsonIn = './temp/RTV_affectedSection_bad.json'
jsonOut = './temp/Section_affectedRTV_bad.json'

with open(jsonIn, 'r') as f:
    dctRtvBin = json.load(f)

dctBinRtv = dict();

for k in dctRtvBin:
    lstBin = dctRtvBin[k]
    for b in lstBin:
        if not b in dctBinRtv.keys():
            dctBinRtv[b] = list();
        dctBinRtv[b].append(k)

with open(jsonOut, 'w') as f:
    json.dump(dctBinRtv, f, indent=4)

