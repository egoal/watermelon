#!/usr/bin/python3

import sys, os
import re

pwd = '.'   # run path

def getArgSafe(i):
    if len(sys.argv)>i:
        return sys.argv[i]
    else:
        return None

def createCppClassFile(_className, path):
    strPre = path+'/'+_className
    strMacro = re.findall(r'[A-Z][a-z]+', _className)
    strMacro = '_'.join(strMacro).upper()+'_H_'
    
    if os.path.exists(strPre+'.h') or os.path.exists(strPre+'.cpp'):
        op = input('file already exists, overwrite?[y/[n]] ')
        if not op=='y':
            return

    with open(strPre+'.h', 'w') as fh, open(strPre+'.cpp', 'w') as fs:
        # header file
        strClassBody = 'public:\n\t{0}();\n\t~{0}();'.format(_className)
        fh.write(
            '#ifndef {0}\n'
            '#define {0}\n'
            '\n'
            'class {1}{2};\n'
            '#endif'.format(strMacro, _className, '{\n'+strClassBody+'\n\n}'))

        # source file
        fs.write('#include "{0}.h"\n'.format(_className))


def createClassFile(_className, lan='cpp'):
    dctCreateFunc = { 'cpp':createCppClassFile }
    if not lan in dctCreateFunc:
        print('unsupported language: ', lan)
        return

    dctCreateFunc[lan](_className, pwd)

def createFile():
    assert(len(sys.argv)>=4)

    lag = getArgSafe(4)
    print(lag)
    createClassFile(sys.argv[3], lag if lag else 'cpp')

if __name__=='__main__':
    # [prag name] [func name] [pwd] [args]
    if len(sys.argv)<3:
        print('usage: {} action pwd [args]'.format(sys.argv[0]))
        exit()
    
    dctActions = {
        'new': createFile, 
    }
    if not sys.argv[1] in dctActions:
        print('aviable actions: ')
        map(print, dctActions)
        for k in dctActions.keys():
            print('\t', k)
        exit()

    dctActions[sys.argv[1]]()

