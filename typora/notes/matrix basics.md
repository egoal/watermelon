**伴随矩阵$A^*$： $AA^*=det(A)I$**

**条件数：$cond(A) = \left \| A \right \| \left \| A^{-1} \right \| $** ，方阵$A$奇异即条件数为$\infty$. 条件数描述了其对应线性系统的稳定性或敏感程度

---

假设$V$是域$\mathbf F$上的向量空间, $V$的**半范数**是一个函数$p: V \rightarrow \mathbb R; x \rightarrow p(x)$, 满足:
$\forall a \in \mathbf F, \forall u, v \in V$,

1. 正定: $p(v) \ge 0$
2. 齐次: $p(av)=|a|\ p(v)$
3. 三角不等式: $p(u+v) \le p(u)+p(v)$

**范数**则加上一个额外性质: $p(v)=0 \Leftrightarrow v=\mathbf 0$
**矩阵范数**还规定满足相容性: $||AB|| \le ||A||\ ||B||$, 引入相容性是为了保持矩阵作为线性算子的特征

* F范数

$$ \lVert A \rVert _F = \sqrt{\sum_{i=1}^m \sum_{j=1}^n \lvert a_{ij} \rvert ^2} = \sqrt {tr(A^TA)} $$

* p范数

$$ ||A||_p = \max_{||x||_p \ne 0} \frac {||Ax||_p}{||x||_p}=
\max_{||x||_p=1}||Ax||_p $$
$$||A||_1 = \max_{j\in n} \sum_{i=1}^m|a_{ij}|$$
$$||A||_{\infty} = \max_{i\in m} \sum_{j=1}^n|a_{ij}|$$
$$||A||_2 = \lambda_{max}^{\frac 12}(A^TA)= \lambda_{max}^{\frac 12}(AA^T)
=\sigma_1(A) $$

---

**矩阵分解**:
- **对角化:** 
$S^{-1}AS = \Lambda$, $S$由特征向量组成, 需要所有特征向量相互独立, 即特征值互不相同

* **LU分解:**
$A=LU$, 对应于高斯消元由上至下消去获得*行梯阵式*, 求解线性方程组时即由下至上进行(消元获得)
* **QR分解:**
将**列满秩**矩阵分解为$A=QR$, 获得正交阵$Q: Q^TQ=I$(将$A$的列不断投影获得一组正交基即得到$Q$)
* **Cholesky分解:**
奖对称正定阵分解为$A=LDL^T$, 对实对称矩阵存在特征分解$A=Q\Lambda Q^T$
* **Schur分解：**
任意$n \times n$矩阵，存在酉矩阵$U$使得 $U^HAU = T$，$T$是上三角阵。

对于对阵矩阵来说:
1) 有实特征值和正交的特征向量
2) 对角化: $A = Q\Lambda Q^T, Q$为正交阵
3) 即使存在重复的特征值也可以对角化
4) 特征值符号与pivots相同

---

**特征值**:
$$
\left \{ \begin{matrix} \sum_i \lambda_i = tr(A) \\ \prod _i \lambda_i = |A|
\end{matrix} \right.
$$
因为 $\quad |A-\lambda I| = \prod_i (\lambda_i-\lambda)=0 $

简单证明:令特征值构成$X = [x_i]$, 有$AX = [Ax_i]\Rightarrow |A||X| = \prod \lambda_i|X|$

对称阵不同特征值对应的特征向量垂直:
$$
\lambda x^T y = x^T A^Ty = x^T \mu y \Rightarrow (\lambda - \mu)x^T y = 0
$$
矩阵$A$正定当且仅当其所有特征值为正

对任意特征向量$r$: $0 < r^TAr = r^T\lambda r = \lambda \ ||r||^2 \Rightarrow \lambda > 0$

对任意$x$: 记$x = \sum \alpha_i r_i$, 则$x^TAx = x^T \sum \lambda_i \alpha_i r_i \ge (\min \lambda_i)\ ||x||^2>0$

---

**几个常用公式**:
$$
(A+BDC)^{-1} = A^{-1}-A^{-1}B(D^{-1}+CA^{-1}B)^{-1}CA^{-1}
$$

$$
(A^{-1} + B^{-1})^{-1} = A- A(A+B)^{-1}A = B- B(A+B)^{-1}B
$$

$$
\begin{bmatrix} A& B \\ 0& D \end{bmatrix}^{-1} = 
\begin{bmatrix} A^{-1}& -A^{-1}BD^{-1} \\ 0& D^{-1} \end{bmatrix}
$$

$$
\begin{bmatrix} A& 0 \\ C& D \end{bmatrix}^{-1} = 
\begin{bmatrix} A^{-1}& 0 \\ -D^{-1}CA^{-1}& D^{-1} \end{bmatrix}
$$

$$
\begin{bmatrix} A& B \\ C& D \end{bmatrix}^{-1} = 
\begin{bmatrix} A^{-1}+ A^{-1}BECA^{-1}& -A^{-1}BE \\ -ECA^{-1}& E \end{bmatrix},\quad E = (D-CA^{-1}B)^{-1}
$$

$$
(I- xy^T)^{-1} = I -\frac 1 {x^Ty-1} xy^T
$$

$$
\det (I- AB) = \det (I- BA)
$$

$$
x^Ty = tr(yx^T) \rightarrow x^TAx = tr(xx^TA)
$$

---

**反对称**
$$
a^\times b = -b^\times a 
$$
$$
aa^T-(a^\times)^2 = I
$$
$$
(Qa)^\times = Qa^\times Q^T 
$$
$$
\phi_1^\times \phi_2^\times - \phi_2^\times \phi_1^\times = (\phi_1^\times \phi_2)^\times
$$

---

**最小二乘变换（含近似R）**
$$
(R, t) = \arg \min_ {R\in SO(d), t\in R^d} \Sigma_i w_i \|(Rp_i+ t)- q_i\| \tag 1
$$
Assume $R$ is fixed:
$$
0=\frac \partial {\partial t} \Sigma w_i \|(Rp_i+ t)- q_i\| \\
\Rightarrow t = \bar q- R\bar p \quad \quad \bar p= \frac {\Sigma w_ip_i}{\Sigma w_i},\ \bar q= \frac {\Sigma w_iq_i}{\Sigma w_i},
\tag 2
$$
then:
$$
\Sigma w_i\|(Rp_i+t)- q_i\| = \Sigma w_i \|R(p_i- \bar p)- (q_i- \bar q)\| = \Sigma w_i\|R x_i - y_i\| \tag 3
$$

$$
R = \arg\min \Sigma w_i\|Rx_i - y_i\|^2 =\arg\min \Sigma w_i (x_i^Tx_i- 2y_i^TRx_i+ y_i^Ty_i) \\
= \arg\max \Sigma w_iy_i^TRx_i = tr (WY^TRX) \tag 4
$$

有$tr(AB)=tr(BA)$
$$
tr(WY^TRX) = tr(RXWY^T) = tr(RS) = tr(RU\Sigma V^T) \\
= tr(V^TRU\Sigma)=tr(M\Sigma)=\Sigma \sigma_i m_{ii} \tag 5
$$
$M^TM=I \Rightarrow |m_{ij}|\le 1$ 因此(5)取最大值时有$M=I$，即 $R=VU^T$

当$\det(VU^T)=-1$时，即须有$\det(M)=-1$，最大值取在仅有$m_{dd}=-1$

近似$R$：
$$
\arg\min \|R-M\|_F^2 = tr((R-M)^T(R-M)) = \arg \max tr(R^TM) = tr(V^TR^TU\cdot \Sigma)
$$




