### distribution
**均匀分布($X\sim U(a, b)$)**
- $E[X] = (a+ b)/2$
- $V[X] = (b-a)^2/12$

**指数分布(exponential distribution, $X\sim Exponential(\lambda)$)**，可以用来表示独立随机时间发生的时间间隔，比如旅客进入进场的时间间隔，打进客服中心电话的时间间隔等
- $f(x; \lambda) = \lambda e^{-\lambda x}; x\ge 0$， $\lambda >0$称为率参数，即单位时间发生该事件的次数。
- $E[X] = \frac1{\lambda}, D[X] = \frac1{\lambda^2}$

**泊松分布(poisson distribution，$X\sim\pi(\lambda), X\sim P(\lambda)$)**，描述单位时间内随机时间发生的次数，如某一服务设施在一定时间内收到的服务请求次数。
- $P(X=k)=\frac{e^{-\lambda}\lambda^k}{k!}$，$\lambda$为单位时间内随机时间的平均发生率
- $EX=DX=\lambda$
- $X\sim P(\lambda_x), Y\sim P(\lambda_y) \Rightarrow X+Y\sim P(\lambda_x+ \lambda_y)$

**伯努利分布(Bernoulli distribution)**
- $f(x)=p^x(1-p)^{1-x}; x\in (0, 1)$
- $EX=p, DX=p(1-p)$

### 5. Inequalities

**inequality**:
> Markov, for *non-negative* X: 
> $$P(X>t) \le \frac{E(X)}t$$
> $$E(x) = \int_0^{\infty} xf(x) dx \ge \int_x^{\infty} xf(x) dx \ge t\int_x^{\infty} f(x) dx = tP(x)$$

> Chebyshev
> $$ P(|x-\mu|\ge t) \le \frac {\sigma^2}{t^2}, \quad P(|Z|\ge k)\le \frac 1{k^2} $$
> $$P(|x-\mu|\ge t)  = P(|x-\mu|^2\ge t^2)\le \frac {E(x-\mu)^2}{t^2} $$

> Hoeffding, 
> for *independent observations* $\{Y_i\}$ with $E(Y_i)=0$ and $a_i\le Y_i\le b_i$, let $\epsilon>0$:
> $$P(\sum_{i=1}^n Y_i\ge \epsilon)\le e^{-t\epsilon} \prod_{i=1}^n e^{t^2(b_i-a_i)^2/8}, \quad \forall t>0$$ 
> let $X_i\sim Bernolli(p)$, then $\overline{X_n}= n^{-1}\sum X_i$:
> $$ P(|\overline X_n - p|>\epsilon) \le 2e^{-2n\epsilon^2}, \quad \forall \epsilon>0$$

> Cauchy-Schwarz, for $X$, $Y$ with *finite variances*:
> $$E|XY| \le \sqrt{E(X^2)E(Y^2)}$$

> Jensen, for *convex* $g$:
> $$E(g(X)) \ge g(E(X))$$
> thus: 
> $$EX^2\ge (EX)^2, E(1/X)\ge 1/E(X)$$

### 6. Convergence of Random Variables

> Let $\{X_i\}$ be a sequence of random variales and let $X$ be another random variable. Let $F_n$ denote the CDF of $X_n$ and let $F$ denote the CDF of $X$.
> 1. **$X_n$ converges to $X$ in probability**, written $X_n \overset P {\to}X$, if for every $\epsilon>0$, 
> $$ P(|X_n- X|> \epsilon)\to 0 $$
> as $n\to \infty$
> 2. **$X_n$ converges to $X$ in distribution**, written $Xn\leadsto X$, if 
> $$\lim_{n\to \infty} F_n(t)= F(t)$$
> at all $t$ for which $F$ is continuous.

> **$X_n$ converges to $X$ in quadratic mean** (also called convergence in $L_2$), written $X_n\overset{qm}{\to} X$, if 
> $$E(X_n- X)^2\to 0$$
> as $n\to \infty$.
>

> **The Weak Law of Large Numbers(WLLN):** 
> if $\{X_i\}$ are IID, then $\bar X_n\overset P \to E(X_i)$

> **The Central Limit Theorem(CLT):**
> let $X_i$ be **IID** with mean $\mu$ and variance $\sigma^2$, let $\overline{X_n}=n^{-1}\sum_{i=1}^n X_i$. then: $\overline{X_n} \approx N(\mu, \sigma^2/n)$

> **The Delta Method:**
> $$Y_n\approx N(\mu, \sigma^2/n) \Rightarrow g(Y_n)\approx N(g(\mu), g'(\mu)^2 \ \sigma^2/n$$
>

---
### Statistical Inference

### 7. Models, Statistical Inference and Learning

### 8. Estimating the CDF and Statistical Functionals

> The **empirical distribution function $\widehat F_n$** is the CDF that puts mass $1/n$ at each data point $X_i$. Formally, 
> $$\widehat F_n(x) = \frac {\sum_i I(X_i\le x)}{n}$$
> where $I(Xi\le x) = 1$ if $X_i\le x$ else $0$
>

### 9. The Bootstrap

### 10. Parametric Inference

> Properties of Maximum Likelihood Estimators(MLE)
> 1. consistent: $\widehat \theta_n \overset P \to \theta_*$
> 2. equivariant: if $\widehat \theta_n$ is MLE of $\theta$, then $g(\widehat \theta_n)$ is MLE of $g(\theta)$
> 3. asymptotically Normal:$\widehat \theta \leadsto N(\theta, \widehat {se}^2/ n)$
> 4. asymptotically optimial or efficient: roughly, this means among all well behaved estimators, the MLE has the smallest variance, at least for large samples.
> 5. MLE is approximately the Bayes estimators.