#! /bin/bash

# binary="./cmake-build-debug/PathsMgr_run"
PATHS_MGR_BIN=${DEBUG_PATHS_MGR_BIN:-PathsMgr_run}

function url_decode() {
	: "${*//+/ }"
	echo -e "${_//%/\\x}"
}

function entry_to() {
    $PATHS_MGR_BIN "r" $1
    cd $1
}

function select_menu() {
	echo "遇到相同的名字，请选择一个路径："
	select item; do
		if [ 1 -le "$REPLY" ] && [ "$REPLY" -le $# ]; then
			echo "切换路径到：""$item"
			entry_to $item
			break
		else
			echo "请输入数字 1-$#"
		fi
	done
}

function change_dir() {
    local str_from_binary
    # 获取程序的输出内容
	str_from_binary=$($PATHS_MGR_BIN $1)
	if [ $? -ne 0 ]; then
		echo "切换路径出现错误，请检查参数是否正确"
	else
	    # 从str_from_binary解析目录path
		str_from_binary=${str_from_binary##*;}
		path=$(url_decode $str_from_binary)

		echo "切换路径到："$path
		entry_to $path
	fi
}

function change_dir_by_cd() {
    local dir_name str_from_binary
	dir_name=$1
	str_from_binary=$($PATHS_MGR_BIN "cd" "$dir_name")

	if [ $? -ne 0 ]; then
		echo "切换路径出现错误，请检查参数是否正确"
	else
		str_from_binary=${str_from_binary##*;}
		IFS=$':' read -r -a array <<<"$str_from_binary"

		for ((i = 0; i < ${#array[@]}; i++)); do
			array[i]=$(url_decode ${array[i]})
		done

		if [ ${#array[@]} -eq 1 ]; then
			echo "切换路径到："${array[0]}
			entry_to ${array[0]}
		else
			select_menu ${array[@]}
		fi
	fi
}

function paths_mgr() {
    if [[ ${DEBUG_PATHS_MGR_BIN} && ${DEBUG_PATHS_MGR_BIN-x} ]]; then
        echo "bin path:"$DEBUG_PATHS_MGR_BIN
    fi

	if [[ $1 =~ ^[0-9]+$ && $# == 1 ]]; then
		change_dir $1
	elif [[ $1 == "cd" && $# == 2 ]]; then
		change_dir_by_cd $2
	else
		# 把所有参数再传递下去，$(echo $*)奇怪的写法
		$PATHS_MGR_BIN $(echo $*)
	fi
}

# todo 有没有别的办法对二级命令做补全判断的
function paths_mgr_completions() {
	# echo " comp words count:" ${#COMP_WORDS[@]} " comp cword:" $COMP_CWORD
	local old_ifs str_from_binary prefix_str
	old_ifs=$IFS
	prefix_str=${COMP_WORDS[COMP_CWORD]}
	if [[ ${#COMP_WORDS[@]} == 2 ]]; then
		str_from_binary=$($PATHS_MGR_BIN "subcommands")
		if [ $? -ne 0 ]; then
			echo $str_from_binary
			echo "切换路径出现错误，请检查参数是否正确"
		else
			str_from_binary=${str_from_binary##*;}
			IFS=$':' read -r -a array <<<"$str_from_binary"
			for ((i = 0; i < ${#array[@]}; i++)); do
				if [[ "${array[i]}" =~ ^${prefix_str}.* ]]; then
					COMPREPLY+=(${array[i]})
				fi
			done
		fi
	elif [[ ${#COMP_WORDS[@]} == 3 ]]; then
	    local sub_command
		sub_command=${COMP_WORDS[1]}

		if [[ $sub_command == "cd" ]]; then
			str_from_binary=$($PATHS_MGR_BIN "predict" "$prefix_str")
			if [ $? -ne 0 ]; then
				echo $str_from_binary
				echo "切换路径出现错误，请检查参数是否正确"
			else
				str_from_binary=${str_from_binary##*;}
				IFS=$':' read -r -a array <<<"$str_from_binary"

				for ((i = 0; i < ${#array[@]}; i++)); do
					array[i]=$(url_decode ${array[i]})
					COMPREPLY+=(${array[i]})
				done
			fi
		fi
	fi
	IFS=$old_ifs
}

alias pa="paths_mgr"

complete -F paths_mgr_completions pa
