## Algebra

### 2. Groups

> A group is a set $G$ with a low of composition that has the following properties:
> - The law of composition is associative: $(ab)c = a(bc)$ for all $a, b, c$ in $G$.
> - G contains an identity element $1$, such that $1a=a$ and $a1=a$ for all $a$ in $G$.
> - Every element $a$ of $G$ has an inverse, an element $b$ such that $ab=ba=1$.

*symmetric group:* $S_n$
$$ S_3 = \{ 1, x, x^2, y, xy, x^2y \} $$
$$ x=(1, 2, 3), y=(1, 2) \quad x^3=1; y^2=1; yx= x^2y $$

> A subset $H$ of a group $G$ is a subgroup if:
> - Closure: $a, b \in H \Rightarrow ab \in H$
> - Identity: $1 \in H$
> - Inverses: $a \in H \Rightarrow a^{-1}\in H$

*general linear group:* $GL_n$ = {$n\times n$ invertible matrices}
*special linear group:* $SL_n$ = $\{A \in GL_n\ |\ det A=1\}$

*Euclidean Algorithm:* one can compute a greatest common divisor easily by repeated division with remainder. 
$$(314, 136)\to (42, 136)\to (42, 10)\to (2, 10)\to (2, 0) \Rightarrow 2$$

*cyclic groups:*
$$<x> = \{..., x^{-2}, x^{-1}, 1, x, x^2, ...\}$$

*homomorphism:*
> Let $G$ and $H$ be groups, a homomorphism $\varphi:G\to H$ is a map from $G$ to $H$ such that for all $a$ and $b$ in $G$:
> $$\varphi(ab) = \varphi(a) \varphi(b)$$
> it provides a way to relate the different groups.

*kernel:*
> The kernel of $\varphi$ often donted by $ker\ \varphi$:
> $$ker\ \varphi = \{ a\in G| \varphi(a)=1 \}$$

*isomorphism:*
> An isomorphism $\varphi:G\to H$ from a group $G$ to $H$ is a bijiective group homomorphism - a bijective map such that $\varphi(ab) = \varphi(a) \varphi(b)$
> Two groups is *isomorphic* if there exists an isomorphism $\varphi$ between them.

*equivalence relation:*
> An equivalence relation on a set $S$ is a relation that holds between certain pairs of elements of $S$. An equivalence relation si required to be:
> - transitive: $a\sim b, b\sim c \Rightarrow a\sim c$
> - symmetric: $a\sim b \Rightarrow b\sim a$
> - reflexive: $\forall a, a\sim a$

*normal subgroup:*
> Let $H$ be a subgroup of a group $G$, the following conditions are equivalent:
> - $H$ is a normal subgroup: $\forall h\in H, g \in G \Rightarrow ghg^{-1}\in H$
> - $\forall g\in G, gHg^{-1}=H$
> - $\forall g\in G, gH = Hg$
> - every left coset of $H$ in $G$ is a right coset
>

**First Isomorphism Theorem:**
> Let $\varphi: G\to G'$ be a surjective group homomorphism with kernel $N$. The quotient group $\bar G=G/N$ is isomorphic to the image $G'$. To be precise, let $\pi: G\to \bar G$ be the canonical map. There is a unique isomorphism $\bar \varphi: \bar G\to G'$ such that $\varphi = \bar \varphi \circ \pi$

### 3. Vector Spaces

*field:*
> a field $F$ is a set together with two laws of composition
> $$F\times F\overset +\to  F, \quad F\times F\overset \times \to F$$
> called addition: $a, b\leadsto a+b$ and multiplication: $a, b\leadsto ab$, which satisfy these axioms:
> - addtion makes $F$ into a abelian group $F^+$; its identity element is denoted by $0$.
> - multiplication is commutative, and it makes the set of nonzero elements of $F$ into a abelian group $F^\times$; its identity element is denoted by $1$.
> - distributive law: $\forall a, b, c\in F \Rightarrow a(b+ c)= ab+ ac$

> An *isomorphism* $\varphi$ from vector space $V$ to $V'$ is a bijective map such that:
> $$\varphi (v+ w) = \varphi (v)+ \varphi (w), \quad \varphi (cv) = c \varphi (v)$$

> - Let $B$ and $B'$ be two bases of a vector space $V$, the basechange matrix $P$ is an invertible matrix that is determined uniquely by the two bases $B$ and $B'$
> - Let $B$ be a basis of a vector space $V$. the other bases are the sets of the form $B' = BP$ where $P$ can be any invertible $n\times n$ matrix.

### 4. Linear Operators

> a general example of a real matrix that has at least one real eigenvalue is one all of whose entries are positive. such matrices, called *positivate matrices*, has one of their most important properties that they always have an eigenvector whose coordinates are positive.

*generalized eigenvector:*
> a generalized eigenvector with eigen value $\lambda$ of $T$ is a nonzero vector $x$ such that $(T-\lambda)^k x = 0$ for some $k> 0$. Its *exponent* is the smallest integer $d$ such that $(T-\lambda)^d x= 0$

*how to find Jordan canonical form of a matrix:*
> $$ AX = XJ = [x_1, x_2, x_3] \begin{bmatrix} \lambda & 1 & \\  & \lambda & 1 \\  &  & \lambda \end{bmatrix} $$
> $$\Rightarrow \left\{\begin{matrix}
> (A-\lambda I) x_1 = 0\\ 
> (A-\lambda I) x_2 = x_1\\ 
> (A-\lambda I) x_3 = x_2 
> \end{matrix}\right.$$
> 最终形式取决与线性无关特征值数量，确定形式之后再求解；

### 5. Applications of Linear Operators

> $M\in SO(3)$ has an eigenvalue equal to 1.
> *proof:* 
> $$\det (M-I) = (-1)^3 \det (I- M) = - \det (I-M)$$
> $$\det (M-I) = \det (M-I)^T = \det M \det (M-I)^T = \det (I-M)$$
> $$\Rightarrow \det (M-I) = 0$$

*Cayley-Hamilton Theorem:*
> Let $p(t) = t^n + c_{n-1}t ^{n-1}+ ...+ c_1t+ c_0$ be the characteristic polynomial of an $n\times n$ complex matrix $A$, Then $p(A)$ is the zero matrix.
> *proof:*
> 1. the case that $A$ is a diagonal matrix:
> with diagonal entries $\lambda_i$, then characteristic polynomial is $p(t)=\prod (t-\lambda_i)$, then $p(A)$ is also a diagonal matrix with diagonal entries $p(\lambda_i)=0$
> 2. the case the eigenvalues of $A$ are distinct:
> then $A$ is diagonalizable, let $A'= P^{-1}AP$ be the diagonal matrix, $p(A) = P^{-1}p(A')P = 0$
> 3. the general case:
> let ${A_k}$ be a sequence of matrices with distinct eigenvalues then converges to $A$, let ${p_k}$ be the related characteristic polynomial. since $\{p_k\} \to p$, $p_k(A_k)\to p(A)$, we have $p_k(A_k)=0$ by 2, therefore $p(A)=0$ 

> Let A be a $n\times n$ matrix, and $P$ be an invertile matrix such that: $\Lambda = P^{-1}AP = diag(\lambda_1, ..., \lambda_n)$. The general solution of the system $\frac {dX}{dt} = AX$ is $X=P\tilde X$, where $\tilde X=(c_1e^{\lambda_1t}, ..., c_ne^{\lambda_nt})$ solves the equation $\frac {d\tilde X}{dt} = \Lambda \tilde X$
> $$ \frac {dX}{dt} = P \frac {d\tilde X}{dt} = P \Lambda \tilde X = AP\tilde X= AX$$
> and, the columns of the matrix $e^{tA}$ form a basis for the space of solutions of the differential equation.

### 6. Symmetry

*isometries:*
> The distance between points of $R^n$ is the length $|u- v|$. An isometry of $n$-dimensional space $R^n$ is a distance-preserving map $f$ from $R^n$ to itself, a map such that, for all $u$ and $v$ in $R^n$:
> $$|f(u)- f(v)|=|u- v|$$
> the following conditions on a map $\varphi: R^n\to R^n$ are equivalent:
> - $\varphi$ is an isometry that fixes the origin: $\varphi (0)=0$
> - $\varphi$ preserves dot products: $(\varphi(u)\cdot \varphi(v))=(u\cdot v)$
> - $\varphi$ is an orthogonal linear operator

> Let $G$ be a finite subgroup of the orthogonal group $O_2$. there is an integer $n$ such that $G$ is one of the following groups:
> - $G_n$: the cyclic group of order $n$ generated by the rotation $\rho_\theta$, where $\theta= \pi/ n$
> - $D_n$: the dihedral group of order $2n$ generated by two elements: the rotation $\rho_\theta$, where $\theta= \pi/ n$, and a rotation $r$ about a line $l$ through the origin.