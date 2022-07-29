## optimization algorithms on matrix manifold

> a **atlas**(图册) of $M$ into $R^d$ is a collection of charts$(U_\alpha, \varphi_\alpha)$ of a set $M$ such that:
> 1. $\bigcup_\alpha U_\alpha = M$
> 2. for any pair $\alpha, \beta$ with $U_\alpha\cap U_\beta \neq \varnothing$, then $\varphi_\alpha \circ \varphi_\beta^{-1}$ is smooth on its domain $\varphi_\beta(U_\alpha\cap U_\beta)$

**local parameterization**:
given a chart $\varphi$ on $M$, the inverse mapping $\varphi^{-1}$ is called a local parameterization of $M$.

**Stiefel manifold**: 
let $St(p, n)$ denote the set of all $n\times p$ orthonormal matrices; i.e.
$$St(p, n):= \{X\in R^{n\times p}: X^TX=I_p\}$$
where $I_p$ denotes the identity matrix. The set $St(p, n)$ is called an (orthogonal or compact) Stiefel manifold.

**real projective space**:
$RP^{n-1}$ is the set of all directions in $R^n$, let $R_*^n:=R^n-\{0\}$:
$$x\sim y \Leftrightarrow \exists t\in R_*: y=xt$$
then 
$$RP^{n-1} \simeq R_*^n/ \sim$$

**Grassmann manifold**:
$Grass(p, n)$ denote the set of all $p$-dimensional subspaces of $R^n$.

**tangent vector**:
let $\Im_x(M)$ denote the set of smooth real-valued functions defined on a neighborhood of $x$, a tangent vector $\xi_x$ to a manifold $M$ at a point $x$ is a mappint from $\Im_x(M)$ to $R$ such that there exists a curve $\gamma$ on $M$ with $\gamma(0)=x$, satisfying
$$\xi_x f=\dot \gamma(0)f:=\frac{d(f(\gamma(t)))}{dt}|_{t=0}$$
for all $f\in \Im_x(M)$, such a curve $\gamma$ is said to realize the tangent vector $\xi_x$
> we emphasize that $\dot \gamma(0)$ is defined as a mapping from $\Im_x(M)$ to $R$ , not as the time derivative $\gamma'(0)$.

**Riemannian manifold**:
a manifold whose tangent spaces are endowed with a smoothly varying inner product is called a *Riemannian manifold*. the smoothly varying inner product is called the *Riemannian metric*.

**Line-Search Method**:
$$x_{k+1} = R_{x_k}(t_k\eta _k)$$

normal: 
$$ \eta_k = -grad\ f(x_k) $$

accelerated: locally optimal conjugate gradient
$$ \eta_k = -grad\ f(x_k) $$
$$ \xi_k = \arg \min_{\xi_k \in span\{\eta_k, R_{x_k}^{-1}(x_{k-1}) \}} f(R_{x_k}(\xi_k))$$
$$ x_{k+1} = R_{x_k}(\xi_k) $$

|                               | $S^{n-1}$                                                    | $R^n$                    |
| ----------------------------- | ------------------------------------------------------------ | ------------------------ |
| metric                        | $g(\xi, \zeta) = \xi^T \zeta$                                |                          |
| tangent space                 | $\xi: x^T\xi=0$                                              | $R^n$                    |
| normal space                  | $\xi: \xi=\alpha x$                                          | $\varnothing$            |
| projection onto tangent space | $P_x\xi = (I-xx^T)\xi$                                       | $I$                      |
| retraction                    | $R_x(\xi)= \frac {x+ \xi}{\|x+\xi\|};\quad x\cos\|\xi\| + \frac \xi{\|\xi\|}\sin \xi$ | $R_x(\xi)= x+ \xi$       |
| cost                          | $f(x) = x^TAx$                                               |                          |
| gradient                      | $\nabla f(x) = P_x\nabla\bar f(x)$                           | $\nabla\bar f(x) = 2 Ax$ |


|                               | $St(p, n)$                                 | $R^{n\times p}$           |
| ----------------------------- | ------------------------------------------ | ------------------------- |
| metric                        | $g(Z_1, Z_2) = \mathrm{tr} (Z_1^TZ_2)$     |                           |
| tangent space                 | $Z\in R^{n\times p}: \mathrm{sym}(X^TZ)=0$ | $R^{n\times p}$           |
| normal space                  | $Z\in R^{n\times p}: Z=XS, S^T=S$          | $\varnothing$             |
| projection onto tangent space | $P_XZ = Z- X\mathrm{sym}(X^TZ)$            | $I$                       |
| retraction                    | $R_X(Z)= \mathrm{qf}(X+Z)$                 | $R_X(Z)= X+ Z$            |
| cost                          | $f(X) = \mathrm{tr}(X^TAXN)$               |                           |
| gradient                      | $\nabla f(x) = P_X\nabla\bar f(x)$         | $\nabla\bar f(X) = 2 AXN$ |

> let $M$ be a Riemannian submanifold of Euclidean Space, then the Riemannian connections on $M$ is: 
> $$\nabla_{\eta_x} \xi = P_x(D\xi(x)[\eta_x])$$
> i.e., a classical directional derivative followed by an orthogonal projection

> let $f$ be a smooth real-valued function on $R^n$, $x\in R^n$, $U$ a bounded neighborhood of $x$, and $H$ **any symmetric matrix**. then there exists $c> 0$ such that, for all $(x+ h)\in U$, 
> $$||f(x+h)- (f(x)+ \partial f(x)h+ \frac 12h^THh)|| \le c||h||^2$$
> if, moreover, $H_{i, j} = \partial_i \partial_j f(x)$, then there exists $c>0$ such that, 
> $$||f(x+h)- (f(x)+ \partial f(x)h+ \frac 12h^THh)|| \le c||h||^3$$