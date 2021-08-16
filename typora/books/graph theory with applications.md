## 1. Graphs and Subgraphs

---

**Walk**，**Trail**不走重复边，**Path**不走重复边和节点

> A graph is *bipartite* if and only if it contains no odd cycle.

## 2. Trees

> A minimal nonempty *edge cut* of a graph is called a *bond*.
>
> A minimum-weight spanning tree of a weighted graph will be called an *optimal tree*.

---

**获得生成树** *Kruskal's Algorithm*：

1. $S = \{\},\quad i=0$, sort $\{e_i\} \rightarrow \{a_i\}$ by weight

2. repeat

   ​	$S'=S \cup \{a_i\}$

   ​	$i+1 \rightarrow i$

   ​	continue if $G[S']$ is cyclic

   ​	$S' \rightarrow S$

   until $|S| = v-1$

## 4. Euler Tours and Hamilton Cycles

> A *tour* of $G$ is a closed walk that traverses each edge of $G$ at least once. An *Euler tour* is a tour which traverses each edge exactly once.
>
> A path that contains every vertex of $G$ is called a *Hamilton path* of $G$.

---

Chinese Postman Problem: 穿过所有节点最后回到出发点的最短走法。

如果$G$是eulerian，自然好说，随便找一个Euler tour就是（*Fleury's Algorithm*：确定初始点之后，每次增长时都不选择剩下图中的cut edge）；否则的话，则：

> 1. find, by duplicating edges, an eulerian weighted supergraph $G^*$ of $G$ such that $\Sigma_{e\in E^* \setminus E} w(e)$ is as small as possible;
> 2. find an Euler tour in $G^*$

其中1可参考Edmonds and Johnson(1973)，对只有两个奇数degree的$G$来说，实际上所需要复制的边就是连接这两个节点的最小路径，自然也是好解决的。

Travelling Salesman Problem: 在完全图中穿过所有节点最后回到出发点的最短走法。也就是最小权的*Hamilton Cycle*.

## 5. Matchings



