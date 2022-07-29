**sql**

```sqlite
select count(*) from [TAB]; 行数

select [COLUMN_1],[COLUMN_2] from [TAB] limit [ROWS]; 查看一定行数

select distinct [COLUMNS] from [TAB];
```



**graphviz**

常规安装 `sudo apt-get install graphviz`		 `brew install graphviz` 

`dot [DOT_FILE] -T [FORMAT] -o [OUTPUT_FILE]`		`dot test.dot -T png -o test.png`

```dot
digraph main{
	graph [rankdir="LR"]
	node [fontsize=12, shape-record]
	edge [style=dashed, arrowhead=vee]
	
	A [ 
		label = "<0>A
		|+U& operator(T*, T*)" 
	]
	B
	
	B -> A:0
}
```





**你的clang-format习惯**

`{ AlignAfterOpenBracket: DontAlign, AllowShortCaseLabelsOnASingleLine: true, AccessModifierOffset: -2 }`



**valgrind基本使用**

`valgrind --tool=memcheck --leak-check=full --log-file=[LOG_FILE] [PROGRAM] [PROGRAM ARGS]` 内存泄漏

`valgrind --tool=callgrind [PROGRAM] [PROGRAM ARGS]` 性能分析

`valgrind --tool=massif [PROGRAM] [ARGS]` 内存占用



**以免需要偶尔使用到vim**

添加 `A`当行尾，`a`当前

撤销 `u`，撤销当前行的全部操作 `U`，回复 `Ctrl-r`

查找模式 `/`，随后使用`n`/`N`调到下一个/上一个

复制 `y`，复制多行`yy(yny, nyy)`

剪切 `d`，剪切到 `dw`下一个词词首，`de`到词尾，`d$`到行

粘贴 `p`

删除单个字符 `x`

新建行 `o`下方，`O`上方



**手动安装字体**
`sudo mkdir /usr/local/share/fonts/ms_fonts`
`sudo cp ~/Downloads/*.ttf /usr/local/share/fonts/ms_fonts/`
`sudo chown root:staff /usr/local/share/fonts/ms_fonts -R`
`sudo chmod 644 /usr/local/share/fonts/ms_fonts/* -R`
`sudo chmod 755 /usr/local/share/fonts/ms_fonts`
`sudo fc-cache -fv`



**包安装失败, 可能是上次安装未完成，占用了资源锁**
`sudo rm /var/cache/apt/archives/lock`
`sudo rm /var/lib/dpkg/lock`



**firefox is already running, but is not responding. to open a new window, you must...**
`way-1: firefox -p[rofilemanager]` 之后选择删除出错的profile, 新建一个即可
`way-2: pidof firefox | xargs kill` 强制关闭，`pkill firefox`



**windows mingw使用**
`cmake .. -G "MinGW Makefiles"`
`mingw32-make `

