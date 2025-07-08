#!/bin/bash

make || exit 1
clear

g_options=('a' 'u' 'g' 'p' 'r' 'A' 'o' 'n')
# g_options=('r' 'n')

generate_options()
{
	local combinations=("")

	for opt in "${g_options[@]}"; do
		local current_length=${#combinations[@]}
		for ((i=0; i<current_length; i++)); do
			combinations+=("${combinations[i]}$opt")
		done
	done

	local formatted=()
	for combo in "${combinations[@]}"; do
		if [ -z "$combo" ]; then
			formatted+=("")
		else
			formatted+=("-$combo")
		fi
	done

	printf '%s\n' "${formatted[@]}"
}

g_ret=0
g_dir=tester/logs

rm -rf $g_dir
mkdir -p $g_dir

g_flags="valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --track-fds=yes"
g_flags=""

n()
{
	local	dir

	if [[ "$1" == -* ]]; then
		dir=$g_dir/$1
	else
		dir=$g_dir/default
	fi

	mkdir -p $dir

	echo "Testing: $@"

	nm "$@" 2>/dev/null > $dir/nm.txt
	local nm_ret=$?

	$g_flags ./ft_nm "$@" 2>>$g_dir/valgrind.txt > $dir/ft_nm.txt
	local ft_nm_ret=$?

	if [ $nm_ret -ne $ft_nm_ret ]; then
		echo "❌ Return values differents (nm:$nm_ret | ft_nm:$ft_nm_ret)"
		let "g_ret+=1"
	fi

	if diff $dir/nm.txt $dir/ft_nm.txt > $dir/diff.txt; then
		echo "Success ✅"
		rm -rf $dir
	else
		echo "Fail ❌ (see $dir/diff.txt)"
		let "g_ret+=1"
	fi
}

mapfile -t options < <(generate_options)
size=${#options[@]}

for opt in "${options[@]}"; do
	n $opt "$@"
done

if [ "$g_ret" -eq 0 ]; then
	echo "✅ All tests passed successfully ($size combinations tested)"
	exit 0
elif [ "$g_ret" -eq 1 ]; then
	echo "❌" $g_ret "test failed /" $size
else
	echo "❌" $g_ret "tests failed /" $size
fi

exit 1
