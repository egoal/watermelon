#!/usr/bin/python3
import os, sys
import re

path = '/media/psf/Home/Documents/Projects/run0913/all_output/1_0/iteration_2/incre_snippets/'

print(path.split('/')[-4])

# quality
os.system('rm qualitylist')
os.system('find {} -name quality.txt >> qualitylist'.format(path))

dctQua = {}
with open('qualitylist') as fin:
    for quaFile in fin.readlines():
        # print(quaFile.strip().split('/')[-2], end=' ')
        with open(quaFile.strip()) as fqin:
            for line in fqin:
                eles = re.match(r'^High Quality SLAM Trajectory Coverage Rate: (.+)%', line)
                if eles:
                    dctQua[quaFile.strip().split('/')[-2]] = eles.groups()[0]


# init frame
os.system('rm loclist')
os.system('find {} -name loc_2017-09*.txt >> loclist'.format(path))

dctFrames = {}
with open('loclist') as fin:
    for locFile in fin.readlines():
        rvtname = locFile.strip().split('/')[-2]
        dctFrames[rvtname] = list()
        with open(locFile.strip()) as floc:
            for line in floc:
                eles = re.match(r'(.+)Alignment init success at frame (.+)(\. \(Tracker.cpp:2619)', line.strip())
                if eles:
                    dctFrames[rvtname].append(eles.groups()[1])

# sort and print
lstKeys = list(dctQua.keys())
lstKeys.sort()
for k in lstKeys:
    print('{} {:>7} {}'.format(k, float(dctQua[k]), dctFrames[k][0] if len(dctFrames[k]) else ''))

exit()
# db size
path = '/media/psf/Home/Documents/Projects/run0913/all_output/'
dbname = '9248586222975057963.bin'
os.system('rm dblist')
os.system('find {} -name {} >> dblist'.format(path, dbname))

with open('dblist') as fin:
    for line in fin.readlines():
        eles = line.strip().split('/')
        if eles[-2]=='section_db':
            print('{} {} section_db/section_out {:.2f}MB/'.format(eles[-4], eles[-3], os.path.getsize(line.strip())/1024/1024), end='')
        elif eles[-2]=='section_out':
            print('{:.2f}MB'.format(os.path.getsize(line.strip())/1024/1024))
       