## conda

`conda update conda`

`conda create --name [ENV_NAME] python=3.7` 创建环境

`conda info --envs`

`source activate [ENV_NAME]` `source deactivate`

`conda list` 显示当前包

`conda search/install/update [PACKAGE_NAME]` 当前环境安装包

`conda remove -n [ENV_NAME] [PACKAGE_NAME, ...]` 删包

`conda env remove --name [ENV_NAME]` 删环境

## python

```python
int('ff', 16) == hex(255) # 进制
chr(ord('a')) == 'a' # asc

# 简单的输入
line = input('input a number: ')
line = line if line else '10'
if line.isdigit():
	return int(line)

# 字符串格式化
f'number is {math.pi: 3.2f}' # 3.14
```

## sys, os

```python
print(sys.version_info) # 版本检查

# 遍历文件树，删除所有txt
for root, dirs, files in os.walk('.'):
    for file in files:
		f = os.path.join(root, file)
        if os.path.isfile(f) and os.path.splitext(f)[1]=='.txt':
            os.remove(f)
            
```

## time

```python
s = time.time()
time.sleep(1)
e = time.time()
print(e- s)

tm = time.localtime() # tm.tm_year, tm.tm_mon
filename = time.strtime('%Y-%m-%d %H:%M:%S')+ '.log'
```

## re

```python
# https://ihateregex.io/
# 数字
p = re.complie(r'[-+]?\d*\.?\d+') # 搞不定科学计数法
re.findall(p, '<str>11,2.0,-3.2,.02</str>')

re.findall(r'[(](.*?)[)]', '(aaa, bbb), (ccc)') # ['aaa, bbb', 'ccc']
```

## json

```python
data = json.loads([JSON_STRINGS]) # 转为 dict or list

with open('out.json', 'w') as f:
    json.dump(data, f, indent=2)
with open('in.json') as f:
    data = json.load(f)
```

## collections

```python
import queue
from collections import namedtuple

q = queue.Queue()
q.empty()
q.put('a')
q.put(1)
q.get()

Student = namedtuple('Student', ['id', 'sex', 'score'])
s = Student(0, 'F', 9) # s.id, s.sex, s.score
```

