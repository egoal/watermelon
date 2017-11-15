#!/usr/bin/python3
import os, sys
import re

path = '/media/psf/Home/Documents/Projects/run0913/all_output/4_0_3/iteration_2/incre_snippets/'

print(path.split('/')[-4])

# quality
os.system('rm temp/qualitylist')
os.system('find {} -name quality.txt >> temp/qualitylist'.format(path))

dctQua = {}
with open('temp/qualitylist') as fin:
    for quaFile in fin.readlines():
        # print(quaFile.strip().split('/')[-2], end=' ')
        with open(quaFile.strip()) as fqin:
            for line in fqin:
                eles = re.match(r'^High Quality SLAM Trajectory Coverage Rate: (.+)%', line)
                if eles:
                    dctQua[quaFile.strip().split('/')[-2]] = eles.groups()[0]


# init frame
os.system('rm temp/loclist')
os.system('find {} -name loc_2017-09*.txt >> temp/loclist'.format(path))

dctFrames = {}
with open('temp/loclist') as fin:
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
# for k in lstKeys:
#     print('{} {:>7} {}'.format(k, float(dctQua[k]), dctFrames[k][0] if len(dctFrames[k]) else ''))

for k in lstKeys:
    print(k)
print('------------------------------')
for k in lstKeys:
    print(dctQua[k])
print('------------------------------')
for k in lstKeys:
    print(dctFrames[k][0] if len(dctFrames[k]) else '')

# exit()
# db size
path = '/media/psf/Home/Documents/Projects/run0913/4_1/'
dbname = '9248586222975057963.bin'
os.system('rm temp/dblist')
os.system('find {} -name {} >> temp/dblist'.format(path, dbname))

fout = open('temp/dbsize', 'w')
with open('temp/dblist') as fin:
    for line in fin.readlines():
        eles = line.strip().split('/')
        if eles[-2]=='section_db':
            fout.write('{} {} section_db/section_out {:.2f}MB/'.format(eles[-4], eles[-3], os.path.getsize(line.strip())/1024/1024))
        elif eles[-2]=='section_out':
            fout.write('{:.2f}MB\n'.format(os.path.getsize(line.strip())/1024/1024))
fout.close()

i = 0
str1, str2 = '', ''
with open('temp/dbsize') as fin:
    for line in fin.readlines():
        i+=1
        if i%2==1:
            str1 += line.strip().split(' ')[-1]+' '
        else:
            str2 += line.strip().split(' ')[-1]+' '

print(str1)
print(str2)
