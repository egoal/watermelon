## 3. Linear Transformations

---

$S \oplus S^{\perp} \subseteq R^n$ 的证明

令 $\{v_1, ..., v_k\}$为$S$的一组标准正交基，对任意$x\in R^n$，创建投影分量
$$
x_1 = \sum (x^T v_i) v_i \quad x_2 = x - x_1
$$
则:
$$
x_2^T v_j = x^T v_j- x_1^Tv_j = x^Tv_j - x^Tv_j = 0
$$
因此 $x_2 \in S^{\perp}$

唯一分解：若$x = x_1+ x_2 = x_1'+ x_2'$，其中$x_1, x_1'\in S$ ， $x_2, x_2'\in S^{\perp}$，则:
$$
0= (x_1'- x_1)^T (x_1'- x_1)= (x_1'- x_1)^T(-(x_1'- x_1)) \Rightarrow x_1=x_1' \quad x_2=x_2'
$$

---

- $\mathcal N(A)^{\perp} = \mathcal R(A^T)$
- $\mathcal R(A)^{\perp} = \mathcal N(A^T)$

对$\forall x\in \mathcal N(A), y$有：$y^TAx=0 \Rightarrow (A^Ty)^Tx=0$ 

---

 $A, B \in R^{n\times n}$ then:
1. $0\le rank(A+B) \le rank(A)+ rank(B)$
2. $rank(A)+ rank(B)- n \le rank(AB) \le \min \{rank(A), rank(B)\}$
3. $nullity(B)\le nullity(AB)\le nullity(A)+ nullity(B)$
4. if $B$ is *nonsingular*, $rank(AB)=rank(BA)=rank(A)$ and $\mathcal N(BA) = \mathcal N(A)$



## 4. Introduction to the Moore-Penrose Pseudoinverse

---
对$A: \mathcal X \to \mathcal Y$, 有变换$T: \mathcal N(A)^{\perp} \to \mathcal R(A)$使得: 
$$
Tx = Ax, \quad \forall x\in N(A)^{\perp}
$$
这显然是个双射，因此有唯一逆变换: $T^{-1}: \mathcal R(A) \to \mathcal N(A)^{\perp}$，由此定义**Moore-Penrose pseudoinverse** $A^+: \mathcal Y \to \mathcal X$:
$$
A^+ y = T^{-1}y_1
$$
其中 $y = y_1+ y_2, \quad y_1\in \mathcal R(A), \quad y_2\in \mathcal R(A)^{\perp}$

---
伪逆特征$G=A^+$
1. $AGA = A$
2. $GAG = G$
3. $(AG)^T = AG$
4. $(GA)^T = GA$

通常来说$(AB)^+ \ne B^+A^+$，(一个充分条件是$A$列满秩，$B$行满秩)

例
- $A^+ = A^T(AA^T)^{-1}$ if $A$ is onto (independent rows) (right invertible)
- $A^+ = (A^TA)^{-1}A^T$ if $A$ is 1-1 (independent columns) (left invertible)
- $v^+ = \frac {v^T}{v^Tv}$



## 5. Introduction to the Singular Value Decomposition

---

SVD简单证明:
$$
AA^T\ge 0 \Rightarrow AA^TX = X\Lambda = XS^2 \Rightarrow S^{-1}X^TAA^TXS^{-1} = I
$$
取$V = A^TXS^{-1}$有$V^TV=I$，且$A = XSV^T$（所以$U$对应$AA^T$的特征向量，类似地，$V$对应$A^TA$）



## 6. Linear Equations

---
通解
$$
AX = B \Rightarrow X = A^+B + (I - A^+A)Y \quad Y\in R^{n\times k}
$$
$(I - A^+A)$这个变换，就相当于保留了在$\mathcal N(A)$中的分量
此外，特解$X = A^+B$最小化了 $Tr(X^TX)$



## 7. Projections, Inner Product Spaces, and Norms

---
$P\in R^{n\times n}$是正交投影的充要条件：$P^2= P= P^T$

$A\in R^{m\times n}$有SVD$A= U\Sigma V^T= U_1 S V_1^T$，各空间投影矩阵：

- $P_{\mathcal R(A)} = AA^+ = U_1 U_1^T = \Sigma_{i=1}^r u_i u_i^T$
- $P_{\mathcal R(A)^\perp} = I- AA^+ = U_2 U_2^T = \Sigma_{i=r+1}^m u_i u_i^T$
- $P_{\mathcal N(A)} = I- A^+A = V_2 V_2^T = \Sigma_{i=r+1}^n v_i v_i^T$
- $P_{\mathcal N(A)^\perp} = A^+A = V_1 V_1^T = \Sigma_{i=1}^r v_i v_i^T$

---
根据定义$ \left \| A \right \|  = \max_{\left \|x \right \|=1} \left \| Ax \right \| $是consistent，即常规使用的p-norms都有 $\left \| Ax \right \| \le \left \| A \right \| \left \| x \right \|$



## 9. Eigenvalues and Eigenvectors

---
$\pi(A)=0$的根$\lambda$的重数成为$\lambda$的**代数重数**($m$)，**几何重数**($g$)则是$\lambda$对应的相互独立的特征向量个数，即$n- rank(A-\lambda I) = \dim \mathcal N (A- \lambda I)$，一般来说$1\le g\le m$。若$A$存在$m\ne g$的特征值，则称$A$ defective（有缺陷的），也就是说$A$并不具有$n$个相互独立的特征向量。

---
Hermitian矩阵的特征值都是实数的简单证明（顺带一提，对应不同特征值的特征向量垂直）：
$$
x^HAx = \lambda x^Hx, \quad x^HA^Hx = \bar \lambda x^Hx \Rightarrow \bar \lambda= \lambda
$$



## 10. Canonical Forms

---

等价(equivalence)：$A \mapsto PAQ$ ，比如$P, Q$正交时称正交等价。$Q=P^{-1}$则称相似(similarity $A \mapsto PAP^{-1}$)

> A general matrix $A \in C^ {n \times n}$ is unitarily similar (正交相似) to a diagonal matrix if and only if it is normal (i.e., $AA^H = A^HA$). Normal matrices include Hermitian, skew-Hermitian, and unitary matrices.

---



## 11. Linear Differential and Difference Equations

---

插值求近似（并不只针对于求指数）：

对$f(\lambda)=e^{t \lambda}$，计算$f(A) = e^{tA}$，若$A$有特征多项式：$\pi (\lambda) = \Sigma_{i=1}^m (\lambda - \lambda_i)^{n_i}$，定义：
$$
g(\lambda) = \alpha_0+ \alpha_1 \lambda + ... + \alpha_{n-1} \lambda^{n-1}
$$
其中$\alpha_i$由一下等式决定：
$$
g^{(k)}(\lambda_i) = f^{(k)}(\lambda_i);\quad k=0,1,...,n_i-1, i\in m
$$
最后计算$f(A) = g(A)$，总的来说就感觉是在特征值处做好多项式近似的样子，或者说，展开之后计算低阶的一部分项。



