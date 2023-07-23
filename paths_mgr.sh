#! /bin/bash

function changeDir() {
	strFromBinary=`PathsMgr_run $1`
	if [ $? -ne 0 ]
	then
		echo "切换路径出现错误，请检查参数是否正确"
	else
		local array=(${strFromBinary//\\n/})
		local size=${#array[@]}
		local path=${array[size-1]}
		echo "切换路径到："$path
		cd $path
	fi
}

function paths-mgr() {
	if [[ $1 =~ ^[0-9]+$ && $# == 1 ]]
	then
		changeDir $1
	else
		# 把所有参数再传递下去，$(echo $*)奇怪的写法
		PathsMgr_run $(echo $*)
	fi
	
}
