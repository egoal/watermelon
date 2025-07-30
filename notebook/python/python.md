## python

```python
# 远程启动jupyter
jupyter lab --no-browser --port=8899 # 远端
ssh -L 8899:localhost:8899 user@server_ip # 本地
# 之后打开 http://localhost:8899
# 如果无法选择conda环境作为内核，可以注册一下
conda install ipykernel
python -m ipykernel install --user --name=ENV_NAME # --display-name XXX

# profile
python -m cProfile -o 1.prof a.py ...
snakeviz 1.prof # pip install snakeviz

# 镜像安装
pip install [xxx] -i https://pypi.tuna.tsinghua.edu.cn/simple

# 简单的并行
from multiprocessing import Pool
with Pool(4) as pool:
	r = list(tqdm(pool.imap(proc_one, tasks), total=len(tasks)))

# Pool中四种map的区别，(f, xs)
Pool.map  # 这种就相当于是map的并行版本，返还list
r = Pool.map_async # 和上面相似，但是不会阻塞当前进程，直到调用r.wait()，随后通过r.get()获得list结果
Pool.imap # 不会立刻展开xs，所以默认不会拆分chunks，所以通常速度更慢、内存更小、可以类似generate的方式不断获取结果
Pool.imap_unordered # 和上面一样，返回东西不安顺序

# 自动登陆服务器之类的
def scp_file(file, dst):
    import pexpect
    pw = 'xxxxxxxx'
    cmd = f'scp -r {file} {dst}'
    child = pexpect.spawn(cmd, logfile=sys.stdout.buffer)
    try:
        child.expect('continue connecting', timeout=1)
        child.sendline('yes')
    except:
        pass
    child.expect('password')
    child.sendline(pw)
    child.expect(pexpect.EOF, timeout=None)


# jupyter 计时
%%time # 当前cell
%time # 下一行

# 控制台进度条
from tqdm import tqdm

for i in tqdm(range(100)):
	pass

import logging

logging.basicConfig(
	level=logging.INFO,
	format="%(asctime)s [%(filename)s:%(lineno)d] [%(levelname)s] %(message)s",
	handlers=[
		logging.FileHandler(".log", 'w'),
		logging.StreamHandler()
	]
)
logging.info('a line of info.')

# 配合命令
import subprocess
res = subprocess.check_output('find . -name "*.txt"', shell=True)
res.decode('utf-8').split('\n')

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

## conda

`conda update conda`

`conda create --name [ENV_NAME] python=3.7` 创建环境

`conda info --envs`

`source activate [ENV_NAME]` `source deactivate`

`conda list` 显示当前包

`conda search/install/update [PACKAGE_NAME]` 当前环境安装包

`conda remove -n [ENV_NAME] [PACKAGE_NAME, ...]` 删包

`conda env remove --name [ENV_NAME]` 删环境


## multiprocessing

```python
import multiprocessing

def fun(name):
    print(f'hello {name}.')

names = ['1', '2', '3']
pool = multiprocessing.Pool(processes=3)
for n in names:
    pool.apply_async(func, (n, ))
pool.close()
pool.join()

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
import time
import datetime

# 字符串转时间
dt = datetime.datetime.strptime('230203 01:38:59', '%y%m%d %H:%M:%S') # 获得秒级精度，年份%y是两位数

s = time.time()
time.sleep(1)
e = time.time()
print(e- s)

tm = time.localtime() # tm.tm_year, tm.tm_mon
filename = time.strftime('%Y-%m-%d %H:%M:%S')+ '.log'
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

