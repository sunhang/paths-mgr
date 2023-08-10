最近在linux shell终端上调试一些项目，要经常切换目录路径，在linux kernel路径编译调试vmlinux，切换到busybox路径编译文件系统，再切换到另外一个c项目搞点事情，频繁cd。

该工具在linux shell终端上管理目录路径，方便快捷地到达某一指定的目录中，从此告别频繁枯燥地来回cd。

# change log
* 添加对tab键的支持，可以在shell中按<tab><tab>做命令和参数的快速补全

# 安装方式
## 第1步 安装bin文件
在项目根目录执行如下命令：
```shell
mkdir my-build && cd my-build # 创建build文件夹并且cd到该目录下
cmake .. # 配置
cmake --build . # 然后build项目
sudo cmake --install . # 然后安装到/usr/local/bin/
```

# 第2步 部署
打开终端配置文件`cd && vim .bashrc`

在该文件尾部添加
```shell
source /usr/local/bin/paths_mgr.sh
```
保存退出后，执行`source .bashrc`

# 使用方式
* `pa a`  添加当前路径到菜单中
* `pa或者pa ls` 列出路径菜单
* `pa <number>` cd到指定的路径下，number是菜单中的某一项的下标
* `pa d <number>` 删除菜单中的某一项
* `pa cd <dirName>` 通过最终目录的名称，快速cd到那一目录中