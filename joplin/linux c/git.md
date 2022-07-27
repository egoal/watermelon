**merge分支**

```
git checkout dev
git pull
git checkout master
git merge dev
git push origin master
```





**取消失败的pull**

1. `git reflog`  查看操作记录
2. `git reset --hard HEAD@{0}` 强退



**撤销**

`git revert <commit>` 这样“撤销”会作为提交记录，便能直接推送



**改动单个项目的提交配置**

`.git/config`下：

```toml
[user]
	name = egoal
	email = 939425550@qq.com
```

直接在仓库下

`git config user.name "egoal"`

`git config user.email 939425550@qq.com`



**修改commit提交信息**

`git rebase -i <commit>` 在交互模式下`rebase`自身，就能输入每步的comment了



**本地直接关联远程仓库**

1. 有一个远端
2. `git remote add origin <url>` 关联之后，当前不再任何分支上
3. `git pull origin master` 合并远端到本地，现在在`master`上，远端在本地之后
4. `git push origin master` 常规推送



**whitespace warning**

`git rebase --whitespace=fix| nowarn`



**`submodule`相关**

`git submodule add git://github.com/...` 创建引用

clone带submodule的repo：

`git submodule init`

`git submodule update`

