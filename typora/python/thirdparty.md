## fp

```python 
from functools import reduce, partial
import itertools as it

from fn import F, _

a = [1, 2, 3]
sorted(a, lambda x: x% 2) # [2, 1, 3]

# partial bind
def fun(a, b):
    pass
b2func = partial(fun, b=2)

# lazy
def lazy_sum(*args):
    def sum():
        return reduce(lambda x, y: x+ y, args, 0)
    return sum

# zip, 'unzip'
a, b = [1, 2, 3], [4, 5, 6]
l = zip(a, b) # [(1, 4), (2, 5), (3, 6)]
zip(*l) # [(1, 2, 3), (4, 5, 6)]

for i, j in it.zip_longest(a, b, fillvalue==DEFAULT):
    if i== DEFAULT or j== DEFAULT:
        print('shorter.')

map(lambda x: x[0]+ x[1], zip([1, 2, 3], it.repeat(10))) # 11, 12, 13
        
# 行遍历文件
def enum_line(filename):
    with open(filename, 'r') as f:
        line = f.readline()
        while line:
            yield line
            line = f.readline()
```

## excel

```python
import xlrd

file = xlrd.open_workbook('file.xls')

# gain table
tab = file.sheets()[0]
tab = file.sheet_by_index(0)
tab = file.sheet_by_name(u'Sheet1')

# gain data
tab.row_values(i)
tab.col_values(i)
# gain cell
cellA1 = tab.cell(0, 0).value
cellC4 = tab.cell(2, 3).value

# write
# ctype: 0-empty, 1-string, 2-number, 3-date, 4-boolean, 5-error 
r, c = 0, 0
tab.put_cell(r, c, ctype, value, cformat)
```

## texttable

```python
# pip install texttable

from texttable import Texttable

header = ['Id', 'Name', 'Sex', 'Height(cm)', 'Weight(kg)']
students = [[0, 'Ana', 'F', 159, 47], [1, 'Joe', 'F', 167, 52], [2, 'Tony', 'M', 173, 62]]

tt = Texttable()
tt.set_deco(Texttable.BORDER | Texttable.HEADER)
tt.add_rows([header])
tt.add_rows(students, header=False)

print(tt.draw())

'''
+-------------------------------------------+
| Id   Name   Sex   Height(cm)   Weight(kg) |
+===========================================+
| 0    Ana    F     159          47         |
| 1    Joe    F     167          52         |
| 2    Tony   M     173          62         |
+-------------------------------------------+
'''
```



