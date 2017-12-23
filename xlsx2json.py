import sys, os
import json, codecs

import xlrd

def xlsx2json(xlsxFile, jsonFile, colKey=0, colStart=0, colEnd=-1):
    dct = {}
    wb = xlrd.open_workbook(xlsxFile)
    sh = wb.sheet_by_index(0)
    title = sh.row_values(0)

    colEnd = len(title) if colEnd<=colStart else colEnd

    for r in range(1, sh.nrows):
        theRow = sh.row_values(r)
        if theRow[colKey]=='':
            print('invalid key at row: %d' % r)
            continue

        kvs = {}
        for c in range(colStart, colEnd):
            if c==colKey or title[c]=='':
                continue
            kvs[title[c]] = theRow[c]

        dct[theRow[colKey]] = kvs

    print('\noutput colums:', end=' ')
    print(title[colStart: colEnd])

    # support chinese
    strJson = json.dumps(dct, ensure_ascii=False, indent=4)
    with codecs.open(jsonFile, 'w', 'utf-8') as f:
        f.write(strJson)

if __name__=='__main__':
    if len(sys.argv)<2:
        print('usage: {} in.xlsx [out.json=*.json] [key_col=0] [start_col=0] [end_col=-1]')
        exit()

    out_json = sys.argv[2] if len(sys.argv)>2 else os.path.basename(sys.argv[1]).split('.')[0]+'.json'
    key_col = sys.argv[3] if len(sys.argv)>3 else 0
    start_col = sys.argv[4] if len(sys.argv)>4 else 0
    end_col = sys.argv[5] if len(sys.argv)>5 else 0

    xlsx2json(sys.argv[1], out_json, int(key_col), int(start_col), int(end_col))
    # xlsx2json('D:/the_gamemaker/Projects/cocos/sf/_diy/cards.xlsx', 'cards.json', (0, 4))
