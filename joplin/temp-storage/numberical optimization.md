# numerical optimization


## 02 Fundamentals of Unconstrainted Optimization:

local minimizer $\Rightarrow \triangledown f(x^1)=0, \triangledown ^2 f(x^1)$ is **positive semidefinite**


local minimizer $\Leftarrow \triangledown f(x^*)=0, \triangledown ^2 f(x^*)$ is **positive definite**

when $f$ is convex, any local minimizer $x^*$ is a global minimizer of $f$, if in addition $f$ is differentiable, then any stationary point $x^*$ is a global minimizer

in general, nonlinear conjugate gradient directions are much more effective than the steepest descent direction and are almost as simple to compute

poorly scaled: 对$x^*$不同维度的影响将导致$f$的巨大不同: $f(x)=10^9 x_1^2 + x_2^2$, 常规而言相比于line search, trust region更难修正和避免

## 03 Line Search Methods:

line search is done in two stages: a bracketing phase finds a interval containing desirable step lengths, and a bisection or interpolation phase computes a good step length within this interval

Wolfe conditions:
$$
f(x_k+\alpha p_k) \le f(x_k) + c_1 \alpha \triangledown f_k^Tp_k
$$

$$
\triangledown f(x_k+\alpha p_k)^T p_k \ge c_2 \triangledown f_k^T p_k
$$

with $0 < c_1 < c_2 < 1$ 【下降了 & 下降不快了】

在一定假设下($\nabla f$ Lipschitz连续), 选择下降方向并挑选合适的步长(如Wolf condition), 则一定会收敛:
$$
\sum_{k\ge0} \cos^2 \theta_k || \nabla f_k||^2 < \infty
$$
**step length selection algorithms**:

主要做法是插值, 在知道初值之后, 可以使用如$\phi(0)$, $\phi'(0)$, $\phi(\alpha_{k-1})$和$\phi(\alpha_{k-2})$进行插值, 取最小值以获得$\alpha_k$

实际在操作中, 分为两个步骤: 
1. 首先在(0, $\alpha_{\max}$)内确定解(满足condition)存在的一个区间 
2. 在区间内进行插值搜索(*Zoom*). 

实现中, 需要保证解不会离边界过近, 以及一些数值精度之类的问题

## 04 Trust-Region Methods



Cauchy point: 在梯度$\nabla f$方向上搜索二次型的最小值所得到的点

$$ \tau _k = 
\left\{ \begin{matrix}
1 & g_k^T B_k g_k \le 0\\
\min(||g_k||^3/(\Delta _k g_k^T B_k g_k), 1)
\end{matrix}\right.
$$

*two-dimensional subspace minimization* can be applied when $B_k$ is indefinite

#### iterative solution of the subproblem
考虑在一定条件(cost)下更为精确地求解子问题: 将带来更好的收敛性

**Theorem 4.1.** the vector $p*$ is a global solution of the trust-region problem

$$ \min _{p\in R^n} m(p) = f+g^Tp+ \frac 12 p^TBp, \quad s.t. ||p|| \le \Delta, $$
if and only if $p^*$ is feasible and there is a scalar $\lambda \ge 0$ such that the following conditions are satisfied:

$$ (B+\lambda I) p^* = -g $$
$$ \lambda(\Delta - ||p^*||)=0 $$
$$ (B+\lambda I) \ is\ positive\ seimidefinite.$$

感觉就是约束问题的一阶条件+KKT约束, 对称正定则表示驻点存在且就是最优解?
通过寻找合适的$\lambda$(牛顿迭代)随后求解即可. (二次型最优解的情况: $B$正定时直接求解, $||p^*|| \le \Delta$时$\lambda=0$)
子问题的求解仍然是满足置信域半径约束的, 在理解里面, 这是与LM不同之处

$f$ poorly scaled时, 球形的置信域是不太合适的, 修改为$||Dp|| \le \Delta$, 对角阵$D$的选择可以参考二阶导$\partial ^2f/ \partial x_i^2$, LM中正是使用了$diag(J^TJ)$以达到类似的效果

#### 05 Conjugate Gradient Methods:

a set of nonzero vectors $\{p_0, p_1, ..., p_l\}$ is said to be **conjugate** with respect to the symmetric positive definite matrix $A$ if

$$p_i^T A p_j=0, \quad for \ all \ i \not = j$$

考虑问题$Ax=b$， $A$正定, 即$\min \phi (x) = \frac 12 x^TAx-b^Tx$任意起点, 可在$n$步内获得结果

共轭方向法(conjugate direction algorithm)最多$n$次迭代终止, 直观上讲, 共轭方向法相当于添加了转换关系将目标函数“对齐”到直角坐标轴方向: 通过$\hat x = S^{-1}x, \ S=[p_0\ p_1\ ...\ p_{n-1}]$, 获得对角的$S^T A S$, 从而“逐方向求解”

共轭梯度法(conjugate gradient method)
初始方向为负梯度, 随后每次选择与上次共轭的方向$p_k$, $p_k$的选择为上次方向与当前负梯度的方向线性组合, 即:

$$p_k = -r_k+\beta _k p_{k-1}$$
左乘$p_{k-1}^TA$即可计算$\beta_k$

the CG method is recommended only for large problems; otherwise, Gaussian elimination or other factorization algorithms such as the singular value decomposition are to be preferred, since they are less sensitive to rounding errors;

$A$有$r$个不同的特征值, 则最多经过$r$次迭代, 算法的收敛速度和$A$特征值分布相关, $A$的条件数越大收敛越慢

the Fletcher-Reeves method: 将共轭梯度法扩展到一般凸优化问题时, [1]步长由line search的方式进行, [2]使用$f$的梯度$\nabla f$代替$r_k = Ax_k-b$:
1. $p_0=-\nabla f(x_0), \ k=0$
2. compute $\alpha_k$: $x_{k+1}=x_k+\alpha_k p_k$
3. compute $\beta_{k+1}$: $\beta_{k+1}= \frac {\nabla f_{k+1}^T \nabla f_{k+1}}{\nabla f_k^T \nabla f_k}$,
4. $p_{k+1} = -\nabla f_k + \beta_{k+1} p_k$
5. goto 2

#### 06 Quasi-Newton Methods:

通过插值拟合直接更新二阶阵$B$或$D$.

secant equation:

$$ \nabla f_k = \nabla m_{k+1}(x_k) = \nabla f_{k+1} - \alpha_k B_{k+1} p_k $$
$$B_{k+1}s_k = y_k$$
with $s_k=x_{k+1}-x_k, y_k=\nabla f_{k+1} - \nabla f_k$

**BFGS**: let $B_{k+1} = B_k+\alpha uu^T + \beta vv^T$, 

$$B_{k+1} = B_k - \frac {B_ks_ks_k^TB_k}{s_k^TB_ks_k} + \frac {s_ks_k^T}{s_k^Ty_k}$$
$$D_{k+1} = (I-\rho_k s_k y_k^T)D_k(I-\rho_k y_k s_k^T) + \rho s_ks_k^T 
, \quad with \ \rho_k = \frac 1{y_k^Ts_k}$$

- 若$D_k$正定则$D_{k+1}$正定
- 具有有效地自校正能力, (当使用足够准确的line search时)

**SR1**: let $B_{k+1} = B_k+\sigma ww^T$,

$$ B_{k+1} = B_k+ \frac {(y_k-B_ks_k)(y_k-B_ks_k)^T}{(y_k-B_ks_k)^Ts_k} $$
$$ D_{k+1} = D_k+ \frac {(s_k-H_ky_k)(s_k-D_ky_k)^T}{(s_k-D_ky_k)^Ty_k} $$
- 无法保持$D$的正定性
- 分母将会接近于0，等于0时上式不再适用，甚至无解。但是可以通过一些检测避免数值不稳定从一定程度上缓解（如跳过掉本次更新）。
- 相比于BFGS方法，更接近于真正的$H$
- BFGS正定的必要条件可能无法满足

#### 07 Large-Scale Unconstrained Optimization:

**Newton-CG**: 使用共轭梯度法求解牛顿迭代形式的线性方程

**L-BFGS**: 拟牛顿法使用递推式, 因此可以保存每次迭代的递推关系($s_k, y_k$序列)来避免保存整个$B$阵(选择初始B_k^0稀疏). 面对可分块的目标函数, 可以对每块进行近似, 随后通过紧致矩阵(compactify matrix)累加构建$B$

#### 08 Calculating Derivatives

#### 09 Derivative-Free Optimization

#### 10 Least-Squares Problems

$$ f(x) = \frac 12 \sum_{j=1}^m r_j^2(x) $$
$$ \nabla f(x) = J(x)^Tr(x) $$
$$ \nabla^2 f(x) = J(x)^TJ(x) + \frac 12 \sum r_j(x) \nabla^2 r_j(x) $$

moreover, this term $J(x)^TJ(x)$ is often more important than the second summation term, either because the residuals $r_j$ are close to affine near the solution, or because of small residuals

the relative error in the final computed solution $x^*$ is usually proportional to the condition number of $J$

线性求解方法:
* Cholesky-based algorithm is particularly useful when $m>>n$, and store $J^TJ$ instead
* QR avoids squaring of the condition number and hence may be more numerically robust
* SVD is the most robust and reliable of all

取决于近似程度, Gauss-Newton法具有1～2阶收敛速度

large-residual时, 近似性丢失, 需要使用变体组合, 确保收敛速度

#### 11 Nonlinear Equations

$$r(x)=\begin{bmatrix} ...\\ r_i(x)\\ ... \end{bmatrix}=0$$

> the differences are that in nonlinear equations, the number of equations equals the number of variables, and that we expect all equations to be satisfied at the solution, rather than just minimizing the sum of squares

许多最小二乘的方法在这里都可以得到变体应用.

牛顿法:直接退化为一阶, 毕竟现在$J$可能具有nonsingularity

$$r(x_{k+1}) = r(x_k)+J(x_k)p_k \rightarrow p_k= -J(x_k)^{-1}r(x_k)$$
拟牛顿法也变为直接对$J$进行拟合(Broyden).

迭代求解时可能失效, 因为局部最小并不意味着全局的$r=0$

**continuation/homotopy methods:** define the so-called *homotopy map*:

$$H(x, \lambda) = \lambda r(x) + (1-\lambda)(x-a)$$
求解时, $\lambda$由0逐渐变为1(并不要求保持上升, 而是沿着*zero path*行进), 重要的一步推导是:

$$ H(x(s), \lambda (s))=0 \quad \Rightarrow \quad [\frac{\partial H}{\partial x}, \frac{\partial H}{\partial \lambda}] \cdot [\frac{dx}{ds}, \frac{d\lambda}{ds}]^T = 0 $$
迭代求解时, 固定一个维度进行牛顿更新, 即:

$$ \begin{bmatrix} \frac {\partial H}{\partial x} \ \frac {\partial H}{\partial \lambda} \\ e_i \end{bmatrix} \begin{bmatrix} \delta x \\ \delta \lambda \end{bmatrix} = \begin{bmatrix} -H \\ 0 \end{bmatrix}$$

> continuation methods **may fail to produce a solution even to a fairly simple system** of nonlinear equations. However, it is generally true that they are more reliable than the merit-function methods described earlier in the chapter

#### 12 Theory of Constrained Optimization

$$\min_{x\in R^n} f(x) \quad s.t. \quad \left\{\begin{matrix}c_i(x)=0, \ i \in \mathcal E \\ c_i(x)\ge 0, \ i \in \mathcal I \end{matrix}\right.$$

> **constraint qualification** are conditions under which the linearized feasible set $\mathcal F(x)$ is similar to the tangent cone $T_{\Omega}(x)$. 即判定一阶条件所得的可行锥是否能描述约束区域的局部特征

*the linear independence constraint qulification*(**LICQ**, 线性独立判别) holds if the set of active constraint gradients $\{\nabla c_i(x), i \in \mathcal A(x) \}$ is linearly independent

*Mangasarian-Fromovitz constraint qualification*(**MFCQ**) holds if there exists a vector $w \in R^n$ such that:

$$ \nabla c_i(x^*)^Tw >0, \quad for\ all\ i \in \mathcal A(x^*) \bigcap \mathcal I $$
$$ \nabla c_i(x^*)^Tw =0, \quad for\ all\ i \in \mathcal E $$
and the set of equality constraint gradients $\{\nabla c_i(x^*), i \in \mathcal E\}$ is *linearly independent*

**First-Order Necessary Conditions**(一阶必要条件): $x^*$ is a local solution, and **LICQ** holds. Then there is a Lagrange multiplier vector $\lambda ^*$, such that:

$$ \nabla_x \mathcal L(x^*, \lambda^*)=0$$
$$ c_i(x^*)=0,\ for\ i \in \mathcal E $$
$$ c_i(x^*)\ge0,\ for\ i \in \mathcal I $$
$$ \lambda_i^* \ge 0,\ for\ i \in \mathcal I $$
$$ \lambda_i^* c_i(x^*)=0,\ for\ i \in \mathcal E \bigcup \mathcal I $$
其实就是KKT条件. 其中$\lambda_i^*$的值一定程度上反应了边界的约束力

critical cone(临界锥)描述了在KKT条件满足时的临界区域:

$$\mathcal C(x^*, \lambda ^*) = \{ w \in \mathcal F(x^*) | \nabla c_i(x^*)^Tw=0, \ all \ i \in  \mathcal A(x^*) \bigcap \mathcal I \ with \ \lambda_i^* > 0\}$$
类似一般问题一阶导为0时, 找到垂直方向, 随后分析二阶情况来确定是否为极值. 类似地, 二阶条件为:

$$\Rightarrow w^T \nabla_{xx}^2 \mathcal L(x^*, \lambda^*) w \ge 0$$
$$\Leftarrow w^T \nabla_{xx}^2 \mathcal L(x^*, \lambda^*) w > 0$$

对没有等式约束的问题:

$$ \min_{x\in R^n} f(x) \quad s.t. \quad c(x)\ge 0 $$
$$  q(\lambda) = \inf_x \mathcal L(x, \lambda), \quad \lambda \in \{\lambda| q(\lambda)>-\infty \} $$
$$ \mathcal D=\max_{\lambda \in R^n} q(\lambda)\quad s.t. \quad \lambda \ge 0 $$
对偶问题为原问题的下界

#### 13 Linear Programming: The Simplex Method

基本形式:
$$ \min c^Tx, \quad s.t.\ Ax=b,\ x\ge 0 $$

形式变换:
$$ \min c^Tx, \quad s.t.\ Ax \le b $$
$$ \rightarrow \quad \min c^Tx, \quad s.t.\ Ax+z=0,\ z\ge 0 $$
$$ \rightarrow \quad \min \begin{bmatrix} c \\ -c \\ 0 \end{bmatrix}^T \begin{bmatrix} x^+ \\ x^- \\ z \end{bmatrix}, \quad s.t.\ [A\ -A\ I] \begin{bmatrix} x^+ \\ x^- \\ z \end{bmatrix} = b,\ \begin{bmatrix} x^+ \\ x^- \\ z \end{bmatrix} \ge 0$$
$x^+$, $x^-$分别为$x$的正负部分: $x = x^+-x^-$

对偶问题
$$\max b^T\lambda, \quad s.t.\ A^T\lambda \le c$$
$$\max b^T\lambda, \quad s.t.\ A^T\lambda +s = c,\ s\ge 0$$

**basic feasible point**:
> A vertor $x$ is a *basic feasible* point if it is feasible and if there exists a subset $\mathcal B$ of the index set {1, 2, 3, ..., n} such that
> - $\mathcal B$ contains exactly $m$ indices
> - $i\not \in \mathcal B \Rightarrow x_i=0$
> - *basis matrix* $B=A[:,\ \mathcal B]$ is nonsingular

> All basic feasible points for the problem are vertices of the feasible polytope $\{x| Ax=b, x>=0\}$ and vice versa.

**degeneracy**
> a basis $\mathcal B$ is said to be degenerate if $x_i=0$ for some $i\in \mathcal B$, where $x$ is the basic feasible solution corresponding to $\mathcal B$, a linear program is said to be degenerate if it has at least one degenerate basis.
>

#### 14 Linear Programming: Interior-Point Method

#### 15 Fundamentals of Algorithms for Nonliear Constrained Optimization:

a nonlinear optimization problem with linear equality constraints is, from a methematical point of view, the same as an unconstrained problem

Maratos effect: some algorithms based on merit functions or filters may fail to converge rapidly beacause they reject steps that make good progress toward a solution

#### 16 Quadratic Programming

等式约束:

$$ \min_x \quad q(x)= \frac 12 x^TGx+x^Tc $$ 
$$ s.t. \quad Ax=b $$

KKT条件:

$$ \begin{bmatrix} G & -A^T \\ A & 0 \end{bmatrix}
\begin{bmatrix} x^* \\ \lambda^* \end{bmatrix} = \begin{bmatrix} -c \\ b \end{bmatrix} $$ 

记，$Z$为$A$零空间的一组基: $AZ=0$

let $A$ have full row rank, $Z^TGZ$ is positive definite, then:

$$ K= \begin{bmatrix} G & A^T \\ A & 0 \end{bmatrix}$$
is nonsingular, and hence there is a unique vector pair $(x^*, \lambda^*)$ satisfying (2)

#### 17 Penalty and Augmented Lagrangian Methods

**Quadratic Penalty Method:**

$$ Q(x; \mu) = f(x)+ \frac \mu2 \sum_{i \in E} c_i^2(x) +
\frac \mu2 \sum_{i \in I} ([c_i(x)]^-)^2 $$
其中: $[y]^- = \max (-y, 0)$

求解时, 根据前面迭代的结果选择本次迭代初值获得(近似)最小值, 不断提高罚因子$\mu$, 收敛时即获得解

#### 18 Sequential Quadratic Programming

#### 19 Interior-Point Methods for Nonlinear Constrained Optimization

#### Background Material

**interplacing eigenvalue theorem**

Let $A \in R^{n\times n}$ be a *symmetrc matrix* with eigenvalues $\lambda_i$ and let $z \in R^n$ be a vector with $||z||=1$, and $\alpha$ be a positive scalar, then, the eigenvalues $\xi_i$ of $A+\alpha z z^T$ satisfy:

$$ \xi_i \ge \lambda_i; \quad \sum_{i=1}^n \xi_i-\lambda_i = \alpha $$

**conditioning and stability**
> for systems $Ax=b$ in which $A$ is *symmetric positive definite*, the Cholesky factorization in combination with triangular substitution constitutes a stable algorithm for producing a solution $x$.

**rates of convergence**

*Q-linear*: for all $k$ sufficiently large

$$\frac {||x_{k+1}- x^*||}{||x_k- x^*||} \le r,\ r\in (0, 1)$$ 

*Q-superlinear*: 

$$\lim_{k \rightarrow \infty} \frac {||x_{k+1}- x^*||}{||x_k- x^*||} = 0$$

*Q-quadratic*: for all $k$ sufficiently large

$$\frac {||x_{k+1}- x^*||}{||x_k- x^*||^2} \le M$$

**order notation**

$$ \eta_k= O(v_k): \quad |\eta_k| \le C|v_k|,\ C>0 $$
$$ \eta_k= o(v_k): \quad \lim_{k\to \infty} \frac {\eta_k}{v_k}=0 $$
$$ \eta_k= \Omega(v_k): \quad C_0|v_k|\le |\eta_k|\le C_1|v_k|, \ 0<C_0\le C_1<\infty$$