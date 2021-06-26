### 关系：

integer program(IP)
$$
\min c^Tx \\
s.t. Ax = b, \quad x\ge 0, x\in Z
$$
linear relaxation(LR)
$$
\min c^Tx \\
s.t. Ax=b,\quad x\ge 0
$$
很显然LR比IP更松弛一些，所以有：

- IP的目标值不小于LR
- 如果LR不可行，则IP也不可行
- 如果LR的解是整型，自然也是IP的解
- 如果$c$各元素为整型，则IP的目标值不小于LR目标值的"round up"



### Branch and Bound

不断进行分支求解子问题，由于有子问题参考目标值的存在，所以可以进行剪枝：

1. solve the linear relaxation of the problem. if is integer, then we are done. Otherwise create two new subproblems by branching on a fractional variable.
2. A subproblem is not active when any of the following occurs: 
   - You used it to branch on
   - All variables in its solution are integer
   - It is infeasible
   - You can fathom the subproblem by bounding argument
3. Choose an active subproblem and branch on a fractional,repeat until there are no active subproblems.



### Cutting Plane Techniques

> The fundamental idea behind cutting planes is to add constraints to the program until the optimal basic feasible solution takes on integer values.
>
> - every feasible integer solution is feasible for the cut
> - the current fractional solution is not feasible for the cut

