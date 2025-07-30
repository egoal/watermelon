**zip打包排除** `zip -r archive.zip folder_to_zip -x "folder_to_zip/exclude_folder/**\*"`

**查看磁盘** `sudo baobab`

**awk按符号分割** `echo "12|23|11" | awk '{split($0,a,"|"); print a[3],a[2],a[1]}'`

**查找打包** `find results/output2/ -name policy_final.kml | zip finalkmls.zip -@`

**新建屏运行** 
- `screen -ls` 查看已有
- `screen -mdS XXXX`  新建
- `screen -rd XXXX`  进入
- `ctrl-a ctrl-d` 退出

**获得绝对路径** `realpath [xxx]`

**ssh免密登陆** 
- `ssh-keygen -t rsa` 生成秘钥
- `ssh-copy-id -i ~/.ssh/id_rsa.pub root@xxx.xxx.xxx.xxx` 把秘钥给对方
- `ssh root@xxx.xxx.xxx.xxx` 直接登陆对方

**aptitude** 更好的apt

**去掉日志前面的时间戳，用于对比** `cut -b13- log_file > trimed_file`

**查看日志打印数目** `less snippet_alignment_city.INFO | awk '{print $4}' | sort | uniq -c | sort`

**列出deb文件** `dpkg -c [DEB FILE]`

**查看系统日志** `dmesg | egrep -i -B100 'killed process'`

**统计文件个数** `find . -type f | wc -l`

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
