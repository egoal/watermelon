**Frechet distance**
$$
\delta _F (f, g) = \inf_{\alpha, \beta} \max_{t\in [0, 1]} d(f(\alpha(t)), g(\beta(t)))
$$
直观理解为所有参数化形式之间的最大距离的下界，度量了两条曲线的相似性；

针对离散化的形式（两条合理采样的折线），可以转为索引匹配后最大距离的最小值，以动态规划的方式求解；
$$
\| L\| = \max_ {i=1, ..., m} d(u_{a_i}, v_{b_i}) \\
\delta _{dF}(Q, P) = \min \{\|L\| \ |\ L\ is\ a\ coupling\ between P\ and\ Q\}
$$




**soft-thresholding**
$$
\arg \min_x \frac 12 \| x-a \|^2_2+ k\| x\|_1
$$

$$
x^* = \left\{\begin{matrix} 
a-k, & \quad a>k \\
0, & \quad |a|\le k \to (1-\frac k{|a|})+ a \\
a+k, & \quad a<-k
\end {matrix}\right.
$$



**叉乘**
$$
(u\times v)' = u'\times v+ u\times v' \\
(u\times v)\times w = (u\cdot w)v- (v\cdot w)u \\
(a\times b)\cdot (c\times d) = (a\cdot c)(b\cdot d)- (a\cdot d)(b\cdot c) \\
\| a\times b \|^2 + (a\cdot b)^2 = \|a\|^2 \|b\|^2
$$


**t-statistic**

一种比较两个随机变量均值的方式：
$$
t = \frac {\bar X_1 - \bar X_2}{ \sqrt {\frac{S_1^2}{n_1}+ \frac{S_2^2}{n_2}} }
$$


**罗德里格公式（Rodrigue）**

点$p$绕过原点的轴$n$旋转$\theta$（其实就是几何求解的常规旋转）
$$
p^* = p\cos \theta + (n\times p) \sin \theta + n(n\cdot p)(1-\cos \theta)
$$


**泰勒展开**
$$
f(x) = \sum_{i=0}^n \frac {f^{(i)}(a)}{i!} (x-a)^i + R_n(x)
$$
$R_n(x) = o[(x-a)^n]$ 皮亚诺余项描述了接近程度；

$R_n(x)=\frac{f^{(n+1)}(\theta)}{(n+1)!}(x-a)^{n+1}$ 拉格朗日余项为微分中值定理的推广；

 $R_n(x) = \int _a^x \frac{f^{(n+1)}(t)}{n!}(x-t)^n dt$ 积分余项为微积分基本定理的推广；

余项估计：

若$|f^{(n+1)}(x)| \le M_n, \forall x\in (a-r, a+r)$则，$R_n(x)\le M_n \frac{r^{n+1}}{(n+1)!}$



**利普希茨连续（Lipschitz continuity）**

对于在实数集的子集的函数$f: D \subseteq \mathbb R \rightarrow \mathbb R$, 若存在常数$K$使得: $|f(a)-f(b)| \leq K |a-b|, \forall a, b \in D$. 称$f$符合利普希茨条件

比连续更强的光滑性条件, 限制了函数改变的速度, 符合利普希茨条件的函数的斜率, 必小于一个称为利普希茨常数的实数.
$f: [-3, 7] \rightarrow \mathbb R, \ f(x)=x^2$符合条件: $K=14$



**均值与方差的描述**
$$
\bar X = \sum X_i/n,\quad S^2 = \frac 1{n-1}\sum (\bar X-X_i)^2
$$
n个自由度，由于均值已经保有一个自由度, 因此为$n-1$. $\bar X$带入$\sum (X_i-\bar X)$, 整理为二次型, $\sum_{i, j=1}^n a_{ij}X_iX_j$, 则方阵$A=(a_{ij})$的秩为$n-1$



**$\chi^2$ distribution（卡方分布）** 
$$
f(x)= \frac 1{\Gamma(p/2)\ 2^{p/2}} x^{p/2-1} e^{-x/2}
$$
if $Z_1, ..., Z_p$ are independent standard Normal random variables, then $\sum_{i=1}^p Z_i^2 \sim \chi_p^2$



> suppose that the range of $X$ and $Y$ is a (possibly infinite) *rectangle*, if $f(x, y)=g(x)h(y)$ for some functions $g$ and $h$ (not necessarily probability density functions) then $X$ and $Y$ are independent.



**multivariate normal(多元正态分布)**:

let $X\sim N(\mu, \Sigma)$, then:
1. *marginal distribution*: $X_a\sim N(\mu_a, \Sigma_{aa})$
2. *conditional distribution*: $(X_b|X_a = x_a)\sim N(\mu_b+\Sigma_{ba}\Sigma_{aa}^{-1}(x_a-\mu_a),\quad \Sigma_{bb}-\Sigma_{ba}\Sigma_{aa}^{-1}\Sigma_{ab})$
3. if $a$​ is a vector then $a^TX\sim N(a^T\mu, a^T\Sigma a)$​
4. $V=(X-\mu)^T\Sigma^{-1}(X-\mu)\sim \chi _k^2$



**期望与方差**
$$
E(\sum_i a_i X_i) = \sum_i a_i E(X_i) \\
E(\prod_i X_i) = \prod_i E(X_i) \quad \quad independ\ required \\
V(X) = E(X^2)-E(X) ^2 \\
V(aX+b) = a^2 V(x) \\
V(\sum_i a_i X_i) = \sum_i a_i^2 V(X_i)+ \sum_i \sum_j Cov(X_i, X_j)
$$

> $X$ is a random vector with mean $\mu$ and variance $\Sigma$ then, if $A$ is a matrix(vector) then $E(AX) = AE(X)$ and $V(AX) = A\Sigma A^T$



**矩估计**

**moment generating function**, or **laplace transform**, of $X$ is defined by: 
$$
\psi_X(t)= E(e^{tX})= \int e^{tx}dF(x)
$$
where $t$ varies over the real numbers. give:
$$
\psi_X'(t)|_{t=0} = E(X), \quad \psi_X''(t)|_{t=0} = E(X^2)
$$
if $\psi_X(t)=\psi_Y(t)$ for all $t$ in an open interval around $0$, then $X\overset d=Y$