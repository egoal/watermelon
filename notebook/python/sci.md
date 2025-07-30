## pandas

```python
import pandas as pd

# 筛选
subframe = frame[(frame["sex"]=="F") & (frame["age"]<=32) & (row.size>="C")] #类似numpy的简单slice

def fn(row):
	return row.sex== "F" and row.age<=32 and row.size>="C"
subframe = frame.apply(fn, axis=1) # 使用某个函数, axis=1即按行来操作（默认为0按列）

# （映射）添加列
frame["new_column"] = [1, 2, 3, 4, 5]
frame["size"] = frame["data"].map(get_size_fn)
frame["size"] = frame.apply(lambda x: fn(x["data"], x["info"], x["level"]), axis=1)

frame.to_csv("a.csv", index=False) # 去掉index列

```



## numpy

```python
import numpy as np

np.set_printoptions(precision=3, suppress=True)

mu, sigma = np.mean(xs), np.std(xs)

# array构造
np.r_[3, [0]*3, 1:2:3j]
np.c_[1:3, [0]*3, 0:1:3j]
np.hstack((a, b))
np.vstack((a, b))
np.repeat(a, (1, 2)) # [1, 2] -> [1, 1, 2, 2]
np.tile(a, (1, 2)) # [1, 2] -> [1, 2, 1, 2]

# 百分位数
np.percentile(data, q=[5, 95])

# 按某列排序
A[np.argsort(A[:0]), :]
```



## scipy

```python
# kdtree
from scipy.spatial import KDTree

updatetree, maptree = KDTree(updatecloud), KDTree(mapcloud)
matches = updatetree.query_ball_tree(maptree, 1.5) # 在`maptree`中寻找`updatetree`附近的点
# 返回的matches与`updatecloud`等长，记录每个索引找到的在`mapcloud`中的索引

import scipy.linalg as la
import scipy.optimize as opt

la.norm(A)
la.det(A)
la.matrix_rank(A)
la.trace(A)
la.inv(A)
la.eig(A) # la.eigh 针对hermitian matrix
la.eigvals(A)

# 线性方程组
la.solve(A, b)
la.lstsq(A, b)

# 曲线拟合
opt.curve_fit(fun, x, y, ...) # 模型由func给出：lambda x, a, b: a* x+ b

```



## matplotlib

```python
import matplotlib.pyplot as plt

plt.style.use('ggplot')
matplotlib.rcParams['font.sans-serif'] = ['Droid Sans Fallback'] # 中文显示

# 饼图
labels = '未生成', '数据不够', '对齐有问题', '编辑器', '通过', '其他'
sizes = [6, 9, 14, 34, 16, 18] 
explode = (0, 0, 0.2, 0, 0, 0) # 饼块和中心的偏移距离 

plt.figure(figsize=(12, 8)) 
plt.subplot(121) 
plt.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%', shadow=True, counterclock=False, startangle=90)
plt.axis('equal') 确保是个圆的

plt.subplot(122) 
plt.pie(sizes[2:-1], explode=explode[2:-1], labels=labels[2:-1], autopct='%1.1f%%', shadow=True, counterclock=False, startangle=90) plt.axis('equal')

# 条形图
plt.bar(x, height=y)

# 垂线
plt.axvline(x=1, color='r', linestyle='--')

# 旋转标签以免重叠
plt.xticks(range(len(xs)), xs, rotation=60)

# 钉图
markerline, stemlines, baseline = plt.stem(x, np.cos(x), '-.')
plt.setp(baseline, color='r', linewidth=2)

# 极坐标
theta = np.linspace(0, np.pi)
rho = np.cos(theta)
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
ax.plot(theta, rho)

# 关闭图示
plt.xticks([])
plt.axis('off') # 'equal'
f = plt.gca()
f.axes.get_yaxis().set_visible(False) # 单独关闭y轴

# 3D
from mpl_toolkits.mplot3d import Axes3D

ax = Axes3D(plt.figure())
ax.scatter(x, y, z) # 散点

x, y = np.linspace(0, 1), np.linspace(0, 1)
xx, yy = np.meshgrid(x, y)
z = xx**2+ y**2
ax.plot_surface(xx, yy, z)
ax.plot_wireframe(xx, yy, z)

# 2d等高线
plt.figure(figsize=(12, 6))
plt.contourf(xx, yy, z, 20, alpha=0.75) # 20等级，填充绘制
cts = plt.contour(xx, yy, z, 20, colors='black') # 轮廓线
plt.clabel(cts, inline=True, fontsize=10) # 高度标识
```



## sympy

```python
import sympy as sm

sm.init_printing(use_unicode=True)
x, y = sm.symbols('x y')

# 化简三角函数
a = sm.cos(x+ sm.pi/ 3)+ sm.cos(x- sm.pi/ 3)
a.trigsimp() # cos(x)

# 带入求值
(x**2+ 1).subs(x, x+ 1)
(x**2+ 1).evalf(subs={'x': 1})

# 化简，展开和因式分解
sm.simplify
sm.expand((x+1)** 2) # x^2+ 2x+ 1 eq.expand
sm.factor
```

### 计算

`sm.diff(sm.sin(x)* sm.exp(x), x)`:	$(\sin x\ e^ x)' = (\cos x + \sin x) e^ x$

`sm.integrate(sm.exp(x)* sm.sin(x)+ sm.exp(x)* sm.cos(x), x)`:	$\int (e^x\sin x+ e^x\cos x) dx = e^x\sin x+ C$

`sm.integrate(sm.exp(x)* sm.sin(x), x, (x, -sm.oo, sm.oo))`:	$ \int_{-\infty}^{+\infty} e^x \sin x dx  $

`sm.limit(sm.sin(x)/x, x, 0)`:	$ \lim_{x \to 0} \frac {\sin x} x = 1$

`sm.solve(x**2- 2, x)` 或 `sm.solve(sm.Eq(x**2, 2), x)`:	$ x^2-2=0 \Rightarrow x = \sqrt 2 $

`y = sm.Function('y')`;`eq = sm.Eq(y(x).diff(x, x)- y(x), sm.exp(x))`;`sm.dsolve(eq, y(x))`; :	$ y''- y =e^x $



