**查看磁盘空间**：`ncdu [FOLDER=.]`（实在不行`du -h`，`-s`不含subfolder，`--maxdepth=1`指定层数） 

**静默执行**：`nohup python run.py &` 返回进行ID用于kill

**远端执行脚本**：`cat hello.py | ssh user@192.168.1.101 python -` （执行目录呢？）

**移动、复制指定文件**： `find . -iname '*.rtv'| xargs mv -t ..`

**软链接**：
`ln -s [SRC] [DST]`  
`sudo rm [DST]`

**查看文件（夹）大小**：`du` 

> `-s` 不含subfolder；`-h` 给人看；`--maxdepth=1`设置层数；

`tar`

> `tar -xvf [FILE_NAME]`
> `tar -czvf [FILE_NAME]`

`zip`

> `zip -r sub.zip sub-dot sub-kml sub.txt` # -q 静默
> `unzip [FILE_NAME]`

`grep`

> `-i` 忽略大小写；`-e` 多模式匹配； `-v` 反选；`-n` 显示行号；


`wc` 查看行数

> `-l` 行数； `-w` 单词数；
> `find . -iname 'quality.txt'| wc `

`head` 查看前几行

> `-n` 行数；

按键等待： `read -p 'press any key'`

强制关闭程序：`xkill`

`ldd` 现实程序所有依赖
