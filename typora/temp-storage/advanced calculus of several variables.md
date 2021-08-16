## 2. Multivariable Differential Calculus

The mapping $F: D\subset R^n \to R^m$, is *differentiable* at the point $a\in D$ if and only if there exists a *linear* mapping $L: R^n\to R^m$ such that
$$ \lim_{h\to 0} \frac {F(a+h)- F(a)- L(h)}{|h|} = 0$$
the linear mapping $L$ is then denoted by $dF_a$, called the *differential* of $F$ at $a$, its matrix $F'(a)$ is called the derivative of $F$ at $a$.

If $F: R^n\to R^m$ is differential at $a$, then the directional derivative $D_vF(a)$ exists for all $v\in R^n$ and $D_vF(a)=dF_a(v)$, then:
$$ D_vF(a) = dF_a(\sum v_ie_i) = \sum v_i\ dF_a(e_i) = \sum v_iD_i(a) $$
for special case $f: R^n\to R$, $D_vf(a) = \nabla f(a)\cdot v$

The mapping $F: R^n\to R^m$ is *continuously differentiable* at $a$ if the partial derivatives $D_iF$ all exist at each point of some open set containing $a$, and are continuous at $a$. every continuously differentiable funciton is differentiable, and every differentiabl function has directional derivatives.

If $M$ is an $(n-1)$-dimensional manifold in $R^n$, then at each of its points, $M$ has an $(n-1)$-dimensional tangent plane.

Suppose that $g: R^n\to R$ is continuously differentiable. if $M$ is the set of all those points $x\in S=g^{-1}(0)$ at which $\nabla g(x)\not =0$, then $M$ is an $(n-1)$-dimensional manifold. given $a\in M$, the gradient vector $\nabla g(a)$$ is orthogonal to the tangent plane to $M$ at $a$.

Suppose that $f^{(k+1)}$ exists in a neighborhood of $a$ and is continuous at $a$. if: 
$$f^{(i)}(a)=0,\quad for\ i=1, 2, ..., k-1$$
but $f^(k)(a)\not =0$, then 
- $f$ has a local minimum at $a$ if $k$ is even, and $f^{(k)}(a)>0$;
- $f$ has a local maximum at $a$ if $k$ is even, and $f^{(k)}(a)<0$;
- $f$ has neither a maximum nor a minimum if $k$ is odd;

Let $f: R^n\to R$ defined on $G^{-1}, G: R^n\to R^m$, that is, on a $(n-m)$-mainifold $M$, let $a$ be a critical point on $M$: 
$$ \nabla f(a) = \nabla G(a) \cdot \lambda $$
let the auxiliary function $H: R^n \to R$ for $f$ at $a$ be:
$$ H(x)=f(x)-G(x)\cdot \lambda $$
then:
- $a$ is local minimal(maximum) if $\nabla^2 H$ is positive(negative)-definite on the tangent space $T_a$ of $M$ as $a$
- neither if nondefinite on $T_a$



## 3. Successive Approximations and Implicit Functions

