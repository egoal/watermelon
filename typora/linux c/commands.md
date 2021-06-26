`tar`
> `tar -xvf [FILE_NAME]`
> `tar -czvf [FILE_NAME]`



`zip`

> `zip -r sub.zip sub-dot sub-kml sub.txt`
> `unzip [FILE_NAME]`



`grep`

> `-i` 忽略大小写；`-e` 多模式匹配； `-v` 反选；`-n` 显示行号；



`du` 查看文件大小

> `-s` 不含subfolder；`-h` 给人看；



`wc` 查看行数

> `-l` 行数； `-w` 单词数；
> `find . -iname 'quality.txt'| wc `



`head` 查看前几行

> `-n` 行数；



按键等待： `read -p 'press any key'`



移动指定文件： `find . -iname '*.rtv'| xargs mv -t ..`



强制关闭程序：`xkill`



`ldd` 现实程序所有依赖