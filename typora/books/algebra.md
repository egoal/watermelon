## 1. Matrices

**Theorem 1.4.7 Uniqueness of the Determinant**. There is a unique function $\delta$ on the space of $n\times n$ matrices with the properties below, namely the determinant.

- $\delta (I) = 1$
- $\delta$ is linear in the rows of the matrix $A$ （比如矩阵A，B，C，除了第$i$行$A_i = B_i+ C_i$之外，元素都一样，这样会有$\delta(A) = \delta (B)+ \delta (C)$）
- If two adjacent rows of a matrix $A$ are equal, then $\delta(A) = 0$



## 2. Groups

A $group$ is a set $G$ with a law of composition（组合方式） that has the following properties:

- associative: $(ab)c=a(bc)$
- $G$ contains an identity element $1$ such that $1a=a$ and $a1=a$
- every element $a$ has an inverse $b$ such that $ab=1$ and $ba=1$



Let $G$ and $G'$ be groups, written with multiplicative notation. A $homomorphism$ （同态） $\varphi: G\to G'$ is a map from $G$ to $G'$ such that for all $a$ and $b$ in $G$,
$$
\varphi (ab) = \varphi (a) \varphi (b)
$$
例子：
1. the determinant function $\det: GL_n(R) \to R^ \times$
2. the exponential map $\exp: R^+ \to R^\times$ defined by $x \leadsto e^x$

If $\varphi$ is a bijective map, then it's $isomorphism$（同构）, the inverse map $\varphi^{-1}: G\to G'$ is also an isomorphism. $G$ and $G'$ are said to be $isomorphic$ if there exists an isomorphism, indicate by $G \approx G'$. （也就是说对于同构的群我们可以随意在其一之上运算，最后作映射即可）



**Definition 2.5.10** A sub group $N$ of a group $G$ is *normal subgroup* if for every $a$ in $N$ and every $g$ in $G$, the *conjugate* $gag^{-1}$ is in $N$.

如：the kernel of a homomorphism is a normal subgroup. $GL_n$在$\det$下的kernel即是$SL_n$是normal subgroup.



*equivalence relation* on a set $S$, write as $a\sim b$ and speak of it as *equivalence* of $a$ and $b$, required:

- transitive: $a\sim b, b\sim c \Rightarrow a\sim c$
- symmetric: $a\sim b \Rightarrow b\sim a$
- reflexive: $\forall a,\ a\sim a$



If $H$ is a subgroup of $G$ and if $a$ is an element of $G$, the subset
$$
aH = \{ah| h \in H\}
$$
is called a *left coset* (陪集). $H$ is a particular left coset because $H = 1 H$.



Since the cosets all have the same order, and since they partition the group, we obtain the important *Counting Formula*:
$$
|G| = |H|[G:H] 
$$



*normal subgroup*: 左右陪集相等.

**Proposition 2.10.4**: Let $\varphi: G\to \mathcal G$ be a homomorphism with kernel $K$ and let $\mathcal H$ be a subgroup of $\mathcal G$. Denote the inverse image $\varphi ^{-1}(\mathcal H)$ by $H$. Then $H$ is a subgroup of $G$ that contains $K$. If $\mathcal H$ is a normal subgroup of $\mathcal G$, then $H$ is a normal subgroup of $G$. If $\varphi$ is surjective and if $H$ is a normal subgroup of $G$ then $\mathcal H$ is a normal subgroup of $\mathcal G$.

