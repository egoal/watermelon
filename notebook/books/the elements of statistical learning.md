
# 2. Overview of Supervised Learning

## 2.7 Structured Regression Models

> 一般来说，绝大多数学习方法中的约束都可以认为是在某种方式上限制*复杂度*。这通常意味着在输入空间中的邻域内引入某些规则行为。

> 现在可以澄清一个事实了。在高维空间中，任何试图在各向同性的局部邻域内产生变化的方法都会陷入困难——因为维度灾难。而一切克服了维数问题的方法都有一种相应的（往往是隐式的或者自适应的）对邻域的度量方式，基本上，这种度量不会允许邻域在所有方向上同时过小。

#### 偏差方差分解
我们希望构建一个模型$f(x)$来预测变量$Y$，使用平方损失来衡量模型的好坏：$L(Y, f(x)) = (Y - f(x))^ 2$
假设数据由一个“真实”的未知函数生成，即$Y = f(x) + \epsilon$，其中：$E(\epsilon) = 0, Var(\epsilon) = \sigma ^2$，即均值为0，且方差恒定；
我们的训练数据集$D$是从数据总体中随机抽样得到的，不同的训练数据集会得到不同的模型$f_D(x)$，是一个依赖于$D$的随机变量；

接下来我们讨论在某一个固定点$x$处，模型$f_D(x)$的预测表现（实际上我们关心的是**所有可能训练集上**模型的平均表现）：
期望预测误差（Expected Prediction Error, EPE）可分解为：
$$ E_D[(Y- f_D(x))^2 | X= x] = Bias(x)^ 2+ Var(x)+ \sigma^ 2 $$
简要推导：
$$E_D[(Y - f_D(x))^ 2] = E_D[(f(x) + \epsilon - f_D(x))] = E_D[((f_D(x)- E_D[f_D(x)]) + (E_D[f_D(x)]- f(x))- \epsilon)^2]\quad 插入E_D[f_D(x)]$$
$$ = E_D[f_D(x)- E_D[f_D(x)]^2] + (E_D[f_D(x)]- f(x))^2+ E_\epsilon[\epsilon^2]+ ... \quad展开 $$
展开后省略的交叉项均为0（利用如$E_D[f_D(x)- E_D(f_D(x))]= 0$, $E_\epsilon[\epsilon]=0$等性质），最终三项即分别为：模型预测值的方差、模型预测与真实量的偏差平方、不可约误差（系统本身的误差）；

# 3. Linear Methods for Regression

基本形式：输入向量$X^T = (X_1, X_2, ..., X_p)$，预测输出$Y$
$$ f(X) = \beta_0+ \sum_{j=1}^p X_j \beta_j $$

## 3.2 Linear Regression Models and Least Squares

> 统计中一个很有名的断言是，对$\beta$的最小二乘估计，是所有线性无偏估计中方差最小的。
> 尽管Gauss-Markov定理这么说，但可能会存在一些很好的有偏估计。考虑方差-偏差的权衡，有可能牺牲少许的偏差，获得较大的方差提升；

## 3.4 Shrinkage Methods

ridge regression相当于按一定比例缩放了参数/特征，不具备将特定参数归0的能力。而lasso则介于其与子集选择之间，保留了一定的归0能力；

可以将子集选择(subset selection)、岭回归(ridge regression)及lasso一般化，统一视为贝叶斯估计：
$$ \tilde \beta = \arg \min_\beta \sum _{i=1}^N (y_i- \beta_0- \sum_{j=1}^p x_{ij}\beta_j)^2 + \lambda \sum_{j=1}^p |\beta_j|^q $$
此时，三者分别对应$q=0, 1, 2$

使用L1和L2的正则，即构成弹性网络(elastic-net)惩罚项：
$$\lambda \sum_{j=1}^p (\alpha \beta_j^2+ (1-\alpha)|\beta_j|)$$

