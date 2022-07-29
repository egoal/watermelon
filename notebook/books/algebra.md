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



**Theorem 2.12.2** Let $N$ be a normal subgroup of a group $G$, and let $\bar G$ denote the set of cosets of $N$ in $G$. There is a low of composition on $\bar G$ that makes this set into a group, such that the map $\pi: G\to \bar G$ defined by $\pi (a)=\bar a$ is a surjective homomorphism whose kernel is $N$.

 考虑*product set*: for subgroups $A$, $B$ of $G$, $AB = \{x\in G| x=ab \ for\ some\ a\in A, b\in B \}$



**Theorem 2.12.10 First Isomorphism Theorem.** Let $\varphi: G\to G'$ be a surjective group homomorphism with kernel $N$. The quotient group $\bar G = G / N$ is isomorphic to the image $G'$. To be precise, let $\pi: G\to G'$ be the canonical map. There is a unique isomorphism $\bar \varphi: \bar G\to G'$ such that $\varphi = \bar \varphi \circ \pi$

比如，复数绝对值$C^\times \to R^\times$的像是正实数，kernel则是单位圆$U$，对应商群$C^\times /U$与正实数群同构。



## 3. Vector Spaces

**Definition 3.2.2** A field $F$ is a set together with two laws of composition
$$
F\times F \overset + \to F, \quad F\times F\overset \times \to F
$$
called addition: $a, b \mapsto a+ b$ and multiplication: $a, b \mapsto ab$, which satisfy these axioms:

1. Addition makes $F$ into an abelian group $F^ +$; its identity element is denoted by $0$
2. Multiplication is commutative, and it makes the set of nonzero elements of $F$ into an abelian group $F^\times$; its identity element is denoted by $1$
3. distributive law: For all $a, b, c \in F, a(b+c)=ab+ ac$



## 4. Linear Operators



## 5. Applications of Linear Operators

**Theorem 5.4.4 (c)** Let $A$ and $B$ be complex $n\times n$ matrices that commute: $AB=BA$. Then $e^{A+B} = e^A e^B$.



## 6. Symmetry

A rigid motion of the plane if called an *isometry*(等距), and if an isometry carries a subset $F$ of the plane to itself, it is called a *symmetry* of $F$.



**Theorem 6.2.3** The following conditions on a map $\varphi: R^n \to R^n$ are equivalent:

- $\varphi$ is an isometry that fixes the origin: $\varphi (0) = 0$
- $\varphi$ preserves dot products: $(\varphi(u)\cdot \varphi(v)) = (u\cdot v)$
- $\varphi$ is an orthogonal linear operator

