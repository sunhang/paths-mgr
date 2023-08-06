#! /bin/bash

binary="PathsMgr_run"

function urlDecode() {
	: "${*//+/ }"
	echo -e "${_//%/\\x}"
}

function selectMenu() {
	echo "遇到相同的名字，请选择一个路径："
	select item; do
		if [ 1 -le "$REPLY" ] && [ "$REPLY" -le $# ]; then
			echo "切换路径到：""$item"
			cd $item
			break
		else
			echo "请输入数字 1-$#"
		fi
	done
}

function changeDir() {
	strFromBinary=$($binary $1)
	if [ $? -ne 0 ]; then
		echo "切换路径出现错误，请检查参数是否正确"
	else
		local array=(${strFromBinary//\\n/})
		local size=${#array[@]}
		local path=${array[size - 1]}
		echo "切换路径到："$path
		cd $path
	fi
}

function changeDirByCd() {
	local dirName=$1
	strFromBinary=$($binary "cd" "$dirName")

	if [ $? -ne 0 ]; then
		echo "切换路径出现错误，请检查参数是否正确"
	else
		strFromBinary=${strFromBinary##*;}
		IFS=$':' read -r -a array <<<"$strFromBinary"

		for ((i = 0; i < ${#array[@]}; i++)); do
			array[i]=$(urlDecode ${array[i]})
		done

		if [ ${#array[@]} -eq 1 ]; then
			echo "切换路径到："${array[0]}
			cd ${array[0]}
		else
			selectMenu ${array[@]}
		fi
	fi
}

function paths-mgr() {
	if [[ $1 =~ ^[0-9]+$ && $# == 1 ]]; then
		changeDir $1
	elif [[ $1 == "cd" && $# == 2 ]]; then
		changeDirByCd $2
	else
		# 把所有参数再传递下去，$(echo $*)奇怪的写法
		$binary $(echo $*)
	fi
}

# todo 有没有别的办法对二级命令做补全判断的
function paths-mgr-completions() {
	# echo " comp words count:" ${#COMP_WORDS[@]} " comp cword:" $COMP_CWORD
	oldIfs=$IFS
	prefixStr=${COMP_WORDS[COMP_CWORD]}
	if [[ ${#COMP_WORDS[@]} == 2 ]]; then
		strFromBinary=$($binary "subcommands")
		if [ $? -ne 0 ]; then
			echo $strFromBinary
			echo "切换路径出现错误，请检查参数是否正确"
		else
			strFromBinary=${strFromBinary##*;}
			IFS=$':' read -r -a array <<<"$strFromBinary"
			for ((i = 0; i < ${#array[@]}; i++)); do
				if [[ "${array[i]}" =~ ^${prefixStr}.* ]]; then
					COMPREPLY+=(${array[i]})
				fi
			done
		fi
	elif [[ ${#COMP_WORDS[@]} == 3 ]]; then
		subCommand=${COMP_WORDS[1]}
		prefixStr=${COMP_WORDS[COMP_CWORD]}

		if [[ $subCommand == "cd" ]]; then
			strFromBinary=$($binary "predict" "$prefixStr")
			if [ $? -ne 0 ]; then
				echo $strFromBinary
				echo "切换路径出现错误，请检查参数是否正确"
			else
				strFromBinary=${strFromBinary##*;}
				IFS=$':' read -r -a array <<<"$strFromBinary"

				for ((i = 0; i < ${#array[@]}; i++)); do
					array[i]=$(urlDecode ${array[i]})
					COMPREPLY+=(${array[i]})
				done
			fi
		fi
	fi
	IFS=$oldIfs
}

alias pa="paths-mgr"

complete -F paths-mgr-completions pa
