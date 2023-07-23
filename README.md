在linux终端上管理路径，方便用户快捷地到达某一指定的目录中

# 安装方式
## 第1步——安装bin文件
在项目根目录执行如下命令：
```shell
mkdir my-build && cd my-build # 创建build文件夹并且cd到该目录下
cmake .. # 配置
cmake --build . # 然后build项目
sudo cmake --install . # 然后安装到/usr/local/bin/
```
## 第2步——添加脚本文件
回到项目根目录中
```shell
cp paths_mgr.sh /your/own/path/ # 把该shell脚本拷贝到你的某一目录下
```
## 第3步——部署
打开终端配置文件`cd && vim .bashrc`
在该文件尾部添加
```shell
# .bashrc
source my-shell/paths_mgr.sh
alias pa='paths-mgr'
```
保存退出后，执行`source .bashrc`

# 使用方式
* `pa a`  添加当前路径到菜单中
* `pa或者pa ls` 列出路径菜单
* `pa {number}` cd到指定的路径下，number是菜单中的某一项的下标
* `pa d {number}` 删除菜单中的某一项